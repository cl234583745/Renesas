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
#include "um_lwip_port.h"
#include "um_lwip_port_cfg.h"
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
static usr_err_t _netif_link_on( lwip_port_netif_ctrl_t * p_ctrl, void * p_context );
static usr_err_t _netif_link_off( lwip_port_netif_ctrl_t * p_ctrl, void * p_context );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Create the task and initialize its controller.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[in] p_netif_list_ctrl	Pointer to callback controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_task_launcher_open( lwip_port_launcher_ctrl_t * const p_ctrl,
										   lwip_port_netif_list_ctrl_t * const p_netif_list_ctrl )
{
	BaseType_t rtos_err;

	/** Set the related submodules. */
	p_ctrl->p_netif_list_ctrl = p_netif_list_ctrl;
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Create frame T. */
    p_ctrl->p_queue_handle = xQueueCreate(UM_LWIP_PORT_CFG_LAUNCHER_QUEUE_LENGTH, sizeof(lwip_port_launcher_request_t));
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_INITIALIZED);

	/** Create netif launcher task */
    rtos_err = xTaskCreate( _task_code,
                            UM_LWIP_PORT_CFG_LAUNCHER_TASK_NAME,
                            UM_LWIP_PORT_CFG_LAUNCHER_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_LWIP_PORT_CFG_LAUNCHER_TASK_PRIORITY,
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
usr_err_t um_lwip_port_task_launcher_start( lwip_port_launcher_ctrl_t * const p_ctrl )
{
    /** Check if the resource is already enabled. */
    USR_ERROR_RETURN( NULL != p_ctrl->p_task_handle, USR_ERR_NOT_ENABLED );

    /** Resume task. */
	vTaskResume(p_ctrl->p_task_handle);

	/** Return error. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @param[in] p_ctrl					Pointer to controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_task_launcher_stop( lwip_port_launcher_ctrl_t * const p_ctrl )
{
    /** Check if the resource is already enabled. */
    USR_ERROR_RETURN( NULL != p_ctrl->p_task_handle, USR_ERR_NOT_ENABLED );

    /** Suspend task. */
	vTaskSuspend(p_ctrl->p_task_handle);

    /** Return error. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Request the RTOS task to launch lwIP network interface
 *
 * @param[in] p_ctrl			Pointer to reader controller
 * @param[in] event			Ethernet link callback event.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_task_launcher_request( lwip_port_launcher_ctrl_t * const p_ctrl,
                                              lwip_port_launcher_event_t const event,
                                              lwip_port_netif_ctrl_t * const p_netif_ctrl)
{
    /** Status */
    BaseType_t rtos_err;

    /** Check if the resource is already enabled. */
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_ENABLED );

    /** Luncher request */
    lwip_port_launcher_request_t request;
    request.event = event;
    request.p_netif_ctrl = p_netif_ctrl;

    /** Send Event. */
    rtos_err = xQueueSend(p_ctrl->p_queue_handle, &request, 0);
    USR_ERROR_RETURN( rtos_err == pdTRUE, USR_ERR_IN_USE );

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
    lwip_port_launcher_ctrl_t * const p_ctrl = (lwip_port_launcher_ctrl_t *) pvParameter;

    /** Status */
    BaseType_t rtos_err;
    usr_err_t usr_err; // FSP status

    /** Luncher event */
    lwip_port_launcher_request_t request;

    /** Notify the parenet task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    while(true)
    {
        /** Notified from Ethernet Link callback. */
        rtos_err = xQueueReceive(p_ctrl->p_queue_handle, &request, portMAX_DELAY);
        if( pdPASS != rtos_err )
        {
        	/** TODO: Implement task assertion */
		}

        switch( request.event )
        {
        case LWIP_PORT_LAUNCHER_EVENT_LINK_UP:
            usr_err = um_lwip_port_netif_list_map(p_ctrl->p_netif_list_ctrl, _netif_link_on, NULL);
    		if( USR_SUCCESS != usr_err)
    		{
    			/** TODO: Implement error handling in task. */
    		}
    		break;

        case LWIP_PORT_LAUNCHER_EVENT_LINK_DOWN:
        	usr_err = um_lwip_port_netif_list_map(p_ctrl->p_netif_list_ctrl, _netif_link_off, NULL);
    		if (USR_SUCCESS != usr_err)
    		{
    			/** TODO: Implement error handling in task. */
    		}
    		break;

        case LWIP_PORT_LAUNCHER_EVENT_IP_UP:
            if( request.p_netif_ctrl )
            {
                usr_err = um_lwip_port_netif_set_netif_state(request.p_netif_ctrl, LWIP_PORT_NETIF_STATE_UP);
                if (USR_SUCCESS != usr_err)
                {
                    /** TODO: Implement error handling in task. */
                }
            }
            break;

        case LWIP_PORT_LAUNCHER_EVENT_IP_DOWN:
            if( request.p_netif_ctrl )
            {
                usr_err = um_lwip_port_netif_set_netif_state(request.p_netif_ctrl, LWIP_PORT_NETIF_STATE_DOWN);
                if (USR_SUCCESS != usr_err)
                {
                    /** TODO: Implement error handling in task. */
                }
            }
            break;

        default:
        	/** TODO: Implement error handling in task. */
        	break;
        }
    }

    /** vTaskDelete(NULL); */
}

/*******************************************************************************************************************//**
 * Process for link up process when the ethernet link state becomes down from up.
 *
 * @param[in] 	p_instance_ctrl		Pointer to instance control.
 *
 * @retval
 **********************************************************************************************************************/
static usr_err_t _netif_link_on( lwip_port_netif_ctrl_t * p_ctrl, void * p_context )
{
	/** No context */
	(void) p_context;

	/** Status */
    usr_err_t usr_err;

    /** Set the network interface to up */
    usr_err = um_lwip_port_netif_set_up( p_ctrl );

    return usr_err;
}

/*******************************************************************************************************************//**
 * Process for link down process when the Ethernet link state becomes down from up.
 *
 * @param[in] 	p_instance_ctrl		Pointer to instance control.
 *
 * @retval
 **********************************************************************************************************************/
static usr_err_t _netif_link_off( lwip_port_netif_ctrl_t * p_ctrl, void * p_context )
{
    (void) p_context;

    /** Status */
    usr_err_t usr_err;

    /** Set the network interface to down */
    usr_err = um_lwip_port_netif_set_down( p_ctrl );

	return usr_err;
}
