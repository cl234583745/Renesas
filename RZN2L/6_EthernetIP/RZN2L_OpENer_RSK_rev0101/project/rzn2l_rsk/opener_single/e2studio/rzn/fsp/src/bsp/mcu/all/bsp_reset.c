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

/** Masks for setting or clearing the MRCTLE register. */
static const uint16_t g_mrctle_masks[] =
{
    0x0001U,                           /* MRCTLE00. */
    0x0002U,                           /* MRCTLE01. */
    0x0004U,                           /* MRCTLE02. */
    0x0008U,                           /* MRCTLE03. */
    0x0010U,                           /* MRCTLE04. */
    0x0020U,                           /* MRCTLE05. */
    0x0040U,                           /* MRCTLE06. */
};

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Occur the system software reset.
 **********************************************************************************************************************/
void R_BSP_SystemReset (void)
{
    /* System software reset. */
    R_SYSC_S->SWRSYS = BSP_PRV_RESET_KEY;
}

/*******************************************************************************************************************//**
 * Occur the CPU software reset.
 *
 * @param[in] cpu to be reset state.
 **********************************************************************************************************************/
void R_BSP_CPUReset (bsp_reset_t cpu)
{
    /* CPU0 software reset. */
    R_SYSC_S->SWRCPU0 = BSP_PRV_RESET_KEY;
    FSP_PARAMETER_NOT_USED(cpu);
}

/*******************************************************************************************************************//**
 * Release the CPU reset state.
 *
 * @param[in] cpu to be release reset state.
 **********************************************************************************************************************/
void R_BSP_CPUResetRelease (bsp_reset_t cpu)
{
    /* Release CPU0 reset state. */
    R_SYSC_S->SWRCPU0 = BSP_PRV_RESET_RELEASE_KEY;
    FSP_PARAMETER_NOT_USED(cpu);
}

/*******************************************************************************************************************//**
 *        Enable module reset state.
 *
 * @param[in] module_to_enable Modules to enable module reset state.
 **********************************************************************************************************************/
void R_BSP_ModuleResetEnable (bsp_module_reset_t module_to_enable)
{
    /** Enable module reset state using MRCTLE register. */
    R_SYSC_NS->MRCTLE = (R_SYSC_NS->MRCTLE | g_mrctle_masks[module_to_enable]);
}

/*******************************************************************************************************************//**
 *        Disable module reset state.
 *
 * @param[in] module_to_disable Modules to disable module reset state.
 **********************************************************************************************************************/
void R_BSP_ModuleResetDisable (bsp_module_reset_t module_to_disable)
{
    volatile uint32_t dummy;

    /** Disable module stop state using MRCTLE register. */
    R_SYSC_NS->MRCTLE = (R_SYSC_NS->MRCTLE & (uint16_t) (~g_mrctle_masks[module_to_disable]));

    /** In order to secure processing after release from module reset,
     *  dummy read the same register at least three times.
     *  Refer to "Notes on Module Reset Control Register Operation" of the RZ microprocessor manual. */
    dummy = R_SYSC_NS->MRCTLE;
    dummy = R_SYSC_NS->MRCTLE;
    dummy = R_SYSC_NS->MRCTLE;

    FSP_PARAMETER_NOT_USED(dummy);
}

/** @} (end addtogroup BSP_MCU) */
