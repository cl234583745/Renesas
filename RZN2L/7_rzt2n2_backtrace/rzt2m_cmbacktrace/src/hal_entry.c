/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event) BSP_PLACE_IN_SECTION(".warm_start");

extern bsp_leds_t g_bsp_leds;

#include <stdio.h>

static void USR_SCI_UART_Write (uart_ctrl_t * const p_api_ctrl, uint8_t const * const p_src, uint32_t const bytes);
static void USR_SCI_UART_Write (uart_ctrl_t * const p_api_ctrl, uint8_t const * const p_src, uint32_t const bytes)
{
    uint32_t i;
    sci_uart_instance_ctrl_t * p_ctrl = (sci_uart_instance_ctrl_t *) p_api_ctrl;

    volatile uint32_t *reg = (uint32_t*)&p_ctrl->p_reg->CSR;
    uint8_t *data  = (uint8_t *)p_src;
    for(i = 0; i < bytes; i++)
    {
        p_ctrl->p_reg->TDR_b.TDAT = *data;
        while(((*reg>>29)& 0x1)== 0){
        }
        data++;
    }

}
int _write(int fd, char *pBuffer, int size);
int _write(int fd, char *pBuffer, int size)
{
   (void)fd;
   USR_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)pBuffer, (uint32_t)size);


   return size;
}
static void fault_test_by_unalign(void);
static void fault_test_by_unalign(void) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    volatile int * p;
    volatile int value;

    *SCB_CCR |= (1 << 3); /* bit3: UNALIGN_TRP. */

    p = (int *) 0x00;
    value = *p;
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

    p = (int *) 0x04;
    value = *p;
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);

    p = (int *) 0x03;
    value = *p;
    printf("addr:0x%02X value:0x%08X\r\n", (int) p, value);
}
static void fault_test_by_div0(void);
static void fault_test_by_div0(void) {
    volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
    int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */

    x = 10;
    y = 0;
    z = x / y;
    printf("z:%d\n", z);
}
static int func_c();
static int func_c() {
    int c=0;
        c++;
    printf("fault_test_by_unalign\n");
    fault_test_by_unalign();
    //printf("fault_test_by_div0\n");
    //fault_test_by_div0();
    return 0;
}
static int func_b();
static int func_b() {
    int b=0;
        b++;
    return func_c();
}
static int func_a();
static int func_a() {
    int a=0;
    a++;
    return func_b();
}


/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
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

    /* This code uses BSP IO functions to show how it is used.*/
    /* Turn off LEDs */
    for (uint32_t i = 0; i < leds.led_count; i++)
    {
        R_BSP_PinClear((bsp_io_region_t) leds.p_leds[i][1], (bsp_io_port_pin_t) leds.p_leds[i][0]);
    }

    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);

    float PI = 3.1415926f;
    printf("PI=%f\n", PI);

    func_a();

    while (1)
    {
        /* Toggle board LEDs */
        for (uint32_t i = 0; i < leds.led_count; i++)
        {
            R_BSP_PinToggle((bsp_io_region_t) leds.p_leds[i][1], (bsp_io_port_pin_t) leds.p_leds[i][0]);
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
    if (BSP_WARM_START_RESET == event)
    {
        /* Pre clock initialization */
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        if (NULL != g_bsp_pin_cfg.p_extend)
        {
            /* Configure pins. */
            R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
        }
    }
}
