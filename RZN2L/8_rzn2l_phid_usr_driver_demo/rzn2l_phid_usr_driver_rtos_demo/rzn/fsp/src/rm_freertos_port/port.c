/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 */

/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "bsp_api.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#ifndef configUNIQUE_INTERRUPT_PRIORITIES
 #error configUNIQUE_INTERRUPT_PRIORITIES must be defined.  Refer to Cortex-A equivalent: http:     // www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html
#endif

#ifndef configMAX_API_CALL_INTERRUPT_PRIORITY
 #error configMAX_API_CALL_INTERRUPT_PRIORITY must be defined.  Refer to Cortex-A equivalent: http: // www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html
#endif

#if configMAX_API_CALL_INTERRUPT_PRIORITY == 0
 #error configMAX_API_CALL_INTERRUPT_PRIORITY must not be set to 0
#endif

#if configMAX_API_CALL_INTERRUPT_PRIORITY > configUNIQUE_INTERRUPT_PRIORITIES
 #error \
    configMAX_API_CALL_INTERRUPT_PRIORITY must be less than or equal to configUNIQUE_INTERRUPT_PRIORITIES as the lower the numeric priority value the higher the logical interrupt priority
#endif

#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1

/* Check the configuration. */
 #if (configMAX_PRIORITIES > 32)
  #error \
    configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
 #endif
#endif                                 /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

/* A critical section is exited when the critical section nesting count reaches
 * this value. */
#define portNO_CRITICAL_NESTING          ((uint32_t) 0)

/* In all GICs 255 can be written to the priority mask register to unmask all
 * (but the lowest) interrupt priority. */
#define portUNMASK_VALUE                 (0xFFUL)

/* Tasks are not created with a floating point context, but can be given a
 * floating point context after they have been created.  A variable is stored as
 * part of the tasks context that holds portNO_FLOATING_POINT_CONTEXT if the task
 * does not have an FPU context, or any other value if the task does have an FPU
 * context. */
#define portNO_FLOATING_POINT_CONTEXT    ((StackType_t) 0)

/* Constants required to setup the initial task context. */
#define portINITIAL_SPSR                 ((StackType_t) 0x1f) /* System mode, ARM mode, IRQ enabled FIQ enabled. */
#define portTHUMB_MODE_BIT               ((StackType_t) 0x20)
#define portTHUMB_MODE_ADDRESS           (0x01UL)
#define portARM_MODE_ADDRESS             (0x00UL)

/* Used by portASSERT_IF_INTERRUPT_PRIORITY_INVALID() when ensuring the binary
 * point is zero. */
#define portBINARY_POINT_BITS            ((uint8_t) 0x07)

/* Masks all bits in the APSR other than the mode bits. */
#define portAPSR_MODE_BITS_MASK          (0x1F)

/* The value of the mode bits in the APSR when the CPU is executing in user
 * mode. */
#define portAPSR_USER_MODE               (0x10)

/* The critical section macros only mask interrupts up to an application
 * determined priority level.  Sometimes it is necessary to turn interrupt off in
 * the CPU itself before modifying certain hardware registers. */
#define portCPU_IRQ_DISABLE()                \
    __asm volatile ("CPSID i" ::: "memory"); \
    __asm volatile ("DSB");                  \
    __asm volatile ("ISB");

#define portCPU_IRQ_ENABLE()                 \
    __asm volatile ("CPSIE i" ::: "memory"); \
    __asm volatile ("DSB");                  \
    __asm volatile ("ISB");

/* Macro to unmask all interrupt priorities. */
#define portCLEAR_INTERRUPT_MASK()       \
    {                                    \
        portCPU_IRQ_DISABLE();           \
        __set_ICC_PMR(portUNMASK_VALUE); \
        __asm volatile ("DSB		\n"  \
                        "ISB		\n");      \
        portCPU_IRQ_ENABLE();            \
    }

