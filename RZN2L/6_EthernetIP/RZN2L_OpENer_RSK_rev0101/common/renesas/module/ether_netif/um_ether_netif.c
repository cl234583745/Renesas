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
/** "UETM" in ASCII, used to determine if channel is open. */
#define ETHER_NETIF_OPEN	(0x45544E49ULL)
#define ETHER_NETIF_CLOSE	(ETHER_NETIF_OPEN - 1)
#define ETHER_NETIF_RUN	    (ETHER_NETIF_OPEN + 1)

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
 * Private global variables
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/** UM_ETHER_NETIF Implementation of General UM_ETHER_NETIF Driver  */
const ether_netif_api_t g_ether_netif_on_ether_netif =
{
    .open          = UM_ETHER_NETIF_Open,
	.start		   = UM_ETHER_NETIF_Start,
	.stop		   = UM_ETHER_NETIF_Stop,
    .send      	   = UM_ETHER_NETIF_Send,
    .callbackAdd   = UM_ETHER_NETIF_CallbackAdd,
    .linkStatusGet = UM_ETHER_NETIF_LinkStatusGet
};

/*******************************************************************************************************************//**
 * @addtogroup UM_ETHER_NETIF
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initializes the UM_ETHER_NETIF instance.
 *
 * Implements @ref rm_ether_netif_api_t::open
 *
 * @retval USR_SUCCESS                 Initialization was successful and each task has started.
 * @retval USR_ERR_ASSERTION           The argument parameter are invalid.
 * @retval USR_ERR_ALREADY_OPEN		   The instance has been already opened.
 * @retval USR_ERR_NOT_INITIALIZED     Initialization was failed.
 **********************************************************************************************************************/
