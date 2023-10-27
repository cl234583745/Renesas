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
/** Interface Header */
#include "um_lwip_port_api.h"

/** Instance Headers */
#include "um_lwip_port.h"
#include "um_lwip_port_cfg.h"
#include "um_lwip_port_internal.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** "LWIP" in ASCII, used to determine if channel is open. */
#define LWIP_PORT_OPEN 		(0x4C574950ULL)
#define LWIP_PORT_CLOSE     (0x00000000ULL)
#define LWIP_PORT_START     (0x4C574950ULL)
#define LWIP_PORT_STOP      (0x00000000ULL)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/** LWIP Implementation of General TCPIP Driver  */
const lwip_port_api_t g_lwip_port_on_lwip_port =
{
    .open         = UM_LWIP_PORT_Open,
	.start 		  = UM_LWIP_PORT_Start,
	.stop		  = UM_LWIP_PORT_Stop,
    .callbackAdd  = UM_LWIP_PORT_CallbackAdd,
    .netifConfigGet  = UM_LWIP_PORT_NetifConfigGet,
    .netifStateGet   = UM_LWIP_PORT_NetifStateGet,
};

/*******************************************************************************************************************//**
 * @addtogroup UM_LWIP_PORT
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Open the instance for network interface.
 * Implements @ref lwip_port_api_t::open.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 * @retval USR_ERR_ASSERTION           The pointers of controller or configuration was NULL.
 * @retval USR_ERR_NOT_INITIALIZED     The instance controller was not opened.
 * @retval USR_ERR_ALREADY_OPEN		   The instance has already been opened.
 **********************************************************************************************************************/
usr_err_t UM_LWIP_PORT_Open (lwip_port_ctrl_t * const p_ctrl, lwip_port_cfg_t const * const p_cfg)
{
#if defined( UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(NULL != p_ctrl)
    USR_ASSERT(NULL != p_cfg)
#endif

    /** Status */
    usr_err_t usr_err;

    /** Resolve instance controller types. */
    lwip_port_instance_ctrl_t * const p_instance_ctrl = (lwip_port_instance_ctrl_t *) p_ctrl;

    /** Get common control via configuration. */
    lwip_port_common_cfg_t const * const p_common_cfg = p_cfg->p_common_cfg;
    lwip_port_common_ctrl_t * const p_common_ctrl = (lwip_port_common_ctrl_t *) p_common_cfg->p_common_ctrl;

    /** Check if the instance is closed yet. */
    USR_ERROR_RETURN( LWIP_PORT_CLOSE == p_instance_ctrl->open, USR_ERR_ALREADY_OPEN );

    /** Set netif configuration to netif control. */
    p_instance_ctrl->p_cfg = p_cfg;

    /** Set netif control to instance control */
    p_instance_ctrl->p_common_ctrl = p_common_ctrl;

    /** Check if the common control is already open. */
    if ( LWIP_PORT_OPEN != p_common_ctrl->open )
    {
        /** Set instance configuration to instance control. */
        p_common_ctrl->p_common_cfg = p_cfg->p_common_cfg;

        /** Open shared submodule */
        {
            /** Open Ethernet link interface */
            usr_err = um_lwip_port_ether_netif_open( &p_common_ctrl->ether_netif_ctrl,
                                                     &p_common_ctrl->launcher_ctrl,
                                                     &p_common_ctrl->receiver_ctrl,
                                                      p_common_ctrl->p_common_cfg->p_ether_netif_instance );
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

            /** Resume sub module task. */
            (void) um_lwip_port_ether_netif_start( &p_instance_ctrl->p_common_ctrl->ether_netif_ctrl );
        }

        /** Open shared internal modules */
        {
            /** Open instance manager control. */
            usr_err = um_lwip_port_netif_list_open( &p_common_ctrl->netif_list_ctrl );
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

            /** Open LwIP Core interface */
            usr_err = um_lwip_port_core_open( &p_common_ctrl->core_ctrl );
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

            /** Open LwIP Netif launcher interface */
            usr_err = um_lwip_port_task_launcher_open( &p_common_ctrl->launcher_ctrl,
                                                       &p_common_ctrl->netif_list_ctrl );
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

            /** Open LwIP Netif input interface */
            usr_err = um_lwip_port_task_receiver_open( &p_common_ctrl->receiver_ctrl,
                                                       &p_common_ctrl->netif_list_ctrl );
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );
        }

        /** Complete opening the common control. */
        p_common_ctrl->open = LWIP_PORT_OPEN;
    }

    /** Open submodules. */
    {
        /** Open callback control. */
        usr_err = um_lwip_port_callback_open( &p_instance_ctrl->callback_ctrl );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

        /** Open lwIP network interface. */
        um_lwip_port_netif_open( &p_instance_ctrl->netif_ctrl,
                                  p_instance_ctrl->p_cfg->p_netif_cfg,
                                 &p_common_ctrl->ether_netif_ctrl,
                                 &p_instance_ctrl->callback_ctrl,
                                 &p_common_ctrl->launcher_ctrl );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );
    }

    /** Add netif controller into list. */
    {
        /** Unlock list */
        usr_err = um_lwip_port_netif_list_unlock( &p_common_ctrl->netif_list_ctrl);
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

        /** Add the network interface into list. */
        um_lwip_port_netif_list_add( &p_common_ctrl->netif_list_ctrl,
                                     &p_instance_ctrl->netif_ctrl );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

        /** Lock list */
        um_lwip_port_netif_list_lock( &p_common_ctrl->netif_list_ctrl);
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );
    }

    /** Complete opening the instance control. */
    p_instance_ctrl->open = LWIP_PORT_OPEN;

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start the each tasks
 * Implements @ref lwip_port_api_t::start.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 **********************************************************************************************************************/
