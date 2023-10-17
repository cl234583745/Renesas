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

#include "cipdlr.h"

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
 * Global Variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static usr_err_t _event_new_state( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_link_lost( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_link_restore( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_own_frame( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_flush_table( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_beacon_timeout( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_new_supervisor( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_locate_fault( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_neighbor_check_request( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_neighbor_check_response( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_neighbor_timeout( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );
static usr_err_t _event_sign_on( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event );

static usr_err_t (*_event_function_table[])( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event ) =
{
		[ DLR_EVENT_LINKLOST 			] = _event_link_lost,
		[ DLR_EVENT_OWNFRAME			] = _event_own_frame,
		[ DLR_EVENT_LINKRESTORED		] = _event_link_restore,
		[ DLR_EVENT_BEACONFRAME     	] =  NULL,				/** Supported by hardware. */
		[ DLR_EVENT_BEACONTIMEOUT		] = _event_beacon_timeout,
		[ DLR_EVENT_NEWSUPERVISOR		] = _event_new_supervisor,
		[ DLR_EVENT_LOCATEFAULT 		] = _event_locate_fault,
		[ DLR_EVENT_NEIGHBORCHECKREQ	] = _event_neighbor_check_request,
		[ DLR_EVENT_NEIGHBORCHECKRES	] = _event_neighbor_check_response,
		[ DLR_EVENT_NEIGHBORTIMEOUT 	] = _event_neighbor_timeout,
		[ DLR_EVENT_FLUSHTABLES			] = _event_flush_table,
		[ DLR_EVENT_SIGNON          	] = _event_sign_on,
		[ DLR_EVENT_NEWSTATE 			] = _event_new_state, 	/** State transition */
};

static dlr_bool_t _is_new_supervisor_precedence_higher( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * p_dlr_event );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize its controller.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_open( dlr_fsm_ctrl_t    		   * const p_ctrl,
		   	   	   	   	   dlr_receiver_ctrl_t         * const p_receiver_ctrl,
						   dlr_ether_netif_ctrl_t      * const p_ether_netif_ctrl,
						   dlr_ether_switch_ctrl_t     * const p_ether_switch_ctrl,
						   dlr_neighbor_checker_ctrl_t * const p_neighbor_cheker_ctrl,
						   uint32_t const	     	   * const p_ip_adress )
{
	/** Set related submodule. */
	p_ctrl->p_neighbor_checker_ctrl = p_neighbor_cheker_ctrl;
    p_ctrl->p_ether_netif_ctrl 		= p_ether_netif_ctrl;
    p_ctrl->p_receiver_ctrl			= p_receiver_ctrl;
    p_ctrl->p_ether_switch_ctrl     = p_ether_switch_ctrl;

    /** Set the configuration arguments. */
    p_ctrl->p_ether_netif_frame = NULL;
    p_ctrl->sequence_id = 0;

	/** Initialize attributes. */
    p_ctrl->node_state = DLR_NODE_STATE_IDLE;
    g_dlr.network_topology = DLR_NETWORK_TOPOLOGY_LINEAR;
    g_dlr.network_status = DLR_NETWORK_STATUS_NORMAL;
    g_dlr.active_supervisor_address.device_ip = 0;
    memset(g_dlr.active_supervisor_address.device_mac, 0, sizeof(g_dlr.active_supervisor_address.device_mac) );
    /** TODO: g_dlr.capability_flags should be overridden by definitions of opener port layer */

    /** Beacon frame information */
    p_ctrl->rx_beacon_info.ring_state = DLR_RING_STATE_NORMAL;
    p_ctrl->rx_beacon_info.source_ip_address = 0;
    p_ctrl->rx_beacon_info.supervisor_precedence = 0;
    p_ctrl->rx_beacon_info.vlan_id = 0;
    p_ctrl->rx_beacon_info.last_beacon_receive_port = DLR_PORT_NONE;

    /** Link/Neighbor status information. */
    p_ctrl->link_active_port = DLR_PORT_NONE;
    p_ctrl->neighbor_active_port = DLR_PORT_BOTH;

    /** IP address */
    p_ctrl->p_ip_address = p_ip_adress;

    memset(p_ctrl->rx_beacon_info.source_mac_address, 0, sizeof(uint8_t) * DLR_MAC_ADDRESS_LENGTH);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute state machine.
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
usr_err_t um_dlr_fsm_execute_with_event( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t * const p_dlr_event )
{
	/** Error code */
	usr_err_t usr_err;

	/** Check if its event is executable */
	USR_ERROR_RETURN( NULL != _event_function_table[p_dlr_event->event], USR_ERR_ABORTED );

	/** Execute state machine with event. */
	usr_err = _event_function_table[p_dlr_event->event]( p_ctrl, p_dlr_event );
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    return usr_err;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Execute with new state event
 *
 * TODO: Add the callback interface so that it can update the following DLR object attributes.
 * Notes:
 *   Network Topology attribute shall be updated at events 1, 2, 3, 12, 13, 18 and 20 to appropriate value.
 *   Network Status attribute shall be updated at events 1, 2, 3, 8, 12, 13, 14, 15, 18, 20, 32, 33, 34, 35,
 *   36, 37, 40 and 41 to appropriate value.
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_new_state( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code. */
	usr_err_t usr_err;

	/** Copy hardware information freezed when hardware ISR occurs. */
	p_ctrl->rx_beacon_info = p_dlr_event->freezed_rx_beacon_info;

	/** Check the node state */
	switch ( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.2(3): Beacon frame from a ring supervisor received on port 0(1).
		 * - Transition to FAULT_STATE.
		 * - Other process should be already done on hardware.
		 */

		break;

	case DLR_NODE_STATE_FAULT:

		/**
		 * Event No.12(13) : Beacon timeout timer expired for port 0(1) and LastBcnRcvPort is 0(1).
		 * - Transition to IDLE_STATE.
		 * - Other process should be done on DLR_EVENT_BEACONTIMEOUT event before this transition process.
		 */
		/**
		 * Event No.14(15) : Beacon frame from active ring supervisor received on port 1(0) and LastBcnRcvPort is 0(1) or both(value 3).
		 * - Transition to NORMAL_STATE.
		 * - Other process should be already done on hardware.
		 */
		/**
		 * Event No.18(20) : Link lost on port 0(1) and LastBcnRcvPort is 0(1).
		 * - Transition to IDLE_STATE.
		 * - Other process should be done on DLR_EVENT_LINKLOST event before this transition process.
		 */
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.34(35) : Link lost on port 0(1).
		 * - Transition to FAULT_STATE.
		 * - Other process should be done on DLR_EVENT_LINKLOST event before this transition process.
		 */
		/**
		 * Event No.34(35) : Beacon frame from active ring supervisor with RING_FAULT_STATE received 0(1).
		 * - Transition to FAULT_STATE.
		 * - Other process should be already done on hardware.
		 */

		break;
	}

	/** Transition to new state. */
	p_ctrl->node_state = p_dlr_event->freezed_node_state;

	/** Update values of Attribute #1(Network Topology) and #2(Network Status), judging from new state */
	switch (p_ctrl->node_state)
	{
	case DLR_NODE_STATE_IDLE:
        g_dlr.network_topology = DLR_NETWORK_TOPOLOGY_LINEAR;
        g_dlr.network_status = DLR_NETWORK_STATUS_NORMAL;
        break;
	case DLR_NODE_STATE_FAULT:
        g_dlr.network_topology = DLR_NETWORK_TOPOLOGY_RING;
        g_dlr.network_status = DLR_NETWORK_STATUS_RING_FAULT;
        break;
	case DLR_NODE_STATE_NORMAL:
        g_dlr.network_topology = DLR_NETWORK_TOPOLOGY_RING;
        g_dlr.network_status = DLR_NETWORK_STATUS_NORMAL;
        break;
	}

	/** Flush unicast MAC address learning table whenever the state is transitioned. */
	usr_err = um_dlr_ether_switch_flush_unicast_mac_table( p_ctrl->p_ether_switch_ctrl );
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with link lost event.
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_link_lost( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code. */
	usr_err_t usr_err;

	/** Update link status. */
	switch ( p_dlr_event->event_port )
	{
	case DLR_PORT0_BIT:	p_ctrl->link_active_port = DLR_PORT1_BIT; break;
	case DLR_PORT1_BIT: p_ctrl->link_active_port = DLR_PORT0_BIT; break;
	default: return USR_ERR_ABORTED;
	}

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event 4(5) : Link lost on port 0(1).
		 * - Stop forwarding frames on port 0(1).
		 */
		usr_err = um_dlr_ether_switch_stop_forwarding_frames( p_ctrl->p_ether_switch_ctrl, p_dlr_event->event_port );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event 18(20) : Link lost on port 0(1) and LastBcnRcvPort is 0(1).
		 * - Stop neighbor check timers of both ports.
		 * The following process should be done on hardware.
		 * - Stop port 0(1) beacon timeout timer.
		 * The following process should be done on DLR_EVENT_NEWSTATE after this process.
		 * - Transition to IDLE_STATE.
		 * - Flush unicast MAC address learning table.
		 */
		if ( ( DLR_PORT0_BIT == p_dlr_event->event_port && DLR_PORT0_BIT == p_ctrl->rx_beacon_info.last_beacon_receive_port ) ||
			 ( DLR_PORT1_BIT == p_dlr_event->event_port && DLR_PORT1_BIT == p_ctrl->rx_beacon_info.last_beacon_receive_port ))
		{
			/** Update link status */
			p_ctrl->link_active_port = DLR_PORT_NONE;

			/** Stop neighbor check timer. */
			usr_err = um_dlr_timer_neighbor_checker_stop(p_ctrl->p_neighbor_checker_ctrl, DLR_PORT_BOTH );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		/**
		 * Event 19(21) : Link lost on port 0(1) and LastBcnRcvPort is 1(0).
		 * - Send Link_Status frame to active ring supervisor.
		 */
		else
		{
			/** Link_Status from active port. */
			usr_err = um_dlr_fsm_send_link_neighbor_status( p_ctrl, DLR_STATUS_FRAME_TYPE_LINK_STATUS );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event 32(33) : Link lost on port 0(1).
		 * - Send Link_Status frame to active ring supervisor.
		 * The following process should be done on hardware.
		 * - Stop beacon timeout timer for port 0(1).
		 * - Set LastBcnRcvPort to 0(1).
		 * The following process should be done on DLR_EVENT_NEWSTATE after this process.
		 * - Transition to FAULT_STATE.
		 * - Flush unicast MAC address learning table.
		 */

		/** Link_Status from active port. */
		usr_err = um_dlr_fsm_send_link_neighbor_status( p_ctrl, DLR_STATUS_FRAME_TYPE_LINK_STATUS );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with link restore event
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_link_restore( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code. */
	usr_err_t usr_err;

	/** Update link status */
	p_ctrl->link_active_port |= p_dlr_event->event_port;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.6(7) : Link restored on port 0(1).
		 * - Start forwarding frames on port 0(1).
		 */
		/** No break to follow next state process. */
	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.22(23) : Link restored on port 0(1).
		 * - Start forwarding frames on port 0(1).
		 */
		usr_err = um_dlr_ether_switch_start_forwarding_frames( p_ctrl->p_ether_switch_ctrl, p_dlr_event->event_port );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Non-defined event : This case should not be reached because the NORMAL state must have the linked port.
		 */
		return USR_ERR_ABORTED;
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with own frame event
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_own_frame( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Currently unused parameter. */
	(void) p_dlr_event;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.8 : Frame from self received on port 0 or port 1.
		 * The following process should be done on hardware.
		 * - Do not forward frame on network and drop it. (If capable of doing so.)
		 */
		break;

	case DLR_NODE_STATE_FAULT:
	case DLR_NODE_STATE_NORMAL:
		/**
		 * Non-defined event : This case should not be reached because the NORMAL state must have the linked port.
		 */
		return USR_ERR_ABORTED;
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with flush table event
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_flush_table( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code. */
	usr_err_t usr_err;

	/** Currently unused parameter. */
	(void) p_dlr_event;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.46 : Flush_Tables frame received.
		 * - Ignore
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.47 : Flush_Tables frame received.
		 * - Flush learning table of unicast and multicast MAC addresses.
		 * - If Learning Update Enable field in Flush_Tables frame is set to TRUE, send Learnin_Update frame.
		 */
		/** No break to follow next state process. */
	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.48 : Flush_Tables frame received.
		 * - Flush learning table of unicast and multicast MAC addresses.
		 * - If Learning Update Enable field in Flush_Tables frame is set to TRUE, send Learnin_Update frame.
		 */
		usr_err = um_dlr_ether_switch_flush_unicast_mac_table( p_ctrl->p_ether_switch_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		usr_err = um_dlr_ether_switch_flush_multicast_mac_table( p_ctrl->p_ether_switch_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

		usr_err = um_dlr_fsm_send_learning_update( p_ctrl, p_dlr_event );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with beacon timeout event.
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_beacon_timeout( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code. */
	usr_err_t usr_err;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Non-defined event : This case should not be reached because the two beacon timer is not launched.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.12(13) : Beacon timeout timer expired for port 0(1) and LastBcnRcvPort is 0(1).
		 * - Stop neighbor check timeout timers for both ports.
		 * The following process should be done on DLR_EVENT_NEWSTATE after this process.
		 * - Transition to IDLE_STATE.
		 * - Flush unicast MAC address learning table.
		 */
		if ( ( DLR_PORT0_BIT == p_dlr_event->event_port && DLR_PORT0_BIT == p_ctrl->rx_beacon_info.last_beacon_receive_port ) ||
			 ( DLR_PORT1_BIT == p_dlr_event->event_port && DLR_PORT1_BIT == p_ctrl->rx_beacon_info.last_beacon_receive_port ))
		{
			/** Stop neighbor check timers of both ports. */
			usr_err = um_dlr_timer_neighbor_checker_stop(p_ctrl->p_neighbor_checker_ctrl, DLR_PORT_BOTH);
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		/**
		 * Non-defined event : This case should not be reached because the other beacon timer is not launched.
		 */
		else
		{
			return USR_ERR_ABORTED;
		}
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.36(37) : Beacon timeout timer expired for port 0(1).
		 * The following process should be done on hardware.
		 * - Set LastBcnRcvPort to 0(1).
		 * The following process should be done on DLR_EVENT_NEWSTATE after this process.
		 * - Transition to IDLE_STATE.
		 * - Flush unicast MAC address learning table.
		 */
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with new supervisor event
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_new_supervisor( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code. */
	usr_err_t usr_err;

	/** Unused parameter */
	(void) p_dlr_event;

	/** Update values of Attribute #10(Active Supervisor Address) */
	g_dlr.active_supervisor_address.device_ip = p_dlr_event->freezed_rx_beacon_info.source_ip_address;
	memcpy(g_dlr.active_supervisor_address.device_mac, p_dlr_event->freezed_rx_beacon_info.source_mac_address, sizeof(g_dlr.active_supervisor_address.device_mac) );

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Non-defined event : This case should not be reached because the two beacon timer is not launched.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.16(17) : Beacon frame from different ring supervisor MAC address than active ring supervisor is received on port 0(1).
		 * If new supervisor has lower precedence (or numerically lower MAC address in case of tie) than active supervisor:
		 */
		if ( DLR_FALSE == _is_new_supervisor_precedence_higher(p_ctrl, p_dlr_event) )
		{
			/**
			 * The following process should be done on hardware.
			 * - If embedded switch hardware determines precedence,
			 * 	 - drop the new Beacon frame and return.
			 * - If embedded switch hardware does not determine precedence,
			 *   - the new Beacon frame shall be forwarded on the other ring port and to the host;
			 *   - the host CPU shall ignore the new Beacon frame and return
			 */
		}
		/**
		 * Else,
		 */
		else
		{
			/**
			 * The following process should be done on hardware.
			 * - Stop beacon timeout timers for both ports
			 * - Save new active supervisor precedence, MAC address, VLAN ID and beacon timeout duration.
			 * - Set LastBcnRcvPort 0(1).
			 * - Start beacon timeout timer for port 0(1).
			 * The following process does not need any process.
			 * - Stay in FAULT_STATE.
			 * The following process should be done HERE because the NEWSTATE event does NOT occurs.
			 * - Flush unicast MAC address learning table.
			 */
			usr_err = um_dlr_ether_switch_flush_unicast_mac_table( p_ctrl->p_ether_switch_ctrl );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}

		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.40(41) : Beacon frame from different ring supervisor MAC address than active ring supoervisor is received on port 0(1).
		 * - If new supervisor has lower precedence (or numerically lower MAC address in case of tie) than active supervisor:
		 */
		if ( DLR_FALSE == _is_new_supervisor_precedence_higher(p_ctrl, p_dlr_event) )
		{
			/**
			 * The following process should be done on hardware.
			 * - If embedded switch hardware determines precedence,
			 * 	 - drop the new Beacon frame and return.
			 * - If embedded switch hardware does not determine precedence,
			 *   - the new Beacon frame shall be forwarded on the other ring port and to the host;
			 *   - the host CPU shall ignore the new Beacon frame and return
			 */
		}
		/**
		 * - Else,
		 */
		else
		{
			/**
			 * The following process should be done on hardware.
			 * - Stop beacon timeout timers for both ports
			 * - Save new active supervisor precedence, MAC address, VLAN ID and beacon timeout duration.
			 * - Set LastBcnRcvPort 0(1).
			 * - Start beacon timeout timer for port 0(1).
			 * The following process should be done on DLR_EVENT_NEWSTATE after this process.
			 * - Transition to FAULT_STATE.
			 * - Flush unicast MAC address learning table.
			 */
		}

		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with locate fault
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_locate_fault( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code */
	usr_err_t usr_err;

	/** Unused parameter */
	(void) p_dlr_event;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.9 : Locate_Fault or Neighbor_Check_Request or Neighbor_Check_Response or Sign_on received on port 0 or port 1.
		 * - Ignore.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.24 : Locate_Fault frame received from active ring supervisor on port 0 or port 1.
		 * If link is not active on port 0 or port 1,
		 */
		if( DLR_PORT_BOTH != p_ctrl->link_active_port )
		{
			/**
			 * - Send Link_Status frame to active ring supervisor.
			 */
			usr_err = um_dlr_fsm_send_link_neighbor_status( p_ctrl, DLR_STATUS_FRAME_TYPE_LINK_STATUS );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		/**
		 * Else, (if link is active on both port 0 and port 1)
		 */
		else
		{
			/**
			 * - Clear neighbor status for port 0 and 1,
			 * - Send Neighbor_Check_Request for port 0 and 1.
			 * - Start neighbor check timeout timer for port 0 and 1.
			 */
			p_ctrl->neighbor_active_port = DLR_PORT_NONE;	/** Clear to non-active. */

			usr_err = um_dlr_timer_neighbor_checker_clear_timeout_count( p_ctrl->p_neighbor_checker_ctrl, DLR_PORT_BOTH );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

			/** Must send individually on each port with each packet with each sequential ID. (Don't use DLR_PORT_BOTH) */
			usr_err = um_dlr_fsm_send_neighbor_check_request( p_ctrl, DLR_PORT0_BIT );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
            usr_err = um_dlr_fsm_send_neighbor_check_request( p_ctrl, DLR_PORT1_BIT );
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

			usr_err = um_dlr_timer_neighbor_checker_start(p_ctrl->p_neighbor_checker_ctrl, DLR_PORT_BOTH);
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.42 : Locate_Fault frame received from active ring supervisor on port 0 or port 1.
		 * - Ignore.
		 */
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with neighbor check request
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_neighbor_check_request( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code */
	usr_err_t usr_err;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.9 : Locate_Fault or Neighbor_Check_Request or Neighbor_Check_Response or Sign_on received on port 0 or port 1.
		 * - Ignore.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.25(26) : Neighbor_Check_Request frame received on port 0(1).
		 * - Send Neighbor_Check_Response frame on port 0(1).
		 */
		usr_err = um_dlr_fsm_send_neighbor_check_response(p_ctrl, p_dlr_event, p_dlr_event->event_port);
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.43 : Neighbor_Check_Request or Neighbor_Check_Response received on port 0 or port 1.
		 * - Ignore.
		 */
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with neighbor check response
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_neighbor_check_response( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code */
	usr_err_t usr_err;

	/** Currently unused parameter. */
	(void) p_dlr_event;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.9 : Locate_Fault or Neighbor_Check_Request or Neighbor_Check_Response or Sign_on received on port 0 or port 1.
		 * - Ignore.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.27(28) : Neighbor_Check_Response frame received on port 0(1).
		 * - Stop neighbor check timeout timer for port 0(1).
		 * - Save neighbor status for port 0(1).
		 */
		usr_err = um_dlr_timer_neighbor_checker_stop( p_ctrl->p_neighbor_checker_ctrl, p_dlr_event->event_port );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

		p_ctrl->neighbor_active_port |= p_dlr_event->event_port;	/** Set to active. */
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.43 : Neighbor_Check_Request or Neighbor_Check_Response received on port 0 or port 1.
		 * - Ignore.
		 */
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with neighbor timeout event
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_neighbor_timeout( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code */
	usr_err_t usr_err;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.9 : Locate_Fault or Neighbor_Check_Request or Neighbor_Check_Response or Sign_on received on port 0 or port 1.
		 * - Ignore.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event 29(30) : Neighbor check timer timeout on port 0(1).
		 * - If number of retires have not exceeded maximum limit,
		 *   - Send Neighbor_Check_Request on port 0(1).
		 *   - Start neighbor check timeout timer for port 0(1).
		 * - Else,
		 * 	 - Send Neighbor_Status frame to active ring supervisor.
		 */
		if ( p_dlr_event->freezed_timeout_count < DLR_NEIGHBOR_CHECK_TRIES )
		{
			usr_err = um_dlr_fsm_send_neighbor_check_request(p_ctrl, p_dlr_event->event_port );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

			usr_err = um_dlr_timer_neighbor_checker_start(p_ctrl->p_neighbor_checker_ctrl, p_dlr_event->event_port);
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		else
		{
			usr_err = um_dlr_fsm_send_link_neighbor_status( p_ctrl, DLR_STATUS_FRAME_TYPE_NEIGHBOR_STATUS );
			USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		}
		break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Non-defined event : This case should not be reached because the neighbor check timer is not launched on NORAML STATE.
		 */
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Execute with sign on
 *
 * @retval USR_SUCCESS
 * @retval USR_EER_ABORTED
 **********************************************************************************************************************/
static usr_err_t _event_sign_on( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * const p_dlr_event )
{
	/** Error code */
	usr_err_t usr_err;

	/** Target port */
	dlr_port_t target_port = DLR_PORT0_BIT;

	switch( p_ctrl->node_state )
	{
	case DLR_NODE_STATE_IDLE:
		/**
		 * Event No.9 : Locate_Fault or Neighbor_Check_Request or Neighbor_Check_Response or Sign_on received on port 0 or port 1.
		 * - Ignore.
		 */
		break;

	case DLR_NODE_STATE_FAULT:
		/**
		 * Event No.31 : Sign_on received on port 0 or port 1.
		 * - Ignore.
		 */
	    break;

	case DLR_NODE_STATE_NORMAL:
		/**
		 * Event No.44(45) : Sign_On frame received on port 0(1).
		 * - Add node data to participant list.
		 * - Send frame through port 1(0).
		 */
		switch( p_dlr_event->event_port )
		{
		case DLR_PORT0_BIT: target_port = DLR_PORT1_BIT; break;
		case DLR_PORT1_BIT: target_port = DLR_PORT0_BIT; break;
		default: return USR_ERR_ABORTED;  break;
		}

		/**
		 * The node data will be added in the next function to create frame which include the participant list.
		 */
		usr_err = um_dlr_fsm_send_sign_on( p_ctrl, p_dlr_event, target_port );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
		break;
	}
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Check if precedence of new Supervisor is higher

 * @retval DLR_TRUE
 * @retval DLR_FALSE
 **********************************************************************************************************************/
static dlr_bool_t _is_new_supervisor_precedence_higher( dlr_fsm_ctrl_t * const p_ctrl, dlr_event_arg_t const * p_dlr_event )
{
    int idx; /* loop counter */

    /** The precedence of new supervisor is higher. */
    if (p_dlr_event->freezed_rx_beacon_info.supervisor_precedence > p_ctrl->rx_beacon_info.supervisor_precedence)
    {
        return DLR_TRUE;
    }

    /** The precedence of new supervisor is lower. */
    if (p_dlr_event->freezed_rx_beacon_info.supervisor_precedence < p_ctrl->rx_beacon_info.supervisor_precedence)
    {
        return DLR_FALSE;
    }

    /**
     * If the precedence of new supervisor same as the old one,
     * The supervisor with the higher MAC address has the higher precedence.
     */
	for (idx = 0; idx < DLR_MAC_ADDRESS_LENGTH; idx++)
	{
		if ( p_dlr_event->freezed_rx_beacon_info.supervisor_precedence > p_ctrl->rx_beacon_info.source_mac_address[idx] )
		{
			return DLR_TRUE;
		}

		/** The */
		else if ( p_dlr_event->freezed_rx_beacon_info.supervisor_precedence < p_ctrl->rx_beacon_info.source_mac_address[idx])
		{
			return DLR_FALSE;
		}
	}

	/** New supervisor is same as current supervisor. */
	return DLR_FALSE;
}
