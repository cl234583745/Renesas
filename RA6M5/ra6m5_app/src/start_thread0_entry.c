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
#include "sys_log.h"

#include "shell.h"
#include "getopt.h"

#include "ringbuffer.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define LOG_TAG          "INIT_TAG"
#define LOG_LVL          ELOG_LVL_VERBOSE
#include "elog.h"

extern TaskHandle_t start_thread0;
extern TaskHandle_t tcp_thread1;
extern TaskHandle_t uart_thread2;

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
lfs_file_t g_log_lfs_file;
static ring_buffer_t uart9_RxBuf;
static uint8_t rx[RING_BUFFER_SIZE] = {0};
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
    switch (p_args->event)
    {
        case UART_EVENT_RX_CHAR:
        {
            ring_buffer_queue(&uart9_RxBuf, (char) p_args->data);
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

/*
 * elogUart for easylogger
 */
void elogUart(const char *pBuffer, uint32_t size);
void elogUart(const char *pBuffer, uint32_t size)
{
    fsp_err_t err = R_SCI_UART_Write(&g_uart9_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
    if(FSP_SUCCESS != err) __BKPT();

    while(uart_send_complete_flag == false);
    uart_send_complete_flag = false;
}
/////////////////////////////////////////////
static fsp_err_t R_ADC_Read_Chip_Temp(double *temp);
static fsp_err_t R_ADC_Read_Chip_Temp(double *temp)
{
    fsp_err_t err = FSP_SUCCESS;
    static double die_temperature = 0 ;
    static double V1 = 0;
    static double Vs = 0;
    static adc_info_t adc_info ; //adc information
    //Output from ADC
    static uint16_t voltage_out ;

    //Open and start ADC
    err = R_ADC_Open(&g_adc0_ctrl, &g_adc0_cfg);
    if(err != FSP_SUCCESS)
    {
        printf("ERR! R_ADC_Open err=%d",err);
        return err;
    }

    //Get scan configurations
    err = R_ADC_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
    if(err != FSP_SUCCESS)
    {
        printf("ERR! R_ADC_ScanCfg err=%d",err);
        return err;
    }

    err = R_ADC_ScanStart(&g_adc0_ctrl);
    if(err != FSP_SUCCESS)
    {
        printf("ERR! R_ADC_ScanStart err=%d",err);
        return err;
    }

    //Read voltage from ADC
    err = R_ADC_Read (&g_adc0_ctrl, ADC_CHANNEL_TEMPERATURE, &voltage_out);
    if(err != FSP_SUCCESS)
    {
        printf("ERR! R_ADC_Read err=%d",err);
        return err;
    }

    //Get ADC info
    err = R_ADC_InfoGet(&g_adc0_ctrl, &adc_info);
    if(err != FSP_SUCCESS)
    {
        printf("ERR! R_ADC_InfoGet err=%d",err);
        return err;
    }

    //Die temperature formula according to TSN documentation: T = ( Vs - V1 ) / slope + 125 C
    V1 = ( 3.3 * (int) adc_info.calibration_data  ) / 4096 ;
    Vs = ( 3.3 * (int) voltage_out ) / 4096 ;

    die_temperature = 1000000 * ( Vs - V1 ) / ( adc_info.slope_microvolts ) + 125.0;

    *temp = die_temperature;

    return err;
}
uint8_t shell(uint8_t argc, char **argv);
uint8_t shell(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hig:s:";//init short_options must include long_options param 4
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"get", required_argument, NULL, 'g'},
        {"set", required_argument, NULL, 's'},
        {"times", required_argument, NULL, 1},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            case 'g' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "g_%s", optarg);

                break;
            }

            /* test */
            case 's' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "s_%s", optarg);

                break;
            }

            /* running times */
            case 1 :
            {
                /* set the times */
                times = (uint32_t)atol(optarg);

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("g_highwater", type) == 0)
    {
        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( start_thread0 );
        log_i("start_thread0 uxHighWaterMark=%ld", uxHighWaterMark);
        uxHighWaterMark = uxTaskGetStackHighWaterMark( tcp_thread1 );
        log_i("tcp_thread1 uxHighWaterMark=%ld", uxHighWaterMark);
        uxHighWaterMark = uxTaskGetStackHighWaterMark( uart_thread2 );
        log_i("uart_thread2 uxHighWaterMark=%ld", uxHighWaterMark);

        return 0;
    }
    else if (strcmp("g_freeheap", type) == 0)
    {
        size_t xFreeBytesRemaining = xPortGetFreeHeapSize();
        log_i("heap4 xFreeBytesRemaining=%ld", xFreeBytesRemaining);

        size_t xMinimumEverFreeBytesRemaining = xPortGetMinimumEverFreeHeapSize();
        log_i("heap4 xMinimumEverFreeBytesRemaining=%ld", xMinimumEverFreeBytesRemaining);

        return 0;
    }
    else if (strcmp("g_dietemp", type) == 0)
    {
        double die_temperature = 0 ;
        while(times--)
        {
            fsp_err_t err = R_ADC_Read_Chip_Temp(&die_temperature);
            if(err == FSP_SUCCESS)
            {
                static char string_die_temp[10] = {0};
                sprintf( string_die_temp, "%.2f", die_temperature  );
                int a = (int)(die_temperature*100);
                log_i("RA6M5 current chip temperature=%s %d", string_die_temp, a);
                //SEGGER_RTT_printf(0, "RA6M5 current chip temperature=%s a=%d\n", string_die_temp, a);
            }
            else
            {
                log_i("ERR!R_ADC_Read_Chip_Temp err=%d", err);
            }
            vTaskDelay (10);
        }
    }
    else if (strcmp("s_param", type) == 0)
    {
        /* set the times */
        times = (uint32_t)atol(optarg);
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        log_i("Usage:");
        log_i("  shell (-h | --help)");
        log_i("  shell (-i | --information)");
        log_i("  shell (-g highwater | --get=highwater)");
        log_i("  shell (-g freeheap | --get=freeheap)");
        log_i("  shell (-g dietemp | --get=dietemp)");
        log_i("Options:");
        log_i("      --times=<num>    Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        fsp_pack_version_t version;
        R_FSP_VersionGet(&version);
        const uint8_t versionstr[] = FSP_VERSION_BUILD_STRING;
        log_i("fsp:%s", versionstr);

        const uint32_t bspVCC = BSP_CFG_MCU_VCC_MV;
        const uint32_t bspStack = BSP_CFG_STACK_MAIN_BYTES;
        const uint32_t bspHeap = BSP_CFG_HEAP_BYTES;
        log_i("bspVCC=%d MV,bspStack:%d Byte,bspHeap=%d Byte", (int)bspVCC, (int)bspStack, (int)bspHeap);

        volatile const bsp_unique_id_t * uniqueID;
        uniqueID = R_BSP_UniqueIdGet();
        log_i("uniqueID:%08X%08X%08X%08X", uniqueID->unique_id_words[0],uniqueID->unique_id_words[1],uniqueID->unique_id_words[2],uniqueID->unique_id_words[3]);
        log_i("MAC:%02X%02X%02X%02X%02X%02X",g_ether0.p_cfg->p_mac_address[0],g_ether0.p_cfg->p_mac_address[1],g_ether0.p_cfg->p_mac_address[2],g_ether0.p_cfg->p_mac_address[3],g_ether0.p_cfg->p_mac_address[4],g_ether0.p_cfg->p_mac_address[5]);

        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( start_thread0 );
        log_i("start_thread0 uxHighWaterMark=%ld", uxHighWaterMark);
        uxHighWaterMark = uxTaskGetStackHighWaterMark( tcp_thread1 );
        log_i("tcp_thread1 uxHighWaterMark=%ld", uxHighWaterMark);
        uxHighWaterMark = uxTaskGetStackHighWaterMark( uart_thread2 );
        log_i("uart_thread2 uxHighWaterMark=%ld", uxHighWaterMark);

        size_t xFreeBytesRemaining = xPortGetFreeHeapSize();
        log_i("heap4 xFreeBytesRemaining=%ld", xFreeBytesRemaining);

        size_t xMinimumEverFreeBytesRemaining = xPortGetMinimumEverFreeHeapSize();
        log_i("heap4 xMinimumEverFreeBytesRemaining=%ld", xMinimumEverFreeBytesRemaining);

        double die_temperature = 0 ;
        fsp_err_t err = R_ADC_Read_Chip_Temp(&die_temperature);
        if(err == FSP_SUCCESS)
        {
            static char string_die_temp[10] = {0};
            sprintf( string_die_temp, "%.2f", die_temperature  );
            int a = (int)(die_temperature*100);
            log_i("RA6M5 current chip temperature=%s %d", string_die_temp, a);
            //SEGGER_RTT_printf(0, "RA6M5 current chip temperature=%s a=%d\n", string_die_temp, a);
        }
        else
        {
            log_i("ERR!R_ADC_Read_Chip_Temp err=%d", err);
        }
        return 0;
    }
    return 0;
}

