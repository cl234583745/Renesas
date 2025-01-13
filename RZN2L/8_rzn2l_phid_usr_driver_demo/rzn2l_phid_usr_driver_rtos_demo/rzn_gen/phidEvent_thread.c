/* generated thread source file - do not edit */
#include "phidEvent_thread.h"

#if 1
static StaticTask_t phidEvent_thread_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
                static uint8_t phidEvent_thread_stack[2048] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #else
static uint8_t phidEvent_thread_stack[2048] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.phidEvent_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif
#endif
TaskHandle_t phidEvent_thread;
void phidEvent_thread_create(void);
static void phidEvent_thread_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
extern uint32_t g_fsp_common_thread_count;

const rm_freertos_port_parameters_t phidEvent_thread_parameters =
{ .p_context = (void*) NULL, };

void phidEvent_thread_create(void)
{
    /* Increment count so we will know the number of threads created in the FSP Configuration editor. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

#if 1
    phidEvent_thread = xTaskCreateStatic (
#else
                    BaseType_t phidEvent_thread_create_err = xTaskCreate(
                    #endif
                                          phidEvent_thread_func,
                                          (const char*) "PhidEvent Thread", 2048 / 4, // In words, not bytes
                                          (void*) &phidEvent_thread_parameters, //pvParameters
                                          5,
#if 1
                                          (StackType_t*) &phidEvent_thread_stack,
                                          (StaticTask_t*) &phidEvent_thread_memory
#else
                        & phidEvent_thread
                        #endif
                                          );

#if 1
    if (NULL == phidEvent_thread)
    {
        rtos_startup_err_callback (phidEvent_thread, 0);
    }
#else
                    if (pdPASS != phidEvent_thread_create_err)
                    {
                        rtos_startup_err_callback(phidEvent_thread, 0);
                    }
                    #endif
}
static void phidEvent_thread_func(void *pvParameters)
{
    /* Initialize common components */
    rtos_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. Pass task handle. */
    phidEvent_thread_entry (pvParameters);
}
