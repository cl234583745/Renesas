/*******************************************************************************
* File Name    : rx_fit_i2c_slave.c
* Description  : I2C Slave non-blocking implementation for RX MCU (FIT driver)
* Creation Date: 2026-08-26
******************************************************************************/
#include "r_smc_entry.h"
#include "r_riic_rx_if.h"
#include "r_riic_rx_config.h"
#include "r_riic_rx_pin_config.h"
#include "rx_fit_i2c_slave.h"

/*******************************************************************************
* Private variables
******************************************************************************/
static riic_info_t   s_riic_info;
static rx_fit_i2c_slave_cb_t s_event_cb;
static uint8_t s_rx_buf[RX_FIT_I2C_SLAVE_RX_BUF_SIZE];
static uint8_t s_tx_buf[RX_FIT_I2C_SLAVE_TX_BUF_SIZE];
static volatile bool s_busy = false;

/*******************************************************************************
* Private callback (called from ISR context, must be fast)
******************************************************************************/
static void rx_fit_i2c_slave_riic_cb(void)
{
    /* Do nothing in ISR context.
     * All processing is done in rx_fit_i2c_slave_process() from main loop. */
}

/*******************************************************************************
* Public functions
******************************************************************************/

/*******************************************************************************
* Function Name: rx_fit_i2c_slave_init
* Description  : Initialize I2C slave module
*                Slave address is configured in Smart Configurator (r_riic_rx_pin_config.h)
* Arguments    : on_event - callback function for transfer events (can be NULL)
* Return Value : none
*******************************************************************************/
void rx_fit_i2c_slave_init(rx_fit_i2c_slave_cb_t on_event)
{
    riic_return_t ret;

    /* Clear buffers */
    uint32_t i;
    for (i = 0; i < RX_FIT_I2C_SLAVE_RX_BUF_SIZE; i++) s_rx_buf[i] = 0;
    for (i = 0; i < RX_FIT_I2C_SLAVE_TX_BUF_SIZE; i++) s_tx_buf[i] = 0;

    /* Store callback */
    s_event_cb = on_event;

    /* Initialize RIIC info */
    s_riic_info.dev_sts    = RIIC_NO_INIT;
    s_riic_info.ch_no      = 0;
    s_riic_info.callbackfunc = &rx_fit_i2c_slave_riic_cb;
    s_riic_info.p_slv_adr  = (uint8_t *)FIT_NO_PTR;

    /* Open RIIC */
    ret = R_RIIC_Open(&s_riic_info);

    /* Start slave transfer (re-arm) */
    if (RIIC_SUCCESS == ret)
    {
        s_riic_info.cnt1st      = RX_FIT_I2C_SLAVE_TX_BUF_SIZE;
        s_riic_info.p_data1st   = s_tx_buf;
        s_riic_info.cnt2nd      = RX_FIT_I2C_SLAVE_RX_BUF_SIZE;
        s_riic_info.p_data2nd   = s_rx_buf;

        ret = R_RIIC_SlaveTransfer(&s_riic_info);

        if (RIIC_SUCCESS == ret)
        {
            s_busy = true;
        }
    }
}

/*******************************************************************************
* Function Name: rx_fit_i2c_slave_process
* Description  : Non-blocking process function, call from main loop
*                Detects transfer completion and invokes user callback
*
* Direction detection methods (select via RX_FIT_I2C_SLAVE_DIR_METHOD):
*
*   Method 0 (default): Use dev_sts to detect direction
*     - FINISH = master wrote data to slave (WRITE event)
*     - NACK   = master read data from slave (READ event)
*     - No driver modification needed.
*
*   Method 1: Use rsv1 field to detect direction (more robust)
*     - Requires modification to r_riic_rx.c
*     - File:  src/smc_gen/r_riic_rx/src/r_riic_rx.c
*     - Func:  riic_advance()
*     - Location: after riic_set_ch_status(p_riic_info, RIIC_FINISH);
*                 and before "Checks the callback function"
*     - Add the following code:
*
*       if (RIIC_MODE_S_SEND == riic_api_info[p_riic_info->ch_no].N_Mode)
*       {
*           p_riic_info->rsv1 = 1;  (Slave transmit - master read)
*       }
*       else if (RIIC_MODE_S_RECEIVE == riic_api_info[p_riic_info->ch_no].N_Mode)
*       {
*           p_riic_info->rsv1 = 0;  (Slave receive - master write)
*       }
*
* Arguments    : none
* Return Value : none
*******************************************************************************/
void rx_fit_i2c_slave_process(void)
{
    riic_return_t ret;
    bool is_write;

    if (!s_busy)
    {
        return;
    }

    /* Check if transfer completed */
    if ((RIIC_FINISH != s_riic_info.dev_sts) && (RIIC_NACK != s_riic_info.dev_sts))
    {
        return; /* Still busy or error */
    }

    /* Determine direction */
#if (RX_FIT_I2C_SLAVE_DIR_METHOD == 1)
    /* Method 1: Use rsv1 field (requires r_riic_rx.c modification) */
    is_write = (s_riic_info.rsv1 == 0);
#else
    /* Method 0: Use dev_sts (no driver modification needed) */
    is_write = (RIIC_FINISH == s_riic_info.dev_sts);
#endif

    /* Invoke callback */
    if (NULL != s_event_cb)
    {
        s_event_cb(is_write ? RX_FIT_I2C_SLAVE_EVT_WRITE : RX_FIT_I2C_SLAVE_EVT_READ);
    }

    /* Re-arm slave for next transfer */
    s_riic_info.cnt1st      = RX_FIT_I2C_SLAVE_TX_BUF_SIZE;
    s_riic_info.p_data1st   = s_tx_buf;
    s_riic_info.cnt2nd      = RX_FIT_I2C_SLAVE_RX_BUF_SIZE;
    s_riic_info.p_data2nd   = s_rx_buf;

    ret = R_RIIC_SlaveTransfer(&s_riic_info);
    if (RIIC_SUCCESS != ret)
    {
        s_busy = false;
    }
}

/*******************************************************************************
* Function Name: rx_fit_i2c_slave_is_busy
* Description  : Check if slave is actively waiting for transfer
* Arguments    : none
* Return Value : true = busy, false = idle/error
*******************************************************************************/
bool rx_fit_i2c_slave_is_busy(void)
{
    return s_busy;
}

/*******************************************************************************
* Function Name: rx_fit_i2c_slave_get_rx_buf
* Description  : Get pointer to receive buffer (master wrote data here)
* Arguments    : none
* Return Value : pointer to RX buffer
*******************************************************************************/
uint8_t *rx_fit_i2c_slave_get_rx_buf(void)
{
    return s_rx_buf;
}

/*******************************************************************************
* Function Name: rx_fit_i2c_slave_get_tx_buf
* Description  : Get pointer to transmit buffer (slave sends this to master)
* Arguments    : none
* Return Value : pointer to TX buffer
*******************************************************************************/
uint8_t *rx_fit_i2c_slave_get_tx_buf(void)
{
    return s_tx_buf;
}