#define portINTERRUPT_PRIORITY_REGISTER_OFFSET    0x420UL
#define portMAX_8_BIT_VALUE                       ((uint8_t) 0xff)
#define portBITS_PER_BYTE                         8
#define portBIT_0_SET                             ((uint8_t) 0x01)
#define portCRITICAL_NESTING_INITIAL_VALUE        (9999UL)
#define portSTACK_INITIAL_VALUE_R1                0x01010101
#define portSTACK_INITIAL_VALUE_R2                (portSTACK_INITIAL_VALUE_R1 * 0x02)
#define portSTACK_INITIAL_VALUE_R3                (portSTACK_INITIAL_VALUE_R1 * 0x03)
#define portSTACK_INITIAL_VALUE_R4                (portSTACK_INITIAL_VALUE_R1 * 0x04)
#define portSTACK_INITIAL_VALUE_R5                (portSTACK_INITIAL_VALUE_R1 * 0x05)
#define portSTACK_INITIAL_VALUE_R6                (portSTACK_INITIAL_VALUE_R1 * 0x06)
#define portSTACK_INITIAL_VALUE_R7                (portSTACK_INITIAL_VALUE_R1 * 0x07)
#define portSTACK_INITIAL_VALUE_R8                (portSTACK_INITIAL_VALUE_R1 * 0x08)
#define portSTACK_INITIAL_VALUE_R9                (portSTACK_INITIAL_VALUE_R1 * 0x09)
#define portSTACK_INITIAL_VALUE_R10               (portSTACK_INITIAL_VALUE_R1 * 0x10)
#define portSTACK_INITIAL_VALUE_R11               (portSTACK_INITIAL_VALUE_R1 * 0x11)
#define portSTACK_INITIAL_VALUE_R12               (portSTACK_INITIAL_VALUE_R1 * 0x12)

#define portMAX_INTERRUPT_NESTING                 (~0UL)
#define portMAX_CRITICAL_NESTING                  (1000UL)

/* Let the user override the pre-loading of the initial LR with the address of
 * prvTaskExitError() in case is messes up unwinding of the stack in the
 * debugger. */
#ifdef configTASK_RETURN_ADDRESS
 #define portTASK_RETURN_ADDRESS                  configTASK_RETURN_ADDRESS
#else
 #define portTASK_RETURN_ADDRESS                  prvTaskExitError
#endif

#define SYS_MODE                                  0x1f
#define SVC_MODE                                  0x13
#define IRQ_MODE                                  0x12

extern void * volatile pxCurrentTCB;

#if __FPU_USED
 #if __FPU_D32
  #define _portSAVE_CONTEX_FPU_D32()        __asm volatile (                          \
        "   VPUSHNE     {D16-D31}                                                 \n" \
        ::: "memory");
 #else
  #define _portSAVE_CONTEX_FPU_D32()
 #endif
 #define _portSAVE_CONTEX_FPU()             __asm volatile (                          \
        "   CMP         R3, #0                                                    \n" \
/* Save the floating point context, if any.                                */         \
        "   FMRXNE      R1,  FPSCR                                                \n" \
        "   VPUSHNE     {D0-D15}                                                  \n" \
        ::: "memory");                                                                \
    _portSAVE_CONTEX_FPU_D32();                                                       \
    __asm volatile (                                                                  \
        "   PUSHNE      {R1}                                                      \n" \
        ::: "memory");
#else
 #define _portSAVE_CONTEX_FPU()
#endif

#define portSAVE_CONTEXT()                  __asm volatile (                          \
/* Save the LR and SPSR onto the system mode(0x1f) stack before switching  */         \
/* to system mode(0x1f) to save the remaining system mode registers.       */         \
        "   SRSDB       sp!, %[sys_mode]                                          \n" \
        "   CPS         %[sys_mode]                                               \n" \
        "   PUSH        {R0-R12, R14}                                             \n" \
/* Push the critical nesting count.                                        */         \
        "   LDR         R2, =ulCriticalNesting                                    \n" \
        "   LDR         R1, [R2]                                                  \n" \
        "   PUSH        {R1}                                                      \n" \
/* Does the task have a floating point context that needs saving?          */         \
/* If ulPortTaskHasFPUContext is zero then no.                             */         \
        "   LDR         R2, =ulPortTaskHasFPUContext                              \n" \
        "   LDR         R3, [R2]                                                  \n" \
        ::[sys_mode] "i" (SYS_MODE) : "memory");                                      \
    _portSAVE_CONTEX_FPU();                                                           \
    __asm volatile (                                                                  \
/* Save ulPortTaskHasFPUContext itself.                                    */         \
        "   PUSH        {R3}                                                      \n" \
/* Save the stack pointer in the TCB.                                      */         \
        "   LDR         R0, =pxCurrentTCB                                         \n" \
        "   LDR         R1, [R0]                                                  \n" \
        "   STR         SP, [R1]                                                  \n" \
        ::: "memory");

#if __FPU_USED
 #if __FPU_D32
  #define _portRESTORE_CONTEXT_FPU_D32()    __asm volatile (                          \
        "   VPOPNE      {D16-D31}                                                 \n" \
        ::: "memory");
 #else
  #define _portRESTORE_CONTEXT_FPU_D32()
 #endif
 #define _portRESTORE_CONTEXT_FPU()         __asm volatile (                          \
        "   CMP         R1, #0                                                    \n" \
