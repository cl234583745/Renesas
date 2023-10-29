/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_SCTLR_BR_BIT        (0x00020000) /* Enable EL1 background region */
#define BSP_SCTLR_M_BIT         (0x00000001) /* Enable EL1-controlled MPU */
#define BSP_SCTLR_I_C_BIT_L     (0x1004)     /* Enable EL1-controlled Instruction and data cache, lower */
#define BSP_SCTLR_I_C_BIT_H     (0x0000)     /* Enable EL1-controlled Instruction and data cache, upper */

#define BSP_PRIORITY_MASK       (0x000000F8) /* Priority mask value for GIC */
#define BSP_ENABLE_GROUP_INT    (0x00000001) /* Enable Group1 interrupt value */
#define BSP_ICC_CTLR            (0x00000001) /* ICC_BPR0 is used for Group1 interrupt */

#define BSP_HACTLR_BIT_L        (0xB783)     /* HACTLR EL1 access enable(0b1011 0111 1000 0011) */
#define BSP_HCR_HCD_DISABLE     (0x20000000) /* HCR.HCD = 1 : HVC disable */
#define BSP_MODE_MASK           (0x1F)       /* Bit mask for mode bits in CPSR */
#define BSP_SVC_MODE            (0x13)       /* Supervisor mode */

#define NON_SHAREABLE           (0 << 3)
#define OUTER_SHAREABLE         (2 << 3)
#define INNER_SHAREABLE         (3 << 3)
#define EL1RW_EL0NO             (0 << 1)
#define EL1RW_EL0RW             (1 << 1)
#define EL1RO_EL0NO             (2 << 1)
#define EL1RO_EL0RO             (3 << 1)
#define EXECUTE_ENABLE          (0)
#define EXECUTE_NEVER           (1)
#define REGION_DISABLE          (0)
#define REGION_ENABLE           (1)
#define ATTRINDEX0              (0 << 1)
#define ATTRINDEX1              (1 << 1)
#define ATTRINDEX2              (2 << 1)
#define ATTRINDEX3              (3 << 1)
#define ATTRINDEX4              (4 << 1)
#define ATTRINDEX5              (5 << 1)
#define ATTRINDEX6              (6 << 1)
#define ATTRINDEX7              (7 << 1)

/* Attr0    1111_1111b Normal Memory, Outer&Inner R/W allocate ,Write-Back non-transient, but treated as WriteThrough in CR52 */
/* Attr1    1011_1011b Normal Memory, Outer&Inner R/W allocate ,Write-Through non-transient */
/* Attr2 1000_1000b Normal Memory, Outer&Inner non-allocate ,Write-Through non-transient */
/* Attr3 0100_0100b Normal Memory, Outer&Inner Non-Cacheable */
/* Attr4 0000_0000b Device-nGnRnE memory */
/* Attr5 0000_0100b Device-nGnRE memory */
/* Attr6 0000_1000b Device-nGRE memory */
/* Attr7 0000_1100b Device-GRE memory */
#define ATTR_3_2_1_0_L              (0xbbff)
#define ATTR_3_2_1_0_H              (0x4488)
#define ATTR_7_6_5_4_L              (0x0400)
#define ATTR_7_6_5_4_H              (0x0c08)

/* region 0 (ATCM) */
#define EL1_MPU_REGION00_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION00_BASE_H     (0x0000 & 0xFFFF)
#define EL1_MPU_REGION00_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION00_LIMIT_H    (0x0001 & 0xFFFF)

/* region 1 (BTCM) */
#define EL1_MPU_REGION01_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION01_BASE_H     (0x0010 & 0xFFFF)
#define EL1_MPU_REGION01_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION01_LIMIT_H    (0x0011 & 0xFFFF)

/* region 2 (System RAM) */
#define EL1_MPU_REGION02_BASE_L     (0x0000 & 0xFFC0) + NON_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION02_BASE_H     (0x1000 & 0xFFFF)
#define EL1_MPU_REGION02_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX1 + REGION_ENABLE
#define EL1_MPU_REGION02_LIMIT_H    (0x1017 & 0xFFFF)

/* region 3 (System RAM mirror) */
#define EL1_MPU_REGION03_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION03_BASE_H     (0x3000 & 0xFFFF)
#define EL1_MPU_REGION03_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION03_LIMIT_H    (0x3017 & 0xFFFF)

/* region 4 (External Address Space mirror) */
#define EL1_MPU_REGION04_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION04_BASE_H     (0x4000 & 0xFFFF)
#define EL1_MPU_REGION04_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION04_LIMIT_H    (0x5FFF & 0xFFFF)

/* region 5 (External Address Space) */
#define EL1_MPU_REGION05_BASE_L     (0x0000 & 0xFFC0) + NON_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION05_BASE_H     (0x6000 & 0xFFFF)
#define EL1_MPU_REGION05_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX1 + REGION_ENABLE
#define EL1_MPU_REGION05_LIMIT_H    (0x7FFF & 0xFFFF)

/* region 6 (Non-Safety Peripheral) */
#define EL1_MPU_REGION06_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_NEVER
#define EL1_MPU_REGION06_BASE_H     (0x8000 & 0xFFFF)
#define EL1_MPU_REGION06_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX5 + REGION_ENABLE
#define EL1_MPU_REGION06_LIMIT_H    (0x80FF & 0xFFFF)

/* region 7 (Safety Peripheral) */
#define EL1_MPU_REGION07_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_NEVER
#define EL1_MPU_REGION07_BASE_H     (0x8100 & 0xFFFF)
#define EL1_MPU_REGION07_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX5 + REGION_ENABLE
#define EL1_MPU_REGION07_LIMIT_H    (0x81FF & 0xFFFF)

/* region 8 (LLPP0 Peripheral) */
#define EL1_MPU_REGION08_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_NEVER
#define EL1_MPU_REGION08_BASE_H     (0x9000 & 0xFFFF)
#define EL1_MPU_REGION08_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX5 + REGION_ENABLE
#define EL1_MPU_REGION08_LIMIT_H    (0x901F & 0xFFFF)

/* region 9 (GIC0) */
#define EL1_MPU_REGION09_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_NEVER
#define EL1_MPU_REGION09_BASE_H     (0x9400 & 0xFFFF)
#define EL1_MPU_REGION09_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX4 + REGION_ENABLE
#define EL1_MPU_REGION09_LIMIT_H    (0x941F & 0xFFFF)

