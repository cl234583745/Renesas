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

#ifndef UM_ETHER_NETIF_H
#define UM_ETHER_NETIF_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_ether_netif_api.h"
#include "um_ether_netif_cfg.h"
#include "um_ether_netif_feature.h"

#include "r_ether_api.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHER_NETIF_PORT_MASK           ((uint32_t)~((uint32_t)(0xFFFFFFFF)<<(UM_ETHER_NETIF_FEATURE_NUMBER_OF_ETHER_PORTS)))

/*******************************************************************************************************************//**
 * @addtogroup UM_ETHER_NETIF
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef struct st_ether_netif_ether_ctrl
{
	ether_instance_t const * p_ether_instance;
	SemaphoreHandle_t p_mutex_handle;
	void *      p_reader_ctrl;
} ether_netif_ether_ctrl_t;

typedef struct st_ether_netif_callback_ctrl
{
	SemaphoreHandle_t  			        p_mutex_handle;
	ether_netif_callback_link_node_t *  p_head_node;
	ether_netif_callback_link_node_t *  p_tail_node;
} ether_netif_callback_ctrl_t;

typedef struct st_ether_netif_monitor_ctrl
{
	TaskHandle_t p_task_handle;
    TaskHandle_t p_parent_task_handle;
    uint32_t     link_status;
	void * 		 p_ether_ctrl;			///< Pointer to Ethernet interface control
	void * 		 p_callback_ctrl;		///< Pointer to Callback interface control
} ether_netif_monitor_ctrl_t;

typedef struct st_ether_netif_writer_ctrl
{
	TaskHandle_t 		  p_task_handle;
    TaskHandle_t          p_parent_task_handle;
	QueueHandle_t 		  p_queue_handle;
	ether_netif_frame_t * p_ether_netif_frame;
	void 				* p_ether_ctrl;	///< Pointer to Ethernet interface control
} ether_netif_writer_ctrl_t;

typedef struct st_ether_netif_reader_ctrl
{
	TaskHandle_t 		  p_task_handle;
    TaskHandle_t          p_parent_task_handle;
	ether_netif_frame_t	* p_ether_netif_frame;
	void  				* p_ether_ctrl;				///< Pointer to Ethernet interface control
	void 	 			* p_callback_ctrl;			///< Pointer to Callback interface control
} ether_netif_reader_ctrl_t;

/** Channel control block. DO NOT INITIALIZE.
 * Initialization occurs when @ref ether_netif_api_t::open is called.
 */
typedef struct st_ether_netif_instance_ctrl
{
    uint32_t                    open;			///< State indicating that this instance is opened.
    ether_netif_cfg_t const *   p_cfg;			///< Pointer to instance configuration.
    ether_netif_ether_ctrl_t	ether_ctrl;	///< Controller for Ethernet driver interface
    ether_netif_callback_ctrl_t callback_ctrl;	///< Controller for callback interface
    ether_netif_monitor_ctrl_t  monitor_ctrl;  	///< Controller for Ethernet link monitor interface.
    ether_netif_writer_ctrl_t	writer_ctrl;	///< Controller for Ethernet output interface
	ether_netif_reader_ctrl_t 	reader_ctrl;///< Controller for Ethernet input interface
} ether_netif_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_netif_api_t g_ether_netif_on_ether_netif;

/** @endcond */
usr_err_t UM_ETHER_NETIF_Open(ether_netif_ctrl_t * const p_ctrl, ether_netif_cfg_t const * const p_cfg);
usr_err_t UM_ETHER_NETIF_Start(ether_netif_ctrl_t * const p_ctrl);
usr_err_t UM_ETHER_NETIF_Stop(ether_netif_ctrl_t * const p_ctrl);
usr_err_t UM_ETHER_NETIF_CallbackAdd (ether_netif_ctrl_t * const p_ctrl, ether_netif_callback_link_node_t * const p_node );
usr_err_t UM_ETHER_NETIF_Send(ether_netif_ctrl_t * const p_ctrl, ether_netif_frame_t * const p_packet_buffer);
usr_err_t UM_ETHER_NETIF_LinkStatusGet(ether_netif_ctrl_t * const p_ctrl, uint32_t * p_link_status, uint8_t notify_callback);

/*******************************************************************************************************************//**
 * @} (end defgroup UM_ETHER_NETIF)
 **********************************************************************************************************************/

#endif // UM_ETHER_NETIF_H
