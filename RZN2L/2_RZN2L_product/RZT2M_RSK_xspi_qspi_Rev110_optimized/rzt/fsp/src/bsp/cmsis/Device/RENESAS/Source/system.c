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
#include <string.h>
#include "bsp_api.h"

#include "../../../../mcu/all/bsp_clocks.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if (0 == BSP_CFG_CPU)
 #define BSP_PRV_M_MPU_REGION_NUM    (7)

 #if defined(BSP_M_MPU0_SUPPORT)
  #define BSP_PRV_M_MPU0_STADD0      (BSP_CFG_MPU0_STADD0 | (BSP_CFG_MPU0_WRITE0 << 1) | BSP_CFG_MPU0_READ0)
  #define BSP_PRV_M_MPU0_STADD1      (BSP_CFG_MPU0_STADD1 | (BSP_CFG_MPU0_WRITE1 << 1) | BSP_CFG_MPU0_READ1)
  #define BSP_PRV_M_MPU0_STADD2      (BSP_CFG_MPU0_STADD2 | (BSP_CFG_MPU0_WRITE2 << 1) | BSP_CFG_MPU0_READ2)
  #define BSP_PRV_M_MPU0_STADD3      (BSP_CFG_MPU0_STADD3 | (BSP_CFG_MPU0_WRITE3 << 1) | BSP_CFG_MPU0_READ3)
  #define BSP_PRV_M_MPU0_STADD4      (BSP_CFG_MPU0_STADD4 | (BSP_CFG_MPU0_WRITE4 << 1) | BSP_CFG_MPU0_READ4)
  #define BSP_PRV_M_MPU0_STADD5      (BSP_CFG_MPU0_STADD5 | (BSP_CFG_MPU0_WRITE5 << 1) | BSP_CFG_MPU0_READ5)
  #define BSP_PRV_M_MPU0_STADD6      (BSP_CFG_MPU0_STADD6 | (BSP_CFG_MPU0_WRITE6 << 1) | BSP_CFG_MPU0_READ6)
  #define BSP_PRV_M_MPU0_STADD7      (BSP_CFG_MPU0_STADD7 | (BSP_CFG_MPU0_WRITE7 << 1) | BSP_CFG_MPU0_READ7)
 #endif
 #if defined(BSP_M_MPU1_SUPPORT)
  #define BSP_PRV_M_MPU1_STADD0      (BSP_CFG_MPU1_STADD0 | (BSP_CFG_MPU1_WRITE0 << 1) | BSP_CFG_MPU1_READ0)
  #define BSP_PRV_M_MPU1_STADD1      (BSP_CFG_MPU1_STADD1 | (BSP_CFG_MPU1_WRITE1 << 1) | BSP_CFG_MPU1_READ1)
  #define BSP_PRV_M_MPU1_STADD2      (BSP_CFG_MPU1_STADD2 | (BSP_CFG_MPU1_WRITE2 << 1) | BSP_CFG_MPU1_READ2)
  #define BSP_PRV_M_MPU1_STADD3      (BSP_CFG_MPU1_STADD3 | (BSP_CFG_MPU1_WRITE3 << 1) | BSP_CFG_MPU1_READ3)
  #define BSP_PRV_M_MPU1_STADD4      (BSP_CFG_MPU1_STADD4 | (BSP_CFG_MPU1_WRITE4 << 1) | BSP_CFG_MPU1_READ4)
  #define BSP_PRV_M_MPU1_STADD5      (BSP_CFG_MPU1_STADD5 | (BSP_CFG_MPU1_WRITE5 << 1) | BSP_CFG_MPU1_READ5)
  #define BSP_PRV_M_MPU1_STADD6      (BSP_CFG_MPU1_STADD6 | (BSP_CFG_MPU1_WRITE6 << 1) | BSP_CFG_MPU1_READ6)
  #define BSP_PRV_M_MPU1_STADD7      (BSP_CFG_MPU1_STADD7 | (BSP_CFG_MPU1_WRITE7 << 1) | BSP_CFG_MPU1_READ7)
 #endif
 #if defined(BSP_M_MPU2_SUPPORT)
  #define BSP_PRV_M_MPU2_STADD0      (BSP_CFG_MPU2_STADD0 | (BSP_CFG_MPU2_WRITE0 << 1) | BSP_CFG_MPU2_READ0)
  #define BSP_PRV_M_MPU2_STADD1      (BSP_CFG_MPU2_STADD1 | (BSP_CFG_MPU2_WRITE1 << 1) | BSP_CFG_MPU2_READ1)
  #define BSP_PRV_M_MPU2_STADD2      (BSP_CFG_MPU2_STADD2 | (BSP_CFG_MPU2_WRITE2 << 1) | BSP_CFG_MPU2_READ2)
  #define BSP_PRV_M_MPU2_STADD3      (BSP_CFG_MPU2_STADD3 | (BSP_CFG_MPU2_WRITE3 << 1) | BSP_CFG_MPU2_READ3)
  #define BSP_PRV_M_MPU2_STADD4      (BSP_CFG_MPU2_STADD4 | (BSP_CFG_MPU2_WRITE4 << 1) | BSP_CFG_MPU2_READ4)
  #define BSP_PRV_M_MPU2_STADD5      (BSP_CFG_MPU2_STADD5 | (BSP_CFG_MPU2_WRITE5 << 1) | BSP_CFG_MPU2_READ5)
  #define BSP_PRV_M_MPU2_STADD6      (BSP_CFG_MPU2_STADD6 | (BSP_CFG_MPU2_WRITE6 << 1) | BSP_CFG_MPU2_READ6)
  #define BSP_PRV_M_MPU2_STADD7      (BSP_CFG_MPU2_STADD7 | (BSP_CFG_MPU2_WRITE7 << 1) | BSP_CFG_MPU2_READ7)
 #endif
 #if defined(BSP_M_MPU3_SUPPORT)
  #define BSP_PRV_M_MPU3_STADD0      (BSP_CFG_MPU3_STADD0 | (BSP_CFG_MPU3_WRITE0 << 1) | BSP_CFG_MPU3_READ0)
  #define BSP_PRV_M_MPU3_STADD1      (BSP_CFG_MPU3_STADD1 | (BSP_CFG_MPU3_WRITE1 << 1) | BSP_CFG_MPU3_READ1)
  #define BSP_PRV_M_MPU3_STADD2      (BSP_CFG_MPU3_STADD2 | (BSP_CFG_MPU3_WRITE2 << 1) | BSP_CFG_MPU3_READ2)
  #define BSP_PRV_M_MPU3_STADD3      (BSP_CFG_MPU3_STADD3 | (BSP_CFG_MPU3_WRITE3 << 1) | BSP_CFG_MPU3_READ3)
  #define BSP_PRV_M_MPU3_STADD4      (BSP_CFG_MPU3_STADD4 | (BSP_CFG_MPU3_WRITE4 << 1) | BSP_CFG_MPU3_READ4)
  #define BSP_PRV_M_MPU3_STADD5      (BSP_CFG_MPU3_STADD5 | (BSP_CFG_MPU3_WRITE5 << 1) | BSP_CFG_MPU3_READ5)
  #define BSP_PRV_M_MPU3_STADD6      (BSP_CFG_MPU3_STADD6 | (BSP_CFG_MPU3_WRITE6 << 1) | BSP_CFG_MPU3_READ6)
  #define BSP_PRV_M_MPU3_STADD7      (BSP_CFG_MPU3_STADD7 | (BSP_CFG_MPU3_WRITE7 << 1) | BSP_CFG_MPU3_READ7)
 #endif
 #if defined(BSP_M_MPU4_SUPPORT)
  #define BSP_PRV_M_MPU4_STADD0      (BSP_CFG_MPU4_STADD0 | (BSP_CFG_MPU4_WRITE0 << 1) | BSP_CFG_MPU4_READ0)
  #define BSP_PRV_M_MPU4_STADD1      (BSP_CFG_MPU4_STADD1 | (BSP_CFG_MPU4_WRITE1 << 1) | BSP_CFG_MPU4_READ1)
  #define BSP_PRV_M_MPU4_STADD2      (BSP_CFG_MPU4_STADD2 | (BSP_CFG_MPU4_WRITE2 << 1) | BSP_CFG_MPU4_READ2)
  #define BSP_PRV_M_MPU4_STADD3      (BSP_CFG_MPU4_STADD3 | (BSP_CFG_MPU4_WRITE3 << 1) | BSP_CFG_MPU4_READ3)
  #define BSP_PRV_M_MPU4_STADD4      (BSP_CFG_MPU4_STADD4 | (BSP_CFG_MPU4_WRITE4 << 1) | BSP_CFG_MPU4_READ4)
  #define BSP_PRV_M_MPU4_STADD5      (BSP_CFG_MPU4_STADD5 | (BSP_CFG_MPU4_WRITE5 << 1) | BSP_CFG_MPU4_READ5)
  #define BSP_PRV_M_MPU4_STADD6      (BSP_CFG_MPU4_STADD6 | (BSP_CFG_MPU4_WRITE6 << 1) | BSP_CFG_MPU4_READ6)
  #define BSP_PRV_M_MPU4_STADD7      (BSP_CFG_MPU4_STADD7 | (BSP_CFG_MPU4_WRITE7 << 1) | BSP_CFG_MPU4_READ7)
 #endif
 #if defined(BSP_M_MPU6_SUPPORT)
  #define BSP_PRV_M_MPU6_STADD0      (BSP_CFG_MPU6_STADD0 | (BSP_CFG_MPU6_WRITE0 << 1) | BSP_CFG_MPU6_READ0)
  #define BSP_PRV_M_MPU6_STADD1      (BSP_CFG_MPU6_STADD1 | (BSP_CFG_MPU6_WRITE1 << 1) | BSP_CFG_MPU6_READ1)
  #define BSP_PRV_M_MPU6_STADD2      (BSP_CFG_MPU6_STADD2 | (BSP_CFG_MPU6_WRITE2 << 1) | BSP_CFG_MPU6_READ2)
  #define BSP_PRV_M_MPU6_STADD3      (BSP_CFG_MPU6_STADD3 | (BSP_CFG_MPU6_WRITE3 << 1) | BSP_CFG_MPU6_READ3)
  #define BSP_PRV_M_MPU6_STADD4      (BSP_CFG_MPU6_STADD4 | (BSP_CFG_MPU6_WRITE4 << 1) | BSP_CFG_MPU6_READ4)
  #define BSP_PRV_M_MPU6_STADD5      (BSP_CFG_MPU6_STADD5 | (BSP_CFG_MPU6_WRITE5 << 1) | BSP_CFG_MPU6_READ5)
  #define BSP_PRV_M_MPU6_STADD6      (BSP_CFG_MPU6_STADD6 | (BSP_CFG_MPU6_WRITE6 << 1) | BSP_CFG_MPU6_READ6)
  #define BSP_PRV_M_MPU6_STADD7      (BSP_CFG_MPU6_STADD7 | (BSP_CFG_MPU6_WRITE7 << 1) | BSP_CFG_MPU6_READ7)
 #endif
 #if defined(BSP_M_MPU7_SUPPORT)
  #define BSP_PRV_M_MPU7_STADD0      (BSP_CFG_MPU7_STADD0 | (BSP_CFG_MPU7_WRITE0 << 1) | BSP_CFG_MPU7_READ0)
  #define BSP_PRV_M_MPU7_STADD1      (BSP_CFG_MPU7_STADD1 | (BSP_CFG_MPU7_WRITE1 << 1) | BSP_CFG_MPU7_READ1)
  #define BSP_PRV_M_MPU7_STADD2      (BSP_CFG_MPU7_STADD2 | (BSP_CFG_MPU7_WRITE2 << 1) | BSP_CFG_MPU7_READ2)
  #define BSP_PRV_M_MPU7_STADD3      (BSP_CFG_MPU7_STADD3 | (BSP_CFG_MPU7_WRITE3 << 1) | BSP_CFG_MPU7_READ3)
  #define BSP_PRV_M_MPU7_STADD4      (BSP_CFG_MPU7_STADD4 | (BSP_CFG_MPU7_WRITE4 << 1) | BSP_CFG_MPU7_READ4)
  #define BSP_PRV_M_MPU7_STADD5      (BSP_CFG_MPU7_STADD5 | (BSP_CFG_MPU7_WRITE5 << 1) | BSP_CFG_MPU7_READ5)
  #define BSP_PRV_M_MPU7_STADD6      (BSP_CFG_MPU7_STADD6 | (BSP_CFG_MPU7_WRITE6 << 1) | BSP_CFG_MPU7_READ6)
  #define BSP_PRV_M_MPU7_STADD7      (BSP_CFG_MPU7_STADD7 | (BSP_CFG_MPU7_WRITE7 << 1) | BSP_CFG_MPU7_READ7)
 #endif
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/** System Clock Frequency (Core Clock) */
uint32_t SystemCoreClock = 0U;