/* Restore the floating point context, if any. */                                     \
        "   POPNE       {R0}                                                      \n" \
        ::: "memory");                                                                \
    _portRESTORE_CONTEXT_FPU_D32();                                                   \
    __asm volatile (                                                                  \
        "   VPOPNE      {D0-D15}                                                  \n" \
        "   VMSRNE      FPSCR, R0                                                 \n" \
        ::: "memory");
#else
 #define _portRESTORE_CONTEXT_FPU()
#endif

#define portRESTORE_CONTEXT()               __asm volatile (                          \
/* Set the SP to point to the stack of the task being restored.            */         \
        "   LDR         R0, =pxCurrentTCB                                         \n" \
        "   LDR         R1, [R0]                                                  \n" \
        "   LDR         SP, [R1]                                                  \n" \
/* Is there a floating point context to restore?                           */         \
/* If the restored ulPortTaskHasFPUContext is zero then no.                */         \
        "   LDR         R0, =ulPortTaskHasFPUContext                              \n" \
        "   POP         {R1}                                                      \n" \
        "   STR         R1, [R0]                                                  \n" \
        ::: "memory");                                                                \
    _portRESTORE_CONTEXT_FPU();                                                       \
    __asm volatile (                                                                  \
/* Restore the critical section nesting depth. */                                     \
        "   LDR         R0, =ulCriticalNesting                                    \n" \
        "   POP         {R1}                                                      \n" \
        "   STR         R1, [R0]                                                  \n" \
/* Ensure the priority mask is correct for the critical nesting depth.     */         \
        "   CMP         R1, #0                                                    \n" \
        "   MOVEQ       R4, #255                                                  \n" \
        "   LDRNE       R4, =ulMaxAPIPriorityMask                                 \n" \
        "   LDRNE       R4, [R4]                                                  \n" \
        "   MCR         p15,0,R4,c4,c6,0	/* Write R4 to ICC_PMR */             \n" \
/* Restore all system mode registers other than the SP (which is already   */         \
/* being used).                                                            */         \
        "   POP         {R0-R12, R14}                                             \n" \
/* Return to the task code, loading CPSR on the way.                       */         \
        "   RFEIA       sp!                                                       \n" \
        ::: "memory");

/*-----------------------------------------------------------*/

BSP_ATTRIBUTE_STACKLESS void freertos_vector_table(void) BSP_PLACE_IN_SECTION(".rtosvec");

/* Generic Timer Registers */
#define GENERIC_TIMER_CLK    (R_GSC->CNTFID0)
#define CNTFRQ_READ()         __get_CNTFRQ()
#define CNTPCT_READ()         __get_CNTPCT()
#define CNTP_CTL_WRITE(v)     __set_CNTP_CTL(v)
#define CNTP_CVAL_READ()      __get_CNTP_CVAL()
#define CNTP_CVAL_WRITE(v)    __set_CNTP_CVAL(v)
static uint32_t g_timer_delay;

extern fsp_vector_t g_sgi_ppi_vector_table[BSP_CORTEX_VECTOR_TABLE_ENTRIES];

/*
 * Exception handlers.
 */
void SVC_Handler(void);
void IRQ_Handler(void);
void vApplicationIRQHandler(uint32_t ulICCIAR);
void Generic_Timer_Interrupt_Handler(void);

/*
 * Setup the timer to generate the tick interrupts.  The implementation in this
 * file is weak to allow application writers to change the timer used to
 * generate the tick interrupt.
 */
void vPortSetupTimerInterrupt(void);

/*
 * Starts the first task executing.
 */
void vPortRestoreTaskContext(void);

/*
 * Used to catch tasks that attempt to return from their implementing function.
 */
static void prvTaskExitError(void);

/*-----------------------------------------------------------*/

/* A variable is used to keep track of the critical section nesting.  This
 * variable has to be stored as part of the task context and must be initialised to
 * a non zero value to ensure interrupts don't inadvertently become unmasked before
 * the scheduler starts.  As it is stored as part of the task context it will
 * automatically be set to 0 when the first task is started. */
volatile uint32_t ulCriticalNesting = portCRITICAL_NESTING_INITIAL_VALUE;

/* Saved as part of the task context.  If ulPortTaskHasFPUContext is non-zero then
 * a floating point context must be saved and restored for the task. */
uint32_t ulPortTaskHasFPUContext = pdFALSE;

/* Set to 1 to pend a context switch from an ISR. */
uint32_t ulPortYieldRequired = pdFALSE;

