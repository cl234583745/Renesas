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

#ifndef UM_ETHER_NETIF_INTERNAL_H
#define UM_ETHER_NETIF_INTERNAL_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_ether_netif.h"
#include "um_ether_netif_api.h"

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
 * Function prototypes
 **********************************************************************************************************************/
/** Callback control functions */
usr_err_t um_ether_netif_callback_open( ether_netif_callback_ctrl_t * const p_ctrl );
usr_err_t um_ether_netif_callback_add( ether_netif_callback_ctrl_t * const p_ctrl, ether_netif_callback_link_node_t * const p_node );
usr_err_t um_ether_netif_callback_request( ether_netif_callback_ctrl_t * const p_ctrl,
                                           ether_netif_callback_event_t event,
                                           uint32_t port_link_status,
                                           ether_netif_frame_t * p_frame_packet );

/** Target driver control functions */
usr_err_t um_ether_netif_ether_open(ether_netif_ether_ctrl_t * const p_ctrl,
                                    ether_instance_t const * const p_ether_instance,
                                    ether_netif_reader_ctrl_t * const p_reader_ctrl);
usr_err_t um_ether_netif_ether_read(ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame );
usr_err_t um_ether_netif_ether_write(ether_netif_ether_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_frame);
usr_err_t um_ether_netif_ether_monitor (ether_netif_ether_ctrl_t * const p_ctrl, uint32_t * p_link_status );
usr_err_t um_ether_netif_ether_set_callback_context(ether_netif_ether_ctrl_t * const p_ctrl, void * const p_callback_context );

/** Monitor task control functions */
usr_err_t um_ether_netif_task_monitor_open( ether_netif_monitor_ctrl_t * const p_ctrl,
	      	  	  	  	  	  	  	  	   ether_netif_ether_ctrl_t * const p_target_ctrl,
										   ether_netif_callback_ctrl_t * const p_callback_ctrl );
usr_err_t um_ether_netif_task_monitor_start( ether_netif_monitor_ctrl_t * const p_ctrl );
usr_err_t um_ether_netif_task_monitor_stop( ether_netif_monitor_ctrl_t * const p_ctrl );
usr_err_t um_ether_netif_task_monitor_get_link_status ( ether_netif_monitor_ctrl_t * const p_ctrl, uint32_t * p_link_status);

/** Reader task control functions */
usr_err_t um_ether_netif_task_reader_open( ether_netif_reader_ctrl_t * const p_ctrl,
									       ether_netif_ether_ctrl_t * const p_target_ctrl,
										   ether_netif_callback_ctrl_t * const p_callback_ctrl );
usr_err_t um_ether_netif_task_reader_start( ether_netif_reader_ctrl_t * const p_instance_ctrl );
usr_err_t um_ether_netif_task_reader_stop( ether_netif_reader_ctrl_t * const p_instance_ctrl );
usr_err_t um_ether_netif_task_reader_get_task_handle( ether_netif_reader_ctrl_t * const p_ctrl, TaskHandle_t * pp_task_handle );

/** Writer task control functions */
usr_err_t um_ether_netif_task_writer_open( ether_netif_writer_ctrl_t * const p_ctrl,
										   ether_netif_ether_ctrl_t * const p_target_ctrl );
usr_err_t um_ether_netif_task_writer_start( ether_netif_writer_ctrl_t * const p_ctrl );
usr_err_t um_ether_netif_task_writer_stop( ether_netif_writer_ctrl_t * const p_ctrl );
usr_err_t um_ether_netif_task_writer_request( ether_netif_writer_ctrl_t * const p_ctrl, ether_netif_frame_t * p_packet_buffer );

#endif /** UM_ETHER_NETIF_INTERNAL_H */