/** @} (end addtogroup BSP_MCU) */

#if defined(__ICCARM__)

 #pragma section="VECTOR_RBLOCK"
 #pragma section="VECTOR_WBLOCK"
 #pragma section="USER_PRG_RBLOCK"
 #pragma section="USER_PRG_WBLOCK"
 #pragma section="USER_DATA_RBLOCK"
 #pragma section="USER_DATA_WBLOCK"
 #pragma section="USER_DATA_ZBLOCK"
 #pragma section="NONCACHE_BUFFER_ZBLOCK"
 #pragma section="SHARED_NONCACHE_BUFFER_ZBLOCK"

#endif                                 // __ICCARM__

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

#if defined(__ICCARM__)

void R_BSP_WarmStart(bsp_warm_start_event_t event);

 #pragma weak R_BSP_WarmStart

#elif defined(__GNUC__) || defined(__ARMCC_VERSION)

void R_BSP_WarmStart(bsp_warm_start_event_t event) __attribute__((weak));

#endif

void bsp_m_mpu_init(void);

#if !(BSP_CFG_RAM_EXECUTION)
void bsp_copy_to_atcm(void);

 #if defined(__ICCARM__)
void bsp_copy_4byte(uint32_t * src, uint32_t * dst, uint32_t bytesize);

 #endif