/* Counts the interrupt nesting depth.  A context switch is only performed if
 * if the nesting depth is 0. */
uint32_t ulPortInterruptNesting = 0UL;

/* Used in asm code. */
__attribute__((used)) const uint32_t ulMaxAPIPriorityMask =
    (configMAX_API_CALL_INTERRUPT_PRIORITY << portPRIORITY_SHIFT);

/*-----------------------------------------------------------*/

#if configUSE_IDLE_HOOK

/***********************************************************************************************************************
 * Suspends tasks and sleeps, waking for each interrupt.
 *
 * @note This is a weak function. It can be overridden by an application specific implementation if desired.
 **********************************************************************************************************************/
__WEAK void vApplicationIdleHook (void)
{
 #if configSUPPORT_DYNAMIC_ALLOCATION
    volatile size_t xFreeHeapSpace;

    /* This is just a trivial example of an idle hook.  It is called on each
     * cycle of the idle task.  It must *NOT* attempt to block.  In this case the
     * idle task just queries the amount of FreeRTOS heap that remains.  See the
     * memory management section on the http://www.FreeRTOS.org web site for memory
     * management options.  If there is a lot of heap memory free then the
     * configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
     * RAM. */
    xFreeHeapSpace = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    (void) xFreeHeapSpace;
 #endif
}

#endif

/*
 * See header file for description.
 */

/***********************************************************************************************************************
 * Setup the stack of a new task so it is ready to be placed under the scheduler control.  The registers have to be
 * placed on the stack in the order that the port expects to find them.
 *
 * @param[in]  pxTopOfStack  Pointer to top of this task's stack
 * @param[in]  pxCode        Task function, stored as initial PC for the task
 * @param[in]  pvParameters  Parameters for task
 **********************************************************************************************************************/
#if portHAS_STACK_OVERFLOW_CHECKING
StackType_t * pxPortInitialiseStack (StackType_t  * pxTopOfStack,
                                     StackType_t  * pxEndOfStack,
                                     TaskFunction_t pxCode,
                                     void         * pvParameters)
#else
StackType_t * pxPortInitialiseStack(StackType_t * pxTopOfStack, TaskFunction_t pxCode, void * pvParameters)
#endif
{
#if portHAS_STACK_OVERFLOW_CHECKING
    FSP_PARAMETER_NOT_USED(pxEndOfStack);
#endif

    /* Setup the initial stack of the task.  The stack is set exactly as
     * expected by the portRESTORE_CONTEXT() macro.
     *
     * The fist real value on the stack is the status register, which is set for
     * system mode, with interrupts enabled.  A few NULLs are added first to ensure
     * GDB does not try decoding a non-existent return address. */
    *pxTopOfStack = (StackType_t) NULL;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) NULL;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) NULL;
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_SPSR;

    if (portARM_MODE_ADDRESS != ((uint32_t) pxCode & portTHUMB_MODE_ADDRESS))
    {
        /* The task will start in THUMB mode. */
        *pxTopOfStack |= portTHUMB_MODE_BIT;
    }

    pxTopOfStack--;

    /* Next the return address, which in this case is the start of the task. */
    *pxTopOfStack = (StackType_t) pxCode;
    pxTopOfStack--;

    /* Next all the registers other than the stack pointer. */
    *pxTopOfStack = (StackType_t) portTASK_RETURN_ADDRESS;     /* R14 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R12; /* R12 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R11; /* R11 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R10; /* R10 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R9;  /* R9 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R8;  /* R8 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R7;  /* R7 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R6;  /* R6 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R5;  /* R5 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R4;  /* R4 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R3;  /* R3 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R2;  /* R2 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) portSTACK_INITIAL_VALUE_R1;  /* R1 */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) pvParameters;                /* R0 */
    pxTopOfStack--;

    /* The task will start with a critical nesting count of 0 as interrupts are
     * enabled. */
    *pxTopOfStack = portNO_CRITICAL_NESTING;
    pxTopOfStack--;

    /* The task will start without a floating point context.  A task that uses
     * the floating point hardware must call vPortTaskUsesFPU() before executing
     * any floating point instructions. */
    *pxTopOfStack = portNO_FLOATING_POINT_CONTEXT;

    return pxTopOfStack;
}

/***********************************************************************************************************************
 * Execution only ends up here if a task exits without deleting itself.
 **********************************************************************************************************************/
