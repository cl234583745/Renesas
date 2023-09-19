#include "hal_data.h"
#include "sio_char.h"
#include <stdio.h>

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER
void handle_error (fsp_err_t err);
void user_uart_callback (uart_callback_args_t * p_args);

uint8_t  g_out_of_band_received[TRANSFER_LENGTH];
volatile uint32_t g_transfer_complete = 0;
volatile uint32_t g_receive_complete  = 0;
uint32_t g_out_of_band_index = 0;

char gbuff[16];

/*******************************************************************************************************************//**
 * @brief  SCI_UART example application
 *
 * Sample program for serial communication.
 * After outputting Hello world, the received character is echoed back.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
    /* Open the transfer instance with initial configuration. */
    fsp_err_t err = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    handle_error(err);

    __asm volatile ("cpsie i");

    printf("Hello World\n");

    while (1)
    {
        gets (gbuff);
        printf ("\n input = %s \n", gbuff);
    }

}

void user_uart_callback (uart_callback_args_t * p_args)
{
    /* Handle the UART event */
    switch (p_args->event)
    {
        /* Received a character */
        case UART_EVENT_RX_CHAR:
        {
            /* Only put the next character in the receive buffer if there is space for it */
            if (sizeof(g_out_of_band_received) > g_out_of_band_index)
            {
                /* Write either the next one or two bytes depending on the receive data size */
                if ((UART_DATA_BITS_7 == g_uart0_cfg.data_bits) || (UART_DATA_BITS_8 == g_uart0_cfg.data_bits))
                {
                    g_out_of_band_received[g_out_of_band_index++] = (uint8_t) p_args->data;
                }
                else
                {
                    uint16_t * p_dest = (uint16_t *) &g_out_of_band_received[g_out_of_band_index];
                    *p_dest              = (uint16_t) p_args->data;
                    g_out_of_band_index += 2;
                }
            }
            break;
        }
        /* Receive complete */
        case UART_EVENT_RX_COMPLETE:
        {
            g_receive_complete = 1;
            break;
        }
        /* Transmit complete */
        case UART_EVENT_TX_COMPLETE:
        {
            g_transfer_complete = 1;
            break;
        }
        default:
        {
        }
    }
}


void handle_error (fsp_err_t err)
{
    FSP_PARAMETER_NOT_USED(err);
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}
