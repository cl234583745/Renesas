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
#include "um_common_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Intervals of indicator flashing. */
#define INDICATOR_FLASHING_INTERVAL_MS        (500)
#define INDICATOR_FLASHING_INTERVAL_TICKS	  (TickType_t)(INDICATOR_FLASHING_INTERVAL_MS * ((float) configTICK_RATE_HZ / (float) 1000) )
#define INDICATOR_SELF_TEST_DELAY_MS          (250)
#define INDICATOR_SELF_TEST_DELAY_TICKS       (TickType_t)(INDICATOR_SELF_TEST_DELAY_MS * (float)(configTICK_RATE_HZ) / (float)(1000) )

#define OPENER_PORT_MODULE_STATE_NUM (6)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void _task_code (void * pvParameter);
static usr_err_t _exec_self_test_sequence( opener_port_indicator_ctrl_t * const p_ctrl );
usr_err_t _check_module_state_transitable( opener_port_module_state_t current_state, opener_port_module_state_t new_state );
usr_err_t _check_network_state_transitable( opener_port_network_state_t current_state, opener_port_network_state_t new_state );

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/**
 * Vol1 3.30 5A-2.7 Behavior
 */
static const opener_port_indicator_status_t g_module_state_indicator_map[OPENER_PORT_MODULE_STATE_NUM] =
{
    [OPENER_PORT_MODULE_STATE_NO_POWER] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    },
    [OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_ON,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    },
    [OPENER_PORT_MODULE_STATE_STANDBY] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_FLASHING,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    },
    [OPENER_PORT_MODULE_STATE_MAJOR_RECOVERABLE_FAULT] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red = OPENER_PORT_INDICATOR_STATE_FLASHING,
    },

    [OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_ON,
    },
    [OPENER_PORT_MODULE_STATE_SELF_TEST] =
    {
         /* indicates the state after self-testing */
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_ON,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    }
};

/**
 * Vol2 1.27 9-4.3.3 States
 */
static const opener_port_indicator_status_t g_network_state_indicator_map[OPENER_PORT_NET_STATE_NUM] =
{
    [OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red   = OPENER_PORT_INDICATOR_STATE_STEADY_OFF
    },
    [OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_FLASHING,
        .red   = OPENER_PORT_INDICATOR_STATE_STEADY_OFF
    },
    [OPENER_PORT_NETWORK_STATE_CONNECTED] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_ON,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_OFF
    },
    [OPENER_PORT_NETWORK_STATE_CONECTION_TIMEOUT] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red = OPENER_PORT_INDICATOR_STATE_FLASHING
    },
    [OPENER_PORT_NETWORK_STATE_DUPLICATE_IP] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_ON
    },

    /** indicates the state after self-testing */
    [OPENER_PORT_NETWORK_STATE_SELF_TEST] =
    {
        .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
        .red = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    }
};