void bsp_application_bss_init(void);

#endif

#if (0 == BSP_CFG_CPU)
void bsp_global_system_counter_init(void);

 #if BSP_FEATURE_TFU_SUPPORTED
void bsp_tfu_init(void);

 #endif
#endif

#if !BSP_CFG_PORT_PROTECT
void bsp_release_port_protect(void);

#endif

#if (0 == BSP_CFG_CPU)
 #if defined(BSP_M_MPU0_SUPPORT)
uint32_t m_mpu0_stadd_cfg[8] =
{
    BSP_PRV_M_MPU0_STADD0, BSP_PRV_M_MPU0_STADD1, BSP_PRV_M_MPU0_STADD2,
    BSP_PRV_M_MPU0_STADD3, BSP_PRV_M_MPU0_STADD4, BSP_PRV_M_MPU0_STADD5,
    BSP_PRV_M_MPU0_STADD6, BSP_PRV_M_MPU0_STADD7
};
 #endif
 #if defined(BSP_M_MPU1_SUPPORT)
uint32_t m_mpu1_stadd_cfg[8] =
{
    BSP_PRV_M_MPU1_STADD0, BSP_PRV_M_MPU1_STADD1, BSP_PRV_M_MPU1_STADD2,
    BSP_PRV_M_MPU1_STADD3, BSP_PRV_M_MPU1_STADD4, BSP_PRV_M_MPU1_STADD5,
    BSP_PRV_M_MPU1_STADD6, BSP_PRV_M_MPU1_STADD7
};
 #endif
 #if defined(BSP_M_MPU2_SUPPORT)
