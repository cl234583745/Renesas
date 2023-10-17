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

#ifndef UM_DLR_H
#define UM_DLR_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** API definitions */
#include "um_dlr_api.h"

/** FreeRTOS related definitions. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/** Ethernet switch APIs definitions */
#include "r_ethsw_extend_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @addtogroup DLR
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** DLR Network Topology, Attribute #1 */
typedef enum e_dlr_network_topology
{
    DLR_NETWORK_TOPOLOGY_LINEAR = 0x00, ///< 0x00: LINEAR TOPOLOGY
    DLR_NETWORK_TOPOLOGY_RING   = 0x01, ///< 0x01: RING TOPOLOGY
} dlr_network_topology_t;

/** DLR Network Status, Attribute #2 */
typedef enum e_dlr_network_status
{
    DLR_NETWORK_STATUS_NORMAL                    = 0x00, ///< 0x00: Normal
    DLR_NETWORK_STATUS_RING_FAULT                = 0x01, ///< 0x01: Ring Fault
    DLR_NETWORK_STATUS_UNEXPECTED_LOOP_DETECTED  = 0x02, ///< 0x02: Unexpected Loop Detected
    DLR_NETWORK_STATUS_PARTIAL_NETWORK_FAULT     = 0x03, ///< 0x03: Partial Network Fault
    DLR_NETWORK_STATUS_RAPID_FAULT_RESTORE_CYCLE = 0x04  ///< 0x04: Rapid Fault/Restore Cycle
} dlr_network_status_t;

/** internal state of a DLR node */
typedef enum e_dlr_node_state
{
    DLR_NODE_STATE_IDLE   = 0x00, ///< 0x00: IDLE_STATE
    DLR_NODE_STATE_NORMAL = 0x01, ///< 0x01: NORMAL_STATE
    DLR_NODE_STATE_FAULT  = 0x02  ///< 0x02: FAULT_STATE
} dlr_node_state_t;

/** network state */
typedef enum e_dlr_ring_state
{
    DLR_RING_STATE_NORMAL = 0x01, ///< Normal state
    DLR_RING_STATE_FAULT  = 0x02, ///< Fault state
} dlr_ring_state_t;

typedef enum e_dlr_port
{
	DLR_PORT_NONE = 0b0000,
	DLR_PORT0_BIT = 0b0001,
	DLR_PORT1_BIT = 0b0010,
	DLR_PORT_BOTH = 0b0011,
} dlr_port_t;

typedef enum e_dlr_bool
{
	DLR_FALSE	= 0x00,
	DLR_TRUE	= 0x01,
} dlr_bool_t;

/** Beacon frame information */
typedef struct st_dlr_beacon_frame_info
{
	/** In common frame header format */
    uint8_t  source_mac_address[6]; 	///< Field: Source MAC address (of active supervisor)
    uint16_t vlan_id;        			///< Field: 802.1Q Tag control (from which 0xE000 subtracted)

    /** In beacon frame format */
    uint32_t source_ip_address;     	///< Field: Source IP address (of active supervisor)
    dlr_ring_state_t ring_state; 		///< Field: Ring State
    uint8_t  supervisor_precedence;    	///< Field: Supervisor supervisor_precedence (of active supervisor)

    /** last beacon receive port */
    dlr_port_t last_beacon_receive_port;   ///< Not filed: Last beacon receive port
} dlr_rx_beacon_t;

/** DLR event */
typedef struct st_dlr_event_arg
{
	ethsw_dlr_event_t        event;
	dlr_port_t				 event_port;
	dlr_rx_beacon_t	 		 freezed_rx_beacon_info;
	dlr_node_state_t		 freezed_node_state;
	uint8_t					 freezed_timeout_count;
	ether_netif_frame_t *    p_ether_frame;
} dlr_event_arg_t;

/**
 * Control block for DLR state machine.
 */
typedef struct st_dlr_state_machine_ctrl
{
	/** DLR ring node state. */
    dlr_node_state_t node_state; ///< Local node state

    /** Received Beacon information. */
    dlr_rx_beacon_t rx_beacon_info; ///< Beacon information (including ring supervisor and network status)

    dlr_port_t link_active_port;
    dlr_port_t neighbor_active_port;

	/** Buffer for sending Frame. */
	ether_netif_frame_t * p_ether_netif_frame;		///< Pointer to Ethernet frame buffer to be sent.

	/** Tx frame sequence id. */
    uint32_t sequence_id;

    /** Own IP address */
	uint32_t const * p_ip_address;		 		///< Pointer to IP address information.

	/** Submodule dependencies. */
	void * p_ether_switch_ctrl;		///< Pointer to controller of Ethernet switch interface.
	void * p_ether_netif_ctrl; 		///< Pointer to controller of DLR Ethernet network interface.
	void * p_neighbor_checker_ctrl;	///< Pointer to controller of neighbor check timer.
	void * p_receiver_ctrl;			///< Pointer to controller of DLR frame receiver task.

} dlr_fsm_ctrl_t;

/**
 * Control block for Ethernet switch interface
 */
