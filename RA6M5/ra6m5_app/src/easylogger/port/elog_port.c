/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */
 
#include <elog.h>

#include <stdio.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


extern void elogUart(const char *pBuffer, uint32_t size);

#ifdef ELOG_ASYNC_OUTPUT_ENABLE
static void elog_async_thread_entry(void *pvParameters);

static elog_async_init_result = 0;
SemaphoreHandle_t xelog_lock_Handle = NULL;
StaticSemaphore_t xelog_lock_SemaphoreBuffer;

SemaphoreHandle_t xelog_async_Handle = NULL;
StaticSemaphore_t xelog_async_SemaphoreBuffer;

#define ELOG_ASYNC_THREAD_STACK     2048
TaskHandle_t elog_async_thread;
const rm_freertos_port_parameters_t elog_async_thread_parameters =
{ .p_context = (void*) NULL, };
static StaticTask_t elog_async_thread_memory;
static uint8_t elog_async_thread_stack[ELOG_ASYNC_THREAD_STACK];
#endif
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    /* add your code here */
#ifdef ELOG_ASYNC_OUTPUT_ENABLE

    xelog_lock_Handle = xSemaphoreCreateBinaryStatic (&xelog_lock_SemaphoreBuffer);
    if(xelog_lock_Handle == NULL)
    {
        printf("xSemaphoreCreateBinaryStatic xelog_lock_Handle failed!\n");
    }

    if(pdPASS != xSemaphoreGive( xelog_lock_Handle ))
    {
        printf("xSemaphoreGive xelog_lock_Handle failed!\n");
    }


    xelog_async_Handle = xSemaphoreCreateBinaryStatic (&xelog_async_SemaphoreBuffer);
    if(xelog_async_Handle == NULL)
    {
        printf("xSemaphoreCreateBinaryStatic xelog_async_Handle failed!\n");
    }

    elog_async_thread = xTaskCreateStatic (
                                        elog_async_thread_entry,
                                       (const char*) "elog async Thread", ELOG_ASYNC_THREAD_STACK / 4, // In words, not bytes
                                       (void*) &elog_async_thread_parameters, //pvParameters
                                       1,
                                       (StackType_t*) &elog_async_thread_stack,
                                       (StaticTask_t*) &elog_async_thread_memory
                                       );

    if(elog_async_thread == NULL)
    {
        printf("xTaskCreateStatic elog_async_thread failed!\n");
    }

    elog_async_init_result = 1;
#endif
    return result;
}

/**
 * EasyLogger port deinitialize
 *
 */
void elog_port_deinit(void) {

    /* add your code here */

}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {
    
    /* add your code here */
#ifndef ELOG_ASYNC_OUTPUT_ENABLE
    vTaskSuspendAll();
#endif

    elogUart(log, size);

#ifndef ELOG_ASYNC_OUTPUT_ENABLE
    xTaskResumeAll();
#endif
}

/**
 * output lock
 */
void elog_port_output_lock(void) {
    
    /* add your code here */
//    BaseType_t yield;
//    if (xPortIsInsideInterrupt())
//    {
//        xSemaphoreTake( xelog_lock_Handle, portMAX_DELAY );
//    }
//    else
//    {
//        yield = pdFALSE;
//        xSemaphoreTakeFromISR(xelog_lock_Handle, &yield);
//        portYIELD_FROM_ISR (yield);
//    }
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    xSemaphoreTake( xelog_lock_Handle, portMAX_DELAY );
#endif
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {
    
    /* add your code here */
//    BaseType_t yield;
//    if (xPortIsInsideInterrupt())
//    {
//        xSemaphoreGive( xelog_lock_Handle );
//    }
//    else
//    {
//        yield = pdFALSE;
//        xSemaphoreGiveFromISR(xelog_lock_Handle, &yield);
//        portYIELD_FROM_ISR (yield);
//    }
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    xSemaphoreGive( xelog_lock_Handle );
#endif
}

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
    
    /* add your code here */
    static char cur_system_time[16] = "";

    TickType_t ticks;
    if (xPortIsInsideInterrupt())
    {
        ticks = xTaskGetTickCountFromISR();
    }
    else
    {
        ticks = xTaskGetTickCount();
    }

    snprintf(cur_system_time, 16, "%lu", ticks);
    return cur_system_time;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {
    
    /* add your code here */
    return "";
}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {
    
    /* add your code here */
    return "";
}

void elog_async_output_notice(void) {
//    BaseType_t yield;
//    if (xPortIsInsideInterrupt())
//    {
//        xSemaphoreGive( xelog_async_Handle );
//    }
//    else
//    {
//        yield = pdFALSE;
//        xSemaphoreGiveFromISR(xelog_lock_Handle, &yield);
//        portYIELD_FROM_ISR (yield);
//    }
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
    xSemaphoreGive( xelog_async_Handle );
#endif
}
#ifdef ELOG_ASYNC_OUTPUT_ENABLE
static void elog_async_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    size_t get_log_size = 0;
    static char poll_get_buf[ELOG_LINE_BUF_SIZE - 4];

    if (elog_async_init_result != 1) {
        printf("start %s failed!\n", __FUNCTION__);
        goto fail;
    }

    printf("start %s successed!\n", __FUNCTION__);
    while (1)
    {
        xSemaphoreTake( xelog_async_Handle, portMAX_DELAY );

        {
            while (1)
            {

                    get_log_size = elog_async_get_line_log(poll_get_buf, sizeof(poll_get_buf));
                    if (get_log_size)
                    {
                        elog_port_output(poll_get_buf, get_log_size);
                    }
                    else
                    {

                        break;
                    }

            }
        }
    }

    fail:
    vTaskDelete (NULL);
}
#endif
