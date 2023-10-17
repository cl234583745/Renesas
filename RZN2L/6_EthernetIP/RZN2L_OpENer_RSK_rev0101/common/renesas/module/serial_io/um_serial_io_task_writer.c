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
#include "um_serial_io.h"
#include "um_serial_io_cfg.h"
#include "um_serial_io_internal.h"

/** For FSP error codes */
#include "fsp_common_api.h"

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
static void _task_code(void * pvParameter);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static void * ps_context = NULL;

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl				Pointer to the controller
 * @param[in] p_uart_instance		Pointer to the instance of uart interface
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_serial_io_task_writer_open(serial_io_writer_ctrl_t * const p_ctrl, serial_io_uart_ctrl_t * const p_uart_ctrl )
{
	/** Error codes */
	BaseType_t rtos_err;

	/** Set the current task as parent task. */
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Initialize the buffer pointer */
    p_ctrl->p_data_buffer = NULL;

    /** Set the pointer to related module. */
    p_ctrl->p_uart_ctrl = p_uart_ctrl;

    /** Set the context for overriding the write function. */
    ps_context = p_ctrl;

    /** Create queue. */
    p_ctrl->p_queue_handle = xQueueCreate(UM_SERIAL_IO_CFG_WRITER_QUEUE_LENGTH, sizeof(serial_io_data_t *));
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_INITIALIZED);

	/** Create task. */
    rtos_err = xTaskCreate( _task_code,
                            UM_SERIAL_IO_CFG_WRITER_TASK_NAME,
                            UM_SERIAL_IO_CFG_WRITER_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_SERIAL_IO_CFG_WRITER_TASK_PRIORITY,
                            &(p_ctrl->p_task_handle) );
    USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_NOT_INITIALIZED );

    /** Wait for notification indicating the created task is initialized. */
    (void) ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

    /** Suspend the created task. */
    vTaskSuspend( p_ctrl->p_task_handle );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close the controller.
 *
 * @param[in] p_ctrl				Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_serial_io_task_writer_close(serial_io_writer_ctrl_t * const p_ctrl )
{
	/** Error codes */
    (void) p_ctrl;

	/**
	 * TODO: Implement the sequence to close.
	 */

    /** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the task.
 *
 * @param[in] p_ctrl					Pointer to controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_serial_io_task_writer_start( serial_io_writer_ctrl_t * const p_ctrl )
{
	vTaskResume(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @param[in] p_ctrl					Pointer to controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_serial_io_task_writer_stop( serial_io_writer_ctrl_t * const p_ctrl )
{
	vTaskSuspend(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Request the RTOS task to output the character.
 *
 * @param[in] p_ctrl			Pointer to reader controller
 * @param[in] p_packet_buffer	Pointer to packet buffer.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_serial_io_task_writer_request( serial_io_writer_ctrl_t * const p_ctrl, serial_io_data_t * p_data )
{
    /** Status */
    BaseType_t rtos_err;

    /** Send data buffer. */
    rtos_err = xQueueSend(p_ctrl->p_queue_handle, &p_data, 0);
    if( rtos_err != pdPASS )
    {
    	/** Free requested frame */
    	USR_HEAP_RELEASE( p_data );
    	return USR_ERR_IN_USE;
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
	serial_io_writer_ctrl_t * p_ctrl = (serial_io_writer_ctrl_t * ) pvParameter;

    /** Status */
    BaseType_t rtos_err;
    usr_err_t usr_err;

    /** Notify the parent task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    /** Task loop */
    while(true)
    {
        /** Get data buffer, to be written, from queue. */
        rtos_err = xQueueReceive(p_ctrl->p_queue_handle, &p_ctrl->p_data_buffer, portMAX_DELAY);
        if( ( pdPASS != rtos_err ) || ( NULL == p_ctrl->p_data_buffer ) )
        {
        	continue;
		}

        /** Write the buffer via uart interface. */
        usr_err = um_serial_io_uart_write(p_ctrl->p_uart_ctrl, p_ctrl->p_data_buffer);
        if ( USR_SUCCESS != usr_err )
        {
            continue;
        }

        /** Release memory of data buffer */
        USR_HEAP_RELEASE( p_ctrl->p_data_buffer );
    }
    /** vTaskDelete(NULL); */
}

/*******************************************************************************************************************//**
 * @brief Override write function of standard library.
 **********************************************************************************************************************/
#if defined(UM_SERIAL_IO_CFG_OVERRIDE_PRINTF_FUNCTION) && UM_SERIAL_IO_CFG_OVERRIDE_PRINTF_FUNCTION
#include <stdarg.h>
#include <stdio.h>
int printf (const char *__restrict format, ...)
{
    /** Resolve context */
    serial_io_writer_ctrl_t * p_ctrl = ps_context;
    serial_io_data_t * p_data = NULL;
    int32_t ret_len;

    /** Allocate memory */
    USR_HEAP_ALLOCATE( p_data, sizeof(serial_io_data_t) );

    /** Resolve formats */
    va_list args;
    va_start (args, format);
    ret_len = vsprintf( (char *) p_data->p_buffer, format, args);

    /** Check the error */
    if( ret_len < 0 )
    {
        return ret_len;
    }

    /** Request the task to write buffer. */
    p_data->length = (uint32_t) ret_len;
    (void) um_serial_io_task_writer_request( p_ctrl, p_data );

    /** Close the va_list variable. */
    va_end(args);

    /** return the length. */
    return (int32_t) p_data->length;
}
#endif
