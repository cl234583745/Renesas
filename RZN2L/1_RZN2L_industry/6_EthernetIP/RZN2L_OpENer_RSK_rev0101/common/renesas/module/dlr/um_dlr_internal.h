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

#ifndef UM_DLR_INTERNAL_H
#define UM_DLR_INTERNAL_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_dlr_cfg.h"
#include "um_dlr_api.h"
#include "um_dlr.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** maximum number of tries to check for a neighbor */
#define DLR_NEIGHBOR_CHECK_TRIES 			(3)            ///< no. of check requests */

#define DLR_MAC_ADDRESS_LENGTH    	(6)

/* Length of EthernetHeader + DLR Header */
#define DLR_HEADER_LENGTH 			(0x14)                     /**< Header Length */

/* DLR LinkStatus bitmap */
#define DLR_LINKNEIGHBOR_STATUS_PORT1    (1<<0)          /**< link status of port 0 */
#define DLR_LINKNEIGHBOR_STATUS_PORT2    (1<<1)          /**< link status of port 1 */
#define DLR_LINKNEIGHBOR_STATUS_NEIGHBOR (1<<7)          /**< neighbor status */
#define DLR_LINKNEIGHBOR_STATUS_LINK 	 (0<<7)          /**< neighbor status */

/** VLAN tag */
#define DLR_802_1Q_TAG_HEADER_LEN   (4)			///< Tag type + Tag control */

/** DLR Common Header constant values */
#define DLR_802_1Q_TAG_TYPE			((uint16_t) 0x8100)
#define DLR_802_1Q_TAG_CONTROL_BIAS ((uint16_t) 0xE000)
#define DLR_RING_ETHER_TYPE   		((uint16_t) 0x80E1)
#define DLR_RING_SUBTYPE 			((uint8_t) 0x02)
#define DLR_RING_PROTOCOL_VERSION   ((uint8_t) 0x01)

#define DLR_802_1Q_ETHER_FRAME_MAXIMUM_LENGTH 	(1518U)	/** Excluding FCS */
#define DLR_SIGN_ON_FRAME_MAXIMUM_NODES			(148U)	/** (1518-32)/10 */
#define DLR_SIGN_ON_FRAME_MINIMUM_LENGTH		(60U)

/* Swap a 16 bit value */
#define DLR_BYTEORDER_SWAP16(x)  ((uint16_t)(                                 \
                        		 (((uint16_t)(x) & (uint16_t) 0x00ffU) << 8) | \
								 (((uint16_t)(x) & (uint16_t) 0xff00U) >> 8)))

/* Swap a 32 bit value */
#define DLR_BYTEORDER_SWAP32(x) ((uint32_t)(                                        \
								(((uint32_t)(x) & (uint32_t) 0x000000ffUL) << 24) |  \
								(((uint32_t)(x) & (uint32_t) 0x0000ff00UL) <<  8) |  \
								(((uint32_t)(x) & (uint32_t) 0x00ff0000UL) >>  8) |  \
								(((uint32_t)(x) & (uint32_t) 0xff000000UL) >> 24)))

#if (1) == UM_DLR_CFG_HOST_ENDIAN_LITTLE
#define GET_NETWORK_ORDER_FIELD_16(x)	DLR_BYTEORDER_SWAP16(x)
#define GET_NETWORK_ORDER_FIELD_32(x)	DLR_BYTEORDER_SWAP32(x)
#else
#define GET_NETWORK_ORDER_FIELD_16(x)	(x)
#define GET_NETWORK_ORDER_FIELD_32(x)	(x)
#endif

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** DLR frame types */
typedef enum e_dlr_frame_type
{
	DLR_FRAME_TYPE_BEACON 				    = (0x01),   ///< Beacon
	DLR_FRAME_TYPE_NEIGHBOR_CHECK_REQUEST   = (0x02),   ///< Neighbor_Check_Request
	DLR_FRAME_TYPE_NEIGHBOR_CHECK_RESPONSE  = (0x03),   ///< Neighbor_Check_Response
	DLR_FRAME_TYPE_LINK_STATUS 			    = (0x04),   ///< Link_Status / Neighbor_Status
	DLR_FRAME_TYPE_LOCATE_FAULT 			= (0x05),   ///< Locate_Fault
	DLR_FRAME_TYPE_ANNOUNCE 				= (0x06),   ///< Announce
	DLR_FRAME_TYPE_SIGN_ON 					= (0x07),   ///< Sign_On
	DLR_FRAME_TYPE_ADVERTISE 				= (0x08),   ///< Advertise
	DLR_FRAME_TYPE_FLUSH_TABLES 			= (0x09),   ///< Flush_Tables
	DLR_FRAME_TYPE_LEARNING_UPDATE 			= (0x0A),   ///< Learning_Update
} dlr_frame_type_t;

/**
 * DLR status frame type
 */
