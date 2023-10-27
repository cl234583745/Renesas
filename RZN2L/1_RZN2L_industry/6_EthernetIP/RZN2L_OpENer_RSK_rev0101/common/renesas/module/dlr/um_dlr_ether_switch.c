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
#include "um_common_api.h"

#include "r_ethsw_extend_api.h"
#include "r_ethsw_extend.h"

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
/** DLR ISR callback function */
static void _dlr_callback_code( ethsw_dlr_event_t event, uint32_t port);

static usr_err_t _get_node_state( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_node_state_t * p_node_state );
static usr_err_t _get_beacon_info( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_rx_beacon_t * const p_beacon_info );
static usr_err_t _get_last_beacon_receive_port( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t * const p_beacon_info );

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/**
 * DLR callback context.
 * TODO: Remove this definition after the DLR callback context is implemented in r_ethsw module.
 */
static void * sgp_dlr_callback_context = NULL;

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Open the instance controller
 *
 * @retval FSP_SUCCESS				Process has been done successfully.
 * @retval FSP_ERR_NOT_INITIALIZED	Controller was not initialized.
 * @retval FSP_IN_USE				Used resource is locked.
 * @retval FSP_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_open( dlr_ether_switch_ctrl_t * const p_ctrl,
									dlr_ether_netif_ctrl_t * const p_ether_netif_ctrl,
									dlr_executor_ctrl_t * const p_executor_ctrl,
									ethsw_instance_t const * const p_ether_switch_instance )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Save the arguments */
	p_ctrl->p_target = p_ether_switch_instance;
	p_ctrl->p_ether_netif_ctrl = p_ether_netif_ctrl;
	p_ctrl->p_executor_ctrl = p_executor_ctrl;

	/** Create a mutex for r_ether_switch */
	p_ctrl->p_mutex_handle = xSemaphoreCreateMutex();
	FSP_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_ERR_NOT_INITIALIZED );

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Open target module. */
		fsp_err = p_ctrl->p_target->p_api->open( p_ctrl->p_target->p_ctrl, p_ctrl->p_target->p_cfg );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err || FSP_ERR_ALREADY_OPEN == fsp_err, USR_ERR_NOT_INITIALIZED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Initialize DLR feature of Ethernet switch.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED	Feature was not initialized.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_init_dlr( dlr_ether_switch_ctrl_t * const p_ctrl )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Get the local host MAC address. */
		fsp_err = um_dlr_ether_netif_get_local_mac_address(p_ctrl->p_ether_netif_ctrl, (uint8_t *) &p_ctrl->dlr_host_mac_addr );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err,  USR_ERR_NOT_INITIALIZED );

		/** Set the local host MAC address. */
		p_ctrl->dlr_init_cfg.p_host_addr = &p_ctrl->dlr_host_mac_addr;

		/** Set the callback function. */
		p_ctrl->dlr_init_cfg.p_dlr_callback = _dlr_callback_code;

		/** Set callback context. */
		sgp_dlr_callback_context = p_ctrl;

		/** Initialize DLR */
		fsp_err = p_ctrl->p_target->p_api->dlrInitSet( p_ctrl->p_target->p_ctrl, &p_ctrl->dlr_init_cfg );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err,  USR_ERR_NOT_INITIALIZED );

		/** Enable DLR extension module. */
		fsp_err = p_ctrl->p_target->p_api->dlrEnableSet( p_ctrl->p_target->p_ctrl );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err,  USR_ERR_NOT_INITIALIZED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Shutdown DLR feature of Ethernet switch.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_shutdown_dlr( dlr_ether_switch_ctrl_t * const p_ctrl )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Shutdown DLR */
		fsp_err = p_ctrl->p_target->p_api->dlrShutdownSet( p_ctrl->p_target->p_ctrl );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Flush unicast MAC table.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_flush_unicast_mac_table( dlr_ether_switch_ctrl_t * const p_ctrl )
{
	/** Error codes */
	fsp_err_t fsp_err;

	ethsw_mactab_clr_modes_t mactab_clr = ETHSW_MACTAB_CLR_DYNAMIC;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Flush unicast MAC table */
		fsp_err = p_ctrl->p_target->p_api->macTableClear( p_ctrl->p_target->p_ctrl, &mactab_clr );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Flush multicast MAC table.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_flush_multicast_mac_table( dlr_ether_switch_ctrl_t * const p_ctrl )
{
	return um_dlr_ether_switch_flush_unicast_mac_table( p_ctrl );

	/** TODO: Is it not possible that Ethernet switch distinguish the multicast MAC table and unicast MAC table? */
	/**
	 * The DLR can initiate a flush of the dynamic unicast entries in the L2 forwarding table by setting the AUTOFLUSH bit in the DLR_CONTROL register.
	 * The auto-flush feature executes whenever the following conditions occur:
	 * 1. The DLR state transitions from IDLE to FAULT
	 * 2. The DLR state transitions from FAULT to IDLE
	 * 3. The DLR state transitions from FAULT to NORMAL
	 * 4. The DLR state transitions from NORMAL to FAULT
	 * 5. The DLR state is FAULT and a beacon frame is received with a precedence higher than the active supervisor’s precedence value.
	 */
}

/*******************************************************************************************************************//**
 * @brief Get DLR node state.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_get_node_state( dlr_ether_switch_ctrl_t * const p_ctrl,
											  dlr_node_state_t * p_node_state )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Get node state */
		fsp_err = _get_node_state( p_ctrl, p_node_state );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get DLR beacon information.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_get_beacon_info( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_rx_beacon_t * const p_beacon_info )
{
	/** Error codes */
	usr_err_t usr_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		usr_err = _get_beacon_info( p_ctrl, p_beacon_info );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_SUCCESS == usr_err, USR_ERR_ABORTED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get DLR beacon information.
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_get_last_beacon_receive_port( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t * const p_last_beacon_receive_port )
{
	/** Error codes */
	usr_err_t usr_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		usr_err = _get_last_beacon_receive_port(p_ctrl, p_last_beacon_receive_port );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_SUCCESS == usr_err, USR_ERR_ABORTED );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the forwarding frames on specified port
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_stop_forwarding_frames( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t port )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Stop forwarding on port 0 if required. */
		if( DLR_PORT0_BIT == port || DLR_PORT_BOTH == port )
		{
			fsp_err = p_ctrl->p_target->p_api->portForwardDel( p_ctrl->p_target->p_ctrl, ETHSW_PORT(0) );
			USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
		}

		/** Stop forwarding on port 1 if required. */
		if( DLR_PORT1_BIT == port || DLR_PORT_BOTH == port )
		{
			fsp_err = p_ctrl->p_target->p_api->portForwardDel( p_ctrl->p_target->p_ctrl, ETHSW_PORT(1) );
			USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
		}
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the forwarding frames on specified port
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_start_forwarding_frames( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t port )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		/** Stop forwarding on port 0 if required. */
		if( DLR_PORT0_BIT == port || DLR_PORT_BOTH == port )
		{
			fsp_err = p_ctrl->p_target->p_api->portForwardAdd( p_ctrl->p_target->p_ctrl, ETHSW_PORT(0) );
			USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
		}

		/** Stop forwarding on port 1 if required. */
		if( DLR_PORT1_BIT == port || DLR_PORT_BOTH == port )
		{
			fsp_err = p_ctrl->p_target->p_api->portForwardAdd( p_ctrl->p_target->p_ctrl, ETHSW_PORT(1) );
			USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
		}
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get link state
 *
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_IN_USE				Used resource is locked.
 * @retval USR_ERR_ABORTED			Process has failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_ether_switch_get_link_state( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t * const p_dlr_port )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Temporary variables */
	uint32_t state_link0;
	uint32_t state_link1;

	/** Enter lock statement. */
	USR_LOCK_SECTION_START(p_ctrl->p_mutex_handle);
	{
		fsp_err = p_ctrl->p_target->p_api->linkStateGet( p_ctrl->p_target->p_ctrl, ETHSW_PORT(0), &state_link0 );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );
		fsp_err = p_ctrl->p_target->p_api->linkStateGet( p_ctrl->p_target->p_ctrl, ETHSW_PORT(0), &state_link1 );
		USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

		*p_dlr_port = (dlr_port_t) ( (state_link1 << 1) | (state_link0 << 0) );
	}
	USR_LOCK_SECTION_END(p_ctrl->p_mutex_handle);

	return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief DLR callback function from Ethernet switch driver.
 **********************************************************************************************************************/
static void _dlr_callback_code( ethsw_dlr_event_t event, uint32_t port )
{
	/** Resolve context. */
	dlr_ether_switch_ctrl_t * p_ctrl = (dlr_ether_switch_ctrl_t * ) sgp_dlr_callback_context;

	/** Error codes */
	usr_err_t usr_err;

	/** Callback event pack. */
	dlr_event_arg_t dlr_event;
	dlr_event.p_ether_frame = NULL;

	/** Pack the callback event and port. */
	dlr_event.event = event;
	switch (port)
	{
	    case 0 : dlr_event.event_port = DLR_PORT0_BIT; break;
	    case 1 : dlr_event.event_port = DLR_PORT1_BIT; break;
	    default : dlr_event.event_port = DLR_PORT_NONE; break;
	}

	/** Freeze DLR information on Ethernet switch hardware. */
	usr_err = _get_node_state( p_ctrl, &dlr_event.freezed_node_state );
	USR_ERROR_RETURN_VOID( USR_SUCCESS == usr_err );
	usr_err = _get_beacon_info( p_ctrl, &dlr_event.freezed_rx_beacon_info );
	USR_ERROR_RETURN_VOID( USR_SUCCESS == usr_err );
	usr_err = _get_last_beacon_receive_port(p_ctrl, &dlr_event.freezed_rx_beacon_info.last_beacon_receive_port );
	USR_ERROR_RETURN_VOID( USR_SUCCESS == usr_err );

    /** Notify the executor of the DLR event. */
	um_dlr_task_executor_receive_event( p_ctrl->p_executor_ctrl, &dlr_event, DLR_CONTEXT_ISR, DLR_ENQUEUE_BACK );
}

static usr_err_t _get_node_state( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_node_state_t * p_node_state )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** For avoiding warning of type cast from signed to unsigned on pointer argument interface. */
	uint32_t node_state;

	/** Get node state */
	fsp_err = p_ctrl->p_target->p_api->dlrNodeStateGet( p_ctrl->p_target->p_ctrl, &node_state );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

	/** Cast the unsigned to signed (enum). */
	*p_node_state = (dlr_node_state_t) node_state;

	return USR_SUCCESS;
}

