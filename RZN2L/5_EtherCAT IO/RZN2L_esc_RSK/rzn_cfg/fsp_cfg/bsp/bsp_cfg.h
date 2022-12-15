/* generated configuration header file - do not edit */
#ifndef BSP_CFG_H_
#define BSP_CFG_H_
#include "bsp_clock_cfg.h"
#include "bsp_mcu_family_cfg.h"
#include "board_cfg.h"
#define FSP_NOT_DEFINED 0
#ifndef BSP_CFG_RTOS
#if (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)
              #define BSP_CFG_RTOS (2)
             #elif (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)
              #define BSP_CFG_RTOS (1)
             #else
#define BSP_CFG_RTOS (0)
#endif
#endif
#undef FSP_NOT_DEFINED
#define BSP_CFG_MCU_VCC_MV (3300)

#define BSP_CFG_PARAM_CHECKING_ENABLE (0)
#define BSP_CFG_ASSERT (0)
#define BSP_CFG_ERROR_LOG (0)

#define BSP_CFG_PORT_PROTECT (1)

#define BSP_CFG_SOFT_RESET_SUPPORTED (0)
#endif /* BSP_CFG_H_ */
