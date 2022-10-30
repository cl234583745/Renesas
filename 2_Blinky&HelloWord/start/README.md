# 瑞萨RA&e2studio快速上手视频笔记 二、点亮LED输出HelloWorld
[TOC]

## 一、推荐安装 
setup_fsp_v4_1_0_e2s_v2022-10.exe（e2s+fsp+toolchain）
RA一键安装

## 二、e2studio安装 
setup_e2_studio_2022-10.zip
安装中选择RA系列，fps版本，toolchain等等（在线下载，失败重新安装）

## 三、第一个工程：点亮LED打印HelloWorld
官方的ra-fsp-examples/example_projects/ek_ra2l1/_quickstart/quickstart_ek_ra2l1_ep/
不够简洁，我们使用如下代码测试 
```
/**
 * 1.uart0 callback:g_uart0CB
 * 2.FSP-BSP-heap size:0x400
 * 3.-u _printf_float
 * 4.other link void
 */
#ifndef PRINTF
#define PRINTF
#endif

#ifdef PRINTF

volatile bool uart_send_complete_flag = false;
void g_uart0CB (uart_callback_args_t * p_args)
{
    if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uart_send_complete_flag = true;
    }
}
#ifdef __GNUC__
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    uint8_t err = R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)&ch, 1);
    if(FSP_SUCCESS != err) __BKPT();
    while(uart_send_complete_flag == false){}
    uart_send_complete_flag = false;
    return ch;
}
int _write(int fd,char *pBuffer,int size)
{
    for(int i=0;i<size;i++)
    {
        __io_putchar(*pBuffer++);
    }
    return size;
}

#endif//PRINTF
```

在void hal_entry (void)中增加下面代码：
```
#ifdef PRINTF
    /* Open the transfer instance with initial configuration. */
    fsp_err_t err = R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    assert(FSP_SUCCESS == err);

    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);

    float PI = 3.1415926;
    printf("PI=%f\n", PI);
#endif//PRINTF
```

成功可以看见如下打印：

date:Oct 30 2022
time:15:40:18
file:../src/hal_entry.c
func:hal_entry,line:85
hello world!
PI=3.141593