usr_err_t UM_ETHER_NETIF_Open(ether_netif_ctrl_t * const p_ctrl, ether_netif_cfg_t const * const p_cfg)
{
#if defined(UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
    USR_ASSERT(p_cfg);
#endif

    /** Status */
    usr_err_t   usr_err;

    /** Resolve control instance type. */
    ether_netif_instance_ctrl_t * const p_instance_ctrl = (ether_netif_ctrl_t *) p_ctrl;

    /** Check if the instance is already started. */
    USR_ERROR_RETURN( ETHER_NETIF_RUN != p_instance_ctrl->open, USR_ERR_ALREADY_RUNNING );

    /** Check if the instance is already opened. */
    USR_ERROR_RETURN( ETHER_NETIF_OPEN != p_instance_ctrl->open, USR_ERR_ALREADY_OPEN );

    /** Initialize instance control */
    p_instance_ctrl->p_cfg = p_cfg;

    /** Initialize internal module */
    {
		/** Open Ethernet driver interface and its controller. */
		usr_err = um_ether_netif_ether_open(&p_instance_ctrl->ether_ctrl,
											 p_instance_ctrl->p_cfg->p_ether_instance,
											&p_instance_ctrl->reader_ctrl);
		USR_ERROR_RETURN( USR_SUCCESS == usr_err , USR_ERR_NOT_INITIALIZED );

		/** Open callback module  */
		usr_err = um_ether_netif_callback_open(&p_instance_ctrl->callback_ctrl);
		USR_ERROR_RETURN( USR_SUCCESS == usr_err , USR_ERR_NOT_INITIALIZED );

		/** Create link monitor task. */
		usr_err = um_ether_netif_task_monitor_open( &p_instance_ctrl->monitor_ctrl,
													&p_instance_ctrl->ether_ctrl,
													&p_instance_ctrl->callback_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err , USR_ERR_NOT_INITIALIZED );

		/** Create network writer task */
		usr_err = um_ether_netif_task_writer_open( &p_instance_ctrl->writer_ctrl,
												   &p_instance_ctrl->ether_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err , USR_ERR_NOT_INITIALIZED );

		/** Create network reader task */
		usr_err = um_ether_netif_task_reader_open( &p_instance_ctrl->reader_ctrl,
												   &p_instance_ctrl->ether_ctrl,
												   &p_instance_ctrl->callback_ctrl );
		USR_ERROR_RETURN( USR_SUCCESS == usr_err , USR_ERR_NOT_INITIALIZED );
    }

    /** Set open state to the instance control. */
    p_instance_ctrl->open = ETHER_NETIF_OPEN;

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Resume each task.
 *
 * Implements @ref um_ether_netif_api_t::start
 *
 * @retval USR_SUCCESS                 Finalizing was successful and each task has started.
 **********************************************************************************************************************/
usr_err_t UM_ETHER_NETIF_Start(ether_netif_ctrl_t * const p_ctrl)
{
#if defined(UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif
    /** Resolve control instance type. */
    ether_netif_instance_ctrl_t * const p_instance_ctrl = (ether_netif_instance_ctrl_t *) p_ctrl;

    /** Resume each Task. */
    (void) um_ether_netif_task_monitor_start(&p_instance_ctrl->monitor_ctrl);
	(void) um_ether_netif_task_writer_start(&p_instance_ctrl->writer_ctrl);
	(void) um_ether_netif_task_reader_start(&p_instance_ctrl->reader_ctrl);

	/** Set the status to be started. */
	p_instance_ctrl->open = ETHER_NETIF_RUN;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Suspend each task.
 *
 * Implements @ref um_ether_netif_api_t::stop
 *
 * @retval USR_SUCCESS                 Finalizing was successful and each task has started.
 **********************************************************************************************************************/
usr_err_t UM_ETHER_NETIF_Stop(ether_netif_ctrl_t * const p_ctrl)
{
#if defined(UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    ether_netif_instance_ctrl_t * const p_instance_ctrl = (ether_netif_instance_ctrl_t *) p_ctrl;

    /** Suspend each Task. */
	(void) um_ether_netif_task_monitor_stop(&p_instance_ctrl->monitor_ctrl);
	(void) um_ether_netif_task_writer_stop(&p_instance_ctrl->writer_ctrl);
	(void) um_ether_netif_task_reader_stop(&p_instance_ctrl->reader_ctrl);

	p_instance_ctrl->open = ETHER_NETIF_OPEN;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Updates the user callback with the option to provide memory for the callback argument structure.
 *
 * Implements @ref um_ether_netif_api_t::callbackSet.
 *
 * @retval USR_SUCCESS      Callback was updated successfully.
 * @retval USR_ERR_IN_USE	Resources are busy.
 **********************************************************************************************************************/
usr_err_t UM_ETHER_NETIF_CallbackAdd (ether_netif_ctrl_t * const p_ctrl, ether_netif_callback_link_node_t * const p_node )
{
#if defined(UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    ether_netif_instance_ctrl_t * p_instance_ctrl = (ether_netif_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Set callback callback function. */
    usr_err = um_ether_netif_callback_add( &p_instance_ctrl->callback_ctrl, p_node );

    /** Return the error without change. */
    return usr_err;
}

/*******************************************************************************************************************//**
 * @brief Send Ethernet frame to be sent via Ethernet driver.
 *
 * Implements @ref um_ether_netif_api_t::send.
 *
 * @retval  USR_SUCCESS                 Send the frame to the TX queue successfully.
 * @retval  USR_ERR_IN_USE              The TX queue is full.
 **********************************************************************************************************************/
usr_err_t UM_ETHER_NETIF_Send(ether_netif_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_packet_buffer)
{
#if defined(UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
    USR_ASSERT(p_packet_buffer);
#endif

    /** Resolve control instance type. */
    ether_netif_instance_ctrl_t * const p_instance_ctrl = (ether_netif_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Request to enqueue Ethernet frame. */
    usr_err = um_ether_netif_task_writer_request(&p_instance_ctrl->writer_ctrl, p_packet_buffer);

    /** Return the error without change */
    return usr_err;
}

/*******************************************************************************************************************//**
 * @brief Get link status of all ports
 *
 * Implements @ref um_ether_netif_api_t::linkStatusGet.
 *
 * @retval  USR_SUCCESS                 Send the frame to the TX queue successfully.
 * @retval  USR_ERR_IN_USE              The TX queue is full.
 **********************************************************************************************************************/
usr_err_t UM_ETHER_NETIF_LinkStatusGet(ether_netif_ctrl_t * const p_ctrl, uint32_t * p_link_status, uint8_t notify_callback)
{
#if defined(UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_ETHER_NETIF_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
    USR_ASSERT(p_link_status);
#endif

    /** Resolve control instance type. */
    ether_netif_instance_ctrl_t * const p_instance_ctrl = (ether_netif_instance_ctrl_t *) p_ctrl;

    /** Status */
    usr_err_t usr_err;

    /** Get the link status of all ports */
    usr_err = um_ether_netif_task_monitor_get_link_status (&p_instance_ctrl->monitor_ctrl, p_link_status);

    if(notify_callback)
    {
        usr_err = um_ether_netif_callback_request(&p_instance_ctrl->callback_ctrl,
                                                  ETHER_NETIF_CALLBACK_EVENT_ANY_PORT_LINK_STATUS_CHANGED,
                                                  *p_link_status,
                                                  NULL
                                                  );
    }

    /** Return the error without change */
    return usr_err;
}

/** @} (end addtogroup UM_ETHER_NETIF) */