uint32_t m_mpu2_stadd_cfg[8] =
{
    BSP_PRV_M_MPU2_STADD0, BSP_PRV_M_MPU2_STADD1, BSP_PRV_M_MPU2_STADD2,
    BSP_PRV_M_MPU2_STADD3, BSP_PRV_M_MPU2_STADD4, BSP_PRV_M_MPU2_STADD5,
    BSP_PRV_M_MPU2_STADD6, BSP_PRV_M_MPU2_STADD7
};
 #endif
 #if defined(BSP_M_MPU3_SUPPORT)
uint32_t m_mpu3_stadd_cfg[8] =
{
    BSP_PRV_M_MPU3_STADD0, BSP_PRV_M_MPU3_STADD1, BSP_PRV_M_MPU3_STADD2,
    BSP_PRV_M_MPU3_STADD3, BSP_PRV_M_MPU3_STADD4, BSP_PRV_M_MPU3_STADD5,
    BSP_PRV_M_MPU3_STADD6, BSP_PRV_M_MPU3_STADD7
};
 #endif
 #if defined(BSP_M_MPU4_SUPPORT)
uint32_t m_mpu4_stadd_cfg[8] =
{
    BSP_PRV_M_MPU4_STADD0, BSP_PRV_M_MPU4_STADD1, BSP_PRV_M_MPU4_STADD2,
    BSP_PRV_M_MPU4_STADD3, BSP_PRV_M_MPU4_STADD4, BSP_PRV_M_MPU4_STADD5,
    BSP_PRV_M_MPU4_STADD6, BSP_PRV_M_MPU4_STADD7
};
 #endif
 #if defined(BSP_M_MPU6_SUPPORT)
uint32_t m_mpu6_stadd_cfg[8] =
{
    BSP_PRV_M_MPU6_STADD0, BSP_PRV_M_MPU6_STADD1, BSP_PRV_M_MPU6_STADD2,
    BSP_PRV_M_MPU6_STADD3, BSP_PRV_M_MPU6_STADD4, BSP_PRV_M_MPU6_STADD5,
    BSP_PRV_M_MPU6_STADD6, BSP_PRV_M_MPU6_STADD7
};
 #endif
 #if defined(BSP_M_MPU7_SUPPORT)
uint32_t m_mpu7_stadd_cfg[8] =
{
    BSP_PRV_M_MPU7_STADD0, BSP_PRV_M_MPU7_STADD1, BSP_PRV_M_MPU7_STADD2,
    BSP_PRV_M_MPU7_STADD3, BSP_PRV_M_MPU7_STADD4, BSP_PRV_M_MPU7_STADD5,
    BSP_PRV_M_MPU7_STADD6, BSP_PRV_M_MPU7_STADD7
};
 #endif
 #if defined(BSP_M_MPU0_SUPPORT)
uint32_t m_mpu0_endadd_cfg[8] =
{
    BSP_CFG_MPU0_ENDADD0, BSP_CFG_MPU0_ENDADD1, BSP_CFG_MPU0_ENDADD2,
    BSP_CFG_MPU0_ENDADD3, BSP_CFG_MPU0_ENDADD4, BSP_CFG_MPU0_ENDADD5,
    BSP_CFG_MPU0_ENDADD6, BSP_CFG_MPU0_ENDADD7
};
 #endif
 #if defined(BSP_M_MPU1_SUPPORT)
