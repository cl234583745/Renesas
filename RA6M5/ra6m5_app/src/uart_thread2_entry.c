#include "uart_thread2.h"

#define LOG_TAG          "UART_TAG"
#define LOG_LVL          ELOG_LVL_VERBOSE
#include "elog.h"

/* Uart Thread entry function */
/* pvParameters contains TaskHandle_t */
void uart_thread2_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    log_i("uart_thread2_entry");
    /* TODO: add your own code here */
    while (1)
    {
        log_i("uart_thread2_thread");
        vTaskDelay (5000);
    }
}
