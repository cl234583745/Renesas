/* generated thread header file - do not edit */
#ifndef START_THREAD0_H_
#define START_THREAD0_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void start_thread0_entry(void * pvParameters);
                #else
extern void start_thread0_entry(void *pvParameters);
#endif
#include "r_sci_uart.h"
#include "r_uart_api.h"
FSP_HEADER
/** UART on SCI Instance. */
extern const uart_instance_t g_uart9;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart9_ctrl;
extern const uart_cfg_t g_uart9_cfg;
extern const sci_uart_extended_cfg_t g_uart9_cfg_extend;

#ifndef g_uart9CB
void g_uart9CB(uart_callback_args_t *p_args);
#endif
FSP_FOOTER
#endif /* START_THREAD0_H_ */
