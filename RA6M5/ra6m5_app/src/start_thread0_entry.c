/***********************************************************************************************************************
 * duokajiang
 * 292812832@qq.com 
 * 2023-6-13
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include "start_thread0.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define LOG_TAG          "INIT_TAG"
#define LOG_LVL          ELOG_LVL_VERBOSE
#include "elog.h"

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
static lfs_file_t g_log_lfs_file;

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

void elogUart(const char *pBuffer, uint32_t size);
void elogUart(const char *pBuffer, uint32_t size)
{
    fsp_err_t err = R_SCI_UART_Write(&g_uart9_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
    if(FSP_SUCCESS != err) __BKPT();

    while(uart_send_complete_flag == false);
    uart_send_complete_flag = false;
}
/////////////////////////////////////////////
uint8_t InfoBuffer[1000];
/* Start Thread entry function */
/* pvParameters contains TaskHandle_t */
void start_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

///////////////////////////////////////////////////////////////////////////
    fsp_err_t err = FSP_SUCCESS;
    err = g_uart9.p_api->open(&g_uart9_ctrl, &g_uart9_cfg);
    if(err == FSP_SUCCESS)
    {
        const char *log_uart_init = "\napp log uart init successed!!!\r\n";
        g_uart9.p_api->write(&g_uart9_ctrl, (uint8_t*)log_uart_init, strlen(log_uart_init));
        while(uart_send_complete_flag == false)
        {;}
        uart_send_complete_flag = false;
    }

    printf("\nprintf redirect successed!!!\ndate:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);
#if 1//test
    /* if littlefs enable thread safe,must copy to common_data.c  g_rm_littlefs0_lfs_cfg,FSP 4.4.0 bug
    #ifdef LFS_THREAD_SAFE
    .lock = &rm_littlefs_flash_lock,
    .unlock = &rm_littlefs_flash_unlock,
    #endif
     */
    err = g_rm_littlefs0.p_api->open(&g_rm_littlefs0_ctrl, &g_rm_littlefs0_cfg);
    if(FSP_SUCCESS != err)
    {
        printf("g_rm_littlefs0.p_api->open err=%d\r\n", err);
        g_rm_littlefs0.p_api->close(&g_rm_littlefs0_ctrl);
    }
    printf("g_rm_littlefs0.p_api->open successed!!!\n");

    err = lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
    if( err )
    {
        printf("lfs_mount err=%d\r\n", err);
        lfs_format(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
        lfs_mount(&g_rm_littlefs0_lfs, &g_rm_littlefs0_lfs_cfg);
    }


    // read current count
    uint32_t boot_count2 = 0;
    lfs_file_open(&g_rm_littlefs0_lfs, &g_log_lfs_file, "boot_count2", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&g_rm_littlefs0_lfs, &g_log_lfs_file, &boot_count2, sizeof(boot_count2));
    printf("g_log_lfs_file.id ID:%d\r\n", g_log_lfs_file.id);

    // update boot count
    boot_count2 += 1;
    lfs_file_rewind(&g_rm_littlefs0_lfs, &g_log_lfs_file);
    lfs_file_write(&g_rm_littlefs0_lfs, &g_log_lfs_file, &boot_count2, sizeof(boot_count2));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&g_rm_littlefs0_lfs, &g_log_lfs_file);

    uint32_t txt2 = 0;
    lfs_file_open(&g_rm_littlefs0_lfs, &g_log_lfs_file, "txt2", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&g_rm_littlefs0_lfs, &g_log_lfs_file, &txt2, sizeof(txt2));
    printf("ID:%d\r\n", g_log_lfs_file.id);

    // update boot count
    txt2 += 1;
    lfs_file_rewind(&g_rm_littlefs0_lfs, &g_log_lfs_file);
    lfs_file_write(&g_rm_littlefs0_lfs, &g_log_lfs_file, &txt2, sizeof(txt2));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&g_rm_littlefs0_lfs, &g_log_lfs_file);

    // release any resources we were using
    lfs_unmount(&g_rm_littlefs0_lfs);

    // print the boot count
    printf("boot_count2: %ld\r\n", boot_count2);
    printf("txt2: %ld\r\n", txt2);
    printf("\r\n");
#endif
    ///////////////////////////////////////////////////
    ///
    elog_init();
#if 0
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
#else
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
#endif

    elog_start();
    vTaskDelay(1000);
#if 1//test
    log_a("Hello EasyLogger!");//断言 assert
    // A/NO_TAG   [10:08:12 pid:1008 tid:24] (../Core/Src/freertos.c LED_TASK03:207)Hello EasyLogger!
    log_e("Hello EasyLogger!");//error
    // E/NO_TAG   [10:08:12] Hello EasyLogger!
    log_w("Hello EasyLogger!");//waring
    // W/NO_TAG   [10:08:12] Hello EasyLogger!
    log_i("Hello EasyLogger!");//info
    // I/NO_TAG   [10:08:12] Hello EasyLogger!
    log_d("Hello EasyLogger!");//dbug
    // D/NO_TAG   [10:08:12] (../Core/Src/freertos.c:211)Hello EasyLogger!
    log_v("Hello EasyLogger!");//日志
    //V/NO_TAG   [10:08:12] (../Core/Src/freertos.c:212)Hello EasyLogger!

    uint8_t buf[256]= {0};
    uint16_t i = 0;

    for (i = 0; i < sizeof(buf); i++)
    {
        buf[i] = (uint8_t)i;
    }
    elog_hexdump("test", 16, buf, sizeof(buf));
    log_d("\nTEXT END!\n");
#endif
///////////////////////////////////////////////////////////////////////////

    /* TODO: add your own code here */
    while (1)
    {
        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        log_i("start_thread0 uxHighWaterMark=%ld", uxHighWaterMark);

        size_t xFreeBytesRemaining = xPortGetFreeHeapSize();
        log_i("heap4 xFreeBytesRemaining=%ld", xFreeBytesRemaining);

        size_t xMinimumEverFreeBytesRemaining = xPortGetMinimumEverFreeHeapSize();
        log_i("heap4 xMinimumEverFreeBytesRemaining=%ld", xMinimumEverFreeBytesRemaining);

        vTaskDelay (5000);
    }
}
