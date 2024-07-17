#include "bsp_dmac.h"

/**
 * 传输 API 的 DMAC 实现
 *
 * const transfer_api_t g_transfer_on_dmac =
 * {
 *     .open          = R_DMAC_Open,           //配置DMAC通道
 *     .reconfigure   = R_DMAC_Reconfigure,    //使用新的传输信息重新配置传输
 *     .reset         = R_DMAC_Reset,          //重置传输源、目标和传输数
 *     .infoGet       = R_DMAC_InfoGet,        //提供有关此传输的信息
 *     .softwareStart = R_DMAC_SoftwareStart,  //
 *     .softwareStop  = R_DMAC_SoftwareStop,   //
 *     .enable        = R_DMAC_Enable,         //
 *     .disable       = R_DMAC_Disable,        //
 *     .close         = R_DMAC_Close,          //
 * };
 */



/* 初始化 DMAC 模块 */
void DMAC_Init(void)
{
    fsp_err_t err;

    /* 配置发送 */
    err = g_transfer_on_dmac.open(g_transfer_dmac_sci4_tx.p_ctrl, g_transfer_dmac_sci4_tx.p_cfg);
    assert(FSP_SUCCESS == err);
    err = g_transfer_on_dmac.enable(g_transfer_dmac_sci4_tx.p_ctrl);
    assert(FSP_SUCCESS == err);

    /* 配置接收 */
    err = g_transfer_on_dmac.open(g_transfer_dmac_sci4_rx.p_ctrl, g_transfer_dmac_sci4_rx.p_cfg);
    assert(FSP_SUCCESS == err);
    err = g_transfer_on_dmac.enable(g_transfer_dmac_sci4_rx.p_ctrl);
    assert(FSP_SUCCESS == err);
}


void set_transfer_length(transfer_cfg_t const * p_config, volatile uint16_t _length)
{
    p_config->p_info->length = _length;
}

/* 设置传输的源地址和目的地址 */
void set_transfer_dst_src_address(transfer_cfg_t const * const p_config,
        const volatile uint8_t * _p_src, const volatile uint8_t * _p_dest)
{
    p_config->p_info->p_src = (void const * volatile) _p_src;
    p_config->p_info->p_dest = (void * volatile) _p_dest;
}




// DMA 传输完成标志位
volatile uint8_t dmac_sci4_tx_flag = 0;
volatile uint8_t dmac_sci4_rx_flag = 0;

/* DMAC 发送中断回调函数 */
void transfer_dmac_sci4_tx_callback(dmac_callback_args_t *p_args)
{
    (void)(p_args);
    dmac_sci4_tx_flag = 1;
}

/* DMAC 接收回调函数 */
void transfer_dmac_sci4_rx_callback(dmac_callback_args_t *p_args)
{
    (void)(p_args);
    dmac_sci4_rx_flag = 1;
}