/***********************************************************************************************************************
 * Global Variables
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
usr_err_t um_opener_port_task_indicator_open( opener_port_indicator_ctrl_t * const p_ctrl,
                                              opener_port_callback_ctrl_t * const p_callback_ctrl,
                                              opener_port_cip_ctrl_t * const p_cip_ctrl )
{
	/** Error codes */
	BaseType_t rtos_err;

	/** Set internal submodule controller */
	p_ctrl->p_callback_ctrl = p_callback_ctrl;
	p_ctrl->p_cip_ctrl = p_cip_ctrl;

    /** Save the current task handler to communicate the generated task when its initialization. */
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();

    /** Initialize indicator status */
    p_ctrl->module_state = OPENER_PORT_MODULE_STATE_NO_POWER;
    p_ctrl->network_state = OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS;

    /** Create task. */
    rtos_err = xTaskCreate( _task_code,
    						UM_OPENER_PORT_CFG_INDICATOR_TASK_NAME,
                            UM_OPENER_PORT_CFG_INDICATOR_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_OPENER_PORT_CFG_INDICATOR_TASK_PRIORITY,
                            &(p_ctrl->p_task_handle) );
    USR_ERROR_RETURN( pdPASS == rtos_err, USR_ERR_NOT_INITIALIZED );

    /** Wait for notification indicating the created task is initialized. */
    (void) ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

    /** Suspend the created task. */
    vTaskSuspend( p_ctrl->p_task_handle );

    /** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Start the task.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_indicator_start( opener_port_indicator_ctrl_t *	const p_ctrl )
{
    /** Error code */
    usr_err_t usr_err;

    /** Transit to self test sequence state . */
    usr_err = um_opener_port_task_indicator_set_module_state( p_ctrl, OPENER_PORT_MODULE_STATE_SELF_TEST );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    usr_err = um_opener_port_task_indicator_set_network_state( p_ctrl, OPENER_PORT_NETWORK_STATE_SELF_TEST );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Execute test sequence. */
    usr_err = _exec_self_test_sequence( p_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Module state transits to standby state. */
    usr_err = um_opener_port_task_indicator_set_module_state( p_ctrl, OPENER_PORT_MODULE_STATE_STANDBY );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED);

    /** Network state transit to no powered no ip address state  */
    usr_err = um_opener_port_task_indicator_set_network_state( p_ctrl, OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Network state transit to no connections state because ip address is already set in this stack on this stack initialization sequence. */
    usr_err = um_opener_port_task_indicator_set_network_state( p_ctrl, OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Resume task */
	vTaskResume(p_ctrl->p_task_handle);

	/** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_indicator_stop( opener_port_indicator_ctrl_t * const p_ctrl )
{
	vTaskSuspend(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get a current network state.
 *
 * @retval USR_SUCCESS                  Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_opener_port_task_indicator_get_network_state( opener_port_indicator_ctrl_t * const p_ctrl,
                                                           opener_port_network_state_t * const p_network_state )
{
    /** Error code */
    usr_err_t usr_err = USR_ERR_ABORTED;

    /** Return network status */
    * p_network_state = p_ctrl->network_state;

    return usr_err;
}

/***********************************************************************************************************************
 * @brief Set a new network state to the module status.
 *
 * This function can transits the current state to the new state, and return the OK or Error value indicating
 * if the transition with the state machine defined in Vol2 1.20 Figure 9-4.2 Network Status Indicator State Diagram.
 *
 * @param  new_net_state    A new network state
 * @retval USR_SUCCESS      The transitions complies with the state machine.
 *                          Return this if the new state is same as the current state also.
 * @retval USR_ERR_ABORTED  The transitions does not comply with the state machine.
 *                          Return this if the current state is RM_OPENER_MS_MAJOR_UNRECOVERABLE_FAULT also.
 ***********************************************************************************************************************/
usr_err_t um_opener_port_task_indicator_set_network_state( opener_port_indicator_ctrl_t * const p_ctrl,
                                                           opener_port_network_state_t network_state )
{
    /** Error code */
    usr_err_t usr_err = USR_ERR_ABORTED;

    /** If the new state is same as the current state, return from this function with OK status. */
    USR_ERROR_RETURN( p_ctrl->network_state != network_state, USR_SUCCESS );

    /** If the state transition is NOT correct, return error code.*/
    usr_err = _check_network_state_transitable( p_ctrl->network_state, network_state );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Apply new state to identity object attribute 5. */
    usr_err = um_opener_port_cip_set_device_status_from_network_state(p_ctrl->p_cip_ctrl, network_state );

    /** Apply new state to current status. */
    p_ctrl->network_state = network_state;

    return usr_err;
}


/***********************************************************************************************************************
 * @brief Set a new module state to the module status.
 *
 * This function can transits the current state to the new state, and return the OK or Error value indicating
 * if the transition with the state machine defined in Vol1 3.30 Figure 5A-2.2 State Transition Diagram for Identity Object.
 *
 * @param  state            A new module state
 * @retval USR_SUCCESS      The transitions complies the state machine.
 *                          Return this if the new state is same as the current state also.
 * @retval USR_ERR_ABORTED  Failed to transit to new state.
 *                          Return this if the current state is RM_OPENER_MS_MAJOR_UNRECOVERABLE_FAULT also.
 ***********************************************************************************************************************/
usr_err_t um_opener_port_task_indicator_set_module_state( opener_port_indicator_ctrl_t * const p_ctrl,
                                                          opener_port_module_state_t module_status )
{
    /** Error code */
    usr_err_t usr_err = USR_ERR_ABORTED;

    /** If the new state is same as the current state, return from this function with OK usr_err. */
    USR_ERROR_RETURN( p_ctrl->module_state != module_status, USR_SUCCESS );

    /** If the state transition is NOT correct, return error code.*/
    usr_err = _check_module_state_transitable( p_ctrl->module_state, module_status );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Apply new state to identity object attribute #5. */
    usr_err = um_opener_port_cip_set_device_status_from_module_state(p_ctrl->p_cip_ctrl, p_ctrl->module_state, module_status );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Apply new state to current module status . */
    p_ctrl->module_state = module_status;

    return usr_err;
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
    opener_port_indicator_ctrl_t * const p_ctrl = (opener_port_indicator_ctrl_t * const) pvParameter;

    /** Error status */
    usr_err_t usr_err;

    /** Notify the parent task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    while( 1 )
    {
        vTaskDelay(INDICATOR_FLASHING_INTERVAL_TICKS);

        usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl,
                                                   OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED,
                                                   g_network_state_indicator_map[p_ctrl->network_state] );
        if ( USR_SUCCESS != usr_err )
        {
            /** Error handling */
        }

        usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl,
                                                   OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED,
                                                   g_module_state_indicator_map[p_ctrl->module_state] );
        if ( USR_SUCCESS != usr_err )
        {
            /** Error handling */
        }
    }
    /** vTaskDelete(NULL); */
}

