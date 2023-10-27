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
#include "um_lwip_port_cfg.h"
#include "um_lwip_port.h"
#include "um_lwip_port_internal.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

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

/** Callback functions called by Ether middleware module functions. */
static usr_err_t _netif_frame_input( lwip_port_netif_ctrl_t * p_ctrl, void * p_context );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Create the task and initialize its controller.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[in] p_target_ctrl		Pointer to target controller
 * @param[in] p_callback_ctrl	Pointer to callback controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_task_receiver_open( lwip_port_receiver_ctrl_t * const p_ctrl,
										   lwip_port_netif_list_ctrl_t * const p_instance_manager_ctrl )
{
	/** Error codes */
	BaseType_t rtos_err;

	/** Set the related submodules. */
	p_ctrl->p_netif_list_ctrl = p_instance_manager_ctrl;
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Initialize Ethernet frame buffer. */
    p_ctrl->p_ether_netif_frame = NULL;

    /** Create frame T. */
    p_ctrl->p_queue_handle = xQueueCreate(UM_LWIP_PORT_CFG_RECEIVER_QUEUE_LENGTH, sizeof(ether_netif_frame_t *));
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_INITIALIZED);

	/** Create netif launcher task */
    rtos_err = xTaskCreate( _task_code,
                            UM_LWIP_PORT_CFG_RECEIVER_TASK_NAME,
                            UM_LWIP_PORT_CFG_RECEIVER_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_LWIP_PORT_CFG_RECEIVER_TASK_PRIORITY,
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
usr_err_t um_lwip_port_task_receiver_start( lwip_port_receiver_ctrl_t * const p_ctrl )
{
    /** Check if the resource is already enabled. */
    USR_ERROR_RETURN( NULL != p_ctrl->p_task_handle, USR_ERR_NOT_ENABLED );

    /** Resume task */
	vTaskResume(p_ctrl->p_task_handle);

	/** Return success */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @param[in] p_ctrl					Pointer to controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_task_receiver_stop( lwip_port_receiver_ctrl_t * const p_ctrl )
{
    /** Check if the resource is already enabled. */
    USR_ERROR_RETURN( NULL != p_ctrl->p_task_handle, USR_ERR_NOT_ENABLED );

    /** Suspend task */
	vTaskSuspend(p_ctrl->p_task_handle);

	/** Return success. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Request the RTOS task to input Ethernet frames to network interface of lwIP.
 *
 * @param[in] p_ctrl			Pointer to reader controller
 * @param[in] p_packet_buffer	Pointer to packet buffer.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_task_receiver_request( lwip_port_receiver_ctrl_t * const p_ctrl, ether_netif_frame_t * p_packet_buffer )
{
    /** Status */
    BaseType_t rtos_err;

    /** Check if the resource is already enabled. */
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_ENABLED );

    /** Send Ethernet frame. */
    rtos_err = xQueueSend(p_ctrl->p_queue_handle, &p_packet_buffer, 0);
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
    lwip_port_receiver_ctrl_t * const p_ctrl = (lwip_port_receiver_ctrl_t *) pvParameter;

    /** Status */
    BaseType_t rtos_err;
    usr_err_t usr_err; // FSP status

    /** Notify the parenet task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );
    
    while(true)
    {
        /** Notified from Ethernet Link callback. */
        rtos_err = xQueueReceive(p_ctrl->p_queue_handle, &p_ctrl->p_ether_netif_frame, portMAX_DELAY);
        if( ( pdPASS != rtos_err ) || ( NULL == p_ctrl->p_ether_netif_frame ) )
        {
        	/** TODO: Implement task assertion */
        	continue;
		}

        /** Broadcast the received to network interface */
		usr_err = um_lwip_port_netif_list_map(p_ctrl->p_netif_list_ctrl, _netif_frame_input, p_ctrl->p_ether_netif_frame);
		if (USR_SUCCESS != usr_err)
		{
			/** TODO: Implement error handling in callback context. */
		}

        /** Free memory of received packet. */
        USR_HEAP_RELEASE( p_ctrl->p_ether_netif_frame );
    }

    /** vTaskDelete(NULL); */
}

/*******************************************************************************************************************//**
 * Static function to received Ethernet frame into lwIP network interface.
 *
 * @param[in] p_netif   Pointer to lwIP network interface structure
 **********************************************************************************************************************/
static usr_err_t _netif_frame_input( lwip_port_netif_ctrl_t * p_ctrl, void * p_context )
{
	usr_err_t usr_err;

	/** Resolve context */
    ether_netif_frame_t * p_frame_packet = (ether_netif_frame_t *) p_context;

    usr_err = um_lwip_port_netif_input( p_ctrl, p_frame_packet );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, usr_err );

    return USR_SUCCESS;
}
