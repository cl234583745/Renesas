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
#include "um_ether_netif.h"
#include "um_ether_netif_cfg.h"
#include "um_ether_netif_internal.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Ethernet frame size restrictions */
#define ETHER_FRAME_MAXIMUM_BYTES        ( 1514U )   // without CRC 4byte
#define ETHER_FRAME_MINIMUM_BYTES        ( 60U )     // without CRC 4byte

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void _task_code(void * pvParameter);

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Create the task and initialize its controller.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[in] p_target_ctrl		Pointer to target controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_task_writer_open( ether_netif_writer_ctrl_t * const p_ctrl,
										   ether_netif_ether_ctrl_t * const p_target_ctrl )
{
	BaseType_t rtos_err;

	/** Set target controller. */
	p_ctrl->p_ether_ctrl = p_target_ctrl;
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();
    p_ctrl->p_ether_netif_frame = NULL;

    /** Create frame TX queue. */
    p_ctrl->p_queue_handle = xQueueCreate(UM_ETHER_NETIF_CFG_WRITER_QUEUE_LENGTH, sizeof(ether_netif_frame_t *));
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_INITIALIZED);

	/** Create the task. */
    rtos_err = xTaskCreate(_task_code,
                           UM_ETHER_NETIF_CFG_WRITER_TASK_NAME ,
                           UM_ETHER_NETIF_CFG_WRITER_TASK_STACK_BYTE_SIZE / sizeof(StackType_t),
                           p_ctrl,
                           UM_ETHER_NETIF_CFG_WRITER_TASK_PRIORITY,
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
 * @param[in] p_ctrl					Pointer to controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_task_writer_start( ether_netif_writer_ctrl_t * const p_ctrl )
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
usr_err_t um_ether_netif_task_writer_stop( ether_netif_writer_ctrl_t * const p_ctrl )
{
	vTaskSuspend(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Request the RTOS task to write the Ethernet frame into buffer register.
 *
 * @param[in] p_ctrl			Pointer to reader controller
 * @param[in] p_packet_buffer	Pointer to packet buffer.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_task_writer_request( ether_netif_writer_ctrl_t * const p_ctrl, ether_netif_frame_t * p_packet_buffer )
{
    /** Status */
    BaseType_t rtos_err;

    /** Send Ethernet frame. */
    rtos_err = xQueueSend(p_ctrl->p_queue_handle, &p_packet_buffer, 0); // No wait
    if( rtos_err != pdPASS )
    {
        /** Free requested frame */
        USR_HEAP_RELEASE( p_packet_buffer );
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
	ether_netif_writer_ctrl_t * p_ctrl = (ether_netif_writer_ctrl_t * ) pvParameter;

    /** Status */
    BaseType_t rtos_err;
    usr_err_t usr_err;

    /** Adjusted output length */
    uint32_t message_length;

    /** Notify the parenet task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    /** Task loop */
    while(true)
    {
        /** Get Tx frame from queue. */
        rtos_err = xQueueReceive(p_ctrl->p_queue_handle, &p_ctrl->p_ether_netif_frame, portMAX_DELAY);
        if( ( pdPASS != rtos_err ) || ( NULL == p_ctrl->p_ether_netif_frame ) )
        {
        	/** TODO: Implement task assertion */
        	continue;
		}

        /** The length of frame should be over minimum byte size of Ethernet frame.*/
        message_length = p_ctrl->p_ether_netif_frame->length;
        if ( message_length < ETHER_FRAME_MINIMUM_BYTES )
        {
            memset( p_ctrl->p_ether_netif_frame->p_buffer + message_length , 0, ETHER_FRAME_MINIMUM_BYTES - message_length);
            message_length = ETHER_FRAME_MINIMUM_BYTES;
        }

        /** Send via Ethernet interface with Lock */
        usr_err = um_ether_netif_ether_write( p_ctrl->p_ether_ctrl, p_ctrl->p_ether_netif_frame );
        if( USR_SUCCESS != usr_err )
        {
        	/** TODO: Implement task assertion */
        }

        /** Free memory of sent packet. */
        USR_HEAP_RELEASE( p_ctrl->p_ether_netif_frame );
    }
    /** vTaskDelete(NULL); */
}