typedef enum e_dlr_status_frame_type
{
	DLR_STATUS_FRAME_TYPE_LINK_STATUS     = (0<<7), ///< DLR_STATUS_FRAME_TYPE_LINK_STATUS */
	DLR_STATUS_FRAME_TYPE_NEIGHBOR_STATUS = (1<<7),	///< DLR_STATUS_FRAME_TYPE_NEIGHBOR_STATUS */
} dlr_status_frame_type_t;

/**
 * DLR context type
 */
typedef enum e_dlr_context_type
{
	DLR_CONTEXT_ISR,		/**< DLR_ISR_CONTEXT */
	DLR_CONTEXT_RTOS_TASK,	/**< DLR_TASK_CONTEXT */
	DLR_CONTEXT_RTOS_TIMER,	/**< DLR TIMER CONTEXT */
} dlr_context_type_t;

typedef enum e_dlr_enqueue_type
{
	DLR_ENQUEUE_BACK,
	DLR_ENQUEUE_FRONT,
} dlr_enqueue_type_t;

#pragma pack (1)    //Declare #pragma pack to handle structure elements that span a 4-byte boundary.
typedef struct st_dlr_frame_common_header
{
	uint8_t  b00_destination_mac_address[6];
	uint8_t  b06_source_mac_address[6];
	uint16_t b12_802_1q_tag_type;
	uint16_t b14_802_1q_tag_control;
	uint16_t b16_ring_ether_type;
	uint8_t  b18_ring_sub_type;
	uint8_t  b19_ring_protocol_version;
} dlr_frame_common_header_t;

typedef struct st_dlr_frame_common_payload
{
	uint8_t b20_frame_type;
	uint8_t b21_source_port;
	uint32_t b22_source_ip_address;
	uint32_t b26_sequence_id;
} dlr_frame_common_payload_t;

typedef struct st_dlr_frame_common
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
} dlr_frame_common_t;

typedef struct st_dlr_frame_neighbor_check_request
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
	uint8_t b30_reserved[30];
} dlr_frame_neighbor_check_request_t;

typedef struct st_dlr_frame_neighbor_check_response
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
	uint8_t b30_requeset_source_port;
	uint8_t b31_reserved[29];
} dlr_frame_neighbor_check_response_t;

typedef struct st_dlr_frame_link_neighbor_status
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
	uint8_t b30_link_neighbor_status;
	uint8_t b31_reserved[29];
} dlr_frame_link_neighbor_status_t;

typedef struct st_dlr_frame_locate_fault
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
	uint8_t b30_reserved[30];
} dlr_frame_locate_fault_t;

typedef struct st_dlr_frame_sign_on_node_field
{
	uint8_t mac_address[6];
	uint32_t ip_address;
} dlr_frame_sign_on_node_field_t;

typedef struct st_dlr_frame_sign_on
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
	uint16_t b30_number_of_nodes_in_list;
	dlr_frame_sign_on_node_field_t b32_node1_supervisor;
} dlr_frame_sign_on_t;

typedef struct st_dlr_frame_flush_tables
{
	dlr_frame_common_header_t  common_header;
	dlr_frame_common_payload_t common_payload;
	uint8_t b30_learning_update_enable;
	uint8_t b31_reserved[29];
} dlr_frame_flush_tables_t;

typedef struct st_dlr_frame_learning_update
{
	/** Uncommon Header */
	uint8_t  b00_destination_mac_address[6];
	uint8_t  b06_source_mac_address[6];
/**	uint16_t tag_type;	  **/
/**	uint16_t tag_control; **/
	uint16_t b12_ring_ether_type;
	uint8_t  b14_ring_sub_type;
	uint8_t  b15_ring_protocol_version;

	/** Common payload */
	uint8_t  b16_frame_type;
	uint8_t  b17_source_port;
	uint32_t b18_source_ip_address;

	/** Uncommon payload */
	uint32_t b22_sequence_id;
	uint8_t  b26_reserved[34];

} dlr_frame_learning_update_t;

#pragma pack () //End of #pragma pack (1) area

/***********************************************************************************************************************
 * Functions Prototypes
 **********************************************************************************************************************/
/** Neighbor check timer APIs. */
usr_err_t um_dlr_timer_neighbor_checker_open( dlr_neighbor_checker_ctrl_t * const p_ctrl,
											  dlr_executor_ctrl_t * const p_executor_ctrl );
usr_err_t um_dlr_timer_neighbor_checker_start( dlr_neighbor_checker_ctrl_t * const p_ctrl, dlr_port_t checked_port );
usr_err_t um_dlr_timer_neighbor_checker_stop( dlr_neighbor_checker_ctrl_t * const p_ctrl, dlr_port_t checked_port );
usr_err_t um_dlr_timer_neighbor_checker_clear_timeout_count( dlr_neighbor_checker_ctrl_t * const p_ctrl, dlr_port_t checked_port );