usr_err_t UM_LWIP_PORT_Start (lwip_port_ctrl_t * const p_ctrl )
{
#if defined( UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    lwip_port_instance_ctrl_t * const p_instance_ctrl = (lwip_port_instance_ctrl_t *) p_ctrl;

    /** Check if the instance is stopped yet */
    USR_ERROR_RETURN( LWIP_PORT_STOP == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Resume own tasks. */
    (void) um_lwip_port_task_launcher_start( &p_instance_ctrl->p_common_ctrl->launcher_ctrl );
    (void) um_lwip_port_task_receiver_start( &p_instance_ctrl->p_common_ctrl->receiver_ctrl );

	/** Set the new status. */
	p_instance_ctrl->start = LWIP_PORT_START;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop the each tasks
 * Implements @ref lwip_port_api_t::start.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 **********************************************************************************************************************/
usr_err_t UM_LWIP_PORT_Stop (lwip_port_ctrl_t * const p_ctrl )
{
#if defined( UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    lwip_port_instance_ctrl_t * const p_instance_ctrl = (lwip_port_instance_ctrl_t *) p_ctrl;

    /** Check if the instance is stopped yet */
    USR_ERROR_RETURN( LWIP_PORT_START == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Stop own tasks. */
    (void) um_lwip_port_task_launcher_stop( &p_instance_ctrl->p_common_ctrl->launcher_ctrl );
    (void) um_lwip_port_task_receiver_stop( &p_instance_ctrl->p_common_ctrl->receiver_ctrl );

	/** Set the new status. */
	p_instance_ctrl->start = LWIP_PORT_STOP;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref lwip_port_api_t::callbackSet.
 *
 * @retval  USR_SUCCESS                  Callback updated successfully.
 **********************************************************************************************************************/
usr_err_t UM_LWIP_PORT_CallbackAdd (lwip_port_ctrl_t * const p_ctrl, lwip_port_callback_link_node_t * p_node )
{
#if defined( UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(NULL != p_ctrl);
#endif
    /** Resolve control instance type. */
    lwip_port_instance_ctrl_t * p_instance_ctrl = (lwip_port_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Open callback module controller. */
    usr_err = um_lwip_port_callback_add(&p_instance_ctrl->callback_ctrl, p_node );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get netif configuration of lwIP network interface
 * Implements @ref lwip_port_api_t::close.
 *
 * @retval  USR_SUCCESS                  Got network configuration successfully.
 * @retval  USR_ERR_ABORTED              Failed to get network configuration
 **********************************************************************************************************************/
usr_err_t UM_LWIP_PORT_NetifConfigGet( lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * p_netif_cfg )
{
#if defined( UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(NULL != p_ctrl);
    USR_ASSERT(NULL != p_netif_cfg );
#endif

    /** Error code */
    usr_err_t usr_err;

    /** Resolve control instance type. */
    lwip_port_instance_ctrl_t * p_instance_ctrl = (lwip_port_instance_ctrl_t *) p_ctrl;

    /** Get the netif configuration. */
    usr_err = um_lwip_port_netif_get_netif_config( &p_instance_ctrl->netif_ctrl, p_netif_cfg );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Return success code. */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get netif state of lwIP network interface
 * Implements @ref lwip_port_api_t::close.
 *
 * @retval  USR_SUCCESS                  Got network configuration successfully.
 * @retval  USR_ERR_ABORTED              Failed to get network configuration
 **********************************************************************************************************************/
usr_err_t UM_LWIP_PORT_NetifStateGet( lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_state_t * p_netif_state, uint8_t notify_callback )
{
#if defined( UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_LWIP_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(NULL != p_ctrl);
    USR_ASSERT(NULL != p_netif_state );
#endif

    /** Error code */
    usr_err_t usr_err;

    /** Resolve control instance type. */
    lwip_port_instance_ctrl_t * p_instance_ctrl = (lwip_port_instance_ctrl_t *) p_ctrl;

    /** Get the netif state. */
    usr_err = um_lwip_port_netif_get_netif_state( &p_instance_ctrl->netif_ctrl, p_netif_state );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Notify the state callbacks */
    if( notify_callback )
    {
        usr_err = um_lwip_port_callback_request( &p_instance_ctrl->callback_ctrl,
                                                LWIP_PORT_NETIF_STATE_UP == *p_netif_state ? LWIP_PORT_CALLBACK_EVENT_NETIF_UP
                                                                                  : LWIP_PORT_CALLBACK_EVENT_NETIF_DOWN);
    }

    /** Return success code. */
    return USR_SUCCESS;
}


/** @} (end addtogroup UM_LWIP_PORT) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