static void prvTaskExitError (void)
{
    /* A function that implements a task must not exit or attempt to return to
     * its caller as there is nothing to return to.  If a task wants to exit it
     * should instead call vTaskDelete( NULL ).
     *
     * Artificially force an assert() to be triggered if configASSERT() is
     * defined, then stop here so application writers can catch the error. */
    configASSERT(portMAX_INTERRUPT_NESTING == ulPortInterruptNesting);
    portDISABLE_INTERRUPTS();
    for ( ; ; )
    {
        ;
    }
}

/***********************************************************************************************************************
 * Setup the hardware for the scheduler to take control. Configures the Tick frequency and starts the first task.
 **********************************************************************************************************************/
BaseType_t xPortStartScheduler (void)
{
    uint32_t ulAPSR;
#if (configASSERT_DEFINED == 1)
    {
        uint32_t                 ulCycles = portBITS_PER_BYTE;
        volatile uint8_t         ucOriginalPriority;
        volatile uint8_t * const pucFirstUserPriorityRegister = (uint8_t *) GICD0->GICD_IPRIORITYR;
        volatile uint8_t         ucMaxPriorityValue;

        /* Determine how many priority bits are implemented in the GIC.
         *
         * Save the interrupt priority value that is about to be clobbered. */
        ucOriginalPriority = *pucFirstUserPriorityRegister;

        /* Determine the number of priority bits available.  First write to
         * all possible bits. */
        *pucFirstUserPriorityRegister = portMAX_8_BIT_VALUE;

        /* Read the value back to see how many bits stuck. */
        ucMaxPriorityValue = *pucFirstUserPriorityRegister;

        /* Shift to the least significant bits. */
        while (portBIT_0_SET != (ucMaxPriorityValue & portBIT_0_SET))
        {
            ucMaxPriorityValue >>= (uint8_t) 0x01;

            /* If ulCycles reaches 0 then ucMaxPriorityValue must have been
             * read as 0, indicating a misconfiguration. */
            ulCycles--;
            if (ulCycles == 0)
            {
                break;
            }
        }

        /* Sanity check configUNIQUE_INTERRUPT_PRIORITIES matches the read
         * value. */
        configASSERT(portLOWEST_INTERRUPT_PRIORITY == ucMaxPriorityValue);

        /* Restore the clobbered interrupt priority register to its original
         * value. */
        *pucFirstUserPriorityRegister = ucOriginalPriority;
    }
#endif                                 /* conifgASSERT_DEFINED */

    /* Only continue if the CPU is not in User mode.  The CPU must be in a
     * Privileged mode for the scheduler to start. */
    __asm volatile ("MRS %0, APSR" : "=r" (ulAPSR)::"memory");
    ulAPSR &= portAPSR_MODE_BITS_MASK;
    configASSERT(portAPSR_USER_MODE != ulAPSR);

    if (portAPSR_USER_MODE != ulAPSR)
    {
        /* Only continue if the binary point value is set to its lowest possible
         * setting.  See the comments in vPortValidateInterruptPriority() below for
         * more information. */
        configASSERT(portMAX_BINARY_POINT_VALUE >= (__get_ICC_BPR1() & portBINARY_POINT_BITS));

        if (portMAX_BINARY_POINT_VALUE >= (__get_ICC_BPR1() & portBINARY_POINT_BITS))
        {
            /* Interrupts are turned off in the CPU itself to ensure tick does
             * not execute  while the scheduler is being started.  Interrupts are
             * automatically turned back on in the CPU when the first task starts
             * executing. */
            portCPU_IRQ_DISABLE();

            /* Setup the timer to generate the tick interrupts.  The implementation in this
             * file is weak to allow application writers to change the timer used to
             * generate the tick interrupt. */
            vPortSetupTimerInterrupt();

            /* Start the first task executing. */
            vPortRestoreTaskContext();
        }
    }

    /* Will only get here if vTaskStartScheduler() was called with the CPU in
     * a non-privileged mode or the binary point register was not set to its lowest
     * possible value.  prvTaskExitError() is referenced to prevent a compiler
     * warning about it being defined but not referenced in the case that the user
     * defines their own exit address. */
    (void) prvTaskExitError;

    return 0;
}

/***********************************************************************************************************************
 * Execution should never get here.
 **********************************************************************************************************************/
void vPortEndScheduler (void)
{
    /* Not implemented in ports where there is nothing to return to.
     * Artificially force an assert. */
    configASSERT(portMAX_CRITICAL_NESTING == ulCriticalNesting);
}

/***********************************************************************************************************************
 * Enter a critical section.
 **********************************************************************************************************************/
