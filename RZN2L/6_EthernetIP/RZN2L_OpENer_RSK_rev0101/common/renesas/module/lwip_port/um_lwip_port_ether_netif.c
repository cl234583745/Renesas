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
#include "um_lwip_port_api.h"
#include "um_lwip_port_cfg.h"
#include "um_lwip_port.h"
#include "um_lwip_port_internal.h"
#include "um_ether_netif_api.h"

#include "FreeRTOS.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void _ether_netif_callback( ether_netif_callback_args_t * p_args );

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
 * @retval USR_ERR_ALREADY_OPEN			Specified module has been already opened.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_ether_netif_open( lwip_port_ether_netif_ctrl_t * const p_ctrl,
										 lwip_port_launcher_ctrl_t * p_launcher_ctrl,
										 lwip_port_receiver_ctrl_t * p_reader_ctrl,
										 ether_netif_instance_t const * const p_ether_netif_instance)
{
	/** Error codes */
	usr_err_t usr_err;

    /** Set related module controls. */
    p_ctrl->p_ether_netif_instance = p_ether_netif_instance;
    p_ctrl->p_launcher_ctrl = p_launcher_ctrl;
    p_ctrl->p_receiver_ctrl = p_reader_ctrl;

    /** Try opening Ethernet network interface module */
    usr_err = p_ctrl->p_ether_netif_instance->p_api->open( p_ctrl->p_ether_netif_instance->p_ctrl, p_ctrl->p_ether_netif_instance->p_cfg );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err || USR_ERR_ALREADY_OPEN == usr_err || USR_ERR_ALREADY_RUNNING == usr_err , USR_ERR_NOT_INITIALIZED );

    /** Set callback to Ethernet network interface module. */
    p_ctrl->callback_node.p_func = _ether_netif_callback;
    p_ctrl->callback_node.p_memory = &(p_ctrl->callback_memory);
    p_ctrl->callback_node.p_context = p_ctrl;
    usr_err = p_ctrl->p_ether_netif_instance->p_api->callbackAdd( (ether_netif_ctrl_t *) p_ctrl->p_ether_netif_instance->p_ctrl, &p_ctrl->callback_node );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Clear Ethernet network interface port status */
    p_ctrl->port_link_status = ETHER_NETIF_PORT_NONE;

    /** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the network interface submodule.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_ether_netif_start( lwip_port_ether_netif_ctrl_t * const p_ctrl )
{
	/** link status of all ports */
	uint32_t link_status;

    /** Start Ethernet network interface module. */
    (void) p_ctrl->p_ether_netif_instance->p_api->start(p_ctrl->p_ether_netif_instance->p_ctrl);

    /** Get link state. */
    (void) p_ctrl->p_ether_netif_instance->p_api->linkStatusGet( p_ctrl->p_ether_netif_instance->p_ctrl, &link_status, true );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the network interface submodule.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_ether_netif_stop( lwip_port_ether_netif_ctrl_t * const p_ctrl )
{
	/** Error codes */
	usr_err_t usr_err;

    /** Start Ethernet network interface module. */
    usr_err = p_ctrl->p_ether_netif_instance->p_api->stop(p_ctrl->p_ether_netif_instance->p_ctrl);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS /** Already stopped */ );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Request the Ethernet Link module to send Ethernet frame.
 *
 * @param[in] p_ctrl			Pointer to reader controller
 * @param[in] p_packet_buffer	Pointer to packet buffer.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_ABORTED		Process has been aborted.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_ether_netif_send( lwip_port_ether_netif_ctrl_t * const p_ctrl,
										 ether_netif_frame_t * const p_packet_buffer )
{
	usr_err_t usr_err;

	/** Set broadcast port. */
	p_packet_buffer->port = ETHER_NETIF_PORT_SEND_PORT_ALL;

	/** Send the frame */
	usr_err = p_ctrl->p_ether_netif_instance->p_api->send( p_ctrl->p_ether_netif_instance->p_ctrl, p_packet_buffer );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get the device host local MAC address
 *
 * @param[in]  p_ctrl			Pointer to reader controller
 * @param[out] p_mac_address	Pointer to MAC address array pointer to be set the host MAC address
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_ether_netif_get_local_mac_address( lwip_port_ether_netif_ctrl_t * const p_ctrl, uint8_t * const p_mac_address )
{
	/** Get Mac address configuration */
	memcpy( p_mac_address, p_ctrl->p_ether_netif_instance->p_cfg->p_ether_instance->p_cfg->p_mac_address, sizeof(uint8_t) * ETHER_NETIF_MAC_ADDRESS_BYTES);

	return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * Callback handler to set to Ethernet network interface module
 *
 * @param[in]   p_args     Pointer to ether netif instance callback arguments
 **********************************************************************************************************************/
void _ether_netif_callback( ether_netif_callback_args_t * p_args )
{
    /** Resolve user context */
	lwip_port_ether_netif_ctrl_t * p_ctrl = ( lwip_port_ether_netif_ctrl_t * ) p_args->p_context;

    /** Status */
    usr_err_t usr_err = USR_SUCCESS;

    /** Check event */
    switch( p_args->event )
    {
	case ETHER_NETIF_CALLBACK_EVENT_ANY_PORT_LINK_STATUS_CHANGED:

        /** If all port link down currently (= any port links up ), */
	    if( ETHER_NETIF_PORT_NONE == p_ctrl->port_link_status && p_args->port_link_status )
	    {
            /** Issue callback event with link up.*/
	        usr_err = um_lwip_port_task_launcher_request(p_ctrl->p_launcher_ctrl, LWIP_PORT_LAUNCHER_EVENT_LINK_UP, NULL);
	        if ( !(USR_SUCCESS == usr_err || USR_ERR_NOT_ENABLED == usr_err) )
	        {
	            /** TODO: Implement error handling in callback context. */
	        }
	    }
        /** If all port become link down, */
	    else if( ETHER_NETIF_PORT_NONE == p_args->port_link_status && p_ctrl->port_link_status )
        {
            /** Issue callback event with link down.*/
            usr_err = um_lwip_port_task_launcher_request(p_ctrl->p_launcher_ctrl, LWIP_PORT_LAUNCHER_EVENT_LINK_DOWN, NULL);
            if ( !(USR_SUCCESS == usr_err || USR_ERR_NOT_ENABLED == usr_err) )
            {
                /** TODO: Implement error handling in callback context. */
            }
        }

        /** Update current link status */
        p_ctrl->port_link_status = p_args->port_link_status;

		break;

	case ETHER_NETIF_CALLBACK_EVENT_RECEIVE_ETHER_FRAME:

		usr_err = um_lwip_port_task_receiver_request(p_ctrl->p_receiver_ctrl, p_args->p_frame_packet);
        if ( !(USR_SUCCESS == usr_err || USR_ERR_NOT_ENABLED == usr_err) )
		{
			/** TODO: Implement error handling in callback context. */
		}
		break;
    }
}