/** Ethernet frame receiver APIs */
usr_err_t um_dlr_task_receiver_open( dlr_receiver_ctrl_t * const p_ctrl,
									 dlr_fsm_ctrl_t * const p_fsm_ctrl,
									 dlr_executor_ctrl_t * const p_executor_ctrl );
usr_err_t um_dlr_task_receiver_start( dlr_receiver_ctrl_t *	const p_ctrl );
usr_err_t um_dlr_task_receiver_stop( dlr_receiver_ctrl_t * const p_ctrl );
usr_err_t um_dlr_task_receiver_take_frame( dlr_receiver_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_packet_buffer );
usr_err_t um_dlr_task_receiver_reference_frame_buffer( dlr_receiver_ctrl_t * const p_ctrl, uint8_t const ** const pp_frame_buffer );
usr_err_t um_dlr_task_receiver_unlock_frame_buffer( dlr_receiver_ctrl_t * const p_ctrl );
usr_err_t um_dlr_task_receiver_inform_neighbor_check_request_sequence_id( dlr_receiver_ctrl_t * const p_ctrl, dlr_port_t port, uint32_t sequence_id );

/** DLR state machine executor APIs */
usr_err_t um_dlr_task_executor_open( dlr_executor_ctrl_t * const p_ctrl,
									 dlr_fsm_ctrl_t * const p_fsm_ctrl );
usr_err_t um_dlr_task_executor_start( dlr_executor_ctrl_t * const p_ctrl );
usr_err_t um_dlr_task_executor_stop( dlr_executor_ctrl_t * const p_ctrl );
usr_err_t um_dlr_task_executor_receive_event( dlr_executor_ctrl_t * const p_ctrl,
											   dlr_event_arg_t * const p_dlr_event,
											   dlr_context_type_t context_type,
											   dlr_enqueue_type_t enqueue_type );

/** Ethernet network interface APIs */
usr_err_t um_dlr_ether_netif_open( dlr_ether_netif_ctrl_t * const p_ctrl,
								   dlr_receiver_ctrl_t * const p_receiver_ctrl,
								   ether_netif_instance_t const * const p_ether_netif_instance );
usr_err_t um_dlr_ether_netif_start( dlr_ether_netif_ctrl_t * const p_ctrl );
usr_err_t um_dlr_ether_netif_stop( dlr_ether_netif_ctrl_t * const p_ctrl );
usr_err_t um_dlr_ether_netif_send( dlr_ether_netif_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame_buffer );
usr_err_t um_dlr_ether_netif_get_local_mac_address( dlr_ether_netif_ctrl_t * const p_ctrl, uint8_t * const p_mac_address );

/** Ethernet swtich interface APIs */
usr_err_t um_dlr_ether_switch_open( dlr_ether_switch_ctrl_t * const p_ctrl,
									dlr_ether_netif_ctrl_t * const p_ether_netif_ctrl,
									dlr_executor_ctrl_t * const p_executor_ctrl,
									ethsw_instance_t const * const p_ether_switch_instance );
usr_err_t um_dlr_ether_switch_init_dlr( dlr_ether_switch_ctrl_t * const p_ctrl );
usr_err_t um_dlr_ether_switch_shutdown_dlr( dlr_ether_switch_ctrl_t * const p_ctrl );
usr_err_t um_dlr_ether_switch_get_node_state( dlr_ether_switch_ctrl_t * const p_ctrl,
											  dlr_node_state_t * p_node_state );
usr_err_t um_dlr_ether_switch_flush_unicast_mac_table( dlr_ether_switch_ctrl_t * const p_ctrl );
usr_err_t um_dlr_ether_switch_flush_multicast_mac_table( dlr_ether_switch_ctrl_t * const p_ctrl );
usr_err_t um_dlr_ether_switch_get_beacon_info( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_rx_beacon_t * const p_beacon_info );
usr_err_t um_dlr_ether_switch_get_last_beacon_receive_port( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t * const p_last_beacon_receive_port );
usr_err_t um_dlr_ether_switch_stop_forwarding_frames( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t port );
usr_err_t um_dlr_ether_switch_start_forwarding_frames( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t port );
usr_err_t um_dlr_ether_switch_get_link_state( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t * const p_dlr_port );

/** DLR finite state machine */
usr_err_t um_dlr_fsm_open( dlr_fsm_ctrl_t    		   * const p_ctrl,
		   	   	   	   	   dlr_receiver_ctrl_t         * const p_receiver_ctrl,
						   dlr_ether_netif_ctrl_t      * const p_ether_netif_ctrl,
						   dlr_ether_switch_ctrl_t     * const p_ether_switch_ctrl,
						   dlr_neighbor_checker_ctrl_t * const p_neighbor_cheker_ctrl,
						   uint32_t const	     	   * const p_ip_adress );
usr_err_t um_dlr_fsm_execute_with_event( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t * const p_dlr_event );

#endif /** UM_DLR_INTERNAL_H */
