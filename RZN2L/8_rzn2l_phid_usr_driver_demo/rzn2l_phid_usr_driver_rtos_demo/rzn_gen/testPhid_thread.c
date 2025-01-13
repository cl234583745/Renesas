/* generated thread source file - do not edit */
#include "testPhid_thread.h"

#if 1
static StaticTask_t testPhid_thread_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
                static uint8_t testPhid_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #else
static uint8_t testPhid_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.testPhid_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif
#endif
TaskHandle_t testPhid_thread;
void testPhid_thread_create(void);
static void testPhid_thread_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
extern uint32_t g_fsp_common_thread_count;

const rm_freertos_port_parameters_t testPhid_thread_parameters =
{ .p_context = (void*) NULL, };

void testPhid_thread_create(void)
{
    /* Increment count so we will know the number of threads created in the FSP Configuration editor. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

#if 1
    testPhid_thread = xTaskCreateStatic (
#else
                    BaseType_t testPhid_thread_create_err = xTaskCreate(
                    #endif
                                         testPhid_thread_func,
                                         (const char*) "TestPhid Thread", 1024 / 4, // In words, not bytes
                                         (void*) &testPhid_thread_parameters, //pvParameters
                                         1,
#if 1
                                         (StackType_t*) &testPhid_thread_stack,
                                         (StaticTask_t*) &testPhid_thread_memory
#else
                        & testPhid_thread
                        #endif
                                         );

#if 1
    if (NULL == testPhid_thread)
    {
        rtos_startup_err_callback (testPhid_thread, 0);
    }
#else
                    if (pdPASS != testPhid_thread_create_err)
                    {
                        rtos_startup_err_callback(testPhid_thread, 0);
                    }
                    #endif
}
static void testPhid_thread_func(void *pvParameters)
{
    /* Initialize common components */
    rtos_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. Pass task handle. */
    testPhid_thread_entry (pvParameters);
}
