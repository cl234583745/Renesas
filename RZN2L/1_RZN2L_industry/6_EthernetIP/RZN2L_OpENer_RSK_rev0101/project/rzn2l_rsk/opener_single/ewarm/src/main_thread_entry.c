#include "main_thread.h"
/* Main Thread entry function */
/* pvParameters contains TaskHandle_t */
void main_thread_entry(void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);
  
    /** Launch the OpENer user application example. */
    extern void opener_port_user_main( void );
    (void) opener_port_user_main();
    
    /* TODO: add your own code here */
    while(1)
    {
      vTaskDelay(1);
    }
}