/* region 11 (Debug Private) */
#define EL1_MPU_REGION11_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_NEVER
#define EL1_MPU_REGION11_BASE_H     (0xC000 & 0xFFFF)
#define EL1_MPU_REGION11_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX4 + REGION_ENABLE
#define EL1_MPU_REGION11_LIMIT_H    (0xC0FF & 0xFFFF)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
#if BSP_CFG_RAM_EXECUTION
 #if defined(__ICCARM__)
extern void __iar_data_init3(void);

 #endif
#endif

extern void bsp_m_mpu_init(void);
extern void bsp_global_system_counter_init(void);

#if BSP_FEATURE_TFU_SUPPORTED
extern void bsp_tfu_init(void);

#endif

#if BSP_CFG_C_RUNTIME_INIT
extern void bsp_static_constructor_init(void);

#endif

#if !(BSP_CFG_RAM_EXECUTION)
extern void bsp_copy_to_atcm(void);
extern void bsp_application_bss_init(void);

#endif

extern void bsp_release_port_protect(void);
extern void R_BSP_WarmStart(bsp_warm_start_event_t event);

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
int32_t main(void);

#if defined(__ICCARM__)
void system_init(void);

#elif defined(__GNUC__)
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void system_init(void) BSP_PLACE_IN_SECTION(".loader_text");

#endif
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void stack_init(void);
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void mpu_cache_init(void);
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void bsp_mpu_init(void);

#if defined(__ICCARM__)
 #pragma section=".irq_stack"
 #pragma section=".fiq_stack"
 #pragma section=".svc_stack"
 #pragma section=".abt_stack"
 #pragma section=".und_stack"
 #pragma section=".sys_stack"

 #pragma section="LDR_DATA_RBLOCK"
 #pragma section="LDR_DATA_WBLOCK"
 #pragma section="LDR_DATA_ZBLOCK"

 #pragma section="SHARED_NONCACHE_BUFFER_ZBLOCK"
 #pragma section="NONCACHE_BUFFER_ZBLOCK"
#endif

void        __Vectors(void) BSP_PLACE_IN_SECTION(".intvec");
__WEAK void IRQ_Handler(void);
void        Default_Handler(void);

#if defined(__GNUC__)
void Default_Handler (void)
{
    /** A error has occurred. The user will need to investigate the cause. Common problems are stack corruption
     *  or use of an invalid pointer. Use the Fault Status window in e2 studio or manually check the fault status
     *  registers for more information.
     */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}

#endif

#if defined(__ICCARM__)
 #define WEAK_REF_ATTRIBUTE

 #pragma weak Reset_Handler         = Default_Handler
 #pragma weak Undefined_Handler     = Default_Handler
 #pragma weak SVC_Handler           = Default_Handler
 #pragma weak Prefetch_Handler      = Default_Handler
 #pragma weak Abort_Handler         = Default_Handler
 #pragma weak Reserved_Handler      = Default_Handler
 #pragma weak FIQ_Handler           = Default_Handler
#elif defined(__GNUC__)

 #define WEAK_REF_ATTRIBUTE    __attribute__((weak, alias("Default_Handler")))
#endif

void Reset_Handler(void) WEAK_REF_ATTRIBUTE;
void Undefined_Handler(void) WEAK_REF_ATTRIBUTE;
void SVC_Handler(void) WEAK_REF_ATTRIBUTE;
void Prefetch_Handler(void) WEAK_REF_ATTRIBUTE;
void Abort_Handler(void) WEAK_REF_ATTRIBUTE;
void Reserved_Handler(void) WEAK_REF_ATTRIBUTE;
void FIQ_Handler(void) WEAK_REF_ATTRIBUTE;

/* Stacks */
BSP_DONT_REMOVE static uint8_t g_fiq_stack[BSP_CFG_STACK_FIQ_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT)
BSP_PLACE_IN_SECTION(BSP_SECTION_FIQ_STACK);
BSP_DONT_REMOVE static uint8_t g_irq_stack[BSP_CFG_STACK_IRQ_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT)
BSP_PLACE_IN_SECTION(BSP_SECTION_IRQ_STACK);
BSP_DONT_REMOVE static uint8_t g_abt_stack[BSP_CFG_STACK_ABT_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT)
BSP_PLACE_IN_SECTION(BSP_SECTION_ABT_STACK);
BSP_DONT_REMOVE static uint8_t g_und_stack[BSP_CFG_STACK_UND_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT)
BSP_PLACE_IN_SECTION(BSP_SECTION_UND_STACK);
BSP_DONT_REMOVE static uint8_t g_sys_stack[BSP_CFG_STACK_SYS_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT)
BSP_PLACE_IN_SECTION(BSP_SECTION_SYS_STACK);
BSP_DONT_REMOVE static uint8_t g_svc_stack[BSP_CFG_STACK_SVC_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT)
BSP_PLACE_IN_SECTION(BSP_SECTION_SVC_STACK);

/* Heap */
#if (BSP_CFG_HEAP_BYTES > 0)

BSP_DONT_REMOVE static uint8_t g_heap[BSP_CFG_HEAP_BYTES] BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT) \
    BSP_PLACE_IN_SECTION(BSP_SECTION_HEAP);
#endif

BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void __Vectors (void)
{
    __asm volatile (
#if 0 // Original program
        "    ldr pc,=Reset_Handler            \n"
#else
        /* After copying application program by the loader program processing,
         * application program is stated with this instruction
         * and branches to system_init in this sample program. */
        "    ldr pc,=system_init              \n"
#endif
        "    ldr pc,=Undefined_Handler        \n"
        "    ldr pc,=SVC_Handler              \n"
        "    ldr pc,=Prefetch_Handler         \n"
        "    ldr pc,=Abort_Handler            \n"
        "    ldr pc,=Reserved_Handler         \n"
        "    ldr pc,=IRQ_Handler              \n"
        "    ldr pc,=FIQ_Handler              \n"
        ::: "memory");
}

