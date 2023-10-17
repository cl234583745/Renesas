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

/** For FSP error codes */
#include "fsp_common_api.h"

/** For FSP Ethernet module. */
#include "r_uart_api.h"
#include "r_sci_uart.h"
#include "r_sci_uart_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SCI_UART_BAUDRATE    (115200)

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
 * Imported function prototypes
 **********************************************************************************************************************/
/** r_uart_api callback function set by r_uart_cfg. */
extern void user_uart_callback (uart_callback_args_t * p_args);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static serial_io_uart_ctrl_t * sgp_r_uart_callback_context;

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief Initialize the controller.
 *
 * @param[in] p_ctrl				Pointer to the controller
 * @param[in] p_uart_instance		Pointer to the instance of uart interface
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 * @retval USR_ERR_NOT_INITIALIZED		Initialization has been failed.
 **********************************************************************************************************************/
usr_err_t um_serial_io_uart_open(serial_io_uart_ctrl_t * const p_ctrl, uart_instance_t const * const p_uart_instance )
{
	/** Error codes */
	fsp_err_t fsp_err;

	/** For setting baud rate */
    baud_setting_t baud_setting;

    /** Set target control. */
    p_ctrl->p_uart_instance = p_uart_instance;

	/** Create a mutex for r_uart */
	p_ctrl->p_mutex_handle = xSemaphoreCreateMutex();
	USR_ERROR_RETURN( p_ctrl->p_mutex_handle, USR_ERR_NOT_INITIALIZED );

	/** Create a semaphore for transmission control. */
	p_ctrl->p_tx_semaphore_handle = xSemaphoreCreateBinary();
	USR_ERROR_RETURN( p_ctrl->p_tx_semaphore_handle, USR_ERR_NOT_INITIALIZED );
	(void) xSemaphoreGive( p_ctrl->p_tx_semaphore_handle );

	/** Open the target interface. */
	fsp_err = p_ctrl->p_uart_instance->p_api->open(p_ctrl->p_uart_instance->p_ctrl, p_ctrl->p_uart_instance->p_cfg );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err || FSP_ERR_ALREADY_OPEN == fsp_err , USR_ERR_NOT_INITIALIZED );

	/** Calculate baud rate using SCI UART specific function. */
	fsp_err = R_SCI_UART_BaudCalculate( SCI_UART_BAUDRATE, false, 5000, &baud_setting );
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_NOT_INITIALIZED );

	/** Set baud rate */
	fsp_err = p_ctrl->p_uart_instance->p_api->baudSet( p_ctrl->p_uart_instance->p_ctrl , (void *) &baud_setting);
	USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_NOT_INITIALIZED );

    /** Set callback */
#if defined(BSP_MCU_GROUP_RA6M3)
    fsp_err = p_ctrl->p_uart_instance->p_api->callbackSet( p_ctrl->p_uart_instance->p_ctrl,
                                                           user_uart_callback,
                                                           p_ctrl,
                                                           &p_ctrl->callback_memory );
    USR_ERROR_RETURN( FSP_SUCCESS == fsp_err, USR_ERR_NOT_INITIALIZED );
#endif // defined(BSP_MCU_GROUP_RA6M3)
#if (defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZN2L))
    /** RZT2M FSP v1.0.0 does NOT support callbackSet API. */
    sgp_r_uart_callback_context = p_ctrl;
#endif // (defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZN2L))

    /** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close the controller.
 *
 * @param[in] p_ctrl				Pointer to the controller
 *
 * @retval USR_SUCCESS					Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_serial_io_uart_close(serial_io_uart_ctrl_t * const p_ctrl )
{
	/** Unused parameter */
	(void) p_ctrl;

	/**
	 * TODO: Implement the sequence to close.
	 */

    /** Return success code */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Output character via UART interface
 *
 * @param[in] p_ctrl		Pointer to the controller
 *
 * @retval USR_SUCCESS		Process has been done successfully.
 **********************************************************************************************************************/
usr_err_t um_serial_io_uart_write(serial_io_uart_ctrl_t * const p_ctrl, serial_io_data_t * const p_data_buffer )
{
	/** Error codes */
	fsp_err_t fsp_err;

	uint8_t offset;
	uint8_t chara;
	uint8_t cr = '\r';

	/** Enter lock section. */
	USR_LOCK_SECTION_START( p_ctrl->p_mutex_handle );
	{
		for( offset = 0; offset < p_data_buffer->length; offset++ )
		{
			chara = p_data_buffer->p_buffer[offset];

			if( 0 != offset )
			{
				if ( '\n' == chara )
				{
	    			/** write the character buffer. */
	    			p_ctrl->p_uart_instance->p_api->write( p_ctrl->p_uart_instance->p_ctrl, &cr, 1 );

	    			/** Wait until the transmission is completed. */
	    			(void) xSemaphoreTake( p_ctrl->p_tx_semaphore_handle, portMAX_DELAY );
				}
			}
			else
			{
	            if ( ( '\n' == chara ) && ( '\r' != (p_data_buffer->p_buffer[offset-1]) ) )
	            {
	    			/** write the character buffer. */
	    			p_ctrl->p_uart_instance->p_api->write( p_ctrl->p_uart_instance->p_ctrl, &cr, 1 );

	    			/** Wait until the transmission is completed. */
	    			(void) xSemaphoreTake( p_ctrl->p_tx_semaphore_handle, portMAX_DELAY );
				}
			}

			/** write the character buffer. */
			fsp_err = p_ctrl->p_uart_instance->p_api->write( p_ctrl->p_uart_instance->p_ctrl, &chara, 1 );
			USR_LOCK_ERROR_RETURN( p_ctrl->p_mutex_handle, FSP_SUCCESS == fsp_err, USR_ERR_ABORTED );

			/** Wait until the transmission is completed. */
			(void) xSemaphoreTake( p_ctrl->p_tx_semaphore_handle, portMAX_DELAY );
		}
	}
	USR_LOCK_SECTION_END( p_ctrl->p_mutex_handle );

	/** Return success code. */
	return USR_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Define callback function registered to uart interface
 *
 * @param[in] p_args				Pointer to the callback memory
 **********************************************************************************************************************/
void user_uart_callback (uart_callback_args_t * p_args)
{
	/** Resolve context */
#if defined(BSP_MCU_GROUP_RA6M3)
    serial_io_uart_ctrl_t const * p_ctrl = p_args->p_context;
#endif // defined(BSP_MCU_GROUP_RA6M3)
#if (defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZN2L))
    serial_io_uart_ctrl_t const * p_ctrl = sgp_r_uart_callback_context;
#endif // (defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZN2L))

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /** Handle the UART event */
    switch (p_args->event)
    {

	/** Detect transmission completed. */
	case UART_EVENT_TX_COMPLETE:
	{
		/** Release the resource for transmission. */
		(void) xSemaphoreGiveFromISR( p_ctrl->p_tx_semaphore_handle, &xHigherPriorityTaskWoken );
		break;
	}

	/** Other cases are not handled. TODO: Implement the reception sequence. */
	default:
		break;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
