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

#ifndef UM_LWIP_PORT_INTERNAL_H
#define UM_LWIP_PORT_INTERNAL_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_lwip_port.h"
#include "um_lwip_port_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/**
 * Enumeration for the state indicating whether netif link is up or down
 */
typedef enum e_lwip_port_launcher_event
{
    LWIP_PORT_LAUNCHER_EVENT_LINK_UP,     ///< Event when link is up
    LWIP_PORT_LAUNCHER_EVENT_LINK_DOWN,   ///< Event when link is down
    LWIP_PORT_LAUNCHER_EVENT_IP_UP,       ///< Event when IP is up
    LWIP_PORT_LAUNCHER_EVENT_IP_DOWN,     ///< Event when IP is down
} lwip_port_launcher_event_t;

typedef struct st_lwip_port_launcher_request
{
    lwip_port_launcher_event_t event;
    lwip_port_netif_ctrl_t * p_netif_ctrl;
} lwip_port_launcher_request_t;

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Function prototypes
 **********************************************************************************************************************/
/** lwIP interface */
usr_err_t um_lwip_port_core_open( lwip_port_core_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_core_task_sync( void );

/** lwIP network interface */
usr_err_t um_lwip_port_netif_open( lwip_port_netif_ctrl_t * const p_ctrl,
                                   lwip_port_netif_cfg_t const * const p_cfg,
                                   lwip_port_ether_netif_ctrl_t * const p_ether_netif_ctrl,
                                   lwip_port_callback_ctrl_t * const p_callback_ctrl,
                                   lwip_port_launcher_ctrl_t * const p_launcher_ctrl );
usr_err_t um_lwip_port_netif_input( lwip_port_netif_ctrl_t * const p_ctrl, ether_netif_frame_t * p_frame_packet );
usr_err_t um_lwip_port_netif_set_default( lwip_port_netif_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_netif_set_up( lwip_port_netif_ctrl_t *const p_ctrl );
usr_err_t um_lwip_port_netif_set_down( lwip_port_netif_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_netif_get_netif_config( lwip_port_netif_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * p_netif_cfg );
usr_err_t um_lwip_port_netif_set_netif_state( lwip_port_netif_ctrl_t * const p_ctrl, lwip_port_netif_state_t state );
usr_err_t um_lwip_port_netif_get_netif_state( lwip_port_netif_ctrl_t * const p_ctrl, lwip_port_netif_state_t * p_netif_state );

/** list interface of lwIP network interface */
usr_err_t um_lwip_port_netif_list_open( lwip_port_netif_list_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_netif_list_add( lwip_port_netif_list_ctrl_t * const p_ctrl, lwip_port_netif_ctrl_t * const p_netif_ctrl );
usr_err_t um_lwip_port_netif_list_lock( lwip_port_netif_list_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_netif_list_unlock( lwip_port_netif_list_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_netif_list_map( lwip_port_netif_list_ctrl_t * const p_ctrl,
									   usr_err_t (* p_callback)( lwip_port_netif_ctrl_t * p_netif_ctrl, void * p_context), void * p_context );

/** Ethernet Link interface */
usr_err_t um_lwip_port_ether_netif_open( lwip_port_ether_netif_ctrl_t * const p_ctrl,
										 lwip_port_launcher_ctrl_t * p_launcher_ctrl,
										 lwip_port_receiver_ctrl_t * p_reader_ctrl,
										 ether_netif_instance_t const * const p_ether_netif_instance);
usr_err_t um_lwip_port_ether_netif_start( lwip_port_ether_netif_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_ether_netif_stop( lwip_port_ether_netif_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_ether_netif_send( lwip_port_ether_netif_ctrl_t * const p_ctrl,
											ether_netif_frame_t * const p_packet_buffer );
usr_err_t um_lwip_port_ether_netif_get_local_mac_address( lwip_port_ether_netif_ctrl_t * const p_ctrl,
														  uint8_t * const p_mac_address );

/** Callback interface */
usr_err_t um_lwip_port_callback_open( lwip_port_callback_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_callback_add( lwip_port_callback_ctrl_t * const p_ctrl, lwip_port_callback_link_node_t * p_node );
usr_err_t um_lwip_port_callback_request( lwip_port_callback_ctrl_t * const p_ctrl,
                                         lwip_port_callback_event_t event );

/** Launcher task interface */
usr_err_t um_lwip_port_task_launcher_open( lwip_port_launcher_ctrl_t * const p_ctrl,
										   lwip_port_netif_list_ctrl_t * const p_netif_list_ctrl );
usr_err_t um_lwip_port_task_launcher_start( lwip_port_launcher_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_task_launcher_stop( lwip_port_launcher_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_task_launcher_request( lwip_port_launcher_ctrl_t * const p_ctrl,
                                              lwip_port_launcher_event_t const event,
                                              lwip_port_netif_ctrl_t * const p_netif_ctrl);

/** Reader task interface */
usr_err_t um_lwip_port_task_receiver_open( lwip_port_receiver_ctrl_t * const p_ctrl,
										   lwip_port_netif_list_ctrl_t * const p_instance_manager_ctrl );
usr_err_t um_lwip_port_task_receiver_start( lwip_port_receiver_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_task_receiver_stop( lwip_port_receiver_ctrl_t * const p_ctrl );
usr_err_t um_lwip_port_task_receiver_request( lwip_port_receiver_ctrl_t * const p_ctrl, ether_netif_frame_t * p_packet_buffer );

#endif /** UM_LWIP_PORT_INTERNAL_H */
