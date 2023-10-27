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

/** OPENER_PORT module definitions. */
#include "um_opener_port_api.h"
#include "um_opener_port.h"
#include "um_opener_port_cfg.h"
#include "um_opener_port_internal.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define OPENER_PORT_OPEN		(0x444C5200)
#define OPENER_PORT_START		(0x444C5200)
#define OPENER_PORT_CLOSE		(0x00000000)
#define OPENER_PORT_STOP		(0x00000000)

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
 * Private Functions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** OPENER Implementation of General TCPIP Driver  */
const opener_port_api_t g_opener_port_on_opener_port =
{
    .open         = UM_OPENER_PORT_Open,
	.start		  = UM_OPENER_PORT_Start,
	.stop	      = UM_OPENER_PORT_Stop,
	.callbackAdd  = UM_OPENER_PORT_CallbackAdd,
	.moduleStateSet = UM_OPENER_PORT_ModuleStateSet,
	.settableNvdataGet = UM_OPENER_PORT_SettableNvDataGet,
};

/*******************************************************************************************************************//**
 * @addtogroup OPENER_PORT
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * Initialize instance control structure based on configuration structure.
 * Implements @ref um_opener_port_api_t::open.
 *
 * @retval USR_SUCCESS                 The instance control structure was initialized successfully.
 * @retval USR_ERR_ASSERTION           The any of argument pointers are NULL.
 * @retval USR_ERR_INITIALIZED         The instance control structure was not initialized.
 **********************************************************************************************************************/
usr_err_t UM_OPENER_PORT_Open (opener_port_ctrl_t * const p_ctrl, opener_port_cfg_t const * const p_cfg)
{
#if UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE
    USR_ASSERT(NULL != p_ctrl)
    USR_ASSERT(NULL != p_cfg)
#endif

	/** Error codes */
	usr_err_t usr_err;

    /** Resolve instance control. */
    opener_port_instance_ctrl_t * p_instance_ctrl = ( opener_port_instance_ctrl_t * ) p_ctrl;

    /** Set the new status. */
    p_instance_ctrl->open = OPENER_PORT_OPEN;

    /** Set the pointer of configuration structure. */
    p_instance_ctrl->p_cfg = p_cfg;

#if UM_OPENER_PORT_FEATURE_DLR
    /** Resolve extend configuration */
    opener_port_extend_cfg_t * p_extend_cfg = p_instance_ctrl->p_cfg->p_extend;
#endif

    /** Open CIP interface submodule . */
    {
        usr_err = um_opener_port_cip_open( &p_instance_ctrl->cip_ctrl,
                                           p_instance_ctrl->p_cfg->p_cip_user_device_cfg );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );
    }

    /** Open submodules. */
    {
        /** Open Ethernet network interface */
        usr_err = um_opener_port_ether_netif_open( &p_instance_ctrl->ether_netif_ctrl,
                                                   p_instance_ctrl->p_cfg->p_ether_netif_instance );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);


    	/** Open TCP/IP network interface. */
    	usr_err = um_opener_port_lwip_port_open( &p_instance_ctrl->lwip_port_ctrl,
    	                                         p_instance_ctrl->p_cfg->p_lwip_port_instance,
    	                                         p_instance_ctrl->p_cfg->p_cip_user_device_cfg,
    	                                         p_instance_ctrl->p_cfg->p_cip_settable_nvdata );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);

#if UM_OPENER_PORT_FEATURE_DLR
        /** Open DLR module. */
    	usr_err = um_opener_port_dlr_open( &p_instance_ctrl->dlr_ctrl, p_extend_cfg->p_dlr_instance );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);
#endif

        /** Start submodules */
        (void) um_opener_port_ether_netif_start( &p_instance_ctrl->ether_netif_ctrl );
        (void) um_opener_port_lwip_port_start( &p_instance_ctrl->lwip_port_ctrl );
#if UM_OPENER_PORT_FEATURE_DLR
        (void) um_opener_port_dlr_start( &p_instance_ctrl->dlr_ctrl );
#endif
    }

    /** Wait for launching lwIP port TCP/IP network interface. */
    usr_err = um_opener_port_lwip_port_wait_netif_up( &p_instance_ctrl->lwip_port_ctrl );

    /** Open internal module. */
    {
        /** Open callback interface */
        usr_err = um_opener_port_callback_open( &p_instance_ctrl->callback_ctrl );

        /** Open Socket interface submodule */
        usr_err = um_opener_port_tcpip_netif_open( &p_instance_ctrl->tcpip_netif_ctrl,
                                                   &p_instance_ctrl->lwip_port_ctrl,
                                                    p_instance_ctrl->p_cfg->p_cip_settable_nvdata);
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

        /** Open Encap interface submodule */
        usr_err = um_opener_port_encap_open( &p_instance_ctrl->encap_ctrl,
                                             &p_instance_ctrl->tcpip_netif_ctrl );
        USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

        {
            /** Open network interface task module after all internal modules become up. */
            usr_err = um_opener_port_task_netif_open( &p_instance_ctrl->netif_ctrl,
                                                      &p_instance_ctrl->lwip_port_ctrl,
                                                      &p_instance_ctrl->tcpip_netif_ctrl,
                                                      &p_instance_ctrl->encap_ctrl,
                                                      &p_instance_ctrl->indicator_ctrl);
            USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );
        }
    }

    /** Open Indicator task. */
    usr_err = um_opener_port_task_indicator_open( &p_instance_ctrl->indicator_ctrl,
                                                  &p_instance_ctrl->callback_ctrl,
                                                  &p_instance_ctrl->cip_ctrl);
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED);

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start the each tasks
 * Implements @ref opener_port_api_t::start.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 **********************************************************************************************************************/