void vPortEnterCritical (void)
{
    /* Mask interrupts up to the max syscall interrupt priority. */
    ulPortSetInterruptMask();

    /* Now interrupts are disabled ulCriticalNesting can be accessed
     * directly.  Increment ulCriticalNesting to keep a count of how many times
     * portENTER_CRITICAL() has been called. */
    ulCriticalNesting++;

    /* This is not the interrupt safe version of the enter critical function so
     * assert() if it is being called from an interrupt context.  Only API
     * functions that end in "FromISR" can be used in an interrupt.  Only assert if
     * the critical nesting count is 1 to protect against recursive calls if the
     * assert function also uses a critical section. */
    if (1 == ulCriticalNesting)
    {
        configASSERT(0 == ulPortInterruptNesting);
    }
}

/***********************************************************************************************************************
 * Exit a critical section.
 **********************************************************************************************************************/
void vPortExitCritical (void)
{
    if (portNO_CRITICAL_NESTING < ulCriticalNesting)
    {
        /* Decrement the nesting count as the critical section is being
         * exited. */
        ulCriticalNesting--;

        /* If the nesting level has reached zero then all interrupt
         * priorities must be re-enabled. */
        if (portNO_CRITICAL_NESTING == ulCriticalNesting)
        {
            /* Critical nesting has reached zero so all interrupt priorities
             * should be unmasked. */
            portCLEAR_INTERRUPT_MASK();
        }
    }
}

/*-----------------------------------------------------------*/

void FreeRTOS_Tick_Handler (void)
{
    /* Set interrupt mask before altering scheduler structures.   The tick
     * handler runs at the lowest priority, so interrupts cannot already be masked,
     * so there is no need to save and restore the current mask value.  It is
     * necessary to turn off interrupts in the CPU itself while the ICCPMR is being
     * updated. */
    portCPU_IRQ_DISABLE();
    __set_ICC_PMR((uint32_t) (configMAX_API_CALL_INTERRUPT_PRIORITY << portPRIORITY_SHIFT));
    __asm volatile ("dsb		\n"
                    "isb		\n"::: "memory");
    portCPU_IRQ_ENABLE();

    /* Increment the RTOS tick. */
    if (xTaskIncrementTick() != pdFALSE)
    {
        ulPortYieldRequired = pdTRUE;
    }

    /* Ensure all interrupt priorities are active again. */
    portCLEAR_INTERRUPT_MASK();
}

/*-----------------------------------------------------------*/

#if __FPU_USED
void vPortTaskUsesFPU (void)
{
    uint32_t ulInitialFPSCR = 0;

    /* A task is registering the fact that it needs an FPU context.  Set the
     * FPU flag (which is saved as part of the task context). */
    ulPortTaskHasFPUContext = pdTRUE;

    /* Initialise the floating point status register. */
    __asm volatile ("FMXR   FPSCR, %0" ::"r" (ulInitialFPSCR) : "memory");
}

#endif

/*-----------------------------------------------------------*/

void vPortClearInterruptMask (uint32_t ulNewMaskValue)
{
    if (pdFALSE == ulNewMaskValue)
    {
        portCLEAR_INTERRUPT_MASK();
    }
}

/*-----------------------------------------------------------*/

uint32_t ulPortSetInterruptMask (void)
{
    uint32_t ulReturn;

    /* Interrupt in the CPU must be turned off while the ICCPMR is being
     * updated. */
    portCPU_IRQ_DISABLE();
    if (__get_ICC_PMR() == (uint32_t) (configMAX_API_CALL_INTERRUPT_PRIORITY << portPRIORITY_SHIFT))
    {
        /* Interrupts were already masked. */
        ulReturn = pdTRUE;
    }
    else
    {
        ulReturn = pdFALSE;
        __set_ICC_PMR((uint32_t) (configMAX_API_CALL_INTERRUPT_PRIORITY << portPRIORITY_SHIFT));
        __asm volatile ("dsb		\n"
                        "isb		\n"::: "memory");
    }

    portCPU_IRQ_ENABLE();

    return ulReturn;
}

/*-----------------------------------------------------------*/

#if (configASSERT_DEFINED == 1)