/*******************************************************************************************************************//**
 * @brief Execute self test sequence
 **********************************************************************************************************************/
static usr_err_t _exec_self_test_sequence( opener_port_indicator_ctrl_t * const p_ctrl )
{
	/** Error codes */
    usr_err_t usr_err;

    /** Prepare all pattern status */
    opener_port_indicator_status_t off =
    {
     .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
     .red   = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    };
    opener_port_indicator_status_t green =
    {
     .green = OPENER_PORT_INDICATOR_STATE_STEADY_ON,
     .red   = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
    };
    opener_port_indicator_status_t red =
    {
     .green = OPENER_PORT_INDICATOR_STATE_STEADY_OFF,
     .red   = OPENER_PORT_INDICATOR_STATE_STEADY_ON,
    };

    /** Initialize to off */
    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED, off );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );
    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED, off );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    /** Self test sequence module status indicator */
    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED, green );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    vTaskDelay( INDICATOR_SELF_TEST_DELAY_TICKS );

    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED, red );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    vTaskDelay( INDICATOR_SELF_TEST_DELAY_TICKS );

    /** Hold turn green after module indicator test. */
    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_MODULE_STATUS_LED, green );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    vTaskDelay( INDICATOR_SELF_TEST_DELAY_TICKS );

    /** Self test network usr_err indicator */
    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED, green );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    vTaskDelay( INDICATOR_SELF_TEST_DELAY_TICKS );

    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED, red );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    vTaskDelay( INDICATOR_SELF_TEST_DELAY_TICKS );

    /** Hold turn off after network indicator test */
    usr_err = um_opener_port_callback_request( p_ctrl->p_callback_ctrl, OPENER_PORT_CALLBACK_CHANGE_NETWORK_STATUS_LED, off );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_ABORTED );

    vTaskDelay( INDICATOR_SELF_TEST_DELAY_TICKS );

    /** return success code */
    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief State Machine refers to Vol1 3.30 Figure 5A-2.2 State Transition Diagram for Identity Object.
 **********************************************************************************************************************/
