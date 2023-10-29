/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

#include "hal_data.h"



void R_BSP_WarmStart(bsp_warm_start_event_t event) BSP_PLACE_IN_SECTION(".warm_start");
void user_copy_to_sram (void);

extern bsp_leds_t g_bsp_leds;
extern void atcm_user_init(void);
extern void coremain(void);//BSP_PLACE_IN_SECTION(".atcm");


///rzn2l may be not support segger rtt print, so exclude file SEGGER_RTT_V780c
#ifndef PRINTF
#define PRINTF
#endif
#ifdef PRINTF
#include <stdio.h>
/**
 * notice: g_uart0CB; g_uart0_ctrl
 *
 * e2s:
 * 1.uart0 callback:g_uart0CB
 * 2.FSP-BSP-heap size:0x400
 * 3.-u _printf_float
 * 4.other link void
 *
 * iar:
 * 1.uart0 callback:g_uart0CB
 * 2.FSP-BSP-heap size:0x400
 * 3.libray=full
 * 4.Semihosted=None
 *
 * keil:
 * 1.uart0 callback:g_uart0CB
 * 2.FSP-BSP-heap size:0x400
 */
volatile bool uart_send_complete_flag = false;
void g_uart0CB (uart_callback_args_t * p_args)
{
    if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uart_send_complete_flag = true;
    }
}
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //??????
int _write(int fd, char *pBuffer, int size)
{
   (void)fd;
   fsp_err_t err = R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
   if(FSP_SUCCESS != err) __BKPT();
   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return size;
}
#else
int fputc(int ch, FILE *f)
{
   (void)f;
   fsp_err_t err = R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)&ch, 1);
   if(FSP_SUCCESS != err) __asm("bkpt 0");
   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return ch;
}
#endif//#if defined __GNUC__ && !defined __clang__
#endif//PRINTF






extern void func_atcm_bss_init (void);
///////////////////////////////////////////////
/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
    //R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);

    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;



    /////////////////////////
#ifdef PRINTF
    g_uart0.p_api->open(&g_uart0_ctrl, &g_uart0_cfg);
    __asm volatile ("cpsie i");
    g_uart0.p_api->write(&g_uart0_ctrl, "hello word!\n", strlen("hello word!\n"));
    while(!uart_send_complete_flag);
    uart_send_complete_flag = false;
    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);

    float PI = 3.1415926;
    printf("PI=%f\n", PI);
#endif//PRINTF


    R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    R_GPT_Start(&g_timer0_ctrl);
    R_GPT_Enable(&g_timer0_ctrl);

    printf("start coremain!!!\r\n");


#if FSP_XSPI0_BOOT_SRAM_ATCM//only use fsp_xspi0_boot_SRAM_ATCM.ld
    //first, change FSP_XSPI0_BOOT_SRAM_ATCM in project property - symbol
    //second，change .ld in project property cross arm c linker - general
    atcm_user_init();

    printf("use fsp_xspi0_boot_SRAM_ATCM.ld!!!\r\n");
#else
    printf("use fsp_xspi0_boot.ld or use fsp_xspi0_boot_SRAM_MIRROR.ld!!!\r\n");
#endif

    coremain();

    //default 200Mhz，modify fsp-clock  CPUOCLK Mul:ICLKx1 -> CPUOCLK Mul:ICLKx2
    uint32_t freq_hz = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_CPU0);
    printf("FSP_PRIV_CLOCK_CPU0=%ld\r\n", freq_hz);

    while (1)
    {
        printf("running!!!\r\n");
        R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);

        R_BSP_PinToggle (BSP_IO_REGION_SAFE, (bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED0]);
        R_BSP_PinToggle (BSP_IO_REGION_SAFE, (bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED1]);
        R_BSP_PinToggle (BSP_IO_REGION_SAFE, (bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED2]);
        R_BSP_PinToggle (BSP_IO_REGION_SAFE, (bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED3]);

    }





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

    while (1)
    {
        /* This code uses BSP IO functions to show how it is used.*/
        R_BSP_PinAccessEnable();

        /* Update all board LEDs */
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED0], pin_level);
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED1], pin_level);
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED2], pin_level);
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED3], pin_level);

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
        R_BSP_SoftwareDelay(delay, bsp_delay_units);
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
    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

    }
}

/*
 * g_timer0CB
 * rzn2l core R52 havenot systick, so init g_timer0 cnt
 */
extern volatile clock_t cnt;
void g_timer0CB(timer_callback_args_t *p_args)
{
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        cnt++;
    }
}


