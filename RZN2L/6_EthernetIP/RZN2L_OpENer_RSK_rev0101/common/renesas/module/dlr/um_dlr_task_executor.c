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
/** Task functions to handling Ethernet input, output and link. */
static void _task_code (void * pvParameter);

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Create the task and initialize its controller.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_executor_open( dlr_executor_ctrl_t * const p_ctrl,
									 dlr_fsm_ctrl_t * const p_fsm_ctrl )
{
	BaseType_t rtos_err;

	/** Set the related submodules controllers. */
    p_ctrl->p_fsm_ctrl = p_fsm_ctrl;

    /** Save the current task handler to communicate the generated task when its initialization. */
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Create event queue. */
    p_ctrl->p_queue_handle = xQueueCreate(UM_DLR_CFG_EXECUTOR_QUEUE_LENGTH, sizeof(dlr_event_arg_t));
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_INITIALIZED);

    /** Create task. */
    rtos_err = xTaskCreate( _task_code,
    						UM_DLR_CFG_EXECUTOR_TASK_NAME,
                            UM_DLR_CFG_EXECUTOR_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_DLR_CFG_EXECUTOR_TASK_PRIORITY,
                            &(p_ctrl->p_task_handle) );
    USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_NOT_INITIALIZED );

    /** Wait for notification indicating the created task is initialized. */
    (void) ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

    /** Suspend the created task. */
    vTaskSuspend( p_ctrl->p_task_handle );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the task.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_executor_start( dlr_executor_ctrl_t * const p_ctrl )
{
	vTaskResume(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_executor_stop( dlr_executor_ctrl_t * const p_ctrl )
{
	vTaskSuspend(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Enqueue the DLR event to pass it to the RTOS task.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_executor_receive_event( dlr_executor_ctrl_t * const p_ctrl,
											  dlr_event_arg_t * const p_dlr_event,
											  dlr_context_type_t context_type,
											  dlr_enqueue_type_t enqueue_type
											  )
{
    /** Status */
    BaseType_t rtos_err;

    /** Indicate whether the task waiting for queue receiving is unblocked by queue sending or not.  */
    BaseType_t higher_priority_task_woken;

    /** Check if the event issued from ISR context. */
    if ( DLR_CONTEXT_ISR == context_type )
    {
    	if ( DLR_ENQUEUE_FRONT == enqueue_type )
    	{
            /** Send DLR event. */
            rtos_err = xQueueSendToFrontFromISR(p_ctrl->p_queue_handle, p_dlr_event, &higher_priority_task_woken );
    	}
    	else
    	{
            /** Send DLR event. */
            rtos_err = xQueueSendFromISR(p_ctrl->p_queue_handle, p_dlr_event, &higher_priority_task_woken );
    	}
        USR_ERROR_RETURN( rtos_err == pdTRUE, USR_ERR_IN_USE );

        /** Require the context switch to the unblocked task by sending queue if the unblocked task has the higher priority than the task works before ISR is called. */
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
    else /** In RTOS context (task or timer )*/
    {
    	if ( DLR_ENQUEUE_FRONT == enqueue_type )
    	{
            /** Send DLR event. */
            rtos_err = xQueueSendToFront(p_ctrl->p_queue_handle, p_dlr_event, portMAX_DELAY);
    	}
    	else
    	{
            /** Send DLR event. */
            rtos_err = xQueueSend(p_ctrl->p_queue_handle, p_dlr_event, portMAX_DELAY);
    	}
        USR_ERROR_RETURN( rtos_err == pdTRUE, USR_ERR_IN_USE );
    }

    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief RTOS task.
 *
 * @param[in] pvParameter   	Pointer to task parameters.
 **********************************************************************************************************************/
static void _task_code (void * pvParameter)
{
    /** Resolve task parameter */
    dlr_executor_ctrl_t * const p_ctrl = (dlr_executor_ctrl_t * const) pvParameter;

	/** Error codes */
    usr_err_t usr_err;
    BaseType_t rtos_err;

    /** DLR event received event queue. */
    dlr_event_arg_t dlr_event;

    /** Notify the parent task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    while( 1 )
    {
        /** Get event from queue */
        rtos_err = xQueueReceive(p_ctrl->p_queue_handle, &dlr_event, portMAX_DELAY);
        if ( pdPASS != rtos_err )
        {
        	/** Error handling. */
        }

        /** Execute the state machine with event. */
        usr_err = um_dlr_fsm_execute_with_event(p_ctrl->p_fsm_ctrl, &dlr_event);
        if ( USR_SUCCESS != usr_err )
        {
        	/** Error handling. */
            USR_HEAP_RELEASE(dlr_event.p_ether_frame);
            continue;
        }

        /** Free the memory of Ethernet frame */
        USR_HEAP_RELEASE(dlr_event.p_ether_frame);
    }
    /** vTaskDelete(NULL); */
}