uint32_t m_mpu1_endadd_cfg[8] =
{
    BSP_CFG_MPU1_ENDADD0, BSP_CFG_MPU1_ENDADD1, BSP_CFG_MPU1_ENDADD2,
    BSP_CFG_MPU1_ENDADD3, BSP_CFG_MPU1_ENDADD4, BSP_CFG_MPU1_ENDADD5,
    BSP_CFG_MPU1_ENDADD6, BSP_CFG_MPU1_ENDADD7
};
 #endif
 #if defined(BSP_M_MPU2_SUPPORT)
uint32_t m_mpu2_endadd_cfg[8] =
{
    BSP_CFG_MPU2_ENDADD0, BSP_CFG_MPU2_ENDADD1, BSP_CFG_MPU2_ENDADD2,
    BSP_CFG_MPU2_ENDADD3, BSP_CFG_MPU2_ENDADD4, BSP_CFG_MPU2_ENDADD5,
    BSP_CFG_MPU2_ENDADD6, BSP_CFG_MPU2_ENDADD7
};
 #endif
 #if defined(BSP_M_MPU3_SUPPORT)
uint32_t m_mpu3_endadd_cfg[8] =
{
    BSP_CFG_MPU3_ENDADD0, BSP_CFG_MPU3_ENDADD1, BSP_CFG_MPU3_ENDADD2,
    BSP_CFG_MPU3_ENDADD3, BSP_CFG_MPU3_ENDADD4, BSP_CFG_MPU3_ENDADD5,
    BSP_CFG_MPU3_ENDADD6, BSP_CFG_MPU3_ENDADD7
};
 #endif
 #if defined(BSP_M_MPU4_SUPPORT)
uint32_t m_mpu4_endadd_cfg[8] =
{
    BSP_CFG_MPU4_ENDADD0, BSP_CFG_MPU4_ENDADD1, BSP_CFG_MPU4_ENDADD2,
    BSP_CFG_MPU4_ENDADD3, BSP_CFG_MPU4_ENDADD4, BSP_CFG_MPU4_ENDADD5,
    BSP_CFG_MPU4_ENDADD6, BSP_CFG_MPU4_ENDADD7
};
 #endif
 #if defined(BSP_M_MPU6_SUPPORT)
uint32_t m_mpu6_endadd_cfg[8] =
{
    BSP_CFG_MPU6_ENDADD0, BSP_CFG_MPU6_ENDADD1, BSP_CFG_MPU6_ENDADD2,
    BSP_CFG_MPU6_ENDADD3, BSP_CFG_MPU6_ENDADD4, BSP_CFG_MPU6_ENDADD5,
    BSP_CFG_MPU6_ENDADD6, BSP_CFG_MPU6_ENDADD7
};
 #endif
 #if defined(BSP_M_MPU7_SUPPORT)
uint32_t m_mpu7_endadd_cfg[8] =
{
    BSP_CFG_MPU7_ENDADD0, BSP_CFG_MPU7_ENDADD1, BSP_CFG_MPU7_ENDADD2,
    BSP_CFG_MPU7_ENDADD3, BSP_CFG_MPU7_ENDADD4, BSP_CFG_MPU7_ENDADD5,
    BSP_CFG_MPU7_ENDADD6, BSP_CFG_MPU7_ENDADD7
};
 #endif
#endif

/* Initialize static constructors */
#if BSP_CFG_C_RUNTIME_INIT
void bsp_static_constructor_init(void);

 #if defined(__GNUC__)

extern void (* __preinit_array_start[])(void);
extern void (* __preinit_array_end[])(void);
extern void (* __init_array_start[])(void);
extern void (* __init_array_end[])(void);
 #elif defined(__ICCARM__)
extern void __call_ctors(void const *, void const *);

  #pragma section = "SHT$$PREINIT_ARRAY" const
  #pragma section = "SHT$$INIT_ARRAY" const
 #endif
#endif

#if (0 == BSP_CFG_CPU)

/*******************************************************************************************************************//**
 * Initialize the Master-MPU settings.
 **********************************************************************************************************************/
