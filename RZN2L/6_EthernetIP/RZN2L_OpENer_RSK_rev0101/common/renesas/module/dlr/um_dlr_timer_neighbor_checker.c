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
#define DLR_NEIGHBOR_CHECK_TIMEOUT_DURATION	(100)          ///< 100 milliseconds */
#define DLR_NEIGHBOR_CHECK_TIMER_ONESHOT 	(pdFALSE)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
static void _timer_timeout_code( TimerHandle_t xTimer );

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
 * @brief Create the task and initialize its controller.
 *
 * @param p_ctrl
 * @param p_state_machine_ctrl
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_timer_neighbor_checker_open( dlr_neighbor_checker_ctrl_t * const p_ctrl,
											  dlr_executor_ctrl_t * const p_executor_ctrl )
{

	/** Store the pointers of related  */
	p_ctrl->p_executor_ctrl = p_executor_ctrl;

	/** Initialize attributes */
	p_ctrl->timeout_count0 = 0;
	p_ctrl->timeout_count1 = 0;

	/** Initialize timer callback argument. */
	p_ctrl->timer_callback_arg0.p_context = p_ctrl;
	p_ctrl->timer_callback_arg0.timedout_port = DLR_PORT0_BIT;
	p_ctrl->timer_callback_arg1.p_context = p_ctrl;
	p_ctrl->timer_callback_arg1.timedout_port = DLR_PORT1_BIT;

    /** Create timers for neighbor checking. */
    p_ctrl->p_timer0 = xTimerCreate("DLR NB Check Timer 1",
    						 	 	 DLR_NEIGHBOR_CHECK_TIMEOUT_DURATION,
									 DLR_NEIGHBOR_CHECK_TIMER_ONESHOT,
									 &p_ctrl->timer_callback_arg0,
									 (TimerCallbackFunction_t) _timer_timeout_code);
    USR_ERROR_RETURN( NULL != p_ctrl->p_timer0, USR_ERR_NOT_INITIALIZED );

    p_ctrl->p_timer1 = xTimerCreate("DLR NB Check Timer 2",
    	                             DLR_NEIGHBOR_CHECK_TIMEOUT_DURATION,
									 DLR_NEIGHBOR_CHECK_TIMER_ONESHOT,
									 &p_ctrl->timer_callback_arg1,
									 (TimerCallbackFunction_t) _timer_timeout_code);
    USR_ERROR_RETURN( NULL != p_ctrl->p_timer1, USR_ERR_NOT_INITIALIZED );

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the timers.
 *
 * @param[in] p_ctrl					Pointer to controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_ABORTED				Process has aborted.
 **********************************************************************************************************************/
usr_err_t um_dlr_timer_neighbor_checker_start( dlr_neighbor_checker_ctrl_t * const p_ctrl, dlr_port_t checked_port )
{
	/** Error codes */
	BaseType_t rtos_err;

	/** Start timer 0 */
	if( DLR_PORT0_BIT == checked_port || DLR_PORT_BOTH == checked_port )
	{
		rtos_err = xTimerStart( p_ctrl->p_timer0, portMAX_DELAY );
		USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_ABORTED );
	}

	/** Start timer 1 */
	if( DLR_PORT1_BIT == checked_port || DLR_PORT_BOTH == checked_port )
	{
		rtos_err = xTimerStart( p_ctrl->p_timer1, portMAX_DELAY);
		USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_ABORTED );
	}

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the timers.
 *
 * TODO: The neighbor check timers can be stopped by Ethernet switch interruption, but but its driver does not support currently.
 * > NOTE: Neighbor check time out timer (100 milliseconds) for neighbor check process should be implemented by the software.
 * > Software may take advantage of the stop neighbor check timeout timer bit of the interrupt status register to stop the timer.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_ABORTED				Process has aborted.
 **********************************************************************************************************************/
usr_err_t um_dlr_timer_neighbor_checker_stop( dlr_neighbor_checker_ctrl_t * const p_ctrl, dlr_port_t checked_port )
{
	/** Error codes */
	BaseType_t rtos_err;

	/** Stop timer 0 */
	if( DLR_PORT0_BIT == checked_port || DLR_PORT_BOTH == checked_port )
	{
		rtos_err = xTimerStop( p_ctrl->p_timer0, portMAX_DELAY );
		USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_ABORTED);
	}

	/** Stop timer 1 */
	if( DLR_PORT1_BIT == checked_port || DLR_PORT_BOTH == checked_port )
	{
		rtos_err = xTimerStop( p_ctrl->p_timer1, portMAX_DELAY );
		USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_ABORTED);
	}

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Clear timeout count.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_timer_neighbor_checker_clear_timeout_count( dlr_neighbor_checker_ctrl_t * const p_ctrl, dlr_port_t checked_port )
{
	/** Stop timer 0 */
	if( DLR_PORT0_BIT == checked_port || DLR_PORT_BOTH == checked_port )
	{
		p_ctrl->timeout_count0 = 0;
	}

	/** Stop timer 1 */
	if( DLR_PORT1_BIT == checked_port || DLR_PORT_BOTH == checked_port )
	{
		p_ctrl->timeout_count1 = 0;
	}

	return USR_SUCCESS;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Timeout callback of Neighbor check timer for port0.
 **********************************************************************************************************************/
void _timer_timeout_code( TimerHandle_t xTimer )
{
	/** Resolve Context */
	dlr_neighbor_check_timer_callback_arg_t * const p_callback_arg = ( dlr_neighbor_check_timer_callback_arg_t * const ) pvTimerGetTimerID( xTimer );
	dlr_neighbor_checker_ctrl_t * const p_ctrl = ( dlr_neighbor_checker_ctrl_t * const ) p_callback_arg->p_context;
	dlr_port_t timeout_port = p_callback_arg->timedout_port;

	/** Error codes */
	usr_err_t usr_err;

	/** DLR event argument */
	dlr_event_arg_t dlr_event;

	/** Update timeout counter. */
	switch ( timeout_port )
	{
	case DLR_PORT0_BIT:
	    p_ctrl->timeout_count0++;
	    dlr_event.freezed_timeout_count = p_ctrl->timeout_count0;
	    break;
	case DLR_PORT1_BIT:
	    p_ctrl->timeout_count1++;
	    dlr_event.freezed_timeout_count = p_ctrl->timeout_count1;
	    break;
	default: return;
	}

	/** Pack DLR event argument */
	dlr_event.event = DLR_EVENT_NEIGHBORTIMEOUT;
	dlr_event.event_port = timeout_port;
	dlr_event.p_ether_frame = NULL;

    /** Notify the executor of the DLR event. */
	usr_err = um_dlr_task_executor_receive_event( p_ctrl->p_executor_ctrl, &dlr_event, DLR_CONTEXT_RTOS_TIMER, DLR_ENQUEUE_BACK );
	if ( USR_SUCCESS != usr_err )
	{
		/**
		 * TODO: Error handling
		 */
	}
}
