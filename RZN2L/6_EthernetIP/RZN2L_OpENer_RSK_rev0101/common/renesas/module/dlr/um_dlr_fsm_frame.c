/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_dlr_cfg.h"
#include "um_dlr_api.h"
#include "um_dlr.h"
#include "um_dlr_internal.h"
#include "um_dlr_fsm_frame.h"
#include "um_ether_netif_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/** Destination MAC for Neighbor_Check_Request/Response and Sign_On */
static const uint8_t sga_dlr_mac_address_02[6] = {0x01, 0x21, 0x6C, 0x00, 0x00, 0x02};
/** Destination MAC for Learning Update */
static const uint8_t sga_dlr_mac_address_05[6] = {0x01, 0x21, 0x6C, 0x00, 0x00, 0x05};

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/** Framing functions */
static usr_err_t _fill_common_header( dlr_fsm_ctrl_t * p_ctrl, uint8_t const *p_dst_mac );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Send a DLR Link Status or Neighbor Status frame
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_send_link_neighbor_status( dlr_fsm_ctrl_t * const p_ctrl, dlr_status_frame_type_t const status_frame_type )
{
	/** Error codes */
	usr_err_t usr_err;

    /** allocate heap */
    USR_HEAP_ALLOCATE( p_ctrl->p_ether_netif_frame, sizeof(ether_netif_frame_t) );

	/** Resolve DLR frame buffer. */
	dlr_frame_link_neighbor_status_t * p_dlr_frame_buffer = ( dlr_frame_link_neighbor_status_t *) p_ctrl->p_ether_netif_frame->p_buffer;

	/** For status calculation */
	uint8_t status = 0;
	uint8_t port_mask = DLR_PORT_BOTH;

	/** Calculate status */
    status |= (uint8_t) status_frame_type;
	switch( status_frame_type )
	{
    case DLR_STATUS_FRAME_TYPE_LINK_STATUS:
        status |= (p_ctrl->link_active_port & port_mask);
        break;
    case DLR_STATUS_FRAME_TYPE_NEIGHBOR_STATUS:
        status |= (p_ctrl->neighbor_active_port & port_mask);
        break;
    default:
        usr_err = USR_ERR_ABORTED;
        USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );
        break;
	}

	/** Fill common header */
    (void) _fill_common_header( p_ctrl, p_ctrl->rx_beacon_info.source_mac_address );

    /** Fill common payload */
    p_dlr_frame_buffer->common_payload.b20_frame_type = DLR_FRAME_TYPE_LINK_STATUS;
    p_dlr_frame_buffer->common_payload.b21_source_port = (uint8_t)(DLR_PORT_NONE);
    p_dlr_frame_buffer->common_payload.b22_source_ip_address = *p_ctrl->p_ip_address; // DLR_BYTEORDER_SWAP32(*p_ctrl->p_ip_address );
    p_dlr_frame_buffer->common_payload.b26_sequence_id = DLR_BYTEORDER_SWAP32(p_ctrl->sequence_id);

    /** Fill specific payload */
    p_dlr_frame_buffer->b30_link_neighbor_status = status;
    memset(p_dlr_frame_buffer->b31_reserved, 0, 29 );

    /** Set the frame length and host port */
    p_ctrl->p_ether_netif_frame->length = 60;
    p_ctrl->p_ether_netif_frame->port = p_ctrl->link_active_port;	/** Send from active active_port */

    /** Update Sequence ID */
    p_ctrl->sequence_id++;

    /** Send frame */
    usr_err = um_dlr_ether_netif_send( p_ctrl->p_ether_netif_ctrl, p_ctrl->p_ether_netif_frame );
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Send a DLR Neighbor Check Request
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_send_neighbor_check_request( dlr_fsm_ctrl_t * const p_ctrl, dlr_port_t const target_port )
{
	/** Error codes */
	usr_err_t usr_err;

    /** allocate heap */
    USR_HEAP_ALLOCATE( p_ctrl->p_ether_netif_frame, sizeof(ether_netif_frame_t) );

	/** Resolve DLR frame buffer. */
	dlr_frame_neighbor_check_request_t * p_dlr_frame_buffer = ( dlr_frame_neighbor_check_request_t *) p_ctrl->p_ether_netif_frame->p_buffer;

	/** Fill common header */
    (void) _fill_common_header( p_ctrl, sga_dlr_mac_address_02 );

    /** Fill common payload */
    p_dlr_frame_buffer->common_payload.b20_frame_type = DLR_FRAME_TYPE_NEIGHBOR_CHECK_REQUEST;
    p_dlr_frame_buffer->common_payload.b21_source_port = (uint8_t)(target_port);
    p_dlr_frame_buffer->common_payload.b22_source_ip_address = *p_ctrl->p_ip_address; // DLR_BYTEORDER_SWAP32(*p_ctrl->p_ip_address );
    p_dlr_frame_buffer->common_payload.b26_sequence_id = DLR_BYTEORDER_SWAP32(p_ctrl->sequence_id);

    /** Fill specific payload */
    memset(p_dlr_frame_buffer->b30_reserved, 0, 30 );

    /** Set the frame length and host port */
    p_ctrl->p_ether_netif_frame->length = 60;
    p_ctrl->p_ether_netif_frame->port = target_port;		/** Requested port */

    /** Inform receiver of requested sequence id. */
	usr_err = um_dlr_task_receiver_inform_neighbor_check_request_sequence_id(p_ctrl->p_receiver_ctrl, target_port, p_ctrl->sequence_id);
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Update Sequence ID */
    p_ctrl->sequence_id++;

    /** Send frame */
    usr_err = um_dlr_ether_netif_send( p_ctrl->p_ether_netif_ctrl, p_ctrl->p_ether_netif_frame );
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Send a DLR Neighbor Check Response
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_send_neighbor_check_response( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event, dlr_port_t const target_port )
{
	/** Error codes */
	usr_err_t usr_err;

    /** allocate heap */
    USR_HEAP_ALLOCATE( p_ctrl->p_ether_netif_frame, sizeof(ether_netif_frame_t) );

	/** Resolve DLR frame buffer. */
	dlr_frame_neighbor_check_response_t * p_dlr_frame_buffer = ( dlr_frame_neighbor_check_response_t *) p_ctrl->p_ether_netif_frame->p_buffer;

	/** For sequence id calculating. */
	dlr_frame_neighbor_check_request_t * p_requeset_frame = NULL;

    /** Get Ethernet frame which device has received. */
	p_requeset_frame = ( dlr_frame_neighbor_check_request_t * ) p_dlr_event->p_ether_frame->p_buffer;
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame,
                           DLR_FRAME_TYPE_NEIGHBOR_CHECK_REQUEST == p_requeset_frame->common_payload.b20_frame_type,
                           USR_ERR_ABORTED );

	/** Fill common header */
    (void) _fill_common_header( p_ctrl, sga_dlr_mac_address_02 );

    /** Fill common payload */
    p_dlr_frame_buffer->common_payload.b20_frame_type = DLR_FRAME_TYPE_NEIGHBOR_CHECK_RESPONSE;
    p_dlr_frame_buffer->common_payload.b21_source_port = (uint8_t)(target_port);
    p_dlr_frame_buffer->common_payload.b22_source_ip_address = *p_ctrl->p_ip_address; //DLR_BYTEORDER_SWAP32(*p_ctrl->p_ip_address);
    p_dlr_frame_buffer->common_payload.b26_sequence_id = p_requeset_frame->common_payload.b26_sequence_id;	/** response with request sequence id */

    /** Fill specific payload */
    p_dlr_frame_buffer->b30_requeset_source_port = p_requeset_frame->common_payload.b21_source_port;	/** response with the request source port. */
    memset( p_dlr_frame_buffer->b31_reserved, 0, 29 );

    /** Set the frame length and host port */
    p_ctrl->p_ether_netif_frame->length = 60;
    p_ctrl->p_ether_netif_frame->port = target_port;

    /** Don't update sequence ID because this frame is response. */

    /** Send frame */
    usr_err = um_dlr_ether_netif_send( p_ctrl->p_ether_netif_ctrl, p_ctrl->p_ether_netif_frame );
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Send a DLR Sing_On frame
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_send_sign_on( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event, dlr_port_t target_port )
{
	/** Error codes */
	usr_err_t usr_err;

	/** allocate heap */
	USR_HEAP_ALLOCATE( p_ctrl->p_ether_netif_frame, sizeof(ether_netif_frame_t) );

	/** Resolve DLR frame buffer. */
	dlr_frame_sign_on_t * p_dlr_frame_buffer = ( dlr_frame_sign_on_t * ) p_ctrl->p_ether_netif_frame->p_buffer;

	/** For inheriting the sing_on frame */
	dlr_frame_sign_on_t * p_prev_sign_on_frame = NULL;

	/** For new node field */
	dlr_frame_sign_on_node_field_t * p_new_node;
	uint16_t number_of_nodes = 0;
	uint32_t new_frame_length = 0;

    /** Get Ethernet frame which device has received. */
	p_prev_sign_on_frame = ( dlr_frame_sign_on_t * ) p_dlr_event->p_ether_frame->p_buffer;
	USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame,
	                       DLR_FRAME_TYPE_SIGN_ON == p_prev_sign_on_frame->common_payload.b20_frame_type,
	                       USR_ERR_ABORTED );

	/** Load the number of nodes from previous sign_on frame. */
	number_of_nodes = DLR_BYTEORDER_SWAP16(p_prev_sign_on_frame->b30_number_of_nodes_in_list);

	/** Fill common header */
    (void) _fill_common_header( p_ctrl, sga_dlr_mac_address_02 );

    /** Fill common payload */
    p_dlr_frame_buffer->common_payload.b20_frame_type = DLR_FRAME_TYPE_SIGN_ON;
    p_dlr_frame_buffer->common_payload.b21_source_port = (uint8_t)( target_port );
    p_dlr_frame_buffer->common_payload.b22_source_ip_address = p_prev_sign_on_frame->common_payload.b22_source_ip_address;
    p_dlr_frame_buffer->common_payload.b26_sequence_id = p_prev_sign_on_frame->common_payload.b26_sequence_id;	/** Inherit the sequence number. */
    /** Fill specific payload */
    memcpy( &p_dlr_frame_buffer->b32_node1_supervisor, &p_prev_sign_on_frame->b32_node1_supervisor, number_of_nodes * sizeof(dlr_frame_sign_on_node_field_t));
	if( number_of_nodes < DLR_SIGN_ON_FRAME_MAXIMUM_NODES )
	{
		/** Copy listed nodes */
	    p_dlr_frame_buffer->b30_number_of_nodes_in_list = DLR_BYTEORDER_SWAP16((uint16_t)(number_of_nodes + 1));	/** Increment number of nodes */

	    /** Add new node */
	    p_new_node = (&p_dlr_frame_buffer->b32_node1_supervisor);
	    p_new_node += number_of_nodes;
	    p_new_node->ip_address = *p_ctrl->p_ip_address; // DLR_BYTEORDER_SWAP32(*p_ctrl->p_ip_address );
	    (void) um_dlr_ether_netif_get_local_mac_address(p_ctrl->p_ether_netif_ctrl, p_new_node->mac_address);

	    /** Calculate new frame length */
	    new_frame_length = (uint32_t)(32 + (number_of_nodes + (uint32_t)1) * sizeof(dlr_frame_sign_on_node_field_t));
	}
	else
	{
		/** Overwrite destination MAC address to */
		memcpy( p_dlr_frame_buffer->common_header.b00_destination_mac_address, p_ctrl->rx_beacon_info.source_mac_address, DLR_MAC_ADDRESS_LENGTH );

	    /** Calculate new frame length */
		new_frame_length = (uint32_t)(32 + (number_of_nodes) * sizeof(dlr_frame_sign_on_node_field_t));
	}

	if( new_frame_length < DLR_SIGN_ON_FRAME_MINIMUM_LENGTH )
	{
		memset( p_dlr_frame_buffer + new_frame_length, 0, (DLR_SIGN_ON_FRAME_MINIMUM_LENGTH - new_frame_length) );
	    new_frame_length = DLR_SIGN_ON_FRAME_MINIMUM_LENGTH;
	}

    /** Set the frame length and target port */
    p_ctrl->p_ether_netif_frame->length = new_frame_length;
    p_ctrl->p_ether_netif_frame->port = target_port;

    /** Don't update sequence ID because this frame is response. */

    /** Send frame */
    usr_err = um_dlr_ether_netif_send( p_ctrl->p_ether_netif_ctrl, p_ctrl->p_ether_netif_frame );
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    return USR_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief Send a DLR LearningUpdate frame
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_send_learning_update( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error codes */
	usr_err_t usr_err;

    /** allocate heap */
    USR_HEAP_ALLOCATE( p_ctrl->p_ether_netif_frame, sizeof(ether_netif_frame_t) );

	/** Resolve DLR frame buffer. */
	dlr_frame_learning_update_t * p_dlr_frame_buffer = ( dlr_frame_learning_update_t * ) p_ctrl->p_ether_netif_frame->p_buffer;

	/** Received flush table frame. */
	dlr_frame_flush_tables_t * p_flush_tables_frame;

    /** Get Ethernet frame which device has received. */
	p_flush_tables_frame = ( dlr_frame_flush_tables_t * ) p_dlr_event->p_ether_frame->p_buffer;
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame,
                           DLR_FRAME_TYPE_FLUSH_TABLES == p_flush_tables_frame->common_payload.b20_frame_type,
                           USR_ERR_ABORTED );

	/** If the flush tables frame does not require the learning update, return with USR_SUCCESS. */
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame,
                           (uint8_t) DLR_TRUE == p_flush_tables_frame->b30_learning_update_enable,
                           USR_SUCCESS );

    /** Fill header fields. */
    memcpy( p_dlr_frame_buffer->b00_destination_mac_address, sga_dlr_mac_address_05, DLR_MAC_ADDRESS_LENGTH );
    (void) um_dlr_ether_netif_get_local_mac_address( p_ctrl->p_ether_netif_ctrl, p_dlr_frame_buffer->b06_source_mac_address );
    p_dlr_frame_buffer->b12_ring_ether_type = DLR_BYTEORDER_SWAP16(DLR_RING_ETHER_TYPE);
    p_dlr_frame_buffer->b14_ring_sub_type = DLR_RING_SUBTYPE;
    p_dlr_frame_buffer->b15_ring_protocol_version = DLR_RING_PROTOCOL_VERSION;

    /** Fill common payload */
    p_dlr_frame_buffer->b16_frame_type = DLR_FRAME_TYPE_LEARNING_UPDATE;
    p_dlr_frame_buffer->b17_source_port = DLR_PORT_NONE;
    p_dlr_frame_buffer->b18_source_ip_address = *p_ctrl->p_ip_address; // DLR_BYTEORDER_SWAP32(*p_ctrl->p_ip_address );
    p_dlr_frame_buffer->b22_sequence_id = DLR_BYTEORDER_SWAP32(p_ctrl->sequence_id);

    /** Fill specific payload */
    memset( p_dlr_frame_buffer->b26_reserved, 0, sizeof(p_dlr_frame_buffer->b26_reserved) );

    /** Set the frame length and host target port */
    p_ctrl->p_ether_netif_frame->length = 26;
    p_ctrl->p_ether_netif_frame->port = DLR_PORT_BOTH;

    /** Update sequence ID. */
    p_ctrl->sequence_id++;

    /** Send frame */
    usr_err = um_dlr_ether_netif_send( p_ctrl->p_ether_netif_ctrl, p_ctrl->p_ether_netif_frame );
    USR_HEAP_ERROR_RETURN( p_ctrl->p_ether_netif_frame, USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Fill DLR frame common header.
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
static usr_err_t _fill_common_header( dlr_fsm_ctrl_t * p_ctrl, uint8_t const *p_dst_mac )
{
	/** Resolve DLR frame buffer. */
	dlr_frame_common_t * p_dlr_frame_buffer = ( dlr_frame_common_t *) p_ctrl->p_ether_netif_frame->p_buffer;

    /** Fill header fields. */
    memcpy( p_dlr_frame_buffer->common_header.b00_destination_mac_address, p_dst_mac, 6 );
    (void) um_dlr_ether_netif_get_local_mac_address(p_ctrl->p_ether_netif_ctrl, p_dlr_frame_buffer->common_header.b06_source_mac_address);
    p_dlr_frame_buffer->common_header.b12_802_1q_tag_type = DLR_BYTEORDER_SWAP16(DLR_802_1Q_TAG_TYPE);
    p_dlr_frame_buffer->common_header.b14_802_1q_tag_control = DLR_BYTEORDER_SWAP16(DLR_802_1Q_TAG_CONTROL_BIAS + p_ctrl->rx_beacon_info.vlan_id);
    p_dlr_frame_buffer->common_header.b16_ring_ether_type = DLR_BYTEORDER_SWAP16(DLR_RING_ETHER_TYPE);
    p_dlr_frame_buffer->common_header.b18_ring_sub_type = DLR_RING_SUBTYPE;
    p_dlr_frame_buffer->common_header.b19_ring_protocol_version = DLR_RING_PROTOCOL_VERSION;

    return USR_SUCCESS;
}