usr_err_t _check_module_state_transitable( opener_port_module_state_t current_state, opener_port_module_state_t new_state )
{
    /**
     * TODO: It may be good to change the order of switch sentences.
     */
    switch(current_state)
    {
    case OPENER_PORT_MODULE_STATE_NO_POWER:
        switch (new_state)
        {
        case OPENER_PORT_MODULE_STATE_SELF_TEST:
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_MODULE_STATE_SELF_TEST:
        switch (new_state)
        {
        case OPENER_PORT_MODULE_STATE_STANDBY:
        case OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT:
        case OPENER_PORT_MODULE_STATE_SELF_TEST:
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_MODULE_STATE_STANDBY:
        switch (new_state)
        {
        case OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL:
        case OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT:
        case OPENER_PORT_MODULE_STATE_SELF_TEST:
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_MODULE_STATE_DEVICE_OPERATIONAL:
        switch (new_state)
        {
        case OPENER_PORT_MODULE_STATE_STANDBY:
        case OPENER_PORT_MODULE_STATE_MAJOR_RECOVERABLE_FAULT:
        case OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT:
        case OPENER_PORT_MODULE_STATE_SELF_TEST:
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_MODULE_STATE_MAJOR_RECOVERABLE_FAULT:
        switch (new_state)
        {
        case OPENER_PORT_MODULE_STATE_STANDBY:
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_MODULE_STATE_MAJOR_UNRECOVERABLE_FAULT:
        /** According to CIP library, the module status cannot leave from Major unrecoverable fault.
         *  This function can leaves from this state, but return error always. */
        break;

    default:
        break;
    }

    /** Return error */
    return USR_ERR_ABORTED;
}

/*******************************************************************************************************************//**
 * @brief State Machine refers to Vol1 3.30 Figure 5A-2.2 State Transition Diagram for Identity Object.
 **********************************************************************************************************************/
usr_err_t _check_network_state_transitable( opener_port_network_state_t current_state, opener_port_network_state_t new_state )
{
    /**
     * TODO: It may be good to change the order of switch sentences.
     */
    switch( current_state )
    {
    case OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS:
        switch ( new_state )
        {
        case OPENER_PORT_NETWORK_STATE_SELF_TEST:            /* Power-Up Reset*/
        case OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS:       /* IP address configured */
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_NETWORK_STATE_SELF_TEST:
        switch ( new_state )
        {
        case OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS:   /* Self Test complete */
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS:
        switch ( new_state )
        {
        case OPENER_PORT_NETWORK_STATE_SELF_TEST:                    /* Power-Up Reset*/
        case OPENER_PORT_NETWORK_STATE_CONNECTED:                    /* CIP connection established */
        case OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS:   /* IP address lost */
        case OPENER_PORT_NETWORK_STATE_DUPLICATE_IP:                 /* Duplicate IP address detected */
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_NETWORK_STATE_CONNECTED:
        switch ( new_state )
        {
        case OPENER_PORT_NETWORK_STATE_SELF_TEST:                    /* Power-Up Reset*/
        case OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS:               /* No CIP connection AND Exclusive Owner not timed out. */
        case OPENER_PORT_NETWORK_STATE_CONECTION_TIMEOUT:            /* Exclusive Owner (target) timed out */
        case OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS:   /* IP address lost */
        case OPENER_PORT_NETWORK_STATE_DUPLICATE_IP:                 /* Duplicate IP address detected */
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_NETWORK_STATE_CONECTION_TIMEOUT:
        switch ( new_state )
        {
        case OPENER_PORT_NETWORK_STATE_SELF_TEST:                    /* Power-Up Reset*/
        case OPENER_PORT_NETWORK_STATE_CONNECTED:                    /* All Exclusive Owners */
        case OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS:   /* IP address lost */
        case OPENER_PORT_NETWORK_STATE_DUPLICATE_IP:                 /* Duplicate IP address detected */
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    case OPENER_PORT_NETWORK_STATE_DUPLICATE_IP:
        switch ( new_state )
        {
        case OPENER_PORT_NETWORK_STATE_SELF_TEST:                    /* Power-Up Reset*/
        case OPENER_PORT_NETWORK_STATE_NO_CONNECTIONS:               /* Duplicated IP address detected. */
        case OPENER_PORT_NETWORK_STATE_NOT_POWERWED_NO_IP_ADDRESS:   /* IP address lost */
            return USR_SUCCESS;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    /** Return error */
    return USR_ERR_ABORTED ;
}