void bsp_m_mpu_init (void)
{
    /* Disable register protection for Master-MPU related registers. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_SYSTEM);

    for (uint8_t region_num = 0; region_num <= BSP_PRV_M_MPU_REGION_NUM; region_num++)
    {
 #if defined(BSP_M_MPU0_SUPPORT)
        R_MPU0->RGN[region_num].STADD  = m_mpu0_stadd_cfg[region_num];
        R_MPU0->RGN[region_num].ENDADD = m_mpu0_endadd_cfg[region_num];
 #endif
 #if defined(BSP_M_MPU1_SUPPORT)
        R_MPU1->RGN[region_num].STADD  = m_mpu1_stadd_cfg[region_num];
        R_MPU1->RGN[region_num].ENDADD = m_mpu1_endadd_cfg[region_num];
 #endif
 #if defined(BSP_M_MPU2_SUPPORT)
        R_MPU2->RGN[region_num].STADD  = m_mpu2_stadd_cfg[region_num];
        R_MPU2->RGN[region_num].ENDADD = m_mpu2_endadd_cfg[region_num];
 #endif
 #if defined(BSP_M_MPU3_SUPPORT)
        R_MPU3->RGN[region_num].STADD  = m_mpu3_stadd_cfg[region_num];
        R_MPU3->RGN[region_num].ENDADD = m_mpu3_endadd_cfg[region_num];
 #endif
 #if defined(BSP_M_MPU4_SUPPORT)
        R_MPU4->RGN[region_num].STADD  = m_mpu4_stadd_cfg[region_num];
        R_MPU4->RGN[region_num].ENDADD = m_mpu4_endadd_cfg[region_num];
 #endif
 #if defined(BSP_M_MPU6_SUPPORT)
        R_MPU6->RGN[region_num].STADD  = m_mpu6_stadd_cfg[region_num];
        R_MPU6->RGN[region_num].ENDADD = m_mpu6_endadd_cfg[region_num];
 #endif
 #if defined(BSP_M_MPU7_SUPPORT)
        R_MPU7->RGN[region_num].STADD  = m_mpu7_stadd_cfg[region_num];
        R_MPU7->RGN[region_num].ENDADD = m_mpu7_endadd_cfg[region_num];
 #endif
    }

    /* Enable register protection for Master-MPU related registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_SYSTEM);
}

/*******************************************************************************************************************//**
 * Initialize global system counter. The counter is enabled and is incrementing.
 **********************************************************************************************************************/
void bsp_global_system_counter_init (void)
{
    /* Initialize registers related the global system counter. */
    R_GSC->CNTCR  &= ~R_GSC_CNTCR_EN_Msk;
    R_GSC->CNTFID0 = BSP_GLOBAL_SYSTEM_COUNTER_CLOCK_HZ;
    R_GSC->CNTCVL  = 0;
    R_GSC->CNTCVU  = 0;
    R_GSC->CNTCR  |= R_GSC_CNTCR_EN_Msk;
}

#endif

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.
 * This function is declared as a weak symbol higher up in this file because it is meant to be overridden by a user
 * implemented version. One of the main uses for this function is to call functional safety code during the startup
 * process. To use this function just copy this function into your own code and modify it to meet your needs.
 *
 * @param[in]  event    Where the code currently is in the start up process
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
        /* C runtime environment has not been setup so you cannot use globals. System clocks are not setup. */
    }

    if (BSP_WARM_START_POST_CLOCK == event)
    {
        /* C runtime environment has not been setup so you cannot use globals. Clocks have been initialized. */
    }
    else if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment, system clocks, and pins are all setup. */
    }
    else
    {
        /* Do nothing */
    }
}

/** @} (end addtogroup BSP_MCU) */

#if !(BSP_CFG_RAM_EXECUTION)

/*******************************************************************************************************************//**
 * Copy the application program from external Flash to ATCM.
 **********************************************************************************************************************/
