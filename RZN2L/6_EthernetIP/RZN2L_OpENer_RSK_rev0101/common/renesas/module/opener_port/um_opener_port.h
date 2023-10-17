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

#ifndef UM_OPENER_PORT_H
#define UM_OPENER_PORT_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** API definitions */
#include "um_opener_port_api.h"
#include "um_opener_port_feature.h"
#include "um_common_api.h"
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
#include "um_dlr_api.h"
#endif

/** FreeRTOS related definitions. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "lwip/sockets.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define OPENER_PORT_NET_STATE_NUM                 (6)       ///< The number of network state

/*******************************************************************************************************************//**
 * @addtogroup OPENER_PORT
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * Vol.2 1.27 9-4.3 Network Status Indicator
 * Each network status indicator shall be labeled with one of the followings, either as shown or in all capital letters:
 * "NS", "Net", "Net State", "Network Status".
 */
typedef enum
{
    OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS, /* The device is powered off, or is powered on but with no IP address configured. *////< OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS
    OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS,             /* An IP address is configured, but no CIP connections are established,             ///< OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS
                                                           * and Exclusive Owner connection has not timed out. */
    OPENER_PORT_NETWORK_STATE_CONNECTED,                  /* At least one CIP connection (any transport class) is established. */             ///< OPENER_PORT_NETWORK_STATE_CONNECTED
    OPENER_PORT_NETWORK_STATE_CONECTION_TIMEOUT,          /* Exclusive Owner connection for which this device is the target has timed out. */ ///< OPENER_PORT_NETWORK_STATE_CONECTION_TIMEOUT
    OPENER_PORT_NETWORK_STATE_DUPLICATE_IP,               /* For devices that support duplicate IP address detection,                         ///< OPENER_PORT_NETWORK_STATE_DUPLICATE_IP
                                                           * the device has detected that (at least one of) its IP address is in use. */
    OPENER_PORT_NETWORK_STATE_SELF_TEST,                                                                                                      ///< OPENER_PORT_NETWORK_STATE_SELF_TEST
} opener_port_network_state_t ;

/**
 * Struct for the callback control to exclusive control.
 */
typedef struct st_opener_port_callback_ctrl
{
    /** Private parameters. */
    opener_port_callback_link_node_t *  p_head_node;    ///< Pointer to head of callback list.
    opener_port_callback_link_node_t *  p_tail_node;    ///< Pointer to tail of callback list.

    /** FreeRTOS resource */
    SemaphoreHandle_t                   p_mutex_handle; ///< Pointer to mutex for controlling exclusion of callback resource.

} opener_port_callback_ctrl_t;

/**
 * Controller of network interface for Ethernet/IP network.
 */
typedef struct st_opener_port_netif_ctrl
{
    /** Private parameters. */
   int32_t explicit_tcp_socket;                 ///< TCP socket for handling EtherNet/IP explicit messages.
   int32_t explicit_udp_socket;                 ///< UDP socket for handling EtherNet/IP explicit messages.

   /** Platform resources */
   TaskHandle_t p_task_handle;              ///< Pointer to task.
   TaskHandle_t p_parent_task_handle;       ///< Pointer to parent task.

   /** Internal submodules */
   void * p_lwip_port_ctrl;                     ///< Pointer to controller of lwIP port module.
   void * p_tcpip_netif_ctrl;                   ///< Pointer to controller of TCP/IP socket interface module
   void * p_encap_ctrl;                         ///< Pointer to controller of EtherNet/IP Encapsulation interface module.
   void * p_indicator_ctrl;                     ///< Pointer to controller of EtherNet/IP LED indicator module.

} opener_port_netif_ctrl_t;

/**
 * Controller of socket interface.
 */
typedef struct st_opener_port_tcpip_netif_ctrl
{
    /** Private parameters. */
    int32_t highest_socket;                     ///< Highest socket value.

    /** Platform resources */
    fd_set socket_set;                          ///< A set of socket.
    fd_set readable_socket_set;                 ///< A set of socket readable.

    /** Configuration to initialize. */
    opener_port_cip_settable_nvdata_t const * p_cip_nvdata;    ///< Pointer to CIP configuration.

    /** Internal submodules */
    void * p_lwip_port_ctrl;                    ///< Pointer to controller of lwIP port module.

    /** OpENer CIP object instance */
    void * p_qos_cip_instance;                  ///< Pointer to QoS CIP instance

} opener_port_tcpip_netif_ctrl_t;

/**
 * Controller of the task of status LED indicator
 */
typedef struct st_opener_port_indicator_ctrl
{
    /** Private parameters. */
	opener_port_module_state_t  module_state;    ///< Module state
	opener_port_network_state_t network_state;  ///< Network state

	/** Platform resources */
    TaskHandle_t    p_task_handle;              ///< Pointer to task handle
    QueueHandle_t   p_queue_handle;             ///< Pointer to queue handle receiving the DLR event.
    TaskHandle_t    p_parent_task_handle;       ///< Pointer to parent task handle.

    /** Internal submodules */
	void * p_callback_ctrl;                     ///< Pointer to controller of callback module
	void * p_cip_ctrl;                          ///< Pointer to controller of CIP interface module.

} opener_port_indicator_ctrl_t;

/**
 * Controller of OpENer Cip Stack
 */
typedef struct st_opener_port_cip_ctrl
{
    /** Configuration to initialize. */
	opener_port_cip_user_device_cfg_t const * p_cip_device_cfg;

	/** OpENer CIP object instance */
	void * p_identity_cip_instance;             ///< Pointer to Identity object instance

} opener_port_cip_ctrl_t;