void vPortValidateInterruptPriority (void)
{
    /* The following assertion will fail if a service routine (ISR) for
     * an interrupt that has been assigned a priority above
     * configMAX_SYSCALL_INTERRUPT_PRIORITY calls an ISR safe FreeRTOS API
     * function.  ISR safe FreeRTOS API functions must *only* be called
     * from interrupts that have been assigned a priority at or below
     * configMAX_SYSCALL_INTERRUPT_PRIORITY.
     *
     * Numerically low interrupt priority numbers represent logically high
     * interrupt priorities, therefore the priority of the interrupt must
     * be set to a value equal to or numerically *higher* than
     * configMAX_SYSCALL_INTERRUPT_PRIORITY.
     *
     * FreeRTOS maintains separate thread and ISR API functions to ensure
     * interrupt entry is as fast and simple as possible. */
    configASSERT(__get_ICC_RPR() >= (uint32_t) (configMAX_API_CALL_INTERRUPT_PRIORITY << portPRIORITY_SHIFT));

    /* Priority grouping:  The interrupt controller (GIC) allows the bits
     * that define each interrupt's priority to be split between bits that
     * define the interrupt's pre-emption priority bits and bits that define
     * the interrupt's sub-priority.  For simplicity all bits must be defined
     * to be pre-emption priority bits.  The following assertion will fail if
     * this is not the case (if some bits represent a sub-priority).
     *
     * The priority grouping is configured by the GIC's binary point register
     * (ICCBPR).  Writing 0 to ICCBPR will ensure it is set to its lowest
     * possible value (which may be above 0). */
    configASSERT((__get_ICC_BPR1() & portBINARY_POINT_BITS) <= portMAX_BINARY_POINT_VALUE);
}

#endif                                 /* configASSERT_DEFINED */
/*-----------------------------------------------------------*/

/***********************************************************************************************************************
 * Setup the timer to generate the tick interrupts.  The implementation in this
 * file is weak to allow application writers to change the timer used to
 * generate the tick interrupt.
 **********************************************************************************************************************/
void vPortSetupTimerInterrupt (void)
{
    uint64_t tempCNTPCT = CNTPCT_READ();

    /* Wait for counter supply */
    while (CNTPCT_READ() == tempCNTPCT)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }

    /* generic timer initialize */
    /* set interrupt handler */
    g_sgi_ppi_vector_table[(int32_t) BSP_VECTOR_NUM_OFFSET +
                           NonSecurePhysicalTimerInt] = Generic_Timer_Interrupt_Handler;

    g_timer_delay = GENERIC_TIMER_CLK / configTICK_RATE_HZ;

    /* set timer expiration from current counter value */
    CNTP_CVAL_WRITE(CNTPCT_READ() + g_timer_delay);

    /* configure CNTP_CTL to enable timer interrupts */
    CNTP_CTL_WRITE(1);
    R_BSP_IrqCfgEnable(NonSecurePhysicalTimerInt, portLOWEST_USABLE_INTERRUPT_PRIORITY, NULL);

    __asm volatile ("cpsie i");
}

/*-----------------------------------------------------------*/

void vApplicationIRQHandler (uint32_t ulICCIAR)
{
    /* Re-enable interrupts. */
    __asm("cpsie i");

    bsp_common_interrupt_handler(ulICCIAR);
}

void Generic_Timer_Interrupt_Handler (void)
{
    /* update next timer expiration */
    CNTP_CVAL_WRITE(CNTP_CVAL_READ() + g_timer_delay);

    /* call FreeRTOS tick interrupt handler */
    FreeRTOS_Tick_Handler();
}

/******************************************************************************
 * SVC handler is used to switch context.
 *****************************************************************************/
BSP_TARGET_ARM
BSP_ATTRIBUTE_STACKLESS void SVC_Handler (void)
{
    /* Save the context of the current task and select a new task to run. */
    portSAVE_CONTEXT();
    __asm volatile (
        "LDR        R0, =vTaskSwitchContext      \n"
        "BLX        R0                           \n"
        ::: "memory");
    portRESTORE_CONTEXT();
}

/******************************************************************************
 * vPortRestoreTaskContext is used to start the scheduler.
 *****************************************************************************/
BSP_TARGET_ARM
BSP_ATTRIBUTE_STACKLESS void vPortRestoreTaskContext (void)
{
    /* Switch to system mode(0x1f). */
    __asm volatile (
        "CPS        %[sys_mode]                  \n"
        "DSB                                     \n"
        "ISB                                     \n"
        ::[sys_mode] "i" (SYS_MODE) : "memory");
    portRESTORE_CONTEXT();
}

