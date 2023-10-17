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
#include "FreeRTOS.h"
#include "um_lwip_port_internal.h"
#include "um_lwip_port_cfg.h"

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
usr_err_t um_lwip_port_netif_list_open( lwip_port_netif_list_ctrl_t * const p_ctrl )
{
	BaseType_t rtos_err;

	/** Create the mutex */
	p_ctrl->p_mutex_handle = xSemaphoreCreateMutex();
	USR_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_ERR_NOT_INITIALIZED );

	/** Lock the mutex. */
	rtos_err = xSemaphoreTake(p_ctrl->p_mutex_handle, portMAX_DELAY);
	USR_ERROR_RETURN( pdTRUE == rtos_err, USR_ERR_IN_USE );

	/** Initialize chain list of instance */
	p_ctrl->p_head_ctrl = NULL;
	p_ctrl->p_tail_ctrl = NULL;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Add an instance.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_IN_USE				Resource is busy currently.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_netif_list_add( lwip_port_netif_list_ctrl_t * const p_ctrl, lwip_port_netif_ctrl_t * const p_netif_ctrl )
{
	BaseType_t rtos_err;
	usr_err_t usr_err;

	/** Lock the mutex. */
	rtos_err = xSemaphoreTake(p_ctrl->p_mutex_handle, portMAX_DELAY);
	USR_ERROR_RETURN( pdTRUE == rtos_err, USR_ERR_IN_USE );

	/** For initial instances */
    if( NULL == p_ctrl->p_head_ctrl )
    {
        /** Set head and tail instance to each pointers. */
    	p_ctrl->p_head_ctrl = p_netif_ctrl;
        p_ctrl->p_tail_ctrl = p_netif_ctrl;

        /** Set the network interface as default  */
    	usr_err = um_lwip_port_netif_set_default(p_netif_ctrl);
    }
    /** For following instances */
    else
    {
        /** Only update instance list following the tail. */
    	p_ctrl->p_tail_ctrl->p_next = p_netif_ctrl;
    	p_ctrl->p_tail_ctrl = p_netif_ctrl;
    }

    /** Unlock the mutex */
    rtos_err = xSemaphoreGive(p_ctrl->p_mutex_handle);
    USR_ERROR_RETURN( pdTRUE == rtos_err, USR_SUCCESS /** Already unlocked */ );

    /** Error check between lock. */
	USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Lock updating instance chain list
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_IN_USE				Resource is busy currently.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_netif_list_lock( lwip_port_netif_list_ctrl_t * const p_ctrl )
{
	BaseType_t rtos_err;

	/** Lock the mutex. */
	rtos_err = xSemaphoreTake(p_ctrl->p_mutex_handle, portMAX_DELAY);
	USR_ERROR_RETURN( pdTRUE == rtos_err, USR_ERR_IN_USE );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Unlock updating instance chain list
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_IN_USE				Resource is busy currently.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_netif_list_unlock( lwip_port_netif_list_ctrl_t * const p_ctrl )
{
	BaseType_t rtos_err;

	/** Unlock the mutex. */
    rtos_err = xSemaphoreGive(p_ctrl->p_mutex_handle);
    USR_ERROR_RETURN( pdTRUE == rtos_err, USR_SUCCESS /** Already unlocked */ );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Map the process to all instance managed by instance manager.
 *
 * This function can be used after the update of instance chain list is locked.
 *
 * @param[in] p_ctrl				Pointer to the controller
 * @param[in] p_callback			Pointer to callback function to be mapped to all instance managed by instance manager.
 * @param[in] p_context				Pointer to callback context to be passed the callback function as an argument.
 * 
 * @retval USR_SUCCESS				Process has been done successfully.
 * @retval USR_ERR_IN_USE			Resource is busy currently.
 * @retval USR_ERR_NOT_ENABLED		Some errors are detected in the map process.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_netif_list_map( lwip_port_netif_list_ctrl_t * const p_ctrl,
									   usr_err_t (* p_callback)( lwip_port_netif_ctrl_t * p_netif_ctrl, void * p_context ),
									   void * p_context )
{
	BaseType_t rtos_err;
	usr_err_t usr_err;
	usr_err_t usr_err_inst;

	/** Try locking the mutex. */
	rtos_err = xSemaphoreTake(p_ctrl->p_mutex_handle, 0);
	if ( pdTRUE == rtos_err )
	{
		/** Give the mutex and return error because this mutex is not locked yet. */
		rtos_err = xSemaphoreGive(p_ctrl->p_mutex_handle);
		return USR_ERR_IN_USE;
	}

	/** Alias pointer */
	lwip_port_netif_ctrl_t * p_instance_ctrl;

    /** Scan the all network interface. */
    for( p_instance_ctrl = p_ctrl->p_head_ctrl;
         p_instance_ctrl != NULL;
         p_instance_ctrl = p_instance_ctrl->p_next )
    {
    	usr_err_inst = p_callback( p_instance_ctrl, p_context );
    	if ( USR_SUCCESS != usr_err_inst )
    	{
    		usr_err = USR_ERR_NOT_ENABLED;
    	}
    }
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