usr_err_t UM_OPENER_PORT_Start (opener_port_ctrl_t * const p_ctrl )
{
#if UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE
    USR_ASSERT(p_ctrl);
#endif

    /** Error status */
    usr_err_t usr_err;

    /** Resolve control instance type. */
    opener_port_instance_ctrl_t * const p_instance_ctrl = (opener_port_instance_ctrl_t * const) p_ctrl;

    /** Check if the instance is stopped yet */
    USR_ERROR_RETURN( OPENER_PORT_STOP == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Start own tasks. */
    usr_err = um_opener_port_task_indicator_start( &p_instance_ctrl->indicator_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    usr_err = um_opener_port_task_netif_start( &p_instance_ctrl->netif_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

	/** Set the new status. */
	p_instance_ctrl->start = OPENER_PORT_START;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Stop the each tasks
 * Implements @ref opener_port_api_t::start.
 *
 * @retval USR_SUCCESS                 The instance have been opened.
 **********************************************************************************************************************/
usr_err_t UM_OPENER_PORT_Stop (opener_port_ctrl_t * const p_ctrl )
{
#if UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    opener_port_instance_ctrl_t * const p_instance_ctrl = (opener_port_instance_ctrl_t * const) p_ctrl;

    /** Check if the instance is working yet. */
    USR_ERROR_RETURN( OPENER_PORT_START == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Stop own tasks */
    (void) um_opener_port_task_indicator_stop( &p_instance_ctrl->indicator_ctrl );
    (void) um_opener_port_task_netif_stop( &p_instance_ctrl->netif_ctrl );

	/** Set the new status. */
	p_instance_ctrl->start = OPENER_PORT_STOP;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback with the option to provide memory for the callback argument structure.
 * Implements @ref opener_port_api_t::callbackAdd.
 *
 * @retval  USR_SUCCESS                  Callback updated successfully.
 **********************************************************************************************************************/
usr_err_t UM_OPENER_PORT_CallbackAdd (opener_port_ctrl_t * const p_ctrl, opener_port_callback_link_node_t * p_node )
{
#if defined( UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif
    /** Resolve control instance type. */
    opener_port_instance_ctrl_t * p_instance_ctrl = (opener_port_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Open callback module controller. */
    usr_err = um_opener_port_callback_add(&p_instance_ctrl->callback_ctrl, p_node );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Set the module state.
 * Implements @ref opener_port_api_t::moduleStateSet.
 *
 * @retval  USR_SUCCESS                  Callback updated successfully.
 **********************************************************************************************************************/
usr_err_t UM_OPENER_PORT_ModuleStateSet (opener_port_ctrl_t * const p_ctrl, opener_port_module_state_t module_state )
{
#if defined( UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    opener_port_instance_ctrl_t * p_instance_ctrl = (opener_port_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Open callback module controller. */
    usr_err = um_opener_port_task_indicator_set_module_state( &p_instance_ctrl->indicator_ctrl, module_state );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Get current settable non-volatile attributes
 * Implements @ref opener_port_api_t::moduleStateSet.
 *
 * @retval  USR_SUCCESS                  Callback updated successfully.
 **********************************************************************************************************************/
usr_err_t UM_OPENER_PORT_SettableNvDataGet (opener_port_ctrl_t * const p_ctrl, opener_port_cip_settable_nvdata_t * const p_nvdata_cfg )
{
#if defined( UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE ) && ((1) == UM_OPENER_PORT_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
    USR_ASSERT(p_nvdata_cfg);
#endif
    /** Resolve control instance type. */
    opener_port_instance_ctrl_t * p_instance_ctrl = (opener_port_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Get TCP/IP interface object attributes. */
    usr_err = um_opener_port_lwip_port_get_cip_tcpip_nvdata( &p_instance_ctrl->lwip_port_ctrl, p_nvdata_cfg->p_tcpip_nvdata );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    /** Get QoS object attributes. */
    usr_err = um_opener_port_tcpip_netif_get_cip_qos_nvdata( &p_instance_ctrl->tcpip_netif_ctrl, p_nvdata_cfg->p_qos_nvdata );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    /** Return success code. */
    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/** @} (end addtogroup UM_OPENER_PORT) */