//uint8_t InfoBuffer[1000];
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

    fsp_pack_version_t version;
    R_FSP_VersionGet(&version);
    const uint8_t versionstr[] = FSP_VERSION_BUILD_STRING;
    printf("fsp:%s", versionstr);

    const uint32_t bspVCC = BSP_CFG_MCU_VCC_MV;
    const uint32_t bspStack = BSP_CFG_STACK_MAIN_BYTES;
    const uint32_t bspHeap = BSP_CFG_HEAP_BYTES;
    printf("bspVCC=%d MV,bspStack:%d Byte,bspHeap=%d Byte", (int)bspVCC, (int)bspStack, (int)bspHeap);

    volatile const bsp_unique_id_t * uniqueID;
    uniqueID = R_BSP_UniqueIdGet();
    printf("uniqueID:%08X%08X%08X%08X", (unsigned int)uniqueID->unique_id_words[0],(unsigned int)uniqueID->unique_id_words[1],(unsigned int)uniqueID->unique_id_words[2],(unsigned int)uniqueID->unique_id_words[3]);
    printf("MAC:%02X%02X%02X%02X%02X%02X",g_ether0.p_cfg->p_mac_address[0],g_ether0.p_cfg->p_mac_address[1],g_ether0.p_cfg->p_mac_address[2],g_ether0.p_cfg->p_mac_address[3],g_ether0.p_cfg->p_mac_address[4],g_ether0.p_cfg->p_mac_address[5]);

