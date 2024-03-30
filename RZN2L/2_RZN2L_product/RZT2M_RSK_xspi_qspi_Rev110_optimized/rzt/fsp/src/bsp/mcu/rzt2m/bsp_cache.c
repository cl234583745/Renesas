/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_RZT2M
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Invalidates instruction and data caches of arm core processor.
 * Use the MRC or MCR instructions to access coprocessor registers. If you access the coprocessor with a C compiler,
 * use inline assembly language.
 **********************************************************************************************************************/
void R_BSP_CacheInvalidate (void)
{
    __asm volatile (

        "    push    {r4, r5, r7, r9, r11}      \n" // ARM calling convention saves these

        /* Ensure memory operations are completed this does cache write back */
        "    dsb                                \n"

        /* Invalidate instruction cache */
        "    mov     r0, #0                     \n"
        "    mcr     p15, 0, r0, c7, c5, 0      \n"

        /* Invalidate data cache. RZ microprocessor only has L1 cache. */
        "    mcr     p15, 2, r0, c0, c0, 0      \n" // Select Data L1 in Cache Size selection register
        "    isb                                \n" // ISB to sync the change to the CacheSizeID reg
        "    mrc     p15, 1, r1, c0, c0, 0      \n" // Read current Cache Size ID register to r1
        "    and     r2, r1, #7                 \n" // Get the line length field in r2
        "    add     r2, r2, #4                 \n" // Add 4 for the line length bit offset (log2 16 bytes)
        "    ldr     r4, =0x3FF                 \n"
        "    and     r4, r4, r1, LSR #3         \n" // r4 is the number of ways-1 (right aligned)
        "    clz     r5, r4                     \n" // r5 is the bit position of the way size increment (CLZ = Count Leading Zeros)
        "    ldr     r7, =0x7FFF                \n"
        "    and     r7, r7, r1, LSR #13        \n" // r7 is the number of sets-1 (right aligned)
        "invalidate_set:                        \n"
        "    mov     r9, r4                     \n" // r9 working copy of number of ways-1 (right aligned)
        "invalidate_way:                        \n"
        "    lsl     r11, r9, r5                \n" // Left shift way number into r11
        "    lsl     r12, r7, r2                \n" // Left shift set number into r12
        "    orr     r11, r11, r12              \n" // Combine set number and way number
        "    mcr     p15, 0, r11, c7, c6, 2     \n" // DCISW - data cache invalidate by set/way

        "    subs    r9, r9, #1                 \n" // Decrement the way number
        "    bge     invalidate_way             \n"

        "    subs    r7, r7, #1                 \n" // Decrement the set number
        "    bge     invalidate_set             \n"

        "    pop     {r4, r5, r7, r9, r11}      \n" // Restore registers needed by C code
        );
}

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_MCU_RZT2M)
 **********************************************************************************************************************/
