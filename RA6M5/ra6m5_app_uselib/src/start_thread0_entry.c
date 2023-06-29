#include "start_thread0.h"
/* Start Thread entry function */
/* pvParameters contains TaskHandle_t */
void start_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    /* TODO: add your own code here */
    while (1)
    {
        vTaskDelay (1);
    }
}
