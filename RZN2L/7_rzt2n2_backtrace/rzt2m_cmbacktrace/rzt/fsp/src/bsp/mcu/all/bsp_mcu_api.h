/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_MCU_API_H
#define BSP_MCU_API_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef struct st_bsp_event_info
{
    IRQn_Type   irq;
    elc_event_t event;
} bsp_event_info_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

void R_BSP_RegisterProtectEnable(bsp_reg_protect_t regs_to_protect);
void R_BSP_RegisterProtectDisable(bsp_reg_protect_t regs_to_unprotect);
void R_BSP_SoftwareDelay(uint32_t delay, bsp_delay_units_t units);
void R_BSP_SystemReset(void);
void R_BSP_CPUReset(bsp_reset_t cpu);
void R_BSP_CPUResetAutoRelease(bsp_reset_t cpu);
void R_BSP_CPUResetRelease(bsp_reset_t cpu);
void R_BSP_ModuleResetEnable(bsp_module_reset_t module_to_enable);
void R_BSP_ModuleResetDisable(bsp_module_reset_t module_to_disable);

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
