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
#include "um_common_api.h"
#include "um_common_cfg.h"
#include "um_opener_port.h"
#include "um_opener_port_api.h"
#include "um_opener_port_internal.h"

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
usr_err_t um_opener_port_callback_open( opener_port_callback_ctrl_t * const p_ctrl )
{
	/** Create the mutex */
	p_ctrl->p_mutex_handle = xSemaphoreCreateMutex();
	USR_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_ERR_NOT_INITIALIZED );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Specify the callback function and the argument memory.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[in] p_callback		Pointer to the callback function to be set.
 * @param[in] p_context			Pointer to the callback context to be set.
 * @param[in] p_memory			Pointer to the callback memory to be set.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_opener_port_callback_add( opener_port_callback_ctrl_t * const p_ctrl, opener_port_callback_link_node_t * p_new_nodes )
{
    opener_port_callback_link_node_t * p_node;

    /** Lock section */
    USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
    {
        /** Set callback by adding callback node into link list. */
        if ( NULL == p_ctrl->p_head_node )
        {
            /** Set the nodes as head node. */
            p_ctrl->p_head_node = p_new_nodes;
        }
        else
        {
            /** Add the new nodes after tail node */
            p_ctrl->p_tail_node->p_next = p_new_nodes;
        }

        /** Scan last node until next node is null. */
        for( p_node = p_ctrl->p_head_node; p_node->p_next != NULL; p_node = p_node->p_next );

        /** Update tail node */
        p_ctrl->p_tail_node = p_node;
    }
    USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Request callback with specifying event and arguments.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[in] event				Event to be set to callback memory.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_opener_port_callback_request( opener_port_callback_ctrl_t * const p_ctrl,
										   opener_port_callback_event_t event,
										   opener_port_indicator_status_t indicator_status)
{
    /** Pointer for iterating node of link list. */
    opener_port_callback_link_node_t * p_node;

    /** Lock section */
    USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
    {
        /** Iterate along link list */
        for ( p_node = p_ctrl->p_head_node; p_node != NULL; p_node = p_node->p_next )
        {
            /** Set callback arguments with event. */
            p_node->p_memory->event = event;
            p_node->p_memory->indicator_status = indicator_status;
            p_node->p_memory->p_context = p_node->p_context;

            /** Execute callback function with the argument. */
            (void) p_node->p_func( p_node->p_memory );
        }
    }
    USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

	return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
