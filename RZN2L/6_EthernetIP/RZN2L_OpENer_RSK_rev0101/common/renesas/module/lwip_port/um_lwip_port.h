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

#ifndef UM_LWIP_PORT_H
#define UM_LWIP_PORT_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Interface Header */
#include "um_lwip_port_api.h"

/** Configuration Header */
#include "um_lwip_port_cfg.h"

/** lwIP Modules */
#include "lwip/tcpip.h"
#include "lwip/ip4_addr.h"

/** FreeRTOS modules */
#include "FreeRTOS.h"
#include "event_groups.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup UM_LWIP_PORT
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * Struct for the callback control to exclusive control.
 */
typedef struct st_lwip_port_callback_ctrl
{
	SemaphoreHandle_t  			        p_mutex_handle;	///< Pointer to mutex for controlling exclusion of callback resource.
    lwip_port_callback_link_node_t *  p_head_node;
    lwip_port_callback_link_node_t *  p_tail_node;
} lwip_port_callback_ctrl_t;

/**
 * Struct for the lwIP Core control.
 */
typedef struct st_lwip_port_core_ctrl
{
    TaskHandle_t		p_tcpip_task_handle;     		///< Pointer to lwIP TCPIP task handler
    TaskHandle_t		p_parent_task_handle;			///< Pointer to parent task handler of lwIP TCPIP task.
} lwip_port_core_ctrl_t;

/**
 * Struct for the lwIP network interface
 */
typedef struct st_lwip_port_netif_ctrl
{
    struct st_lwip_port_netif_ctrl * p_next;     			///< Pointer to next lwIP network interface to be managed by network interface list.
    struct netif  		 			 netif; 				///< lwIP network interface structure.
    lwip_port_netif_cfg_t const    * p_netif_cfg; 			///< Pointer to netif configuration.
    lwip_port_netif_state_t			 netif_state;			///< State to indicate netif is up or not.
    ether_netif_frame_t *    	   	 p_ether_netif_frame;	///< Ethernet frame packet buffer for getting it from queue.
    void * 						     p_callback_ctrl;		///< Pointer to Callback interface control.
    void * 							 p_ether_netif_ctrl;	///< Pointer to Ethernet network interface control.
    void *                           p_launcher_ctrl;       ///< Pointer to Launcher task control.
} lwip_port_netif_ctrl_t;

/**
 * Struct for the lwIP network interface control
 */
typedef struct st_lwip_port_netif_list_ctrl
{
	SemaphoreHandle_t        p_mutex_handle;	///< Pointer to mutex for controlling exclusion of the list of network controllers.
	lwip_port_netif_ctrl_t * p_head_ctrl;      ///< Pointer to first instance
	lwip_port_netif_ctrl_t * p_tail_ctrl;      ///< Pointer to last instance
} lwip_port_netif_list_ctrl_t;

/**
 * Struct for the task control for launching lwIP network interface.
 */
typedef struct st_lwip_port_launcher_ctrl
{
	TaskHandle_t 	p_task_handle;		///< Pointer to task handle for launching network interface
    TaskHandle_t    p_parent_task_handle;
	QueueHandle_t	p_queue_handle;		///< Pointer to queue handle for notifying that the Ethernet network interface is up.
    void * 			p_netif_list_ctrl;	///< Pointer to network interface list control to be switched to up/down.
} lwip_port_launcher_ctrl_t;

/**
 * Struct for the task control for receiving Ethernet frame from Ethernet network interface and passing it into lwIP network interface.
 */
typedef struct st_lwip_port_receiver_ctrl
{
	TaskHandle_t 	      p_task_handle;			///< Pointer to task handle for passing Ethernet frame from Ethernet network interface into lwIP network interface.
	TaskHandle_t          p_parent_task_handle;
    QueueHandle_t	      p_queue_handle;			///< Pointer to queue handle for passing Ethernet frame from Ethernet network interface into the task.
	ether_netif_frame_t * p_ether_netif_frame;			///< Ethernet frame packet buffer for getting it from queue.
    void * 				  p_netif_list_ctrl;		///< Pointer to network interface list controller to be passed received Ethernet frames.
} lwip_port_receiver_ctrl_t;

