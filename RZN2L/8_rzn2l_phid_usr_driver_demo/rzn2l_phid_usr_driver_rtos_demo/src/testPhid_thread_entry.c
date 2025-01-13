#include "testPhid_thread.h"
#include "usr_hal_data.h"

extern uint8_t send_buf[64];
/* TestPhid Thread entry function */
/* pvParameters contains TaskHandle_t */
void testPhid_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    //wait usbphid init
    vTaskDelay (10000);
    R_USB_Read(&g_basic0_ctrl, (uint8_t *) &send_buf, 64, USB_CLASS_PHID);
    /* TODO: add your own code here */
    while (1)
    {
        R_USB_Write(&g_basic0_ctrl, send_buf, 64, USB_CLASS_PHID);
        vTaskDelay (1000);
    }
}