/*******************************************************************************************************************//**
 * After boot processing, LSI starts executing here.
 **********************************************************************************************************************/
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void system_init (void)
{
/* These settings are invalid for application project.
 * The necessary processing has been performed in the loader program. */
#if 0 // Original program
    __asm volatile (
        "set_hactlr:                              \n"
        "   MOVW  r0, %[bsp_hactlr_bit_l]         \n" /* Set HACTLR bits(L) */
        "   MOVT  r0, #0                          \n"
        "   MCR   p15, #4, r0, c1, c0, #1         \n" /* Write r0 to HACTLR */
        ::[bsp_hactlr_bit_l] "i" (BSP_HACTLR_BIT_L) : "memory");

    __asm volatile (
        "set_hcr:                                 \n"
        "    MRC   p15, #4, r1, c1, c1, #0        \n" /* Read Hyp Configuration Register */
        "    ORR   r1, r1, %[bsp_hcr_hcd_disable] \n" /* HVC instruction disable */
        "    MCR   p15, #4, r1, c1, c1, #0        \n" /* Write Hyp Configuration Register */
        ::[bsp_hcr_hcd_disable] "i" (BSP_HCR_HCD_DISABLE) : "memory");

    __asm volatile (
        "set_vbar:                           \n"
        "    LDR   r0, =__Vectors            \n"
        "    MCR   p15, #0, r0, c12, c0, #0  \n" /* Write r0 to VBAR */
        ::: "memory");

    __asm volatile (
        "LLPP_access_enable:                     \n"

        /* Enable PERIPHPREGIONR (LLPP) */
        "    mrc        p15,#0, r1, c15, c0,#0   \n" /* PERIPHPREGIONR */
        "    orr        r1, r1, #(0x1 << 1)      \n" /* Enable PERIPHPREGIONR EL2 */
        "    orr        r1, r1, #(0x1)           \n" /* Enable PERIPHPREGIONR EL1 and EL0 */
        "    dsb                                 \n" /* Ensuring memory access complete */
        "    mcr        p15,#0, r1, c15, c0,#0   \n" /* PERIPHREGIONR */
        "    isb                                 \n" /* Ensuring Context-changing */
        ::: "memory");

    __asm volatile (
        "cpsr_save:                              \n"
        "    MRS   r0, cpsr                      \n" /* Original PSR value */
        "    BIC   r0, r0, %[bsp_mode_mask]      \n" /* Clear the mode bits */
        "    ORR   r0, r0, %[bsp_svc_mode]       \n" /* Set SVC mode bits */
        "    MSR   SPSR_hyp, r0                  \n"
        ::[bsp_mode_mask] "i" (BSP_MODE_MASK), [bsp_svc_mode] "i" (BSP_SVC_MODE) : "memory");

    __asm volatile (
        "exception_return:                       \n"
        "    LDR   r1, =stack_init               \n"
        "    MSR   ELR_hyp, r1                   \n"
        "    ERET                                \n" /* Branch to stack_init and enter EL1 */
        ::: "memory");
#else

    /* Set exception vector offset for application program. */
    __asm volatile (
        "set_vbar:                           \n"
        "    LDR   r0, =__Vectors            \n"
        "    MCR   p15, #0, r0, c12, c0, #0  \n" /* Write r0 to VBAR */
        ::: "memory");

    __asm volatile (
        "jump_stack_init:                      \n"
        "    ldr r0, =stack_init               \n"
        "    blx r0                            \n" /* Jump to stack_init */
        );
#endif
}

