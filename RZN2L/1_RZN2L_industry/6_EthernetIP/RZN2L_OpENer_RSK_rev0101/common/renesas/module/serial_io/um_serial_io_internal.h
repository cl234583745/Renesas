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

#ifndef UM_SERIAL_IO_INTERNAL_H
#define UM_SERIAL_IO_INTERNAL_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "um_serial_io.h"
#include "um_serial_io_api.h"

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
 * Function prototypes
 **********************************************************************************************************************/
usr_err_t um_serial_io_uart_open(serial_io_uart_ctrl_t * const p_ctrl, uart_instance_t const * const p_uart_instance );
usr_err_t um_serial_io_uart_close(serial_io_uart_ctrl_t * const p_ctrl );
usr_err_t um_serial_io_uart_write(serial_io_uart_ctrl_t * const p_ctrl, serial_io_data_t * const p_data_buffer );

usr_err_t um_serial_io_task_writer_open(serial_io_writer_ctrl_t * const p_ctrl, serial_io_uart_ctrl_t * const p_uart_ctrl );
usr_err_t um_serial_io_task_writer_close(serial_io_writer_ctrl_t * const p_ctrl );
usr_err_t um_serial_io_task_writer_start( serial_io_writer_ctrl_t * const p_ctrl );
usr_err_t um_serial_io_task_writer_stop( serial_io_writer_ctrl_t * const p_ctrl );
usr_err_t um_serial_io_task_writer_request( serial_io_writer_ctrl_t * const p_ctrl, serial_io_data_t * p_data_buffer );

#endif /** UM_SERIAL_IO_INTERNAL_H */
