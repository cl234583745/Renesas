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
#include "um_lwip_port_internal.h"

#include "FreeRTOS.h"
// require including FreeRTOS.h before the following FreeRTOS resources.
#include "event_groups.h"
#include "queue.h"

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
/** Callback functions when initializing lwIP TCP/IP stack task. */
static void _tcpip_init_callback(void * arg);
static void _tcpip_task_sync_callback( void *ctx );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl					Pointer to the controller
 *
 * @retval FSP_SUCCESS					Process has been done successfully.
 * @retval FSP_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_core_open( lwip_port_core_ctrl_t * const p_ctrl )
{
    /** Set the handler of current task. */
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Power on lwIP and create task of lwIP whose mode is not NO_SYS. */
    (void) tcpip_init( (tcpip_init_done_fn) _tcpip_init_callback, (void*) p_ctrl);

    /** Wait for notification indicating the created task is initialized */
    (void) ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * Synchronize with TCPIP task.
 **********************************************************************************************************************/
usr_err_t um_lwip_port_core_task_sync( void )
{
    tcpip_callback(_tcpip_task_sync_callback, xTaskGetCurrentTaskHandle());
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * Callback function called by lwIP TCP/IP API tcpip_init() on TCP/IP stack task context.
 *
 * @param[in] p_event   Pointer to lwIP network interface structure
 **********************************************************************************************************************/
static void _tcpip_init_callback(void * arg)
{
    /** Resolve callback context pointer */
	lwip_port_core_ctrl_t * p_ctrl = arg;

    /** Get lwIP TCP/IP Task handler. */
    p_ctrl->p_tcpip_task_handle = xTaskGetCurrentTaskHandle();

    /** Notify the parenet task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );
}

/*******************************************************************************************************************//**
 * Synchronize with TCPIP task.
 *
 * @param[in]   ctx     Callback context.
 **********************************************************************************************************************/
static void _tcpip_task_sync_callback( void *ctx )
{
    /** Resolve context */
    TaskHandle_t sync_task = (TaskHandle_t) ctx;

    /** Notify the task asking the TCPIP task call this functions. */
    xTaskNotifyGive( sync_task );
}