typedef struct st_dlr_ether_switch_ctrl
{
	/** Target module instance. */
	ethsw_instance_t const * p_target;

	/** Target module configurations */
	ethsw_dlr_init_t dlr_init_cfg;
	ethsw_mac_addr_t dlr_host_mac_addr;

	/** For exclusive control */
	SemaphoreHandle_t p_mutex_handle;

	/** Submodule dependencies. */
	void * p_executor_ctrl;
	void * p_ether_netif_ctrl;

} dlr_ether_switch_ctrl_t;

/**
 * Control block for DLR state machine executor
 */
typedef struct st_dlr_receiver_ctrl
{
	TaskHandle_t 		  p_task_handle;		 ///< Pointer to task handle of DLR frame receiver task.
    QueueHandle_t 		  p_queue_handle;		 ///< Pointer to queue handle receiving the Ethernet frame.
	TaskHandle_t 		  p_parent_task_handle;	 ///< Pointer to parent task handle.

    ether_netif_frame_t * p_ether_frame;		 ///< Pointer to buffer receiving the Ethernet frame from queue.
    SemaphoreHandle_t     p_ether_frame_mutex;  ///< Pointer to mutex to lock the frame buffer.

    uint32_t	last_neighbor_check_request_sequence_id0; 	 ///< Sequence ID for check that the sequence id of response matches the one of request on port0.
    uint32_t	last_neighbor_check_request_sequence_id1; 	 ///< Sequence ID for check that the sequence id of response matches the one of request on port1.

	/** Submodule dependencies. */
    void * p_fsm_ctrl; 					///< Pointer to controller for DLR state machine.
    void * p_executor_ctrl;				///< Pointer to controller for DLR state machine executor

} dlr_receiver_ctrl_t;

/**
 * Control block for DLR state machine executor
 */
typedef struct st_dlr_executor_ctrl
{
    TaskHandle_t		  p_task_handle;	///< Pointer to task handle of DLR event executor task
    QueueHandle_t		  p_queue_handle;	///< Pointer to queue handle receiving the DLR event.
	TaskHandle_t 		  p_parent_task_handle;	///< Pointer to parent task handle.

	/** Submodule dependencies. */
    void * p_fsm_ctrl; ///< Pointer to controller for DLR state machine.

} dlr_executor_ctrl_t;

/**
 * Control block for Ethernet network interface
 */
typedef struct st_dlr_ether_netif_ctrl
{
	/** Target module instance */
	ether_netif_instance_t const * p_target; ///< Pointer to Ethernet network interface instance.

	/** Target module callback settings */
    ether_netif_callback_args_t    callback_memory;  	   ///< Callback argument memory for Ethernet network interface callback.
    ether_netif_callback_link_node_t    callback_node;	 	   ///< Callback link node for Ethernet network interface callback.

	/** Submodule dependencies. */
    void * p_receiver_ctrl;	   ///< Pointer to controller for receiving the Ethernet frame.

} dlr_ether_netif_ctrl_t;

typedef struct st_dlr_neighbor_check_timer_callback_arg
{
	dlr_port_t timedout_port;
	void * p_context;
} dlr_neighbor_check_timer_callback_arg_t;

/**
 * Control block for DLR neighbor check timer.
 */
typedef struct st_dlr_neighbor_checker_ctrl
{
	/** RTOS Software Timer */
    TimerHandle_t p_timer0;					///< Pointer to neighbor check timer handler 1
    TimerHandle_t p_timer1;					///< Pointer to neighbor check timer handler 2

    /** Software timer callback argument */
    dlr_neighbor_check_timer_callback_arg_t timer_callback_arg0;
	dlr_neighbor_check_timer_callback_arg_t timer_callback_arg1;

    /** Neighbor check information */
    uint8_t timeout_count0;     	///< NeighborCheck execution counts for port0
    uint8_t timeout_count1;			///< NeighborCheck execution counts for port1

	/** Submodule dependencies. */
    void * p_executor_ctrl;	///< Pointer to controller for executing state machine.

} dlr_neighbor_checker_ctrl_t;

/** Channel control block.
 * DO NOT INITIALIZE.  Initialization occurs when @ref tcpip_api_t::open is called.
 */
typedef struct st_dlr_instance_ctrl
{
    uint32_t          open;           			///< Whether or not the instance is opened.
    uint32_t		  start;					///< Whether or not the tasks is started.
    dlr_cfg_t const * p_cfg;					///< Pointer to configuration structure

    /** Controllers of DLR submodules. */
    dlr_fsm_ctrl_t    			fsm_ctrl;
    dlr_receiver_ctrl_t			receiver_ctrl;
    dlr_executor_ctrl_t 	    executor_ctrl;
    dlr_ether_netif_ctrl_t 	    ether_netif_ctrl;
    dlr_ether_switch_ctrl_t     ether_switch_ctrl;
    dlr_neighbor_checker_ctrl_t neighbor_checker_ctrl;

} dlr_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const dlr_api_t g_dlr_on_dlr;

/** @endcond */
usr_err_t UM_DLR_Open(dlr_ctrl_t * const p_ctrl, dlr_cfg_t const * const p_cfg );
usr_err_t UM_DLR_Start(dlr_ctrl_t * const p_ctrl );
usr_err_t UM_DLR_Stop(dlr_ctrl_t * const p_ctrl );
usr_err_t UM_DLR_Close(dlr_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end defgroup DLR)
 **********************************************************************************************************************/

#endif                                 // UM_DLR_H