void bsp_copy_to_atcm (void)
{
 #if defined(__ICCARM__)

    /* Define destination/source address pointer and block size */
    uint32_t * src;
    uint32_t * dst;
    uint32_t   size;

    /* Copy exception vector block */

    /* Casting the pointer and address to a (uint32_t *) and (uint32_t) are valid
     * because these will reference 32 bit address and address size */
    src  = (uint32_t *) __section_begin("VECTOR_RBLOCK");
    dst  = (uint32_t *) __section_begin("VECTOR_WBLOCK");
    size = (uint32_t) __section_size("VECTOR_WBLOCK");
    bsp_copy_4byte(src, dst, size);

    /* Copy user program block */

    /* Casting the pointer and address to a (uint32_t *) and (uint32_t) are valid
     * because these will reference 32 bit address and address size */
    src  = (uint32_t *) __section_begin("USER_PRG_RBLOCK");
    dst  = (uint32_t *) __section_begin("USER_PRG_WBLOCK");
    size = (uint32_t) __section_size("USER_PRG_WBLOCK");
    bsp_copy_4byte(src, dst, size);

    /* Copy user data block */

    /* Casting the pointer and address to a (uint32_t *) and (uint32_t) are valid
     * because these will reference 32 bit address and address size */
    src  = (uint32_t *) __section_begin("USER_DATA_RBLOCK");
    dst  = (uint32_t *) __section_begin("USER_DATA_WBLOCK");
    size = (uint32_t) __section_size("USER_DATA_WBLOCK");
    bsp_copy_4byte(src, dst, size);
 #elif defined(__GNUC__)
    __asm volatile (
        "copy_to_atcm:                             \n"

        /* Variables has no initialized value already be initialized to zero */
        /* in boot sequence(Clear ATCM and BTCM). */
        "    ldr  r0, =_mfvector                   \n"
        "    ldr  r1, =_fvector_start              \n"
        "    ldr  r2, =_fvector_end                \n"
        "    cmp  r2, r1                           \n"
        "    beq  copy_FVECTOR_end                 \n"

        "copy_to_FVECTOR:                          \n"
        "    ldrb  r3, [r0], #0                    \n"
        "    strb  r3, [r1], #0                    \n"
        "    add   r0, r0, #1                      \n"
        "    add   r1, r1, #1                      \n"
        "    cmp   r2, r1                          \n"
        "    bne   copy_to_FVECTOR                 \n"
        "copy_FVECTOR_end:                         \n"
        "    dsb                                   \n" /* Ensuring data-changing */

        "    ldr  r0, =_mtext                      \n"
        "    ldr  r1, =_text_start                 \n"
        "    ldr  r2, =_text_end                   \n"
        "    cmp  r2, r1                           \n"
        "    beq  copy_APP_TEXT_end                \n"

        "copy_to_APP_TEXT:                         \n"
        "    ldrb  r3, [r0], #0                    \n"
        "    strb  r3, [r1], #0                    \n"
        "    add   r0, r0, #1                      \n"
        "    add   r1, r1, #1                      \n"
        "    cmp   r2, r1                          \n"
        "    bne   copy_to_APP_TEXT                \n"
        "copy_APP_TEXT_end:                        \n"
        "    dsb                                   \n" /* Ensuring data-changing */

        "    ldr  r0, =_mdata                      \n"
        "    ldr  r1, =_data_start                 \n"
        "    ldr  r2, =_data_end                   \n"
        "    cmp  r2, r1                           \n"
        "    beq  copy_APP_DATA_end                \n"

        "copy_to_APP_DATA:                         \n"
        "    ldrb  r3, [r0], #0                    \n"
        "    strb  r3, [r1], #0                    \n"
        "    add   r0, r0, #1                      \n"
        "    add   r1, r1, #1                      \n"
        "    cmp   r2, r1                          \n"
        "    bne   copy_to_APP_DATA                \n"
        "copy_APP_DATA_end:                        \n"
        "    dsb                                   \n" /* Ensuring data-changing */
        );
 #endif
}

 #if defined(__ICCARM__)

/*******************************************************************************************************************//**
 * Copy the memory block from Source address to Destination address by the 4byte unit.
 **********************************************************************************************************************/
void bsp_copy_4byte (uint32_t * src, uint32_t * dst, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;

    /* Copy Count in 4Byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *dst++ = *src++;
    }

    /* Ensuring data-changing */
    asm volatile ("dsb");
}

 #endif

/*******************************************************************************************************************//**
 * Clear LDR_DATA_ZBLOCK section in ATCM.
 **********************************************************************************************************************/
void bsp_application_bss_init (void)
{
 #if defined(__ICCARM__)
    uint8_t   zero = 0;
    uint8_t * src;
    uint8_t * dst;

    /* Clear BSS. */
    dst = (uint8_t *) __section_end("USER_DATA_ZBLOCK");

    for (src = (uint8_t *) __section_begin("USER_DATA_ZBLOCK"); src < dst; src++)
    {
        *src = zero;
    }

    /* Ensuring Context-changing */
    asm volatile ("dsb");

    /* Clear shared non-cache buffer. */
    dst = (uint8_t *) __section_end("SHARED_NONCACHE_BUFFER_ZBLOCK");

    for (src = (uint8_t *) __section_begin("SHARED_NONCACHE_BUFFER_ZBLOCK"); src < dst; src++)
    {
        *src = zero;
    }

    /* Ensuring Context-changing */
    asm volatile ("dsb");

    /* Clear non-cache buffer. */
    dst = (uint8_t *) __section_end("NONCACHE_BUFFER_ZBLOCK");

    for (src = (uint8_t *) __section_begin("NONCACHE_BUFFER_ZBLOCK"); src < dst; src++)
    {
        *src = zero;
    }

    /* Ensuring Context-changing */
    asm volatile ("dsb");
 #elif defined(__GNUC__)
    __asm volatile (

        /* Clear BSS. */
        "    ldr  r0, =0x00000000                            \n"
        "    ldr  r1, =__bss_start__                         \n"
        "    ldr  r2, =__bss_end__                           \n"
        "    cmp  r2, r1                                     \n"
        "    beq  bss_init_end                               \n"
        "clear_bss:                                          \n"
        "    strb  r0, [r1], #0                              \n"
        "    add   r1, r1, #1                                \n"
        "    cmp   r2, r1                                    \n"
        "    bne   clear_bss                                 \n"
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

        );
 #endif
}

