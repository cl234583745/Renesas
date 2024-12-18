/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_RESET_MRCTL_BIT_SHIFT_MASK        (0x0000001FU)
#define BSP_RESET_MRCTL_SELECT_MASK           (0x001F0000U)
#define BSP_RESET_MRCTL_REGION_SELECT_MASK    (0x00400000U)

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
    if (BSP_RESET_CR52_0 == cpu)
    {
        /* CR52_0 software reset. */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
        R_SYSC_S->SWRCPU0 = BSP_PRV_RESET_KEY;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
        __WFI();
    }

#if (2 == BSP_FEATURE_BSP_CR52_CORE_NUM)
    else if (BSP_RESET_CR52_1 == cpu)
    {
        /* CR52_1 software reset. */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
        R_SYSC_S->SWRCPU1 = BSP_PRV_RESET_KEY;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
        __WFI();
    }
#endif
    else
    {
        /* Do Nothing. */
    }
}

/*******************************************************************************************************************//**
 * Occur the CPU software reset. When using this function, the CPU reset state is automatically released
 * after an elapsed time corresponding to the setting value in SCKCR2.DIVSELSUB bit.
 *
 * @param[in] cpu to be reset state.
 **********************************************************************************************************************/
void R_BSP_CPUResetAutoRelease (bsp_reset_t cpu)
{
    if (BSP_RESET_CR52_0 == cpu)
    {
        /* CR52_0 software reset. */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
        R_SYSC_S->SWRCPU0 = BSP_PRV_RESET_KEY_AUTO_RELEASE;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
        __WFI();
    }

#if (2 == BSP_FEATURE_BSP_CR52_CORE_NUM)
    else if (BSP_RESET_CR52_1 == cpu)
    {
        /* CR52_1 software reset. */
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
        R_SYSC_S->SWRCPU1 = BSP_PRV_RESET_KEY_AUTO_RELEASE;
        R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
        __WFI();
    }
#endif
    else
    {
        /* Do Nothing. */
    }
}

/*******************************************************************************************************************//**
 * Release the CPU reset state.
 *
 * @param[in] cpu to be release reset state.
 **********************************************************************************************************************/
void R_BSP_CPUResetRelease (bsp_reset_t cpu)
{
    if (BSP_RESET_CR52_0 == cpu)
    {
        /* Release CR52_0 reset state. */
        R_SYSC_S->SWRCPU0 = BSP_PRV_RESET_RELEASE_KEY;
    }

#if (2 == BSP_FEATURE_BSP_CR52_CORE_NUM)
    else if (BSP_RESET_CR52_1 == cpu)
    {
        /* Release CR52_1 reset state. */
        R_SYSC_S->SWRCPU1 = BSP_PRV_RESET_RELEASE_KEY;
    }
#endif
    else
    {
        /* Do Nothing. */
    }
}

/*******************************************************************************************************************//**
 *        Enable module reset state.
 *
 * @param[in] module_to_enable Modules to enable module reset state.
 **********************************************************************************************************************/
void R_BSP_ModuleResetEnable (bsp_module_reset_t module_to_enable)
{
    volatile uint32_t mrctl;
    uint32_t        * p_reg;

    /** When MRCTLn register exists in the safety region,
     *  it is necessary to add an offset of safety region. */
    p_reg = (uint32_t *) &R_SYSC_NS->MRCTLA +
            (((module_to_enable & BSP_RESET_MRCTL_SELECT_MASK) >> 16U) +
             (module_to_enable & BSP_RESET_MRCTL_REGION_SELECT_MASK));
    mrctl = 1U << (module_to_enable & BSP_RESET_MRCTL_BIT_SHIFT_MASK);

    /** Enable module reset state using MRCTLE register. */
    *p_reg |= mrctl;

    /** To ensure processing after module reset. */
    mrctl = *(volatile uint32_t *) (p_reg);
}

/*******************************************************************************************************************//**
 *        Disable module reset state.
 *
 * @param[in] module_to_disable Modules to disable module reset state.
 **********************************************************************************************************************/
void R_BSP_ModuleResetDisable (bsp_module_reset_t module_to_disable)
{
    volatile uint32_t mrctl;
    uint32_t        * p_reg;

    /** When MRCTLn register exists in the safety region,
     *  it is necessary to add an offset of safety region. */
    p_reg = (uint32_t *) &R_SYSC_NS->MRCTLA +
            (((module_to_disable & BSP_RESET_MRCTL_SELECT_MASK) >> 16U) +
             (module_to_disable & BSP_RESET_MRCTL_REGION_SELECT_MASK));
    mrctl = 1U << (module_to_disable & BSP_RESET_MRCTL_BIT_SHIFT_MASK);

    /** Disable module stop state using MRCTLn register. */
    *p_reg &= ~mrctl;

    /** In order to secure processing after release from module reset,
     *  dummy read the same register at least three times.
     *  Refer to "Notes on Module Reset Control Register Operation" of the RZ microprocessor manual. */
    mrctl = *(volatile uint32_t *) (p_reg);
    mrctl = *(volatile uint32_t *) (p_reg);
    mrctl = *(volatile uint32_t *) (p_reg);
}

/** @} (end addtogroup BSP_MCU) */
