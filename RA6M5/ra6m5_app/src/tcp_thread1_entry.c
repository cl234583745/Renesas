#include "tcp_thread1.h"
/* Tcp Thread entry function */
/* pvParameters contains TaskHandle_t */
void tcp_thread1_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    /* TODO: add your own code here */
    while (1)
    {
        vTaskDelay (1);
    }
}
