#include "tcp_thread1.h"

#define LOG_TAG          "TCP_TAG"
#define LOG_LVL          ELOG_LVL_VERBOSE
#include "elog.h"

/* Tcp Thread entry function */
/* pvParameters contains TaskHandle_t */
void tcp_thread1_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    log_i("tcp_thread1_entry");
    /* TODO: add your own code here */
    while (1)
    {
        log_i("tcp_thread1_thread in app test");
        vTaskDelay (5000);
    }
}
//--specs=rdimon.specs -Wl,--whole-archive -lra6m5_app -Wl,--no-whole-archive