BSP_TARGET_ARM
BSP_ATTRIBUTE_STACKLESS void IRQ_Handler (void)
{
    /* Return to the interrupted instruction. */
    __asm volatile (
        "SUB        lr, lr, #4                   \n"

        /* Push the return address and SPSR. */
        "PUSH       {lr}                         \n"
        "MRS        lr, SPSR                     \n"
        "PUSH       {lr}                         \n"

        /* Change to supervisor mode(0x13) to allow reentry. */
        "CPS        %[svc_mode]                  \n"

        /* Push used registers. */
        "PUSH       {r0-r4, r12}                 \n"
#if __FPU_USED
        "FMRX       r0, FPSCR                    \n"
        "VPUSH      {d0-d15}                     \n"
 #if __FPU_D32
        "VPUSH      {d16-d31}                    \n"
 #endif
        "PUSH       {r0}                         \n"
#endif

        /* Increment nesting count.  r3 holds the address of ulPortInterruptNesting
         * for future use.  r1 holds the original ulPortInterruptNesting value for
         * future use. */
        "LDR        r3, =ulPortInterruptNesting  \n"
        "LDR        r1, [r3]                     \n"
        "ADD        r4, r1, #1                   \n"
        "STR        r4, [r3]                     \n"

        /* Read value from the interrupt acknowledge register, which is stored in r0
         * for future parameter and interrupt clearing use. */
        "MRC        p15,0,r0,c12,c12,0           \n" /* Read ICC_IAR1 into R0 */

        /* Ensure bit 2 of the stack pointer is clear.  r2 holds the bit 2 value for
         * future use.  _RB_ Is this ever needed provided the start of the stack is
         * alligned on an 8-byte boundary? */
        "MOV        r2, sp                       \n"
        "AND        r2, r2, #4                   \n"
        "SUB        sp, sp, r2                   \n"

        /* Call the interrupt handler. */
        "PUSH       {r0-r4, lr}                  \n"
        "LDR        r1, =vApplicationIRQHandler  \n"
        "BLX        r1                           \n"
        "POP        {r0-r4, lr}                  \n"
        "ADD        sp, sp, r2                   \n"

        "CPSID      i                            \n"
        "DSB                                     \n"
        "ISB                                     \n"

        /* Write the value read from ICCIAR to ICCEOIR. */
        "MCR        p15,0,r0,c12,c12,1           \n" /* Write R0 to ICC_EOIR1 */
        /* Restore the old nesting count. */
        "STR        r1, [r3]                     \n"

        /* A context switch is never performed if the nesting count is not 0. */
        "CMP        r1, #0                       \n"
        "BNE        exit_without_switch          \n"

        /* Did the interrupt request a context switch?  r1 holds the address of
         * ulPortYieldRequired and r0 the value of ulPortYieldRequired for future
         * use. */
        "LDR        r1, =ulPortYieldRequired     \n"
        "LDR        r0, [r1]                     \n"
        "CMP        r0, #0                       \n"
        "BNE        switch_before_exit           \n"
        "exit_without_switch:                    \n"

        /* No context switch.  Restore used registers, LR_irq and SPSR before
         * returning. */
#if __FPU_USED
        "POP        {r0}                         \n"
 #if __FPU_D32
        "VPOP       {d16-d31}                    \n"
 #endif
        "VPOP       {d0-d15}                     \n"
        "VMSR       FPSCR, r0                    \n"
#endif
        "POP		{r0-r4, r12}                 \n"
        "CPS        %[irq_mode]                  \n"
        "POP        {LR}                         \n"
        "MSR        SPSR_cxsf, LR                \n"
        "POP        {LR}                         \n"
        "MOVS       PC, LR                       \n"
        "switch_before_exit:                     \n"

        /* A context swtich is to be performed.  Clear the context switch pending
         * flag. */
        "MOV        r0, #0                       \n"
        "STR        r0, [r1]                     \n"

        /* Restore used registers, LR-irq and SPSR before saving the context
         * to the task stack. */
#if __FPU_USED
        "POP        {r0}                         \n"
 #if __FPU_D32
        "VPOP       {d16-d31}                    \n"
 #endif
        "VPOP       {d0-d15}                     \n"
        "VMSR       FPSCR, r0                    \n"
#endif
        "POP        {r0-r4, r12}                 \n"
        "CPS        %[irq_mode]                  \n"
        "POP        {LR}                         \n"
        "MSR        SPSR_cxsf, LR                \n"
        "POP        {LR}                         \n"
        ::[svc_mode] "i" (SVC_MODE), [irq_mode] "i" (IRQ_MODE) : "memory");
    portSAVE_CONTEXT();

    /* Call the function that selects the new task to execute.
     * vTaskSwitchContext() if vTaskSwitchContext() uses LDRD or STRD
     * instructions, or 8 byte aligned stack allocated data.  LR does not need
     * saving as a new LR will be loaded by portRESTORE_CONTEXT anyway. */
    __asm volatile (
        "LDR        R0, =vTaskSwitchContext      \n"
        "BLX        R0                           \n"
        ::: "memory");

    /* Restore the context of, and branch to, the task selected to execute
     * next. */
    portRESTORE_CONTEXT();
}