///////////////////////////////////////////////////////////////////////////

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
    //start easylogger
    elog_start();
    vTaskDelay(1000);
#if 1//test easylogger
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

    //init simple shell, then start it
    shell_init();
    shell_register("shell", shell);
    log_i("shell: welcome to shell ra6m5");

    ring_buffer_init(&uart9_RxBuf);
    /* TODO: add your own code here */
    while (1)
    {
        //get strings from uart0 rx ring buffer
        if(!ring_buffer_is_empty(&uart9_RxBuf))
        {
            //get strings from uart0 rx ring buffer
            uint8_t cnt = ring_buffer_dequeue_arr(&uart9_RxBuf, (char*)rx, sizeof(uart9_RxBuf.buffer));
            //log_i("rx=%s cnt=%d",rx,cnt);//print input string for test

            //shell parse, shell register callback 'shell'
            uint8_t res = shell_parse((char *)rx, cnt);

            //shell parse result informations
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                log_i("shell: run failed.\n");
            }
            else if (res == 2)
            {
                log_i("shell: unknown command.\n");
            }
            else if (res == 3)
            {
                log_i("shell: length is too long.\n");
            }
            else if (res == 4)
            {
                log_i("shell: pretreat failed.\n");
            }
            else if (res == 5)
            {
                log_i("shell: param is invalid.\n");
            }
            else
            {
                log_i("shell: unknown status code.\n");
            }
        }

        vTaskDelay (100);
    }
}
