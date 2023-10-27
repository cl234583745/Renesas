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

#ifndef UM_SERIAL_IO_H
#define UM_SERIAL_IO_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_serial_io_api.h"
#include "um_serial_io_cfg.h"

#include "r_uart_api.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup UM_SERIAL_IO
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * Controller for writer task.
 */
typedef struct st_serial_io_writer_ctrl
{
	TaskHandle_t 		p_task_handle;
	TaskHandle_t		p_parent_task_handle;
	QueueHandle_t		p_queue_handle;
	serial_io_data_t *  p_data_buffer;

	void * 			p_uart_ctrl;

} serial_io_writer_ctrl_t;

/**
 * Controller for UART interface
 */
typedef struct st_serial_io_uart_ctrl
{
	uart_instance_t const * p_uart_instance;
	SemaphoreHandle_t 	    p_mutex_handle;
	uart_callback_args_t    callback_memory;
	SemaphoreHandle_t	    p_tx_semaphore_handle;

} serial_io_uart_ctrl_t;

/** Channel control block. DO NOT INITIALIZE.
 * Initialization occurs when @ref serial_io_api_t::open is called.
 */
typedef struct st_serial_io_instance_ctrl
{
    uint32_t		  		open;	///< State indicating that this instance is opened.
    uint32_t		  		start;	///< State indicating that this instance is started.
    serial_io_cfg_t const * p_cfg;	///< Pointer to instance configuration.

    serial_io_uart_ctrl_t   uart_ctrl;
    serial_io_writer_ctrl_t writer_ctrl;

} serial_io_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const serial_io_api_t g_serial_io_on_serial_io;

/** @endcond */
usr_err_t UM_SERIAL_IO_Open(serial_io_ctrl_t * const p_ctrl, serial_io_cfg_t const * const p_cfg);
usr_err_t UM_SERIAL_IO_Start(serial_io_ctrl_t * const p_ctrl);
usr_err_t UM_SERIAL_IO_Stop(serial_io_ctrl_t * const p_ctrl);
usr_err_t UM_SERIAL_IO_Write(serial_io_ctrl_t * const p_ctrl, void const * const buffer, uint32_t length );

/*******************************************************************************************************************//**
 * @} (end defgroup UM_SERIAL_IO)
 **********************************************************************************************************************/

#endif // UM_ETHER_LINK_H
