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

#ifndef UM_SERIAL_IO_API_H
#define UM_SERIAL_IO_API_H

/*******************************************************************************************************************//**
 * @defgroup TCPIP network interface
 * @ingroup RENESAS_INTERFACES
 * @brief Interface for UM_SERIAL_IO functions.
 *
 * @section UM_SERIAL_IO_API_SUMMARY Summary
 * The UM_SERIAL_IO interface provides the network interface for utilizing TCP/IP communication.
 * - Provide the network interface for TCP/IP communication with TCP/IP protocol stack.
 *
 * Implemented by:
 * - @ref UM_SERIAL_IO
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Include APIs */
#include "um_common_api.h"
#include "r_uart_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SERIAL_IO_MAX_DATA_LENGTH		(128U)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * Structure for Ethernet frame packet.
 */
typedef struct serial_io_data_t
{
    uint32_t length;								///< Length of string.
    uint8_t  p_buffer[SERIAL_IO_MAX_DATA_LENGTH]; 	///< Buffer of string.
} serial_io_data_t;

/** UM_SERIAL_IO control block. Allocate an instance specific control block to pass into the UM_SERIAL_IO API calls.
 * @par Implemented as
 * - lwip_instance_ctrl_t
 */
typedef void serial_io_ctrl_t;

/**
 * Structure for instance configuration.
 */
typedef struct st_serial_io_cfg
{
	uart_instance_t const * const p_uart_instance;
} serial_io_cfg_t;

/** UM_SERIAL_IO API structure. General net functions implemented at the HAL layer follow this API. */
typedef struct st_um_net_api
{
    /** Open network interface.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Open()
     * @return
     */
    usr_err_t (* open)( serial_io_ctrl_t * const p_ctrl, serial_io_cfg_t const * const p_cfg );

    /** Start tasks.
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Start()
     * @return
     */
    usr_err_t (* start)( serial_io_ctrl_t * const p_ctrl );

    /** Stop tasks
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Stop()
     * @return
     */
    usr_err_t (* stop)( serial_io_ctrl_t * const p_ctrl );

    /** Write character
     * @par Implemented as
     * - @ref UM_LWIP_PORT_Write()
     * @return
     */
    usr_err_t (* write)( serial_io_ctrl_t * const p_ctrl, void const * const buffer, uint32_t length );

} serial_io_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_serial_io_instance
{
    serial_io_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    serial_io_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    serial_io_api_t const * p_api;           ///< Pointer to the API structure for this instance
} serial_io_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup UM_SERIAL_IO_API)
 **********************************************************************************************************************/

#endif		// UM_SERIAL_IO_API_H
