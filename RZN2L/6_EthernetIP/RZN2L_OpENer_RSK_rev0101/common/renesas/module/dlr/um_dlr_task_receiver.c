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
usr_err_t _interpret_dlr_frame( dlr_receiver_ctrl_t * p_ctrl, dlr_event_arg_t * const p_dlr_event );

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Create the task and initialize its controller.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_receiver_open( dlr_receiver_ctrl_t * const p_ctrl,
									 dlr_fsm_ctrl_t * const p_fsm_ctrl,
									 dlr_executor_ctrl_t * const p_executor_ctrl )
{
	/** Error code */
	BaseType_t rtos_err;

	/** Set the related submodules controllers. */
    p_ctrl->p_fsm_ctrl = p_fsm_ctrl;
    p_ctrl->p_executor_ctrl = p_executor_ctrl;

    /** Initialize Ethernet frame buffer. */
	p_ctrl->p_ether_frame = NULL;

    /** Save the current task handler to communicate the generated task when its initialization. */
    p_ctrl->p_parent_task_handle = xTaskGetCurrentTaskHandle();
    USR_ERROR_RETURN( NULL != p_ctrl->p_parent_task_handle, USR_ERR_NOT_INITIALIZED);

    /** Create frame queue. */
    p_ctrl->p_queue_handle = xQueueCreate(UM_DLR_CFG_RECEIVER_QUEUE_LENGTH, sizeof(ether_netif_frame_t *));
    USR_ERROR_RETURN( NULL != p_ctrl->p_queue_handle, USR_ERR_NOT_INITIALIZED);

    /** Create frame buffer mutex. */
    p_ctrl->p_ether_frame_mutex = xSemaphoreCreateMutex();
    USR_ERROR_RETURN( NULL != p_ctrl->p_ether_frame_mutex, USR_ERR_NOT_INITIALIZED);

	/** Create task */
    rtos_err = xTaskCreate( _task_code,
                            UM_DLR_CFG_RECEIVER_TASK_NAME,
                            UM_DLR_CFG_RECEIVER_TASK_STACK_BTYE_SIZE / sizeof(StackType_t),
                            p_ctrl,
                            UM_DLR_CFG_RECEIVER_TASK_PRIORITY,
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
usr_err_t um_dlr_task_receiver_start( dlr_receiver_ctrl_t *	const p_ctrl )
{
	vTaskResume(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Stop the task.
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_receiver_stop( dlr_receiver_ctrl_t * const p_ctrl )
{
	vTaskSuspend(p_ctrl->p_task_handle);
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Enqueue the Ethernet frames to pass it to the RTOS task.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_receiver_take_frame( dlr_receiver_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_packet_buffer )
{
    /** Status */
    BaseType_t rtos_err;

    /** Send Ethernet frame. */
    rtos_err = xQueueSend(p_ctrl->p_queue_handle, &p_packet_buffer, portMAX_DELAY);
    USR_ERROR_RETURN( pdTRUE == rtos_err, USR_ERR_IN_USE );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Get the frame buffer
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_receiver_reference_frame_buffer( dlr_receiver_ctrl_t * const p_ctrl, uint8_t const ** const pp_frame_buffer )
{
    /** Get frame buffer pointer */
    *pp_frame_buffer = p_ctrl->p_ether_frame->p_buffer;

    /** Avoid warning. */
	(void) pp_frame_buffer;

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Notify the task of the completion of the process for event generated by this task.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_receiver_unlock_frame_buffer( dlr_receiver_ctrl_t * const p_ctrl )
{
    /** Free the memory of Ethernet frame */
    USR_HEAP_RELEASE( p_ctrl->p_ether_frame );

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Notify the task of the completion of the process for event generated by this task.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_dlr_task_receiver_inform_neighbor_check_request_sequence_id( dlr_receiver_ctrl_t * const p_ctrl, dlr_port_t port, uint32_t sequence_id )
{
    /** Status */
    switch( port )
    {
    case DLR_PORT0_BIT:
        p_ctrl->last_neighbor_check_request_sequence_id0 = sequence_id;
        break;
    case DLR_PORT1_BIT:
        p_ctrl->last_neighbor_check_request_sequence_id1 = sequence_id;
        break;
    case DLR_PORT_BOTH:
        p_ctrl->last_neighbor_check_request_sequence_id0 = sequence_id;
        p_ctrl->last_neighbor_check_request_sequence_id1 = sequence_id;
        break;
    default:
        return USR_ERR_ABORTED;
        break;
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
    dlr_receiver_ctrl_t * const p_ctrl = (dlr_receiver_ctrl_t * const) pvParameter;

	/** Error codes */
    usr_err_t usr_err;
    BaseType_t rtos_err;

    /** DLR event received event queue. */
    dlr_event_arg_t dlr_event;

    /** Notify the parent task launch of this task. */
    xTaskNotifyGive( p_ctrl->p_parent_task_handle );

    while( 1 )
    {
    	/** Get frame from Queue */
        rtos_err = xQueueReceive(p_ctrl->p_queue_handle, &p_ctrl->p_ether_frame, portMAX_DELAY);
        if( ( pdPASS != rtos_err ) || ( NULL == p_ctrl->p_ether_frame ) )
        {
            /** Free the memory of Ethernet frame */
            USR_HEAP_RELEASE( p_ctrl->p_ether_frame );

        	/** TODO: Implement task assertion */
        	continue;
		}

        /** Store DLR frame for each DLR event */
        dlr_event.p_ether_frame = p_ctrl->p_ether_frame;

        /** Take frame information and generate the DLR event */
        usr_err = _interpret_dlr_frame( p_ctrl, &dlr_event );
        if ( USR_SUCCESS != usr_err )
        {
            /** Free the memory of Ethernet frame */
            USR_HEAP_RELEASE( p_ctrl->p_ether_frame );

        	continue;  	/** TODO: Add error handling */
        }

        /** Send DLR event into the front of event queue. */
        usr_err = um_dlr_task_executor_receive_event(p_ctrl->p_executor_ctrl, &dlr_event, DLR_CONTEXT_RTOS_TASK, DLR_ENQUEUE_FRONT );
        if ( USR_SUCCESS != usr_err )
        {
            /** Free the memory of Ethernet frame */
            USR_HEAP_RELEASE( p_ctrl->p_ether_frame );

        	continue;  	/** TODO: Add error handling */
        }
    }
    /** vTaskDelete(NULL); */
}

/*******************************************************************************************************************//**
 * @brief Execute state machine with non-beacon frame.
 *
 * @retval USR_SUCCESS
 * @retval USR_ABORTED
 * @retval USR_INVALID_ARGUMENT
 **********************************************************************************************************************/
usr_err_t _interpret_dlr_frame( dlr_receiver_ctrl_t * p_ctrl, dlr_event_arg_t * const p_dlr_event )
{
    /** Resolve DLR frame type. */
    dlr_frame_common_t * p_dlr_frame = (dlr_frame_common_t *) p_ctrl->p_ether_frame->p_buffer;

    /** Resolve the port from which the frame was received. */
    p_dlr_event->event_port = p_ctrl->p_ether_frame->port & DLR_PORT_BOTH;
    USR_ERROR_RETURN( p_dlr_event->event_port == DLR_PORT0_BIT || p_dlr_event->event_port == DLR_PORT1_BIT, USR_ERR_ABORTED );

    /** Initialize DLR event. */
    p_dlr_event->event = DLR_EVENT_NEIGHBORCHECKREQ;	/** tentative value. */

	/** Check if VLAN TAG exists. */
    if ( DLR_802_1Q_TAG_TYPE != DLR_BYTEORDER_SWAP16( p_dlr_frame->common_header.b12_802_1q_tag_type ) )
    {
    	/** Resolve it as learning update frame. */
    	dlr_frame_learning_update_t * p_learning_update_frame = (dlr_frame_learning_update_t *) p_dlr_frame;

    	/** Check if constant values is correct. */
        USR_ERROR_RETURN( DLR_RING_ETHER_TYPE            == DLR_BYTEORDER_SWAP16( p_learning_update_frame->b12_ring_ether_type ) &&
        				  DLR_RING_SUBTYPE               == p_learning_update_frame->b14_ring_sub_type   &&
						  DLR_RING_PROTOCOL_VERSION      == p_learning_update_frame->b15_ring_protocol_version &&
						  DLR_FRAME_TYPE_LEARNING_UPDATE == p_learning_update_frame->b16_frame_type,
						  USR_ERR_ABORTED );
        /**
         * DLR node does not receive Learning_Update frame.
         */
        return USR_ERR_ABORTED;
    }

	/** Check if constant values is correct. */
    USR_ERROR_RETURN( DLR_RING_ETHER_TYPE 	    == DLR_BYTEORDER_SWAP16( p_dlr_frame->common_header.b16_ring_ether_type ) &&
    				  DLR_RING_SUBTYPE 		    == p_dlr_frame->common_header.b18_ring_sub_type   &&
					  DLR_RING_PROTOCOL_VERSION == p_dlr_frame->common_header.b19_ring_protocol_version,
					  USR_ERR_ABORTED );

	/** Check the frame type */
	switch ( p_dlr_frame->common_payload.b20_frame_type )
	{

	/**
	 * The following frame type is required to check sequence id.
	 */
	case DLR_FRAME_TYPE_NEIGHBOR_CHECK_RESPONSE:

		/** Filter the response frame by checking the sequence id */
		switch( p_dlr_event->event_port )
		{
		case DLR_PORT0_BIT:
			USR_ERROR_RETURN( p_ctrl->last_neighbor_check_request_sequence_id0
			                  == DLR_BYTEORDER_SWAP32(p_dlr_frame->common_payload.b26_sequence_id),
			                  USR_ERR_ABORTED );
			break;
		case DLR_PORT1_BIT:
			USR_ERROR_RETURN( p_ctrl->last_neighbor_check_request_sequence_id1
			                  == DLR_BYTEORDER_SWAP32(p_dlr_frame->common_payload.b26_sequence_id),
			                  USR_ERR_ABORTED );
			break;
		default:
			return USR_ERR_ABORTED;
		}

		/** Set DLR event */
		p_dlr_event->event = DLR_EVENT_NEIGHBORCHECKRES;

		break;

	/**
	 * The following frame type only generates the DLR event.
	 */
	case DLR_FRAME_TYPE_NEIGHBOR_CHECK_REQUEST:	p_dlr_event->event = DLR_EVENT_NEIGHBORCHECKREQ;	break;
	case DLR_FRAME_TYPE_LOCATE_FAULT: 			p_dlr_event->event = DLR_EVENT_LOCATEFAULT;			break;
	case DLR_FRAME_TYPE_SIGN_ON:	  			p_dlr_event->event = DLR_EVENT_SIGNON;				break;
	case DLR_FRAME_TYPE_FLUSH_TABLES: 			p_dlr_event->event = DLR_EVENT_FLUSHTABLES;			break;

	/**
	 * Other frame type cannot be accepted by ring node.
	 */
	default: return USR_ERR_ABORTED;
	}

	return USR_SUCCESS;
}
