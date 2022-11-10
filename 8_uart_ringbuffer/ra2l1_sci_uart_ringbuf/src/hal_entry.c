/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "hal_data.h"
#include <stdio.h>
#include "SEGGER_RTT.h"
#include "ringbuffer.h"
void R_BSP_WarmStart(bsp_warm_start_event_t event);

extern bsp_leds_t g_bsp_leds;
static ring_buffer_t uart0_TxBuf, uart0_RxBuf;
volatile bool uart_send_complete_flag = false;

uint8_t My_Uart0_Tx_While_Complete(uint8_t * buf, uint8_t len);
uint8_t My_Uart0_Tx_UseRingBuf(uint8_t * buf, uint8_t len);
void My_Uart0_Shedule(void);

/**
 * uart0 Tx While Complete
 * @param buf
 * @param len
 * @return
 */
uint8_t My_Uart0_Tx_While_Complete(uint8_t * buf, uint8_t len)
{
    R_SCI_UART_Write(&g_uart0_ctrl, buf, len);

    while(!uart_send_complete_flag);

    return 0;
}
/**
 * uart0 tx use ringbuf
 * @param buf
 * @param len
 * @return
 */
uint8_t My_Uart0_Tx_UseRingBuf(uint8_t * buf, uint8_t len)
{
    /* Add array */
    ring_buffer_queue_arr(&uart0_TxBuf, (const char *)buf, len);
    return 0;
}
/**
 * uart0 shedule
 */
void My_Uart0_Shedule(void)
{
    //rx ringbuf
    /* Is buffer empty? */
    if(!ring_buffer_is_empty(&uart0_RxBuf))
    {
        uint8_t rx[RING_BUFFER_SIZE] = {0};
        uint8_t cnt = ring_buffer_dequeue_arr(&uart0_RxBuf, (char*)rx, sizeof(uart0_RxBuf.buffer));

        //print to RTT
        rx[RING_BUFFER_SIZE-1] = 0;
        printf("uart0 rx ringbuffer data[%d]:%s\n", cnt, rx);
        printf("\t\tthe data can be used for protocol resolution\n\n");
    }

    ////////////////////////////////////////////////
    //tx ringbuf
    /* Is buffer empty? */
    if(!ring_buffer_is_empty(&uart0_TxBuf))
    {
        uint8_t tx[RING_BUFFER_SIZE] = {0};
        uint8_t cnt = ring_buffer_dequeue_arr(&uart0_TxBuf, (char*)tx, sizeof(uart0_TxBuf.buffer));

        My_Uart0_Tx_While_Complete(tx, cnt);
        printf("uart0 tx data user ringbuffer complete! pls check use sscom!!!\n\n");
    }

}

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif

    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* If this board has no LEDs then trap here */
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;


    fsp_err_t err = FSP_SUCCESS;
    err = R_SCI_UART_Open (&g_uart0_ctrl, &g_uart0_cfg);
    assert(FSP_SUCCESS == err);

    char test[20] = "uart test\n";
    My_Uart0_Tx_While_Complete((uint8_t*)test, (uint8_t)strlen(test));

    //init uart tx/rx ringbuffer
    ring_buffer_init(&uart0_TxBuf);
    ring_buffer_init(&uart0_RxBuf);

    //Load test data into the tx ringbuffer  ASCII
    char tx[RING_BUFFER_SIZE] = {0};
    for(uint8_t i = 48; i < 122; i++)
    {
        tx[i-48] = i;
    }tx[RING_BUFFER_SIZE-1] = 0;


    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);
    printf("the demo show uart0 use ring buffer,Please connect demo board to the computer with a serial-usb\n");
    printf("tx:p411 \nrx:p410\n");
    printf("\r\n1:===>>>The demo board will send data after 5 seconds\n");
    for(uint8_t i = 1; i <= 5; i++)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_SECONDS);
        printf("%d\n", i);
    }printf("launched!!!\n");
    My_Uart0_Tx_UseRingBuf((uint8_t*)tx, (uint8_t)strlen(tx));
    printf("You can see the string in your computer's sscom tool\n");
    printf("\r\n2:===>>>Now it's your turn, please send the string again with sscom, \nthen you can see the same string in RTT\n\n");


    while (1)
    {
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
        R_BSP_PinAccessEnable();

        /* Update all board LEDs */
        for (uint32_t i = 0; i < leds.led_count; i++)
        {
            /* Get pin to toggle */
            uint32_t pin = leds.p_leds[i];

            /* Write to this pin */
            R_BSP_PinWrite((bsp_io_port_pin_t) pin, pin_level);
        }

        /* Protect PFS registers */
        R_BSP_PinAccessDisable();

        /* Toggle level for next write */
        if (BSP_IO_LEVEL_LOW == pin_level)
        {
            pin_level = BSP_IO_LEVEL_HIGH;
        }
        else
        {
            pin_level = BSP_IO_LEVEL_LOW;
        }

        /* Delay */
        //R_BSP_SoftwareDelay(delay, bsp_delay_units);
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

        My_Uart0_Shedule();
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, g_ioport.p_cfg);
    }
}

void g_uart0CB (uart_callback_args_t * p_args)
{
   switch (p_args->event)
   {
      case UART_EVENT_RX_CHAR:
      {
            ring_buffer_queue(&uart0_RxBuf, (char) p_args->data);
            break;
      }
      case UART_EVENT_TX_COMPLETE:
      {
            uart_send_complete_flag = true;
            break;
      }
      default:
            break;
   }
}
