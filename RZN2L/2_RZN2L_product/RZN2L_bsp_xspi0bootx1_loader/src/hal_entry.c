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
#include "loader_table.h"

#define BSC_PROTECT_KEY     (0xa55a0000)

void R_BSP_WarmStart(bsp_warm_start_event_t event) BSP_PLACE_IN_SECTION(".warm_start");
#if 0 /* SDRAM and XSPI0 cannot be used at the same time on RZ/N2L RSK board. */
void bsp_sdram_init(void);
#endif

volatile bool uartTxCompleteFlg = 0;

extern bsp_leds_t g_bsp_leds;
extern void bsp_copy_4byte(uint32_t *src, uint32_t *dst, uint32_t bytesize);
extern const loader_table table[TABLE_ENTRY_NUM];

/*******************************************************************************************************************//**
 * @brief  Example application
 *
 * Copies the program in application project from external flash memory to RAM.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
    uint8_t table_num;

    void (*app_prg)(void);

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

#if 0 /* SDRAM and XSPI0 cannot be used at the same time on RZ/N2L RSK board. */
    /* SDRAM settings. */
    bsp_sdram_init();
#endif

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_HIGH;

    /* Unprotect I/O port registers */
    R_BSP_PinAccessEnable();

    /* Turn on LED0: Indicate application program copy start */
    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED0], pin_level);

    /* Copy application program with parameter in loader table. */
    /* In this sample, application program is copied from Flash memory to System SRAM.*/
    for ( table_num = 0; table_num < TABLE_ENTRY_NUM; table_num++)
    {
      if (table[table_num].enable_flag == TABLE_ENABLE)
      {
        bsp_copy_4byte(table[table_num].src, table[table_num].dst, table[table_num].size);
      }
    }

    /* Ensuring data-changing */
    __asm volatile("dsb");

    /* Turn on LED3: Indicate application program copy end */
    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[BSP_LED_RLED3], pin_level);

    /* Protect I/O port registers */
    R_BSP_PinAccessDisable();

//正确完整的编译loader+app
// 1: 必须首先clean loader，构建前增加make -r -j8 clean
// 2: 使能startu的初始化mpu代码 #if 1 // Original program
    /* Invalid these settings for loader project.
    * These settings are done in the application program.
    * Settings can also be made in the loader program if necessary. */

#if 1
    __enable_irq();
    g_uart0.p_api->open(g_uart0.p_ctrl, g_uart0.p_cfg);
    g_uart0.p_api->write(g_uart0.p_ctrl, (uint8_t const *)"Loader start!\n*****\nReady to Jump to the app!\n\n", strlen("Loader start!\n*****\nReady to Jump to the app!\n\n"));
    while(!uartTxCompleteFlg);
    uartTxCompleteFlg = 0;
    g_uart0.p_api->close(g_uart0.p_ctrl);
    __disable_irq();
#endif

    /* Delay */
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);

    /* Set application program destination to app_prg */
    app_prg = (void(*)(void))table[0].dst;

    /* Jump to the application project */
    app_prg();
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

#if 0 /* SDRAM and XSPI0 cannot be used at the same time on RZ/N2L RSK board. */
void bsp_sdram_init(void)
{
    volatile uint32_t val;

    /* NOTE: Port setting and CKIO configuration should have been done before */
    /* Enable BSC and CKIO module */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_BSC, 0);
    R_BSP_MODULE_START(FSP_IP_CKIO, 0);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    /* SDRAM:W9825G6KH-6 */
    /* Row address: A0-A12. Column address: A0-A8. */
    val = ( 2 <<  9)    // BSZ: data bus witdh: 16-bits
        | ( 1 << 11)    // Reserved
        | ( 4 << 12)    // TYPE: SDRAM
        | ( 0 << 16)    // IWRRS: Idle State Insertion between Read-Read Cycles in the Same CS Space
        | ( 0 << 19)    // IWRRD: Idle State Insertion between Read-Read Cycles in Different CS Spaces
        | ( 0 << 22)    // IWRWS: Idle State Insertion between Read-Write Cycles in the Same CS Space
        | ( 0 << 25)    // IWRWD: Idle State Insertion between Read-Write Cycles in Different CS Spaces
        | ( 0 << 28);   // IWW: Idle Cycles between Write-Read Cycles and Write-Write Cycles
    R_BSC->CSnBCR[3] = val;

    val = R_SYSC_NS->SCKCR_b.CKIO;
    switch (val)
    {
        case 1:
            R_BSC->CS3WCR_1 = 0x0000249a;
            R_BSC->RTCOR = BSC_PROTECT_KEY | 32;
            break;
        case 2:
            R_BSC->CS3WCR_1 = 0x00000090;
            R_BSC->RTCOR = BSC_PROTECT_KEY | 24;
            break;
        case 3:
            R_BSC->CS3WCR_1 = 0x00000090;
            R_BSC->RTCOR = BSC_PROTECT_KEY | 19;
            break;
        case 4:
            R_BSC->CS3WCR_1 = 0x00000088;
            R_BSC->RTCOR = BSC_PROTECT_KEY | 16;
            break;
        case 5:
            R_BSC->CS3WCR_1 = 0x00000088;
            R_BSC->RTCOR = BSC_PROTECT_KEY | 13;
            break;
        case 6:
        default:
            R_BSC->CS3WCR_1 = 0x00000088;
            R_BSC->RTCOR = BSC_PROTECT_KEY | 12;
            break;
    }

    /* Configure drive ability for CKIO/P04_1 */
    R_BSP_PinAccessEnable();
    if (val <= 2)
    {
        R_PORT_SR->DRCTL[4].L_b.DRV1 = 2;    // High-drive
    }
    else
    {
        R_PORT_SR->DRCTL[4].L_b.DRV1 = 1;    // Mid-drive
    }
    R_BSP_PinAccessDisable();

    R_BSC->SDCR = 0x00000811;       // auto-refresh, auto-precharge mode, Col 9-bits, Row 13-bits for CS3#

    val = R_BSC->RTCSR;                // To clear CMT flag, read before write
    R_BSC->RTCSR = BSC_PROTECT_KEY
                 | (  0 <<  7)         // Compare match Flag: clear
                 | (  0 <<  6)         // Compare match interrupt enable: Disabled
                 | (  2 <<  3)         // Refresh timer count clock: CKIO/16 (start count)
                 | (  0 <<  0);        // Refresh count: 1 time

    /* wait 200us*/
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MICROSECONDS);

    *((uint16_t *)0x80212040) = 0x0000;   // Burst length=1, Sequential, CL=2, Burst read and burst write for CS3#

    /* wait 1us*/
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
}
#endif

void user_uart_callback(uart_callback_args_t *p_args)
{
    if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uartTxCompleteFlg = 1;
    }
}