#endif

#if (0 == BSP_CFG_CPU)
 #if BSP_FEATURE_TFU_SUPPORTED
void bsp_tfu_init (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_TFU, 0U);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
}

 #endif
#endif

#if !BSP_CFG_PORT_PROTECT
void bsp_release_port_protect (void)
{
    /** When writing to the PRCR register the upper 8-bits must be the correct key. Set lower bits to 0 to
     * disable writes. */
    R_RWP_NS->PRCRN = ((R_RWP_NS->PRCRN | BSP_IO_PRV_PRCR_KEY) | BSP_IO_REG_PROTECT_GPIO);
    R_RWP_S->PRCRS  = ((R_RWP_S->PRCRS | BSP_IO_PRV_PRCR_KEY) | BSP_IO_REG_PROTECT_GPIO);
}

#endif

/*******************************************************************************************************************//**
 * Initialize static constructors.
 **********************************************************************************************************************/
#if BSP_CFG_C_RUNTIME_INIT
void bsp_static_constructor_init (void)
{
 #if defined(__ICCARM__)
    void const * pibase = __section_begin("SHT$$PREINIT_ARRAY");
    void const * ilimit = __section_end("SHT$$INIT_ARRAY");
    __call_ctors(pibase, ilimit);
 #elif defined(__GNUC__)
    int32_t count;
    int32_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
    {
        __preinit_array_start[i]();
    }

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
    {
        __init_array_start[i]();
    }
 #endif
}

#endif

/* This vector table is for SGI and PPI interrupts implemented in Cortex-R52. */
BSP_DONT_REMOVE fsp_vector_t g_sgi_ppi_vector_table[BSP_CORTEX_VECTOR_TABLE_ENTRIES] =
{
    NULL,                              /* INTID0    : SOFTWARE_GENERATE_INT0                   */
    NULL,                              /* INTID1    : SOFTWARE_GENERATE_INT1                   */
    NULL,                              /* INTID2    : SOFTWARE_GENERATE_INT2                   */
    NULL,                              /* INTID3    : SOFTWARE_GENERATE_INT3                   */
    NULL,                              /* INTID4    : SOFTWARE_GENERATE_INT4                   */
    NULL,                              /* INTID5    : SOFTWARE_GENERATE_INT5                   */
    NULL,                              /* INTID6    : SOFTWARE_GENERATE_INT6                   */
    NULL,                              /* INTID7    : SOFTWARE_GENERATE_INT7                   */
    NULL,                              /* INTID8    : SOFTWARE_GENERATE_INT8                   */
    NULL,                              /* INTID9    : SOFTWARE_GENERATE_INT9                   */
    NULL,                              /* INTID10   : SOFTWARE_GENERATE_INT10                  */
    NULL,                              /* INTID11   : SOFTWARE_GENERATE_INT11                  */
    NULL,                              /* INTID12   : SOFTWARE_GENERATE_INT12                  */
    NULL,                              /* INTID13   : SOFTWARE_GENERATE_INT13                  */
    NULL,                              /* INTID14   : SOFTWARE_GENERATE_INT14                  */
    NULL,                              /* INTID15   : SOFTWARE_GENERATE_INT15                  */
    NULL,                              /* INTID16   : RESERVED                                 */
    NULL,                              /* INTID17   : RESERVED                                 */
    NULL,                              /* INTID18   : RESERVED                                 */
    NULL,                              /* INTID19   : RESERVED                                 */
    NULL,                              /* INTID20   : RESERVED                                 */
    NULL,                              /* INTID21   : RESERVED                                 */
    NULL,                              /* INTID22   : DEBUG_COMMUNICATIONS_CHANNEL_INT         */
    NULL,                              /* INTID23   : PERFORMANCE_MONITOR_COUNTER_OVERFLOW_INT */
    NULL,                              /* INTID24   : CROSS_TRIGGER_INTERFACE_INT              */
    NULL,                              /* INTID25   : VIRTUAL_CPU_INTERFACE_MAINTENANCE_INT    */
    NULL,                              /* INTID26   : HYPERVISOR_TIMER_INT                     */
    NULL,                              /* INTID27   : VIRTUAL_TIMER_INT                        */
    NULL,                              /* INTID28   : RESERVED                                 */
    NULL,                              /* INTID29   : RESERVED                                 */
    NULL,                              /* INTID30   : NON-SECURE_PHYSICAL_TIMER_INT            */
    NULL,                              /* INTID31   : RESERVED                                 */
};
