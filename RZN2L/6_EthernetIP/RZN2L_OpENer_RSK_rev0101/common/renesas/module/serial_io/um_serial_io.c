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
#include "um_serial_io.h"
#include "um_serial_io_cfg.h"
#include "um_serial_io_internal.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** "UETM" in ASCII, used to determine if channel is open. */
#define SERIAL_IO_OPEN	(0x5345494FULL)
#define SERIAL_IO_CLOSE	(0x00000000ULL)
#define SERIAL_IO_START	(0x5345494FULL)
#define SERIAL_IO_STOP	(0x00000000ULL)

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

/** UM_SERIAL_IO Implementation of General UM_SERIAL_IO Driver  */
const serial_io_api_t g_serial_io_on_serial_io =
{
    .open         = UM_SERIAL_IO_Open,
	.start		  = UM_SERIAL_IO_Start,
	.stop		  = UM_SERIAL_IO_Stop,
	.write        = UM_SERIAL_IO_Write,
};

/*******************************************************************************************************************//**
 * @addtogroup UM_SERIAL_IO
 * @{
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initializes the UM_SERIAL_IO instance.
 *
 * Implements @ref rm_serial_io_api_t::open
 *
 * @retval USR_SUCCESS                 Initialization was successful and each task has started.
 * @retval USR_ERR_ASSERTION           The argument parameter are invalid.
 * @retval USR_ERR_ALREADY_OPEN		   The instance has been already opened.
 * @retval USR_ERR_NOT_INITIALIZED     Initialization was failed.
 **********************************************************************************************************************/
usr_err_t UM_SERIAL_IO_Open(serial_io_ctrl_t * const p_ctrl, serial_io_cfg_t const * const p_cfg)
{
#if defined(UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
    USR_ASSERT(p_cfg);
#endif

    /** Status */
    usr_err_t   usr_err;

    /** Resolve control instance type. */
    serial_io_instance_ctrl_t * const p_instance_ctrl = (serial_io_ctrl_t *) p_ctrl;

    /** Check if the instance is closed yet. */
    USR_ERROR_RETURN( SERIAL_IO_CLOSE == p_instance_ctrl->open, USR_ERR_ALREADY_OPEN );

    /** Initialize instance control */
    p_instance_ctrl->p_cfg = p_cfg;

    /** Initialize uart interface */
    usr_err = um_serial_io_uart_open( &p_instance_ctrl->uart_ctrl, p_instance_ctrl->p_cfg->p_uart_instance );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Initialize writer task. */
    usr_err = um_serial_io_task_writer_open( &p_instance_ctrl->writer_ctrl, &p_instance_ctrl->uart_ctrl );
    USR_ERROR_RETURN( USR_SUCCESS == usr_err, USR_ERR_NOT_INITIALIZED );

    /** Set open state to the instance control. */
    p_instance_ctrl->open = SERIAL_IO_OPEN;

    return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Resume each task.
 *
 * Implements @ref um_serial_io_api_t::start
 *
 * @retval USR_SUCCESS                 Finalizing was successful and each task has started.
 **********************************************************************************************************************/
usr_err_t UM_SERIAL_IO_Start(serial_io_ctrl_t * const p_ctrl)
{
#if defined(UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    serial_io_instance_ctrl_t * const p_instance_ctrl = (serial_io_instance_ctrl_t *) p_ctrl;

    /** Check if the instance is stopped yet */
    USR_ERROR_RETURN( SERIAL_IO_STOP == p_instance_ctrl->start, USR_SUCCESS /** Already started */ );

    /** Resume each Task. */
    (void) um_serial_io_task_writer_start( &p_instance_ctrl->writer_ctrl );

	/** Set the status to be started. */
	p_instance_ctrl->start = SERIAL_IO_START;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Suspend each task.
 *
 * Implements @ref um_serial_io_api_t::stop
 *
 * @retval USR_SUCCESS                 Finalizing was successful and each task has started.
 **********************************************************************************************************************/
usr_err_t UM_SERIAL_IO_Stop(serial_io_ctrl_t * const p_ctrl)
{
#if defined(UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Resolve control instance type. */
    serial_io_instance_ctrl_t * const p_instance_ctrl = (serial_io_instance_ctrl_t *) p_ctrl;

    /** Check if the instance is running yet. */
    USR_ERROR_RETURN( SERIAL_IO_START == p_instance_ctrl->start, USR_SUCCESS /** Already stopped */ );

    /** Resume each Task. */
    (void) um_serial_io_task_writer_stop( &p_instance_ctrl->writer_ctrl );

    /** Set the status to be stopped. */
	p_instance_ctrl->start = SERIAL_IO_STOP;

	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Write characters.
 *
 * Implements @ref um_serial_io_api_t::character
 *
 * @retval USR_SUCCESS                 Finalizing was successful and each task has started.
 **********************************************************************************************************************/
usr_err_t UM_SERIAL_IO_Write(serial_io_ctrl_t * const p_ctrl, void const * const buffer, uint32_t length )
{
#if defined(UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE) && ((1) == UM_SERIAL_IO_CFG_PARAM_CHECKING_ENABLE)
    USR_ASSERT(p_ctrl);
#endif

    /** Error codes */
    usr_err_t usr_err;

    /** Resolve control instance type. */
    serial_io_instance_ctrl_t * const p_instance_ctrl = (serial_io_instance_ctrl_t *) p_ctrl;

    /** Buffer for request */
    serial_io_data_t * p_data = NULL;

    /** Allocate memory */
    USR_HEAP_ALLOCATE( p_data, sizeof(serial_io_data_t) );

    /** Copy the buffer */
    memcpy ( p_data->p_buffer, buffer, length * sizeof(uint8_t) );
    p_data->length = length;

    usr_err = um_serial_io_task_writer_request(&p_instance_ctrl->writer_ctrl, p_data);
    USR_ERROR_RETURN( usr_err != USR_SUCCESS, USR_ERR_ABORTED );

    return USR_SUCCESS;
}

/** @} (end addtogroup UM_SERIAL_IO) */