static usr_err_t _get_beacon_info( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_rx_beacon_t * const p_beacon_info )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Tentative variable to be casted. */
	uint32_t precedence;
	uint32_t vlan_id;
	uint32_t ring_state;

    /** Get store supervisor MAC address stored in DLR_SUPR_MAC register */
	fsp_err = p_ctrl->p_target->p_api->dlrSvMacGet( p_ctrl->p_target->p_ctrl, &p_beacon_info->source_mac_address );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

    /** Get store supervisor IPADDR address stored in DLR_SUPR_IPADR register */
	fsp_err = p_ctrl->p_target->p_api->dlrSvIpGet( p_ctrl->p_target->p_ctrl, &p_beacon_info->source_ip_address );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

    /** Get precedence value of the supervisor stored in DLR_SUPR_MAC register. */
	fsp_err = p_ctrl->p_target->p_api->dlrSvPrecGet( p_ctrl->p_target->p_ctrl, &precedence );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

    /** Get VLAN tag control filed in Beacon frame stored DLR_STATE_VLAN register. */
	fsp_err = p_ctrl->p_target->p_api->dlrVlanGet( p_ctrl->p_target->p_ctrl, &vlan_id );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

    /** Get Ring State field in Beacon frame stored DLR_STATE_VLAN register. */
	fsp_err = p_ctrl->p_target->p_api->dlrRingStateGet( p_ctrl->p_target->p_ctrl, &ring_state );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

	/** Store values needing cast. */
	p_beacon_info->supervisor_precedence = (uint8_t) precedence;
	p_beacon_info->vlan_id    = (uint16_t) vlan_id;
	p_beacon_info->ring_state = (dlr_ring_state_t) ring_state;

	return USR_SUCCESS;
}

static usr_err_t _get_last_beacon_receive_port( dlr_ether_switch_ctrl_t * const p_ctrl, dlr_port_t * const p_last_beacon_receive_port )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** Set ISR argument */
	uint32_t port0_last_beacon_received = 0;
	uint32_t port1_last_beacon_received = 0;

    /** Get LastBcnRcvPort[0] */
	fsp_err = p_ctrl->p_target->p_api->dlrLastBeaconGet( p_ctrl->p_target->p_ctrl, ETHSW_PORT(0), &port0_last_beacon_received );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

    /** Get LastBcnRcvPort[1] */
	fsp_err = p_ctrl->p_target->p_api->dlrLastBeaconGet( p_ctrl->p_target->p_ctrl, ETHSW_PORT(1), &port1_last_beacon_received );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

	/** Store values needing cast. */
	*p_last_beacon_receive_port = (dlr_port_t) ( (port1_last_beacon_received << 1) |
												 (port0_last_beacon_received << 0));

	return USR_SUCCESS;
}
