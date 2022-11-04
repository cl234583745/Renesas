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
#include "SEGGER_RTT.h"
void R_BSP_WarmStart(bsp_warm_start_event_t event);

extern bsp_leds_t g_bsp_leds;
/**
 * if enable SEGGER_RTT_V780c/Syscalls, pls #define PRINTF 0,
 * then redirect printf to SEGGER_RTT.
 */
#pragma message("if enable files:SEGGER_RTT_V780c/Syscalls,pls #define PRINTF 0,then redirect printf to SEGGER_RTT.")
#define PRINTF 0

#ifndef PRINTF
#define PRINTF 1
#endif

#if PRINTF
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


/* Variable to store user input for DAC conversion */
static int32_t g_inputRead = 100;
/* variable to store scanned ADC data */
static uint16_t g_adc0_data = 0;
static uint16_t g_adc1_data = 0;
static uint16_t g_adc2_data = 0;
static uint16_t g_adc3_data = 0;
volatile bool scan_complete_flag = false;

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
#if PRINTF
    /* Open the transfer instance with initial configuration. */
    fsp_err_t err = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    assert(FSP_SUCCESS == err);

    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);

    double PI = 3.1415926;
    printf("PI=%f\n", PI);
#else
    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);
#endif//PRINTF
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

    printf("Welcome to test the project!\nDAC output will start after 5 seconds, \nand ADC collection will start at the same time \n");

    //////////////////////////////////////////////////////////
    /* Open the DAC channel */
    fsp_err_t err = FSP_SUCCESS;
    err = R_DAC_Open (&g_dac0_ctrl, &g_dac0_cfg);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* DAC module open failed */
        printf("\r\n ** DAC Open API failed ** \r\n");
        printf("ERR[%d]%d %d",err,__FUNCTION__,__LINE__);;
    }

    /* Write value to DAC module */
    err = R_DAC_Write (&g_dac0_ctrl, (uint16_t) (g_inputRead));
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* dac Write Failure message */
        printf("** DAC Write API failed ** \r\n");
        return err;
    }

    /* Start DAC conversion */
    err = R_DAC_Start (&g_dac0_ctrl);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* dac start failure message */
        printf("** DAC Start API failed ** \r\n");
        return err;
    }
    /* As mentioned in DAC properties and Electrical characteristics of RA2A1 User Manual (Section 47.7),
     * a software delay has to be given for stabilizing the voltage on DAC output pin.
     * Also referred as DAC conversion time delay.*/
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    //////////////////////////////////////////////////////////
    /* Open/Initialize ADC module */
    err = R_ADC_Open (&g_adc0_ctrl, &g_adc0_cfg);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        /*ADC module open failed */
        printf("\r\n ** ADC module Open failed ** \r\n");
        printf("ERR[%d]%d %d",err,__FUNCTION__,__LINE__);;
    }

    /* Configures the ADC scan parameters */
    err = R_ADC_ScanCfg (&g_adc0_ctrl, &g_adc0_channel_cfg);

    /* handle error */
    if (FSP_SUCCESS != err)
    {
        R_ADC_Close(&g_adc0_ctrl);
        R_DAC_Close(&g_dac0_ctrl);
        /* ADC Failure message */
        printf("** ADC ScanCfg API failed ** \r\n");
        printf("ERR[%d]%d %d",err,__FUNCTION__,__LINE__);;
    }

    /* Start the ADC scan in Single scan mode*/
    err = R_ADC_ScanStart(&g_adc0_ctrl);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* ADC Scan Failure message */
        printf("** ADC ScanStart API failed ** \r\n");
        return err;
    }
    ///////////////////////////////////////////////////////////
    for(uint8_t i = 1; i <= 5; i++)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_SECONDS);
        printf("%d\n", i);
    }printf("Let`s begin!!!\n");

    while (1)
    {
        if(scan_complete_flag)
        {
            scan_complete_flag = 0;


            err = R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_0, &g_adc0_data);
                assert(FSP_SUCCESS == err);
            err = R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_1, &g_adc1_data);
            assert(FSP_SUCCESS == err);
            err = R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_2, &g_adc2_data);
                assert(FSP_SUCCESS == err);
            err = R_ADC_Read(&g_adc0_ctrl, ADC_CHANNEL_3, &g_adc3_data);
                assert(FSP_SUCCESS == err);

            printf("u can input change: DAC=%d ADC=%4d %4d %4d %4d\n", (int)g_inputRead,g_adc0_data,g_adc1_data,g_adc2_data,g_adc3_data);

            R_ADC_ScanStart(&g_adc0_ctrl);
        }

        /* Validate input value from user */
        if(SEGGER_RTT_HasKey())
        {
            uint8_t readBuff[16] = { 0 };
            SEGGER_RTT_Read (0, readBuff, sizeof(readBuff));

            /* function returns the converted integral number as an int value.*/
            g_inputRead = atoi ((char *) readBuff);
            if ((1 <= g_inputRead) && (4095 >= g_inputRead))
            {
                R_DAC_Write (&g_dac0_ctrl, (uint16_t) (g_inputRead));
            }
        }



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

/*******************************************************************************************************************//**
 * @brief ADC callback function.
 * @param[in]  p_args
 * @retval     None
 **********************************************************************************************************************/
void g_adc0CB(adc_callback_args_t *p_args)
{
    if (ADC_EVENT_SCAN_COMPLETE == p_args->event)
    {
        fsp_err_t err = FSP_SUCCESS;     // Error status

        scan_complete_flag = true;

    }

}
