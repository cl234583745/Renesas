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
usr_err_t um_ether_netif_callback_open( ether_netif_callback_ctrl_t * const p_ctrl )
{
	/** Create the mutex */
	p_ctrl->p_mutex_handle = xSemaphoreCreateMutex();
	USR_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_ERR_NOT_INITIALIZED );

	/** Initialize the head/tail of node list. */
	p_ctrl->p_head_node = NULL;
	p_ctrl->p_tail_node = NULL;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Specify the callback function and the argument memory.
 *
 * @param[in] p_ctrl			Pointer to the controller
 * @param[in] p_node    		Pointer to the callback node
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_callback_add( ether_netif_callback_ctrl_t * const p_ctrl, ether_netif_callback_link_node_t * const p_new_nodes )
{
    ether_netif_callback_link_node_t * p_node;

    /** Lock section */
    USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
    {
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
 * @param[in] port_link_status       Linked port information when link up/down event occurs.
 * @param[in] p_frame_packet	Pointer to the frame packet to be set to callback memory.
 *
 * @retval USR_SUCCESS			Process has been done successfully.
 * @retval USR_ERR_IN_USE		Resources are busy.
 **********************************************************************************************************************/
usr_err_t um_ether_netif_callback_request( ether_netif_callback_ctrl_t * const p_ctrl,
										   ether_netif_callback_event_t event,
										   uint32_t port_link_status,
										   ether_netif_frame_t * p_frame_packet )
{
	/** Pointer for iterating node of link list. */
	ether_netif_callback_link_node_t * p_node;

	/** Lock section */
	USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
	{
		/** Iterate along link list */
		for ( p_node = p_ctrl->p_head_node; p_node != NULL; p_node = p_node->p_next )
		{
			/** Set callback arguments with event. */
			p_node->p_memory->event = event;
			p_node->p_memory->p_context = p_node->p_context;
			p_node->p_memory->port_link_status = port_link_status;

			/** Set callback packet. */
			if ( p_frame_packet )
			{
			    /** If the callback node is last. */
			    if( NULL == p_node->p_next )
			    {
			        /** Send the original packet. */
			        p_node->p_memory->p_frame_packet = p_frame_packet;
			    }

			    /** other callback nodes */
			    else
			    {
			        /** Send the copy packet. */
	                USR_HEAP_ALLOCATE( p_node->p_memory->p_frame_packet, sizeof(ether_netif_frame_t) );
	                memcpy( p_node->p_memory->p_frame_packet, p_frame_packet, sizeof(ether_netif_frame_t) );
			    }
			}
			else
			{
				p_node->p_memory->p_frame_packet = NULL;
			}

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
