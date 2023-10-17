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
#include "um_opener_port_api.h"
#include "um_opener_port_cfg.h"
#include "um_opener_port.h"
#include "um_opener_port_internal.h"
#include "um_opener_port_feature.h"
#include "um_common_api.h"

#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
#include "um_dlr_api.h"

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
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_opener_port_dlr_open( opener_port_dlr_ctrl_t * const p_ctrl, dlr_instance_t const * const p_dlr_instance )
{
	/** Error codes*/
	usr_err_t usr_err;

    /** Set target module instance. */
	p_ctrl->p_dlr_instance = p_dlr_instance;

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start target module.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_dlr_start( opener_port_dlr_ctrl_t * const p_ctrl )
{
	/** Error codes. */
	usr_err_t usr_err;

	/** Start target module */
	usr_err = p_ctrl->p_dlr_instance->p_api->start(p_ctrl->p_dlr_instance->p_ctrl);
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop target module.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_dlr_stop( opener_port_dlr_ctrl_t * const p_ctrl )
{
	/** Error codes. */
	usr_err_t usr_err;

	/** Stop target module. */
	usr_err = p_ctrl->p_dlr_instance->p_api->stop(p_ctrl->p_dlr_instance->p_ctrl);
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_SUCCESS );

    /** Return success code. */
	return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

#endif
