/*******************************************************************************
* File Name    : rx_fit_i2c_slave.h
* Description  : I2C Slave non-blocking API for RX MCU (FIT driver)
* Creation Date: 2026-08-26
******************************************************************************/
#ifndef RX_FIT_I2C_SLAVE_H
#define RX_FIT_I2C_SLAVE_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
* Macro definitions
******************************************************************************/
#define RX_FIT_I2C_SLAVE_RX_BUF_SIZE   16
#define RX_FIT_I2C_SLAVE_TX_BUF_SIZE   16

/*******************************************************************************
* Direction detection method selection
*   0 = Use dev_sts (FINISH=NACK) - no driver modification needed (default)
*   1 = Use rsv1 field - requires r_riic_rx.c modification, see .c file
*******************************************************************************/
#ifndef RX_FIT_I2C_SLAVE_DIR_METHOD
#define RX_FIT_I2C_SLAVE_DIR_METHOD    0
#endif

/*******************************************************************************
* Enum definitions
******************************************************************************/
typedef enum
{
    RX_FIT_I2C_SLAVE_EVT_WRITE,     /* Master wrote data to slave */
    RX_FIT_I2C_SLAVE_EVT_READ       /* Master read data from slave */
} rx_fit_i2c_slave_event_t;

/*******************************************************************************
* Typedef definitions
******************************************************************************/
/* Event callback: called from main loop when transfer completes */
typedef void (*rx_fit_i2c_slave_cb_t)(rx_fit_i2c_slave_event_t event);

/*******************************************************************************
* Exported functions
******************************************************************************/
void     rx_fit_i2c_slave_init(rx_fit_i2c_slave_cb_t on_event);
void     rx_fit_i2c_slave_process(void);
bool     rx_fit_i2c_slave_is_busy(void);
uint8_t *rx_fit_i2c_slave_get_rx_buf(void);
uint8_t *rx_fit_i2c_slave_get_tx_buf(void);

#endif /* RX_FIT_I2C_SLAVE_H */