/**
 * Controller of OpENer Encap layer
 */
typedef struct st_opener_port_encap_ctrl
{
    /** Private parameters. */
    uint32_t prev_actual_time_ms;
    uint32_t current_active_peer_address;

    /** OpENer CIP object instance */
    void * p_tcpip_netif_ctrl;                  ///< Pointer to controller of TCP/IP socket interface module

} opener_port_encap_ctrl_t;

#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
typedef struct st_opener_port_extend_cfg
{
    dlr_instance_t const * const p_dlr_instance;
} opener_port_extend_cfg_t;
#endif

/**
 * Controller of DLR module
 */
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
typedef struct st_opener_port_dlr_ctrl
{
	dlr_instance_t const * p_dlr_instance;
} opener_port_dlr_ctrl_t;
#endif

/**
 * Controller of Ethernet interface module.
 */
typedef struct st_opener_port_ether_netif_ctrl
{
    /** Private parameters. */
    ether_netif_instance_t const *   p_ether_netif_instance;        ///< Pointer to Ethernet network interface submodule.
    ether_netif_callback_link_node_t callback_node;                 ///< Callback node for callback of Ethernet interface module.
    ether_netif_callback_args_t      callback_memory;               ///< Callback memory for callback of Ethernet interface module.
    ether_phy_instance_t const * p_ether_phy_instance[BSP_FEATURE_GMAC_MAX_PORTS];  ///< Pointer to PHY instances.

    /** OpENer CIP object instance */
    void * p_ethlink_cip_instance[BSP_FEATURE_GMAC_MAX_PORTS];      ///< Pointer to CIP Ethernet Link object instances.

} opener_port_ether_netif_ctrl_t;

/**
 * Controller of lwIP port TCP/IP Network interface module
 */
typedef struct st_opener_port_lwip_port_ctrl
{
    /** Private parameters. */
    lwip_port_instance_t const *    p_lwip_port_instance;       ///< Pointer to lwIP port TCP/IP network interface submodule.
	lwip_port_callback_link_node_t  callback_node;              ///< Callback node for callback of lwIP port module
	lwip_port_callback_args_t       callback_memory;            ///< Callback memory for callback of lwIP port module

	/** Platform resources */
	TaskHandle_t 				    p_parent_task_handle;       ///< Pointer to parent task for synchronizing with callback.

	/** Configuration to initialize. */
    opener_port_cip_user_device_cfg_t const * p_cip_device_cfg; ///< Pointer to CIP device configuration
    opener_port_cip_settable_nvdata_t const * p_cip_nvdata;     ///< Pointer to CIP settable nvdata configuration.

	/** OpENer CIP object instance */
    void * p_tcpip_cip_instance;                                ///< Pointer to CIP TCP/IP interface object

} opener_port_lwip_port_ctrl_t;

/** Channel control block.
 * DO NOT INITIALIZE.  Initialization occurs when @ref tcpip_api_t::open is called.
 */
typedef struct st_opener_port_instance_ctrl
{
    /** Private parameters. */
    uint32_t          open;           	///< Whether or not the instance is opened.
    uint32_t		  start;			///< Whether or not the tasks is started.

    /** Configuration to initialize. */
    opener_port_cfg_t const * p_cfg;	///< Pointer to configuration structure

    /** Internal modules */
    opener_port_ether_netif_ctrl_t  ether_netif_ctrl;   ///< Controller of Ethernet network interface internal module.
    opener_port_lwip_port_ctrl_t	lwip_port_ctrl;	    ///< Controller of lwIP port interface internal module
    opener_port_cip_ctrl_t          cip_ctrl;			///< Controller of OpENer Cip Interface module
    opener_port_encap_ctrl_t        encap_ctrl;         ///< Controller of OpENer Encap interface module
    opener_port_netif_ctrl_t        netif_ctrl;         ///< Controller of Ethernet/IP network interface.
    opener_port_tcpip_netif_ctrl_t  tcpip_netif_ctrl;   ///< Controller of lwIP TCP/IP socket interface.
    opener_port_indicator_ctrl_t    indicator_ctrl;		///< Controller of task of status LED indicator.
    opener_port_callback_ctrl_t     callback_ctrl;      ///< Controller of callback interface.
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
    opener_port_dlr_ctrl_t          dlr_ctrl;           ///< Controller of DLR module
#endif

} opener_port_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const opener_port_api_t g_opener_port_on_opener_port;

/** @endcond */
usr_err_t UM_OPENER_PORT_Open(opener_port_ctrl_t * const p_ctrl, opener_port_cfg_t const * const p_cfg );
usr_err_t UM_OPENER_PORT_Start(opener_port_ctrl_t * const p_ctrl );
usr_err_t UM_OPENER_PORT_Stop(opener_port_ctrl_t * const p_ctrl );
usr_err_t UM_OPENER_PORT_CallbackAdd (opener_port_ctrl_t * const p_ctrl, opener_port_callback_link_node_t * p_node );
usr_err_t UM_OPENER_PORT_ModuleStateSet (opener_port_ctrl_t * const p_ctrl, opener_port_module_state_t module_state );
usr_err_t UM_OPENER_PORT_SettableNvDataGet (opener_port_ctrl_t * const p_ctrl, opener_port_cip_settable_nvdata_t * const p_nvdata_cfg );

/*******************************************************************************************************************//**
 * @} (end defgroup UM_OPENER_PORT)
 **********************************************************************************************************************/

#endif                                 // UM_OPENER_PORT_H
