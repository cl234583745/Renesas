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

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DLR_PORT0_ETHER_PORT_BIT	( (uint32_t) ((1) << (UM_DLR_CFG_PORT0_ETHER_PORT_NUMBER)) )
#define DLR_PORT1_ETHER_PORT_BIT	( (uint32_t) ((1) << (UM_DLR_CFG_PORT1_ETHER_PORT_NUMBER)) )
#define DLR_PORT_ETHER_PORT_MASK	( DLR_PORT0_ETHER_PORT_BIT | DLR_PORT1_ETHER_PORT_BIT )

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void _ether_netif_callback( ether_netif_callback_args_t * p_arg );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_netif_open( dlr_ether_netif_ctrl_t * const p_ctrl,
								   dlr_receiver_ctrl_t * const p_receiver_ctrl,
								   ether_netif_instance_t const * const p_target )
{
	usr_err_t usr_err;

    /** Set related module controls. */
    p_ctrl->p_target = p_target;
    p_ctrl->p_receiver_ctrl = p_receiver_ctrl;

    /** Open Ethernet network interface module */
    usr_err = p_ctrl->p_target->p_api->open( p_ctrl->p_target->p_ctrl, p_ctrl->p_target->p_cfg );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err || USR_ERR_ALREADY_OPEN == usr_err || USR_ERR_ALREADY_RUNNING == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Set callback to Ethernet interface module. */
    p_ctrl->callback_node.p_func = _ether_netif_callback;
    p_ctrl->callback_node.p_context = p_ctrl;
    p_ctrl->callback_node.p_memory = &(p_ctrl->callback_memory);
    usr_err = p_ctrl->p_target->p_api->callbackAdd( (ether_netif_ctrl_t * const) p_ctrl->p_target->p_ctrl, &p_ctrl->callback_node );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Return FSP error of lower module. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start target module.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_netif_start( dlr_ether_netif_ctrl_t * const p_ctrl )
{
	/** Error codes */
	usr_err_t usr_err;

	/** Start Ethernet Link module. */
	usr_err = p_ctrl->p_target->p_api->start(p_ctrl->p_target->p_ctrl);
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop target module.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_netif_stop( dlr_ether_netif_ctrl_t * const p_ctrl )
{
	/** Error codes */
	usr_err_t usr_err;

	/** Start Ethernet Link module. */
	usr_err = p_ctrl->p_target->p_api->stop(p_ctrl->p_target->p_ctrl);
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

	return USR_SUCCESS;
}


/*******************************************************************************************************************//**
 * @brief Send the Ethernet network interface module to send Ethernet frame.
 *
 * @param[in] p_ctrl			Pointer to reader controller
 * @param[in] p_packet_buffer	Pointer to packet buffer.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_netif_send( dlr_ether_netif_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame_packet )
{
	usr_err_t usr_err;

	/** Port conversion */
	switch( p_frame_packet->port )
	{
	case DLR_PORT0_BIT: p_frame_packet->port = DLR_PORT0_ETHER_PORT_BIT; break;
	case DLR_PORT1_BIT: p_frame_packet->port = DLR_PORT1_ETHER_PORT_BIT; break;
    case DLR_PORT_BOTH: p_frame_packet->port = DLR_PORT1_ETHER_PORT_BIT | DLR_PORT0_ETHER_PORT_BIT ; break;
	default:
		return USR_ERR_ABORTED;
	}

	usr_err = p_ctrl->p_target->p_api->send( p_ctrl->p_target->p_ctrl, p_frame_packet );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get the device host mac address
 *
 * @param[in]  p_ctrl			Pointer to reader controller
 * @param[out] p_mac_address	Pointer to MAC address array pointer to be set the host MAC address
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_netif_get_local_mac_address( dlr_ether_netif_ctrl_t * const p_ctrl, uint8_t * const p_mac_address )
{
	/** Get Mac address configuration */
	memcpy( p_mac_address, p_ctrl->p_target->p_cfg->p_ether_instance->p_cfg->p_mac_address, sizeof(uint8_t) * DLR_MAC_ADDRESS_LENGTH);

	return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Callback function of ethernet interface
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ASSERTION
 **********************************************************************************************************************/
static void _ether_netif_callback( ether_netif_callback_args_t * p_arg )
{
	/** Error codes */
	usr_err_t usr_err;

	/** Resolve context */
	dlr_ether_netif_ctrl_t * p_ctrl = ( dlr_ether_netif_ctrl_t * ) p_arg->p_context;

	/** Check the event. */
	switch ( p_arg->event )
	{
	/**
	 * The event Ethernet frame is received.
	 */
	case ETHER_NETIF_CALLBACK_EVENT_RECEIVE_ETHER_FRAME:

		/** Port information conversion. */
		switch( p_arg->p_frame_packet->port )
		{
		case DLR_PORT0_ETHER_PORT_BIT:	p_arg->p_frame_packet->port = DLR_PORT0_BIT; break;
		case DLR_PORT1_ETHER_PORT_BIT:	p_arg->p_frame_packet->port = DLR_PORT1_BIT; break;
		case DLR_PORT_ETHER_PORT_MASK:	p_arg->p_frame_packet->port = DLR_PORT_BOTH; break;
		default:
	        USR_HEAP_RELEASE( p_arg->p_frame_packet );
			return; /** TODO: Error handling. */
		}

		usr_err = um_dlr_task_receiver_take_frame( p_ctrl->p_receiver_ctrl, p_arg->p_frame_packet );
		if( usr_err != USR_SUCCESS )
		{
			return; /** TODO: Error handling. */
		}
		break;

	/**
	 * Other events.
	 */
	case ETHER_NETIF_CALLBACK_EVENT_ANY_PORT_LINK_STATUS_CHANGED:
		/** Can write the dynamic control according to link state. */
		break;
	}
}