/*******************************************************************************************************************//**
 * After system_init, EL1 settings start here.
 **********************************************************************************************************************/
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void stack_init (void)
{
#if defined(__ICCARM__)
    __asm volatile (
        "stack_initialization:        \n"

        /* Stack setting for EL1 */
        "    cps  #17                 \n" /* FIQ mode */
        "    mov  sp, %[fiq_stack]    \n"
        ::[fiq_stack] "r" (__section_end(".fiq_stack")) : "memory");

    __asm volatile (
        "    cps  #18                 \n" /* IRQ mode */
        "    mov  sp, %[irq_stack]    \n"
        ::[irq_stack] "r" (__section_end(".irq_stack")) : "memory");

    __asm volatile (
        "    cps  #23                 \n" /* Abort mode */
        "    mov  sp, %[abt_stack]    \n"
        ::[abt_stack] "r" (__section_end(".abt_stack")) : "memory");

    __asm volatile (
        "    cps  #27                 \n" /* Undef mode */
        "    mov  sp, %[und_stack]    \n"
        ::[und_stack] "r" (__section_end(".und_stack")) : "memory");

    __asm volatile (
        "    cps  #31                 \n" /* System mode */
        "    mov  sp, %[sys_stack]    \n"
        ::[sys_stack] "r" (__section_end(".sys_stack")) : "memory");

    __asm volatile (
        "    cps  #19                 \n" /* SVC mode */
        "    mov  sp, %[svc_stack]    \n"
        ::[svc_stack] "r" (__section_end(".svc_stack")) : "memory");
#elif defined(__GNUC__)
    __asm volatile (
        "stack_initialization:             \n"

        /* Stack setting for EL1 */
        "    cps  #17                      \n" /* FIQ mode */
        "    ldr  sp, =__FiqStackLimit     \n"
        "    cps  #18                      \n" /* IRQ mode */
        "    ldr  sp, =__IrqStackLimit     \n"
        "    cps  #23                      \n" /* Abort mode */
        "    ldr  sp, =__AbtStackLimit     \n"
        "    cps  #27                      \n" /* Undef mode */
        "    ldr  sp, =__UndStackLimit     \n"
        "    cps  #31                      \n" /* System mode */
        "    ldr  sp, =__SysStackLimit     \n"
        "    cps  #19                      \n" /* SVC mode */
        "    ldr  sp, =__SvcStackLimit     \n"
        ::: "memory");
#endif

#if __FPU_USED
    __asm volatile (
        "FPU_AdvancedSIMD_init:                         \n"

        /* Initialize FPU and Advanced SIMD setting */
        "    mrc  p15, #0, r0, c1, c0, #2               \n" /* Enables cp10 and cp11 accessing */
        "    orr  r0, r0, #0xF00000                     \n"
        "    mcr  p15, #0, r0, c1, c0, #2               \n"
        "    isb                                        \n" /* Ensuring Context-changing */

        "    vmrs r0, fpexc                             \n" /* Enables the FPU */
        "    orr  r0, r0, #0x40000000                   \n"
        "    vmsr  fpexc, r0                            \n"
        "    isb                                        \n" /* Ensuring Context-changing */
        ::: "memory");
#endif

    __asm volatile (
        "clock_init:                     \n"
        "    ldr r0, =bsp_clock_init     \n"
        "    blx r0                      \n" /* Jump to bsp_clock_init */
        );

#if BSP_CFG_C_RUNTIME_INIT
 #if defined(__ICCARM__)
    __asm volatile (
        "loader_data_init:                                  \n"

        "    mov  r1, %[sfb_ldr_data_wblock]                \n"
        ::[sfb_ldr_data_wblock] "r" (__section_begin("LDR_DATA_WBLOCK")) : "r1");

    __asm volatile (
        "    mov  r2, %[sizeof_ldr_data_wblock]             \n"
        ::[sizeof_ldr_data_wblock] "r" (__section_size("LDR_DATA_WBLOCK")) : "r2");

    __asm volatile (
        "    mov  r3, %[sfb_ldr_data_rblock]                \n"
        ::[sfb_ldr_data_rblock] "r" (__section_begin("LDR_DATA_RBLOCK")) : "r3");

    __asm volatile (
        "    cmp  r2, #0                                    \n"
        "    beq  loader_data_init_end                      \n"

        "copy_to_LDR_DATA:                                  \n"
        "    ldrb  r0, [r3], #1                             \n"
        "    strb  r0, [r1], #1                             \n"
        "    subs  r2, r2, #1                               \n"
        "    bne   copy_to_LDR_DATA                         \n"
        "    dsb                                            \n" /* Ensuring data-changing */

        "loader_data_init_end:                              \n"
        ::: "memory");

    __asm volatile (
        "loader_bss_init:                                   \n"

        /* Clear the loader bss used by the loader */
        "    mov  r1, %[sfb_ldr_data_zblock]                \n"
        ::[sfb_ldr_data_zblock] "r" (__section_begin("LDR_DATA_ZBLOCK")) : "r1");

    __asm volatile (
        "    mov  r2, %[sfe_ldr_data_zblock]                \n"
        ::[sfe_ldr_data_zblock] "r" (__section_end("LDR_DATA_ZBLOCK")) : "r2");

    __asm volatile (
        "    cmp  r2, r1                                    \n"
        "    beq  loader_bss_init_end                       \n"

        "clear_loader_bss:                                  \n"
        "    ldr   r0, =0x00000000                          \n"
        "    strb  r0, [r1], #0                             \n"
        "    add   r1, r1, #1                               \n"
        "    cmp   r2, r1                                   \n"
        "    bne   clear_loader_bss                         \n"
        "    dsb                                            \n" /* Ensuring data-changing */

        "loader_bss_init_end:                               \n"
        ::: "memory");
  #if BSP_CFG_RAM_EXECUTION

    /* Initialize the application data and clear the application bss.
     * This code is for RAM Execution. If you want to boot with ROM,
     * enable app_copy and app_bss_init, and disable this code.
     * Also need to change icf file. */
    __iar_data_init3();

    __asm volatile (
        "shared_noncache_buffer_init:                       \n"

        /* Clear the shared-non cache buffer */
        "    mov  r1, %[sfb_shared_noncache_buffer_zblock]  \n"
        ::[sfb_shared_noncache_buffer_zblock] "r" (__section_begin("SHARED_NONCACHE_BUFFER_ZBLOCK")) : "r1");

    __asm volatile (
        "    mov  r2, %[sfb_shared_noncache_buffer_zblock]  \n"
        ::[sfb_shared_noncache_buffer_zblock] "r" (__section_end("SHARED_NONCACHE_BUFFER_ZBLOCK")) : "r2");

    __asm volatile (
        "    cmp  r2, r1                                    \n"
        "    beq  shared_noncache_buffer_init_end           \n"

        "clear_shared_noncache_buffer:                      \n"
        "    ldr   r0, =0x00000000                          \n"
        "    strb  r0, [r1], #0                             \n"
        "    add   r1, r1, #1                               \n"
        "    cmp   r2, r1                                   \n"
        "    bne   clear_shared_noncache_buffer             \n"
        "    dsb                                            \n" /* Ensuring data-changing */

        "shared_noncache_buffer_init_end:                  \n"
        ::: "memory");

    __asm volatile (
        "noncache_buffer_init:                             \n"

        /* Clear the shared-non cache buffer */
        "    mov  r1, %[sfb_noncache_buffer_zblock]        \n"
        ::[sfb_noncache_buffer_zblock] "r" (__section_begin("NONCACHE_BUFFER_ZBLOCK")) : "r1");

    __asm volatile (
        "    mov  r2, %[sfb_noncache_buffer_zblock]        \n"
        ::[sfb_noncache_buffer_zblock] "r" (__section_end("NONCACHE_BUFFER_ZBLOCK")) : "r2");

    __asm volatile (
        "    cmp  r2, r1                                    \n"
        "    beq  noncache_buffer_init_end                  \n"

        "clear_noncache_buffer:                             \n"
        "    ldr   r0, =0x00000000                          \n"
        "    strb  r0, [r1], #0                             \n"
        "    add   r1, r1, #1                               \n"
        "    cmp   r2, r1                                   \n"
        "    bne   clear_noncache_buffer                    \n"
        "    dsb                                            \n" /* Ensuring data-changing */

        "noncache_buffer_init_end:                         \n"
        ::: "memory");
  #endif
 #elif defined(__GNUC__)
#if 0 // Original program
    __asm volatile (
  #if !(BSP_CFG_RAM_EXECUTION)
        "loader_data_init:                                   \n"

        /* Initialize loader_data. */
        "    ldr r1, = __loader_data_start                   \n"
        "    ldr r2, = __loader_data_end                     \n"
        "    ldr r3, = _mloader_data                         \n"
        "    cmp r2, r1                                      \n"
        "    beq loader_data_init_end                        \n"

        "set_loader_data:                                    \n"
        "    ldrb r0, [r3], #1                               \n"
        "    strb r0, [r1], #1                               \n"
        "    cmp r2, r1                                      \n"
        "    bne set_loader_data                             \n"
        "    loader_data_init_end:                           \n"
        "    dsb                                             \n" /* Ensuring data-changing */
  #endif
        "loader_bss_init:                                    \n"

        /* Clear loader_bss. */
        "    ldr r0, = 0x00000000                            \n"
        "    ldr r1, = __loader_bss_start                    \n"
        "    ldr r2, = __loader_bss_end                      \n"
        "    cmp r2, r1                                      \n"
        "    beq application_bss_init                        \n"

        "clear_loader_bss:                                   \n"
        "    strb r0, [r1], #0                               \n"
        "    add r1, r1, #1                                  \n"
        "    cmp r2, r1                                      \n"
        "    bne clear_loader_bss                            \n"
        "    dsb                                             \n" /* Ensuring data-changing */

        "application_bss_init:                               \n"
#else
    __asm volatile (
 #if !(BSP_CFG_RAM_EXECUTION)
      "data_init:                                          \n"

      /* Initialize data. */
      "    ldr r1, = _data_start                           \n"
      "    ldr r2, = _data_end                             \n"
      "    ldr r3, = _mdata                                \n"
      "    cmp r2, r1                                      \n"
      "    beq data_init_end                               \n"

      "set_data:                                           \n"
      "    ldrb r0, [r3], #1                               \n"
      "    strb r0, [r1], #1                               \n"
      "    cmp r2, r1                                      \n"
      "    bne set_data                                    \n"
      "data_init_end:                                      \n"
      "    dsb                                             \n" /* Ensuring data-changing */
 #endif
      "bss_init:                                           \n"

      /* Clear bss. */
      "    ldr r0, = 0x00000000                            \n"
      "    ldr r1, = _bss_start                            \n"
      "    ldr r2, = _bss_end                              \n"
      "    cmp r2, r1                                      \n"
      "    beq bss_init_end                                \n"

      "clear_bss:                                          \n"
      "    strb r0, [r1], #0                               \n"
      "    add r1, r1, #1                                  \n"
      "    cmp r2, r1                                      \n"
      "    bne clear_bss                                   \n"
      "    dsb                                             \n" /* Ensuring data-changing */

      "bss_init_end:                                       \n"
#endif
  #if BSP_CFG_RAM_EXECUTION

        /* Clear BSS. */
        "    ldr r0, = 0x00000000                            \n"
        "    ldr r1, = __bss_start__                         \n"
        "    ldr r2, = __bss_end__                           \n"
        "    cmp r2, r1                                      \n"
        "    beq bss_init_end                                \n"

        "clear_bss:                                          \n"
        "    strb r0, [r1], #0                               \n"
        "    add r1, r1, #1                                  \n"
        "    cmp r2, r1                                      \n"
        "    bne clear_bss                                   \n"
        "    bss_init_end:                                   \n"
        "    dsb                                             \n" /* Ensuring data-changing */

        /* Clear shared non-cache buffer. */
        "    ldr  r0, =0x00000000                            \n"
        "    ldr  r1, =_sncbuffer_start                      \n"
        "    ldr  r2, =_sncbuffer_end                        \n"
        "    cmp  r2, r1                                     \n"
        "    beq  shared_noncache_buffer_init_end            \n"
        "clear_shared_noncache_buffer:                       \n"
        "    strb  r0, [r1], #0                              \n"
        "    add   r1, r1, #1                                \n"
        "    cmp   r2, r1                                    \n"
        "    bne   clear_shared_noncache_buffer              \n"
        "    shared_noncache_buffer_init_end:                \n"
        "    dsb                                             \n" /* Ensuring data-changing */

        /* Clear non-cache buffer. */
        "    ldr  r0, =0x00000000                            \n"
        "    ldr  r1, =_ncbuffer_start                       \n"
        "    ldr  r2, =_ncbuffer_end                         \n"
        "    cmp  r2, r1                                     \n"
        "    beq  noncache_buffer_init_end                   \n"
        "clear_noncache_buffer:                              \n"
        "    strb  r0, [r1], #0                              \n"
        "    add   r1, r1, #1                                \n"
        "    cmp   r2, r1                                    \n"
        "    bne   clear_noncache_buffer                     \n"
        "    noncache_buffer_init_end:                       \n"
        "    dsb                                             \n" /* Ensuring data-changing */
  #endif
        ::: "memory");
 #endif
#endif

    __asm volatile (
        "system_core_clock_update:                           \n"
        "    ldr r0, =SystemCoreClockUpdate                  \n"
        "    blx r0                                          \n" /* Jump to SystemCoreClockUpdate */
        );

    __asm volatile (
        "    b mpu_cache_init                                \n"
        );
}

/*******************************************************************************************************************//**
 * EL1 core initialization block.
 **********************************************************************************************************************/
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void mpu_cache_init (void)
{
    __asm volatile (
        "MPU_default_memory_map_enable:                            \n"

        /* Adopt EL1 default memory map as background map. */
        "    mov  r0, %[bsp_sctlr_br_bit]                          \n" /* Set SCTLR.BR bit to 1 */
        "    mrc  p15, #0, r1, c1, c0, #0                          \n"
        "    orr  r1, r1, r0                                       \n"
        "    dsb                                                   \n" /* Ensuring memory access complete */
        "    mcr  p15, #0, r1, c1, c0, #0                          \n"
        "    isb                                                   \n" /* Ensuring Context-changing */
        ::[bsp_sctlr_br_bit] "i" (BSP_SCTLR_BR_BIT) : "memory");

    __asm volatile (
        "MPU_init:                                                 \n"

        /* Configure Memory Attribute Indirection Registers */
        "    movw r0, %[attr_3_2_1_0_l]                            \n"
        "    movt r0, %[attr_3_2_1_0_h]                            \n"
        "    movw r1, %[attr_7_6_5_4_l]                            \n"
        "    movt r1, %[attr_7_6_5_4_h]                            \n"
        "    mcr  p15, #0, r0, c10, c2, #0                         \n" /* Set MAIR0 */
        "    mcr  p15, #0, r1, c10, c2, #1                         \n" /* Set MAIR1 */
        "    dsb                                                   \n" /* Ensuring memory access complete */
        ::[attr_3_2_1_0_l] "i" (ATTR_3_2_1_0_L), [attr_3_2_1_0_h] "i" (ATTR_3_2_1_0_H),
        [attr_7_6_5_4_l] "i" (ATTR_7_6_5_4_L), [attr_7_6_5_4_h] "i" (ATTR_7_6_5_4_H) : "memory");

    /* Setup region 0 - 12 */
    __asm volatile (
        "    mov  r0, #0                                           \n" /* region No.0 */
        "    movw r2, %[el1_mpu_region00_base_l]                   \n"
        "    movt r2, %[el1_mpu_region00_base_h]                   \n"
        "    movw r3, %[el1_mpu_region00_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region00_limit_h]                  \n"
        ::[el1_mpu_region00_base_l] "i" (EL1_MPU_REGION00_BASE_L),
        [el1_mpu_region00_base_h] "i" (EL1_MPU_REGION00_BASE_H),
        [el1_mpu_region00_limit_l] "i" (EL1_MPU_REGION00_LIMIT_L),
        [el1_mpu_region00_limit_h] "i" (EL1_MPU_REGION00_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #1                                           \n" /* region No.1 */
        "    movw r2, %[el1_mpu_region01_base_l]                   \n"
        "    movt r2, %[el1_mpu_region01_base_h]                   \n"
        "    movw r3, %[el1_mpu_region01_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region01_limit_h]                  \n"
        ::[el1_mpu_region01_base_l] "i" (EL1_MPU_REGION01_BASE_L),
        [el1_mpu_region01_base_h] "i" (EL1_MPU_REGION01_BASE_H),
        [el1_mpu_region01_limit_l] "i" (EL1_MPU_REGION01_LIMIT_L),
        [el1_mpu_region01_limit_h] "i" (EL1_MPU_REGION01_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #2                                           \n" /* region No.2 */
        "    movw r2, %[el1_mpu_region02_base_l]                   \n"
        "    movt r2, %[el1_mpu_region02_base_h]                   \n"
        "    movw r3, %[el1_mpu_region02_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region02_limit_h]                  \n"
        ::[el1_mpu_region02_base_l] "i" (EL1_MPU_REGION02_BASE_L),
        [el1_mpu_region02_base_h] "i" (EL1_MPU_REGION02_BASE_H),
        [el1_mpu_region02_limit_l] "i" (EL1_MPU_REGION02_LIMIT_L),
        [el1_mpu_region02_limit_h] "i" (EL1_MPU_REGION02_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #3                                           \n" /* region No.3 */
        "    movw r2, %[el1_mpu_region03_base_l]                   \n"
        "    movt r2, %[el1_mpu_region03_base_h]                   \n"
        "    movw r3, %[el1_mpu_region03_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region03_limit_h]                  \n"
        ::[el1_mpu_region03_base_l] "i" (EL1_MPU_REGION03_BASE_L),
        [el1_mpu_region03_base_h] "i" (EL1_MPU_REGION03_BASE_H),
        [el1_mpu_region03_limit_l] "i" (EL1_MPU_REGION03_LIMIT_L),
        [el1_mpu_region03_limit_h] "i" (EL1_MPU_REGION03_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #4                                           \n" /* region No.4 */
        "    movw r2, %[el1_mpu_region04_base_l]                   \n"
        "    movt r2, %[el1_mpu_region04_base_h]                   \n"
        "    movw r3, %[el1_mpu_region04_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region04_limit_h]                  \n"
        ::[el1_mpu_region04_base_l] "i" (EL1_MPU_REGION04_BASE_L),
        [el1_mpu_region04_base_h] "i" (EL1_MPU_REGION04_BASE_H),
        [el1_mpu_region04_limit_l] "i" (EL1_MPU_REGION04_LIMIT_L),
        [el1_mpu_region04_limit_h] "i" (EL1_MPU_REGION04_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #5                                           \n" /* region No.5 */
        "    movw r2, %[el1_mpu_region05_base_l]                   \n"
        "    movt r2, %[el1_mpu_region05_base_h]                   \n"
        "    movw r3, %[el1_mpu_region05_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region05_limit_h]                  \n"
        ::[el1_mpu_region05_base_l] "i" (EL1_MPU_REGION05_BASE_L),
        [el1_mpu_region05_base_h] "i" (EL1_MPU_REGION05_BASE_H),
        [el1_mpu_region05_limit_l] "i" (EL1_MPU_REGION05_LIMIT_L),
        [el1_mpu_region05_limit_h] "i" (EL1_MPU_REGION05_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #6                                           \n" /* region No.6 */
        "    movw r2, %[el1_mpu_region06_base_l]                   \n"
        "    movt r2, %[el1_mpu_region06_base_h]                   \n"
        "    movw r3, %[el1_mpu_region06_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region06_limit_h]                  \n"
        ::[el1_mpu_region06_base_l] "i" (EL1_MPU_REGION06_BASE_L),
        [el1_mpu_region06_base_h] "i" (EL1_MPU_REGION06_BASE_H),
        [el1_mpu_region06_limit_l] "i" (EL1_MPU_REGION06_LIMIT_L),
        [el1_mpu_region06_limit_h] "i" (EL1_MPU_REGION06_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #7                                           \n" /* region No.7 */
        "    movw r2, %[el1_mpu_region07_base_l]                   \n"
        "    movt r2, %[el1_mpu_region07_base_h]                   \n"
        "    movw r3, %[el1_mpu_region07_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region07_limit_h]                  \n"
        ::[el1_mpu_region07_base_l] "i" (EL1_MPU_REGION07_BASE_L),
        [el1_mpu_region07_base_h] "i" (EL1_MPU_REGION07_BASE_H),
        [el1_mpu_region07_limit_l] "i" (EL1_MPU_REGION07_LIMIT_L),
        [el1_mpu_region07_limit_h] "i" (EL1_MPU_REGION07_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #8                                           \n" /* region No.8 */
        "    movw r2, %[el1_mpu_region08_base_l]                   \n"
        "    movt r2, %[el1_mpu_region08_base_h]                   \n"
        "    movw r3, %[el1_mpu_region08_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region08_limit_h]                  \n"
        ::[el1_mpu_region08_base_l] "i" (EL1_MPU_REGION08_BASE_L),
        [el1_mpu_region08_base_h] "i" (EL1_MPU_REGION08_BASE_H),
        [el1_mpu_region08_limit_l] "i" (EL1_MPU_REGION08_LIMIT_L),
        [el1_mpu_region08_limit_h] "i" (EL1_MPU_REGION08_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #9                                           \n" /* region No.9 */
        "    movw r2, %[el1_mpu_region09_base_l]                   \n"
        "    movt r2, %[el1_mpu_region09_base_h]                   \n"
        "    movw r3, %[el1_mpu_region09_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region09_limit_h]                  \n"
        ::[el1_mpu_region09_base_l] "i" (EL1_MPU_REGION09_BASE_L),
        [el1_mpu_region09_base_h] "i" (EL1_MPU_REGION09_BASE_H),
        [el1_mpu_region09_limit_l] "i" (EL1_MPU_REGION09_LIMIT_L),
        [el1_mpu_region09_limit_h] "i" (EL1_MPU_REGION09_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "    mov  r0, #11                                          \n" /* region No.11 */
        "    movw r2, %[el1_mpu_region11_base_l]                   \n"
        "    movt r2, %[el1_mpu_region11_base_h]                   \n"
        "    movw r3, %[el1_mpu_region11_limit_l]                  \n"
        "    movt r3, %[el1_mpu_region11_limit_h]                  \n"
        ::[el1_mpu_region11_base_l] "i" (EL1_MPU_REGION11_BASE_L),
        [el1_mpu_region11_base_h] "i" (EL1_MPU_REGION11_BASE_H),
        [el1_mpu_region11_limit_l] "i" (EL1_MPU_REGION11_LIMIT_L),
        [el1_mpu_region11_limit_h] "i" (EL1_MPU_REGION11_LIMIT_H) : "memory");
    __asm volatile (
        "bl bsp_mpu_init                                           \n"
        );

    __asm volatile (
        "cache_invalidate:\n"

        /* Invalidate instruction cache */
        "    dsb                                                   \n" /* Ensure memory operations are completed this does cache write back */
        "    mov     r0, #0                                        \n"
        "    mcr     p15, 0, r0, c7, c5, 0                         \n" /* Invalidate entire instruction cache */

        /* Invalidate data cache. RZ microprocessor only has L1 cache. */
        "    mcr     p15, 2, r0, c0, c0, 0                         \n" /* Select Data L1 in Cache Size selection register */
        "    isb                                                   \n" /* ISB to sync the change to the CacheSizeID reg */
        "    mrc     p15, 1, r1, c0, c0, 0                         \n" /* Read current Cache Size ID register to r1 */
        "    and     r2, r1, #7                                    \n" /* Get the line length field in r2 */
        "    add     r2, r2, #4                                    \n" /* Add 4 for the line length bit offset (log2 16 bytes) */
        "    ldr     r4, =0x3FF                                    \n"
        "    and     r4, r4, r1, LSR #3                            \n" /* r4 is the number of ways-1 (right aligned) */
        "    clz     r5, r4                                        \n" /* r5 is the bit position of the way size increment (CLZ = Count Leading Zeros) */
        "    ldr     r7, =0x7FFF                                   \n"
        "    and     r7, r7, r1, LSR #13                           \n" /* r7 is the number of sets-1 (right aligned) */
        "invalidate_set:                                           \n"
        "    mov     r9, r4                                        \n" /* r9 working copy of number of ways-1 (right aligned) */
        "invalidate_way:                                           \n"
        "    lsl     r11, r9, r5                                   \n" /* Left shift way number into r11 */
        "    lsl     r12, r7, r2                                   \n" /* Left shift set number into r12 */
        "    orr     r11, r11, r12                                 \n" /* Combine set number and way number */
        "    mcr     p15, 0, r11, c7, c6, 2                        \n" /* DCISW - data cache invalidate by set/way */

        "    subs    r9, r9, #1                                    \n" /* Decrement the way number */
        "    bge     invalidate_way                                \n"

        "    subs    r7, r7, #1                                    \n" /* Decrement the set number */
        "    bge     invalidate_set                                \n"
        );

    __asm volatile (
        "MPU_enable:                                               \n"

        /* Enables EL1 MPU operation */
        "    mov  r0, %[bsp_sctlr_m_bit]                           \n" /* Set SCTLR.M bit to 1 */
        "    mrc  p15, #0, r1, c1, c0, #0                          \n"
        "    orr  r1, r1, r0                                       \n"
        "    dsb                                                   \n" /* Ensuring memory access complete */
        "    mcr  p15, #0, r1, c1, c0, #0                          \n"
        "    isb                                                   \n" /* Ensuring Context-changing */
        ::[bsp_sctlr_m_bit] "i" (BSP_SCTLR_M_BIT) : "memory");

    __asm volatile (
        "cache_validate:\n"

        /* Enables I1,D1 cache operation */
        "     movw  r0, %[bsp_sctlr_i_c_bit_l]                     \n" /* Set SCTLR.I and C bit to 1 */
        "     movt  r0, %[bsp_sctlr_i_c_bit_h]                     \n" /* Set SCTLR.I and C bit to 1 */
        "     mrc  p15, #0, r1, c1, c0, #0                         \n"
        "     orr  r1, r1, r0                                      \n"
        "     dsb                                                  \n" /* Ensuring memory access complete */
        "     mcr  p15, #0, r1, c1, c0, #0                         \n"
        "     isb                                                  \n" /* Ensuring Context-changing */
        ::[bsp_sctlr_i_c_bit_l] "i" (BSP_SCTLR_I_C_BIT_L), [bsp_sctlr_i_c_bit_h] "i" (BSP_SCTLR_I_C_BIT_H) : "memory");

/* Invalid application copy operation for application project.
 * Application copy processing is done in loader program.      */
#if 0 // Original program
#if !(BSP_CFG_RAM_EXECUTION)
    __asm volatile (
        "app_copy:                               \n"
        "    ldr r0, =bsp_copy_to_atcm           \n"
        "    blx r0                              \n" /* Jump to bsp_copy_to_atcm */
        );

    __asm volatile (
        "app_bss_init:                           \n"
        "    ldr r0, =bsp_application_bss_init   \n"
        "    blx r0                              \n" /* Jump to bsp_application_bss_init */
        );
#endif
#endif

#if BSP_CFG_C_RUNTIME_INIT

    /* Initialize static constructors */
    __asm volatile (
        "static_constructor_init:                            \n"
        "    ldr r0, =bsp_static_constructor_init            \n"
        "    blx r0                                          \n" /* Jump to bsp_static_constructor_init */
        );
#endif

#if !BSP_CFG_PORT_PROTECT
    __asm volatile (
        "release_port_protect:                               \n"
        "    ldr r0, =bsp_release_port_protect               \n"
        "    blx r0                                          \n" /* Jump to bsp_release_port_protect */
        );
#endif

    __asm volatile (
        "ioport_init:                                        \n"
        "    mov r0, %[bsp_warm_start_post_c]                \n"
        "    ldr r1, =R_BSP_WarmStart                        \n"
        "    blx r1                                          \n"
        ::[bsp_warm_start_post_c] "i" (BSP_WARM_START_POST_C) : "memory");

    /* Jump to bsp_m_mpu_init */
    __asm volatile (
        "m_mpu_init:                       \n"
        "ldr r0, =bsp_m_mpu_init           \n"
        "blx r0                            \n"
        );

    __asm volatile (
        "SlaveTCM_enable:                              \n"

        /* Enable SLAVEPCTLR TCM access lvl slaves */
        "     mrc        p15,#0, r1, c11, c0,#0        \n" /* SLAVEPCTLR */
        "     orr        r1, r1, #(0x3)                \n" /* Enable TCM access priv and non priv */
        "     dsb                                      \n" /* Ensuring memory access complete */
        "     mcr        p15,#0, r1, c11, c0,#0        \n" /* SLAVEPCTLR */
        "     isb                                      \n" /* Ensuring Context-changing */
        ::: "memory");

    /* Jump to bsp_global_system_counter_init */
    __asm volatile (
        "global_system_counter_init:                   \n"
        "ldr r0, =bsp_global_system_counter_init       \n"
        "blx r0                                        \n"
        );

    __asm volatile (
        "bsp_irq_cfg_common:                           \n"

        /* GIC initialization */
        "     mov  r0, %[bsp_priority_mask]            \n" /* Set ICC_PMR */
        "     mcr  p15, #0, r0, c4, c6, #0             \n"

        "     mov  r0, %[bsp_enable_group_int]         \n"
        "     mcr  p15, #0, r0, c12, c12, #7           \n" /* Set ICC_IGRPEN1 */

        "     mov  r0, %[bsp_icc_ctlr]                 \n" /* Set ICC_CTLR */
        "     mrc  p15, #0, r1, c12, c12, #4           \n"
        "     orr  r1, r1, r0                          \n"
        "     mcr  p15, #0, r1, c12, c12, #4           \n"
        "     isb                                      \n" /* Ensuring Context-changing */
        ::[bsp_priority_mask] "i" (BSP_PRIORITY_MASK), [bsp_enable_group_int] "i" (BSP_ENABLE_GROUP_INT),
        [bsp_icc_ctlr] "i" (BSP_ICC_CTLR) : "memory");

    /* Jump to bsp_irq_cfg */
    __asm volatile (
        "ldr r0, =bsp_irq_cfg                          \n"
        "blx r0                                        \n"
        );

#if BSP_FEATURE_TFU_SUPPORTED
    __asm volatile (
        "tfu_init:                                     \n"
        "    ldr r0, =bsp_tfu_init                     \n"
        "    blx r0                                    \n" /* Jump to bsp_tfu_init*/
        );
#endif

    /* Jump to main */
    __asm volatile (
        "    ldr r0, =main                             \n"
        "    blx r0                                    \n"
        );
}

/*******************************************************************************************************************//**
 * Core MPU initialization block.
 **********************************************************************************************************************/
BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void bsp_mpu_init (void)
{
    __asm volatile (
        "    mcr  p15, #0, r0, c6, c2, #1              \n" /* Set PRSELR */
        "    dsb                                       \n" /* Ensuring memory access complete */
        "    mcr  p15, #0, r2, c6, c3, #0              \n" /* Write Rt to PRBAR */
        "    dsb                                       \n" /* Ensuring memory access complete */
        "    mcr  p15, #0, r3, c6, c3, #1              \n" /* Write Rt to PRLAR */
        "    dsb                                       \n" /* Ensuring memory access complete */
        "    bx  lr                                    \n"
        ::: "memory");
}

#if defined(__ICCARM__)
void Default_Handler (void)
{
    /** A error has occurred. The user will need to investigate the cause. Common problems are stack corruption
     *  or use of an invalid pointer. Use the Fault Status window in e2 studio or manually check the fault status
     *  registers for more information.
     */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}

#endif

__WEAK BSP_TARGET_ARM BSP_ATTRIBUTE_STACKLESS void IRQ_Handler (void)
{
    __asm volatile (
        "SUB     lr, lr, #4                       \n"
        "SRSDB   sp!, #31                         \n" /* Store LR_irq and SPSR_irq in system mode stack. */
        "CPS     #31                              \n" /* Switch to system mode. */
        "PUSH    {r0-r3, r12}                     \n" /* Store other AAPCS registers. */

#if __FPU_USED
        "VMRS    r0, fpscr                        \n"
        "STMDB   sp!, {r0}                        \n" /* Store fpscr register. */
        "SUB     sp, sp, #4                       \n"
        "VPUSH   {d0-d15}                         \n" /* Store fpu registers. */
        "VPUSH   {d16-d31}                        \n" /* Store fpu registers. */
#endif

        "mrc p15, #0, r3, c12, c12, #2            \n" /* Read HPPIR1 to r3. */
        "mrc p15, #0, r0, c12, c12, #0            \n" /* Read IAR1 to r0. */

        "PUSH    {r0}                             \n" /* Store the INTID. */
        "MOV     r1, sp                           \n" /* Make alignment for stack. */
        "AND     r1, r1, #4                       \n"
        "SUB     sp, sp, r1                       \n"
        "PUSH    {r1, lr}                         \n"

        "LDR     r1,=bsp_common_interrupt_handler \n"
        "BLX     r1                               \n" /* Jump to bsp_common_interrupt_handler, First argument (r0) = ICC_IAR1 read value. */

        "POP     {r1, lr}                         \n"
        "ADD     sp, sp, r1                       \n"
        "POP     {r0}                             \n" /* Restore the INTID to r0. */

        "mcr p15, #0, r0, c12, c12, #1            \n" /* Write INTID to EOIR. */

#if __FPU_USED
        "VPOP    {d16-d31}                        \n" /* Restore fpu registers. */
        "VPOP    {d0-d15}                         \n" /* Restore fpu registers. */
        "ADD     sp, sp, #4                       \n"
        "POP     {r0}                             \n"
        "VMSR    fpscr, r0                        \n" /* Restore fpscr register. */
#endif

        "POP     {r0-r3, r12}                     \n" /* Restore registers. */
        "RFEIA   sp!                              \n" /* Return from system mode tack using RFE. */
        ::: "memory");
}

/** @} (end addtogroup BSP_MCU) */