/**
 * Struct for the control for utilizing Ethernet network interface APIs.
 */
typedef struct st_lwip_port_ether_netif_ctrl
{
	ether_netif_instance_t const * p_ether_netif_instance;	///< Pointer to Ethernet network interface instance.
	ether_netif_callback_args_t    callback_memory; 		///< Callback memory returned as argument when the callback Ethernet network interface is called.
	ether_netif_callback_link_node_t    callback_node;	    ///< Callback node for utilizing callback of Ethernet network interface.
    void * 						   p_launcher_ctrl;			///< Pointer to launcher task control to be notified that Ethernet network interface is up.
    void * 						   p_receiver_ctrl;			///< Pointer to receiver task control to be passed Ethernet frame from Ethernet network interface.
    uint32_t                       port_link_status;  ///< Ethernet port link status.
} lwip_port_ether_netif_ctrl_t;

/**
 * Structure for common configuration of all instances.
 */
typedef struct st_lwip_port_common_cfg
{
    const ether_netif_instance_t          * p_ether_netif_instance;   ///< Pointer to um_ether_netif instance.
    const struct st_lwip_port_common_ctrl * p_common_ctrl;            ///< Pointer to common controller over multiple instance.
} lwip_port_common_cfg_t;

/**
 * Structure for common control block
 */
typedef struct st_lwip_port_common_ctrl
{
    uint32_t 							open;  				///< State of whether or not the common control is open.
    const lwip_port_common_cfg_t *  	p_common_cfg;		///< Pointer to common configuration
    lwip_port_core_ctrl_t 				core_ctrl;
    lwip_port_ether_netif_ctrl_t    	ether_netif_ctrl;
    lwip_port_netif_list_ctrl_t			netif_list_ctrl;
    lwip_port_launcher_ctrl_t			launcher_ctrl;
    lwip_port_receiver_ctrl_t			receiver_ctrl;
} lwip_port_common_ctrl_t;

/** Channel control block.
 * DO NOT INITIALIZE.  Initialization occurs when @ref tcpip_api_t::open is called.
 */
typedef struct st_lwip_port_instance_ctrl
{
    uint32_t                	open;           	///< Whether or not the instance is open
    uint32_t					start;				///< Whether or not the tasks is started.
    const lwip_port_cfg_t *     p_cfg;          	///< Pointer to initial configurations
    lwip_port_common_ctrl_t *   p_common_ctrl;  	///< Pointer to Common controller between instances
    lwip_port_netif_ctrl_t 		netif_ctrl;			///< Pointer to LwIP Netif controller
    lwip_port_callback_ctrl_t 	callback_ctrl;		///< Pointer to callback controller
} lwip_port_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const lwip_port_api_t g_lwip_port_on_lwip_port;

/** @endcond */
usr_err_t UM_LWIP_PORT_Open(lwip_port_ctrl_t * const p_ctrl, lwip_port_cfg_t const * const p_cfg );
usr_err_t UM_LWIP_PORT_Start(lwip_port_ctrl_t * const p_ctrl);
usr_err_t UM_LWIP_PORT_Stop(lwip_port_ctrl_t * const p_ctrl);
usr_err_t UM_LWIP_PORT_CallbackAdd (lwip_port_ctrl_t * const p_ctrl, lwip_port_callback_link_node_t * p_node );
usr_err_t UM_LWIP_PORT_NetifConfigGet( lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * p_netif_cfg );
usr_err_t UM_LWIP_PORT_NetifStateGet( lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_state_t * p_netif_state, uint8_t notify_callback );

/*******************************************************************************************************************//**
 * @} (end defgroup UM_LWIP_PORT)
 **********************************************************************************************************************/

#endif                                 // UM_LWIP_PORT_H
