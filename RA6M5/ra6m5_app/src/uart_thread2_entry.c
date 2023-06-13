#include "uart_thread2.h"
/* Uart Thread entry function */
/* pvParameters contains TaskHandle_t */
void uart_thread2_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    /* TODO: add your own code here */
    while (1)
    {
        vTaskDelay (1);
    }
}
