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
/** FSP BSP definitions. */
#include "bsp_api.h"

/** Other module dependencies definitions. */
#include "um_ether_netif_api.h"

/** DLR module definitions. */
#include "um_dlr.h"
#include "um_dlr_api.h"
#include "um_dlr_cfg.h"
#include "um_dlr_internal.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DLR_OPEN		(0x444C5200)
#define DLR_START		(0x444C5200)
#define DLR_CLOSE		(0x00000000)
#define DLR_STOP		(0x00000000)

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
/** DLR Implementation of General DLR Driver  */
const dlr_api_t g_dlr_on_dlr =
{
    .open         = UM_DLR_Open,
	.start		  = UM_DLR_Start,
	.stop	      = UM_DLR_Stop,
	.close		  = UM_DLR_Close,
};

/*******************************************************************************************************************//**
 * @addtogroup DLR
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * Initialize instance control structure based on configuration structure.
 * Implements @ref um_dlr_api_t::open.
 *
 * @retval USR_SUCCESS                 The instance control structure was initialized successfully.
 * @retval USR_ERR_ASSERTION           The any of argument pointers are NULL.
 * @retval USR_ERR_INITIALIZED         The instance control structure was not initialized.
 **********************************************************************************************************************/
usr_err_t UM_DLR_Open (dlr_ctrl_t * const p_ctrl, dlr_cfg_t const * const p_cfg)
{
#if UM_DLR_CFG_PARAM_CHECKING_ENABLE
    USR_ASSERT(NULL != p_ctrl)
    USR_ASSERT(NULL != p_cfg)
#endif

	/** Error codes */
	usr_err_t usr_err;

    /** Resolve instance control. */
    dlr_instance_ctrl_t * p_instance_ctrl = ( dlr_instance_ctrl_t * ) p_ctrl;

    /** Set the pointer of configuration structure. */
    p_instance_ctrl->p_cfg = p_cfg;

    /** Open submodules. */
    {
		/** Open DLR state machine. */
		usr_err = um_dlr_fsm_open( &p_instance_ctrl->fsm_ctrl,
								   &p_instance_ctrl->receiver_ctrl,
								   &p_instance_ctrl->ether_netif_ctrl,
								   &p_instance_ctrl->ether_switch_ctrl,
								   &p_instance_ctrl->neighbor_checker_ctrl,
								    p_instance_ctrl->p_cfg->p_ip_address );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

		/** Open Ethernet switch interface module. */
		usr_err = um_dlr_ether_switch_open( &p_instance_ctrl->ether_switch_ctrl,
											&p_instance_ctrl->ether_netif_ctrl,
											&p_instance_ctrl->executor_ctrl,
											 p_instance_ctrl->p_cfg->p_ether_switch_instance );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

		/** Open Ethernet network interface module. */
		usr_err = um_dlr_ether_netif_open( &p_instance_ctrl->ether_netif_ctrl,
										   &p_instance_ctrl->receiver_ctrl,
										    p_cfg->p_ether_netif_instance );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

		/** Open DLR frame receiver task. */
		usr_err = um_dlr_task_receiver_open( &p_instance_ctrl->receiver_ctrl,
											 &p_instance_ctrl->fsm_ctrl,
											 &p_instance_ctrl->executor_ctrl);
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

		/** Open DLR receiver task. */
		usr_err = um_dlr_task_executor_open( &p_instance_ctrl->executor_ctrl,
											 &p_instance_ctrl->fsm_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

		/** Open DLR Neighbor check timers. */
		usr_err = um_dlr_timer_neighbor_checker_open( &p_instance_ctrl->neighbor_checker_ctrl,
													  &p_instance_ctrl->executor_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    }

	/** Init DLR */
    um_dlr_ether_switch_init_dlr(&p_instance_ctrl->ether_switch_ctrl);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start the each tasks
 * Implements @ref dlr_api_t::start.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 **********************************************************************************************************************/
usr_err_t UM_DLR_Start (dlr_ctrl_t * const p_ctrl )
{
#if (UM_DLR_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    dlr_instance_ctrl_t * const p_instance_ctrl = (dlr_instance_ctrl_t * const) p_ctrl;

    /** Check if the instance is stopped yet */
    USR_ERROR_RETURN( DLR_STOP == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Resume own tasks. */
    (void) um_dlr_task_executor_start( &p_instance_ctrl->executor_ctrl );
    (void) um_dlr_task_receiver_start( &p_instance_ctrl->receiver_ctrl );

    /** Resume sub module task. */
    (void) um_dlr_ether_netif_start( &p_instance_ctrl->ether_netif_ctrl );

	/** Set the new status. */
	p_instance_ctrl->start = DLR_START;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop the each tasks
 * Implements @ref dlr_api_t::start.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 **********************************************************************************************************************/
usr_err_t UM_DLR_Stop (dlr_ctrl_t * const p_ctrl )
{
#if (UM_DLR_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    dlr_instance_ctrl_t * const p_instance_ctrl = (dlr_instance_ctrl_t * const) p_ctrl;

    /** Check if the instance is started yet */
    USR_ERROR_RETURN( DLR_START == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Resume own tasks. */
    (void) um_dlr_task_executor_start( &p_instance_ctrl->executor_ctrl );
    (void) um_dlr_task_receiver_start( &p_instance_ctrl->receiver_ctrl );

    /** Resume sub module task. */
    (void) um_dlr_ether_netif_start( &p_instance_ctrl->ether_netif_ctrl );

	/** Set the new status. */
	p_instance_ctrl->start = DLR_STOP;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Finalize the instance control structure and close related resources.
 * Implements @ref um_dlr_api_t::close.
 *
 * @retval  USR_SUCCESS
 **********************************************************************************************************************/
usr_err_t UM_DLR_Close (dlr_ctrl_t * const p_ctrl)
{
#if UM_DLR_PARAM_CHECKING_ENABLE
    USR_ASSERT(NULL != p_ctrl);
#endif
    /** Resolve control instance type. */
    dlr_instance_ctrl_t * p_instance_ctrl = (dlr_instance_ctrl_t *) p_ctrl;

    (void) p_instance_ctrl;

    /**
     * TODO: Implement sequence for closing
     */
    return USR_SUCCESS;
};

/** @} (end addtogroup UM_DLR) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
