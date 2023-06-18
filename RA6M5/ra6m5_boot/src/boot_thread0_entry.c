/***********************************************************************************************************************
 * duokajiang
 * 292812832@qq.com 
 * 2023-6-13
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include "boot_thread0.h"
#include "do_boot.h"
#include "flash_hp.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/




////////////////////////////
/// Reserving MD pins and jtag interfaces is strongly recommended in hardware design.
/// Because you can use the swd interface in Single-Chip mode;
/// In addition, you can lower the MD pin to enter the SCI Boot mode during power-on.
/// In this way, you can modify some necessary Settings using the sci interface
///
/**
 * if enable SEGGER_RTT_V780c/Syscalls, pls #define PRINTF 0,
 * then redirect printf to SEGGER_RTT.
 */
#pragma message("if enable files:SEGGER_RTT_V780c/Syscalls,pls #define PRINTF 0,then redirect printf to SEGGER_RTT.")
#define PRINTF 1

#ifndef PRINTF
#define PRINTF 1
#endif

#if PRINTF
/**
 * notice: g_uart9CB; g_uart9_ctrl
 *
 * e2s:
 * 1.uart9 callback:g_uart9CB
 * 2.FSP-BSP-heap size:0x1000
 * 3.-u _printf_float
 * 4.other link void
 *
 * iar:
 * 1.uart9 callback:g_uart9CB
 * 2.FSP-BSP-heap size:0x1000
 * 3.libray=full
 * 4.Semihosted=None
 *
 * keil:
 * 1.uart9 callback:g_uart9CB
 * 2.FSP-BSP-heap size:0x1000
 */
volatile bool uart_send_complete_flag = false;
void g_uart9CB (uart_callback_args_t * p_args)
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
   fsp_err_t err = R_SCI_UART_Write(&g_uart9_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
   if(FSP_SUCCESS != err) __BKPT();

   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return size;
}
#else
int fputc(int ch, FILE *f)
{
   (void)f;
   fsp_err_t err = R_SCI_UART_Write(&g_uart9_ctrl, (uint8_t *)&ch, 1);
   if(FSP_SUCCESS != err) __asm("bkpt 0");
   while(uart_send_complete_flag == false);
   uart_send_complete_flag = false;

   return ch;
}
#endif//#if defined __GNUC__ && !defined __clang__
#endif//PRINTF
/////////////////////////////////////////////

/*******************************************************************************************************************//**
 * @brief    Check if reset caused by LVD0 or not. If yes, Turn LED ON.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static fsp_err_t check_reset_status(void)
{
    fsp_err_t io_err = FSP_SUCCESS;

    printf ("\nRSTSR0=%X RSTSR1=%X RSTSR2=%X\n", R_SYSTEM->RSTSR0, R_SYSTEM->RSTSR1, R_SYSTEM->RSTSR2);

    //RSTSR0
    if (1 == R_SYSTEM->RSTSR0_b.PORF)
    {
        printf ("\r\n************************ Power-On Reset detected ************************\r\n");
    }
    /* Check if reset was caused by the LVD0? If it was, turn on LED to indicate LVD0 reset triggered. */
    if (1 == R_SYSTEM->RSTSR0_b.LVD0RF)
    {
        printf ("\r\n************************ LVD0 Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR0_b.LVD1RF)
    {
        printf ("\r\n************************ LVD1 Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR0_b.LVD2RF)
    {
        printf ("\r\n************************ LVD2 Reset detected ************************\r\n");
    }
    //RSTSR1
    if (1 == R_SYSTEM->RSTSR1_b.IWDTRF)
    {
        printf ("\r\n************************ Independent Watchdog Timer Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.WDTRF)
    {
        printf ("\r\n************************ Watchdog Timer Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.SWRF)
    {
        /* Clear the flag once read the value */
        R_SYSTEM->RSTSR1_b.SWRF = 0;

        printf ("\r\n************************ Software Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.RPERF)
    {
        /* Clear the flag once read the value */
        R_SYSTEM->RSTSR1_b.RPERF = 0;

        printf ("\r\n************************ SRAM Parity Error Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.REERF)
    {
        printf ("\r\n************************ SRAM ECC Error Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.BUSMRF)
    {
        /* Clear the flag once read the value */
        R_SYSTEM->RSTSR1_b.BUSMRF = 0;

        printf ("\r\n************************ Bus Master MPU Error Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.TZERF)
    {
        printf ("\r\n************************ TrustZone Error Reset detected ************************\r\n");
    }
    if (1 == R_SYSTEM->RSTSR1_b.CPERF)
    {
        printf ("\r\n************************ Cache Parity Error Reset detected ************************\r\n");
    }
    //RSTSR2
    if (1 == R_SYSTEM->RSTSR2_b.CWSF)
    {
        printf ("\r\n************************ Warm start ************************\r\n");
    }
    else
    {
        printf ("\r\n************************ Cold start ************************\r\n");
    }

    return io_err;
}

/* Boot Thread entry function */
/* pvParameters contains TaskHandle_t */
void boot_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

///////////////////////////////////////////////////////////////////////////
    fsp_err_t err = FSP_SUCCESS;
    err = g_uart9.p_api->open(&g_uart9_ctrl, &g_uart9_cfg);
    if(err == FSP_SUCCESS)
    {
        const char *log_uart_init = "\nboot log uart init successed!!!\r\n";
        g_uart9.p_api->write(&g_uart9_ctrl, (uint8_t*)log_uart_init, strlen(log_uart_init));
        while(uart_send_complete_flag == false)
        {;}
        uart_send_complete_flag = false;
    }
    printf("\nprintf redirect successed!!!\ndate:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);

    check_reset_status();

    /* Open Flash_HP */
    err = R_FLASH_HP_Open(&g_flash0_ctrl, &g_flash0_cfg);
    /* Handle Error */
    if (FSP_SUCCESS != err)
    {
        printf("R_FLASH_HP_Open API failed=%d", err);
    }

    uint8_t fwUpgradeFlg = 0;
    /*Read code flash data */
    readCodeFlash((uint8_t *) FLASH_HP_CF_BLCOK_69, &fwUpgradeFlg, sizeof(fwUpgradeFlg));
    printf("fwUpgradeFlg=%d", fwUpgradeFlg);

    if(fwUpgradeFlg == 1)
    {
        //do frameware upgrade over Ethernet or other
        //creat frameware thread
        //download frameware and check crc
        //write fwUpgradeFlg = 1, jump to app or reset
    }
    else
    {
        unsigned long Boot_Addr = 0x10000;//memory_regions_boot.ld FLASH_LENGTH
        printf("\n**************************************\n");
        printf(  "**********JUMP TO APP:0x%lX**********\n", Boot_Addr);
        printf(  "**************************************\n");

        //close any peripherals before do_boot
        g_uart9.p_api->close(&g_uart9_ctrl);
        R_FLASH_HP_Close(&g_flash0_ctrl);

        //jump
        do_boot(Boot_Addr);

        printf("this message cant print!\n");
    }
///////////////////////////////////////////////////////////////////////////
    /* TODO: add your own code here */
    while (1)
    {
        vTaskDelay (1);
    }
}
