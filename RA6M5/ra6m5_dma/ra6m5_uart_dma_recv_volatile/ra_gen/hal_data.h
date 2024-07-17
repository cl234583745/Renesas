/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef g_timer0CB
void g_timer0CB(timer_callback_args_t *p_args);
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer_dmac_sci4_rx;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer_dmac_sci4_rx_ctrl;
extern const transfer_cfg_t g_transfer_dmac_sci4_rx_cfg;

#ifndef transfer_dmac_sci4_rx_callback
void transfer_dmac_sci4_rx_callback(dmac_callback_args_t *p_args);
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer_dmac_sci4_tx;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer_dmac_sci4_tx_ctrl;
extern const transfer_cfg_t g_transfer_dmac_sci4_tx_cfg;

#ifndef transfer_dmac_sci4_tx_callback
void transfer_dmac_sci4_tx_callback(dmac_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart4;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart4_ctrl;
extern const uart_cfg_t g_uart4_cfg;
extern const sci_uart_extended_cfg_t g_uart4_cfg_extend;

#ifndef debug_uart4_callback
void debug_uart4_callback(uart_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
