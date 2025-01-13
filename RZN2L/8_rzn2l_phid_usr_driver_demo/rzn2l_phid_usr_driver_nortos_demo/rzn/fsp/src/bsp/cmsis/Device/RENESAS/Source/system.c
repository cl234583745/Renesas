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
#define BSP_PRV_M_MPU_REGION_NUM                              (7)

#if defined(BSP_M_MPU0_SUPPORT)
 #define BSP_PRV_M_MPU0_STADD0                                (BSP_CFG_MPU0_STADD0 | (BSP_CFG_MPU0_WRITE0 << 1) | \
                                                               BSP_CFG_MPU0_READ0)
 #define BSP_PRV_M_MPU0_STADD1                                (BSP_CFG_MPU0_STADD1 | (BSP_CFG_MPU0_WRITE1 << 1) | \
                                                               BSP_CFG_MPU0_READ1)
 #define BSP_PRV_M_MPU0_STADD2                                (BSP_CFG_MPU0_STADD2 | (BSP_CFG_MPU0_WRITE2 << 1) | \
                                                               BSP_CFG_MPU0_READ2)
 #define BSP_PRV_M_MPU0_STADD3                                (BSP_CFG_MPU0_STADD3 | (BSP_CFG_MPU0_WRITE3 << 1) | \
                                                               BSP_CFG_MPU0_READ3)
 #define BSP_PRV_M_MPU0_STADD4                                (BSP_CFG_MPU0_STADD4 | (BSP_CFG_MPU0_WRITE4 << 1) | \
                                                               BSP_CFG_MPU0_READ4)
 #define BSP_PRV_M_MPU0_STADD5                                (BSP_CFG_MPU0_STADD5 | (BSP_CFG_MPU0_WRITE5 << 1) | \
                                                               BSP_CFG_MPU0_READ5)
 #define BSP_PRV_M_MPU0_STADD6                                (BSP_CFG_MPU0_STADD6 | (BSP_CFG_MPU0_WRITE6 << 1) | \
                                                               BSP_CFG_MPU0_READ6)
 #define BSP_PRV_M_MPU0_STADD7                                (BSP_CFG_MPU0_STADD7 | (BSP_CFG_MPU0_WRITE7 << 1) | \
                                                               BSP_CFG_MPU0_READ7)
#endif
#if defined(BSP_M_MPU1_SUPPORT)
 #define BSP_PRV_M_MPU1_STADD0                                (BSP_CFG_MPU1_STADD0 | (BSP_CFG_MPU1_WRITE0 << 1) | \
                                                               BSP_CFG_MPU1_READ0)
 #define BSP_PRV_M_MPU1_STADD1                                (BSP_CFG_MPU1_STADD1 | (BSP_CFG_MPU1_WRITE1 << 1) | \
                                                               BSP_CFG_MPU1_READ1)
 #define BSP_PRV_M_MPU1_STADD2                                (BSP_CFG_MPU1_STADD2 | (BSP_CFG_MPU1_WRITE2 << 1) | \
                                                               BSP_CFG_MPU1_READ2)
 #define BSP_PRV_M_MPU1_STADD3                                (BSP_CFG_MPU1_STADD3 | (BSP_CFG_MPU1_WRITE3 << 1) | \
                                                               BSP_CFG_MPU1_READ3)
 #define BSP_PRV_M_MPU1_STADD4                                (BSP_CFG_MPU1_STADD4 | (BSP_CFG_MPU1_WRITE4 << 1) | \
                                                               BSP_CFG_MPU1_READ4)
 #define BSP_PRV_M_MPU1_STADD5                                (BSP_CFG_MPU1_STADD5 | (BSP_CFG_MPU1_WRITE5 << 1) | \
                                                               BSP_CFG_MPU1_READ5)
 #define BSP_PRV_M_MPU1_STADD6                                (BSP_CFG_MPU1_STADD6 | (BSP_CFG_MPU1_WRITE6 << 1) | \
                                                               BSP_CFG_MPU1_READ6)
 #define BSP_PRV_M_MPU1_STADD7                                (BSP_CFG_MPU1_STADD7 | (BSP_CFG_MPU1_WRITE7 << 1) | \
                                                               BSP_CFG_MPU1_READ7)
#endif
#if defined(BSP_M_MPU2_SUPPORT)
 #define BSP_PRV_M_MPU2_STADD0                                (BSP_CFG_MPU2_STADD0 | (BSP_CFG_MPU2_WRITE0 << 1) | \
                                                               BSP_CFG_MPU2_READ0)
 #define BSP_PRV_M_MPU2_STADD1                                (BSP_CFG_MPU2_STADD1 | (BSP_CFG_MPU2_WRITE1 << 1) | \
                                                               BSP_CFG_MPU2_READ1)
 #define BSP_PRV_M_MPU2_STADD2                                (BSP_CFG_MPU2_STADD2 | (BSP_CFG_MPU2_WRITE2 << 1) | \
                                                               BSP_CFG_MPU2_READ2)
 #define BSP_PRV_M_MPU2_STADD3                                (BSP_CFG_MPU2_STADD3 | (BSP_CFG_MPU2_WRITE3 << 1) | \
                                                               BSP_CFG_MPU2_READ3)
 #define BSP_PRV_M_MPU2_STADD4                                (BSP_CFG_MPU2_STADD4 | (BSP_CFG_MPU2_WRITE4 << 1) | \
                                                               BSP_CFG_MPU2_READ4)
 #define BSP_PRV_M_MPU2_STADD5                                (BSP_CFG_MPU2_STADD5 | (BSP_CFG_MPU2_WRITE5 << 1) | \
                                                               BSP_CFG_MPU2_READ5)
 #define BSP_PRV_M_MPU2_STADD6                                (BSP_CFG_MPU2_STADD6 | (BSP_CFG_MPU2_WRITE6 << 1) | \
                                                               BSP_CFG_MPU2_READ6)
 #define BSP_PRV_M_MPU2_STADD7                                (BSP_CFG_MPU2_STADD7 | (BSP_CFG_MPU2_WRITE7 << 1) | \
                                                               BSP_CFG_MPU2_READ7)
#endif
#if defined(BSP_M_MPU3_SUPPORT)
 #define BSP_PRV_M_MPU3_STADD0                                (BSP_CFG_MPU3_STADD0 | (BSP_CFG_MPU3_WRITE0 << 1) | \
                                                               BSP_CFG_MPU3_READ0)
 #define BSP_PRV_M_MPU3_STADD1                                (BSP_CFG_MPU3_STADD1 | (BSP_CFG_MPU3_WRITE1 << 1) | \
                                                               BSP_CFG_MPU3_READ1)
 #define BSP_PRV_M_MPU3_STADD2                                (BSP_CFG_MPU3_STADD2 | (BSP_CFG_MPU3_WRITE2 << 1) | \
                                                               BSP_CFG_MPU3_READ2)
 #define BSP_PRV_M_MPU3_STADD3                                (BSP_CFG_MPU3_STADD3 | (BSP_CFG_MPU3_WRITE3 << 1) | \
                                                               BSP_CFG_MPU3_READ3)
 #define BSP_PRV_M_MPU3_STADD4                                (BSP_CFG_MPU3_STADD4 | (BSP_CFG_MPU3_WRITE4 << 1) | \
                                                               BSP_CFG_MPU3_READ4)
 #define BSP_PRV_M_MPU3_STADD5                                (BSP_CFG_MPU3_STADD5 | (BSP_CFG_MPU3_WRITE5 << 1) | \
                                                               BSP_CFG_MPU3_READ5)
 #define BSP_PRV_M_MPU3_STADD6                                (BSP_CFG_MPU3_STADD6 | (BSP_CFG_MPU3_WRITE6 << 1) | \
                                                               BSP_CFG_MPU3_READ6)
 #define BSP_PRV_M_MPU3_STADD7                                (BSP_CFG_MPU3_STADD7 | (BSP_CFG_MPU3_WRITE7 << 1) | \
                                                               BSP_CFG_MPU3_READ7)
#endif
#if defined(BSP_M_MPU4_SUPPORT)
 #define BSP_PRV_M_MPU4_STADD0                                (BSP_CFG_MPU4_STADD0 | (BSP_CFG_MPU4_WRITE0 << 1) | \
                                                               BSP_CFG_MPU4_READ0)
 #define BSP_PRV_M_MPU4_STADD1                                (BSP_CFG_MPU4_STADD1 | (BSP_CFG_MPU4_WRITE1 << 1) | \
                                                               BSP_CFG_MPU4_READ1)
 #define BSP_PRV_M_MPU4_STADD2                                (BSP_CFG_MPU4_STADD2 | (BSP_CFG_MPU4_WRITE2 << 1) | \
                                                               BSP_CFG_MPU4_READ2)
 #define BSP_PRV_M_MPU4_STADD3                                (BSP_CFG_MPU4_STADD3 | (BSP_CFG_MPU4_WRITE3 << 1) | \
                                                               BSP_CFG_MPU4_READ3)
 #define BSP_PRV_M_MPU4_STADD4                                (BSP_CFG_MPU4_STADD4 | (BSP_CFG_MPU4_WRITE4 << 1) | \
                                                               BSP_CFG_MPU4_READ4)
 #define BSP_PRV_M_MPU4_STADD5                                (BSP_CFG_MPU4_STADD5 | (BSP_CFG_MPU4_WRITE5 << 1) | \
                                                               BSP_CFG_MPU4_READ5)
 #define BSP_PRV_M_MPU4_STADD6                                (BSP_CFG_MPU4_STADD6 | (BSP_CFG_MPU4_WRITE6 << 1) | \
                                                               BSP_CFG_MPU4_READ6)
 #define BSP_PRV_M_MPU4_STADD7                                (BSP_CFG_MPU4_STADD7 | (BSP_CFG_MPU4_WRITE7 << 1) | \
                                                               BSP_CFG_MPU4_READ7)
#endif
#if defined(BSP_M_MPU6_SUPPORT)
 #define BSP_PRV_M_MPU6_STADD0                                (BSP_CFG_MPU6_STADD0 | (BSP_CFG_MPU6_WRITE0 << 1) | \
                                                               BSP_CFG_MPU6_READ0)
 #define BSP_PRV_M_MPU6_STADD1                                (BSP_CFG_MPU6_STADD1 | (BSP_CFG_MPU6_WRITE1 << 1) | \
                                                               BSP_CFG_MPU6_READ1)
 #define BSP_PRV_M_MPU6_STADD2                                (BSP_CFG_MPU6_STADD2 | (BSP_CFG_MPU6_WRITE2 << 1) | \
                                                               BSP_CFG_MPU6_READ2)
 #define BSP_PRV_M_MPU6_STADD3                                (BSP_CFG_MPU6_STADD3 | (BSP_CFG_MPU6_WRITE3 << 1) | \
                                                               BSP_CFG_MPU6_READ3)
 #define BSP_PRV_M_MPU6_STADD4                                (BSP_CFG_MPU6_STADD4 | (BSP_CFG_MPU6_WRITE4 << 1) | \
                                                               BSP_CFG_MPU6_READ4)
 #define BSP_PRV_M_MPU6_STADD5                                (BSP_CFG_MPU6_STADD5 | (BSP_CFG_MPU6_WRITE5 << 1) | \
                                                               BSP_CFG_MPU6_READ5)
 #define BSP_PRV_M_MPU6_STADD6                                (BSP_CFG_MPU6_STADD6 | (BSP_CFG_MPU6_WRITE6 << 1) | \
                                                               BSP_CFG_MPU6_READ6)
 #define BSP_PRV_M_MPU6_STADD7                                (BSP_CFG_MPU6_STADD7 | (BSP_CFG_MPU6_WRITE7 << 1) | \
                                                               BSP_CFG_MPU6_READ7)
#endif
#if defined(BSP_M_MPU7_SUPPORT)
 #define BSP_PRV_M_MPU7_STADD0                                (BSP_CFG_MPU7_STADD0 | (BSP_CFG_MPU7_WRITE0 << 1) | \
                                                               BSP_CFG_MPU7_READ0)
 #define BSP_PRV_M_MPU7_STADD1                                (BSP_CFG_MPU7_STADD1 | (BSP_CFG_MPU7_WRITE1 << 1) | \
                                                               BSP_CFG_MPU7_READ1)
 #define BSP_PRV_M_MPU7_STADD2                                (BSP_CFG_MPU7_STADD2 | (BSP_CFG_MPU7_WRITE2 << 1) | \
                                                               BSP_CFG_MPU7_READ2)
 #define BSP_PRV_M_MPU7_STADD3                                (BSP_CFG_MPU7_STADD3 | (BSP_CFG_MPU7_WRITE3 << 1) | \
                                                               BSP_CFG_MPU7_READ3)
 #define BSP_PRV_M_MPU7_STADD4                                (BSP_CFG_MPU7_STADD4 | (BSP_CFG_MPU7_WRITE4 << 1) | \
                                                               BSP_CFG_MPU7_READ4)
 #define BSP_PRV_M_MPU7_STADD5                                (BSP_CFG_MPU7_STADD5 | (BSP_CFG_MPU7_WRITE5 << 1) | \
                                                               BSP_CFG_MPU7_READ5)
 #define BSP_PRV_M_MPU7_STADD6                                (BSP_CFG_MPU7_STADD6 | (BSP_CFG_MPU7_WRITE6 << 1) | \
                                                               BSP_CFG_MPU7_READ6)
 #define BSP_PRV_M_MPU7_STADD7                                (BSP_CFG_MPU7_STADD7 | (BSP_CFG_MPU7_WRITE7 << 1) | \
                                                               BSP_CFG_MPU7_READ7)
#endif
#if defined(BSP_M_MPU8_SUPPORT)
 #define BSP_PRV_M_MPU8_STADD0                                (BSP_CFG_MPU8_STADD0 | (BSP_CFG_MPU8_WRITE0 << 1) | \
                                                               BSP_CFG_MPU8_READ0)
 #define BSP_PRV_M_MPU8_STADD1                                (BSP_CFG_MPU8_STADD1 | (BSP_CFG_MPU8_WRITE1 << 1) | \
                                                               BSP_CFG_MPU8_READ1)
 #define BSP_PRV_M_MPU8_STADD2                                (BSP_CFG_MPU8_STADD2 | (BSP_CFG_MPU8_WRITE2 << 1) | \
                                                               BSP_CFG_MPU8_READ2)
 #define BSP_PRV_M_MPU8_STADD3                                (BSP_CFG_MPU8_STADD3 | (BSP_CFG_MPU8_WRITE3 << 1) | \
                                                               BSP_CFG_MPU8_READ3)
 #define BSP_PRV_M_MPU8_STADD4                                (BSP_CFG_MPU8_STADD4 | (BSP_CFG_MPU8_WRITE4 << 1) | \
                                                               BSP_CFG_MPU8_READ4)
 #define BSP_PRV_M_MPU8_STADD5                                (BSP_CFG_MPU8_STADD5 | (BSP_CFG_MPU8_WRITE5 << 1) | \
                                                               BSP_CFG_MPU8_READ5)
 #define BSP_PRV_M_MPU8_STADD6                                (BSP_CFG_MPU8_STADD6 | (BSP_CFG_MPU8_WRITE6 << 1) | \
                                                               BSP_CFG_MPU8_READ6)
 #define BSP_PRV_M_MPU8_STADD7                                (BSP_CFG_MPU8_STADD7 | (BSP_CFG_MPU8_WRITE7 << 1) | \
                                                               BSP_CFG_MPU8_READ7)
#endif

#if defined(__ICCARM__)
 #if BSP_CFG_C_RUNTIME_INIT
  #define BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS                __section_begin("LDR_DATA_RBLOCK")
  #define BSP_PRV_SECTION_LDR_DATA_RAM_START                  __section_begin("LDR_DATA_WBLOCK")
  #define BSP_PRV_SECTION_LDR_DATA_RAM_END                    __section_end("LDR_DATA_WBLOCK")

  #define BSP_PRV_SECTION_LDR_DATA_BSS_START                  __section_begin("LDR_DATA_ZBLOCK")
  #define BSP_PRV_SECTION_LDR_DATA_BSS_END                    __section_end("LDR_DATA_ZBLOCK")

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
  #define BSP_PRV_SECTION_VECTOR_ROM_ADDRESS                  __section_begin("VECTOR_RBLOCK")
  #define BSP_PRV_SECTION_VECTOR_RAM_START                    __section_begin("VECTOR_WBLOCK")
  #define BSP_PRV_SECTION_VECTOR_RAM_END                      __section_end("VECTOR_WBLOCK")

  #define BSP_PRV_SECTION_USER_PRG_ROM_ADDRESS                __section_begin("USER_PRG_RBLOCK")
  #define BSP_PRV_SECTION_USER_PRG_RAM_START                  __section_begin("USER_PRG_WBLOCK")
  #define BSP_PRV_SECTION_USER_PRG_RAM_END                    __section_end("USER_PRG_WBLOCK")

  #define BSP_PRV_SECTION_USER_DATA_ROM_ADDRESS               __section_begin("USER_DATA_RBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_RAM_START                 __section_begin("USER_DATA_WBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_RAM_END                   __section_end("USER_DATA_WBLOCK")

  #define BSP_PRV_SECTION_USER_DATA_BSS_START                 __section_begin("USER_DATA_ZBLOCK")
  #define BSP_PRV_SECTION_USER_DATA_BSS_END                   __section_end("USER_DATA_ZBLOCK")

 #endif

 #if BSP_CFG_RAM_EXECUTION
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_START           __section_begin("NONCACHE_BUFFER_ZBLOCK")
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_END             __section_end("NONCACHE_BUFFER_ZBLOCK")

  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_START    __section_begin("SHARED_NONCACHE_BUFFER_ZBLOCK")
  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_END      __section_end("SHARED_NONCACHE_BUFFER_ZBLOCK")

  #define BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_START            __section_begin("DMAC_LINK_MODE_ZBLOCK")
  #define BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_END              __section_end("DMAC_LINK_MODE_ZBLOCK")
 #endif

#elif defined(__GNUC__)
 #if BSP_CFG_C_RUNTIME_INIT
  #define BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS                &_mloader_data
  #define BSP_PRV_SECTION_LDR_DATA_RAM_START                  &__loader_data_start
  #define BSP_PRV_SECTION_LDR_DATA_RAM_END                    &__loader_data_end

  #define BSP_PRV_SECTION_LDR_DATA_BSS_START                  &__loader_bss_start
  #define BSP_PRV_SECTION_LDR_DATA_BSS_END                    &__loader_bss_end

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
  #define BSP_PRV_SECTION_VECTOR_ROM_ADDRESS                  &_mfvector
  #define BSP_PRV_SECTION_VECTOR_RAM_START                    &_fvector_start
  #define BSP_PRV_SECTION_VECTOR_RAM_END                      &_fvector_end

  #define BSP_PRV_SECTION_USER_PRG_ROM_ADDRESS                &_mtext
  #define BSP_PRV_SECTION_USER_PRG_RAM_START                  &_text_start
  #define BSP_PRV_SECTION_USER_PRG_RAM_END                    &_text_end

  #define BSP_PRV_SECTION_USER_DATA_ROM_ADDRESS               &_mdata
  #define BSP_PRV_SECTION_USER_DATA_RAM_START                 &_data_start
  #define BSP_PRV_SECTION_USER_DATA_RAM_END                   &_data_end

 #endif

 #define BSP_PRV_SECTION_USER_DATA_BSS_START                  &__bss_start__
 #define BSP_PRV_SECTION_USER_DATA_BSS_END                    &__bss_end__

 #if BSP_CFG_RAM_EXECUTION
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_START           &_ncbuffer_start
  #define BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_END             &_ncbuffer_end

  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_START    &_sncbuffer_start
  #define BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_END      &_sncbuffer_end

  #define BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_START            &_DmacLinkMode_start
  #define BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_END              &_DmacLinkMode_end

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
 #if BSP_CFG_C_RUNTIME_INIT
  #pragma section="LDR_DATA_RBLOCK"
  #pragma section="LDR_DATA_WBLOCK"
  #pragma section="LDR_DATA_ZBLOCK"

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
  #pragma section="VECTOR_RBLOCK"
  #pragma section="VECTOR_WBLOCK"

  #pragma section="USER_PRG_RBLOCK"
  #pragma section="USER_PRG_WBLOCK"

  #pragma section="USER_DATA_RBLOCK"
  #pragma section="USER_DATA_WBLOCK"
  #pragma section="USER_DATA_ZBLOCK"

 #endif

 #if BSP_CFG_RAM_EXECUTION
  #pragma section="NONCACHE_BUFFER_ZBLOCK"
  #pragma section="SHARED_NONCACHE_BUFFER_ZBLOCK"
  #pragma section="DMAC_LINK_MODE_ZBLOCK"

 #endif

#elif defined(__GNUC__)
 #if BSP_CFG_C_RUNTIME_INIT
extern void * _mloader_data;
extern void * __loader_data_start;
extern void * __loader_data_end;

extern void * __loader_bss_start;
extern void * __loader_bss_end;

 #endif

 #if !(BSP_CFG_RAM_EXECUTION)
extern void * _mfvector;
extern void * _fvector_start;
extern void * _fvector_end;

extern void * _mtext;
extern void * _text_start;
extern void * _text_end;

extern void * _mdata;
extern void * _data_start;
extern void * _data_end;

 #endif

extern void * __bss_start__;
extern void * __bss_end__;

 #if BSP_CFG_RAM_EXECUTION
extern void * _ncbuffer_start;
extern void * _ncbuffer_end;

extern void * _sncbuffer_start;
extern void * _sncbuffer_end;

extern void * _DmacLinkMode_start;
extern void * _DmacLinkMode_end;

 #endif

#endif

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
#if defined(__ICCARM__)

void R_BSP_WarmStart(bsp_warm_start_event_t event);

 #pragma weak R_BSP_WarmStart

#elif defined(__GNUC__) || defined(__ARMCC_VERSION)

void R_BSP_WarmStart(bsp_warm_start_event_t event) __attribute__((weak));

#endif

#if BSP_CFG_C_RUNTIME_INIT
void bsp_loader_data_init(void);
void bsp_loader_bss_init(void);

#endif

void bsp_copy_4byte(uint32_t * src, uint32_t * dst, uint32_t bytesize);
void bsp_bss_init_4byte(uint32_t * src, uint32_t bytesize);

#if !(BSP_CFG_RAM_EXECUTION)
void bsp_copy_to_ram(void);
void bsp_application_bss_init(void);

#endif

void bsp_m_mpu_init(void);
void bsp_global_system_counter_init(void);

#if BSP_FEATURE_TFU_SUPPORTED
void bsp_tfu_init(void);

#endif

#if !BSP_CFG_PORT_PROTECT
void bsp_release_port_protect(void);

#endif

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
#if defined(BSP_M_MPU8_SUPPORT)
uint32_t m_mpu8_stadd_cfg[8] =
{
    BSP_PRV_M_MPU8_STADD0, BSP_PRV_M_MPU8_STADD1, BSP_PRV_M_MPU8_STADD2,
    BSP_PRV_M_MPU8_STADD3, BSP_PRV_M_MPU8_STADD4, BSP_PRV_M_MPU8_STADD5,
    BSP_PRV_M_MPU8_STADD6, BSP_PRV_M_MPU8_STADD7
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
#if defined(BSP_M_MPU8_SUPPORT)
uint32_t m_mpu8_endadd_cfg[8] =
{
    BSP_CFG_MPU8_ENDADD0, BSP_CFG_MPU8_ENDADD1, BSP_CFG_MPU8_ENDADD2,
    BSP_CFG_MPU8_ENDADD3, BSP_CFG_MPU8_ENDADD4, BSP_CFG_MPU8_ENDADD5,
    BSP_CFG_MPU8_ENDADD6, BSP_CFG_MPU8_ENDADD7
};
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

  #if BSP_CFG_RAM_EXECUTION

extern void __iar_data_init3(void);

  #endif

  #pragma section = "SHT$$PREINIT_ARRAY" const
  #pragma section = "SHT$$INIT_ARRAY" const
 #endif
#endif

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
#if defined(BSP_M_MPU8_SUPPORT)
        R_MPU8->RGN[region_num].STADD  = m_mpu8_stadd_cfg[region_num];
        R_MPU8->RGN[region_num].ENDADD = m_mpu8_endadd_cfg[region_num];
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
 *
 *
 * @note All programs to be executed when BSP_WARM_START_RESET or BSP_WARM_START_POST_CLOCK event occurs should be
 *       placed in BTCM. These events occur before copying the application program in startup code is executed, and
 *       therefore the application program is located on ROM and cannot be executed at that time.
 *       Linker script for FSP specifies that .warm_start section is to be placed at BTCM. Adding the section
 *       designation to function or variable definition makes it easy to place at BTCM.
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

#if BSP_CFG_C_RUNTIME_INIT

/*******************************************************************************************************************//**
 * Copy the loader data block from external Flash to internal RAM.
 **********************************************************************************************************************/
void bsp_loader_data_init (void)
{
 #if (!defined(__GNUC__) || !(BSP_CFG_RAM_EXECUTION))

    /* Define destination/source address pointer and block size */
    uint32_t * src;
    uint32_t * dst;
    uint32_t   size;

    /* Copy loader data block */
    src  = (uint32_t *) BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS;
    dst  = (uint32_t *) BSP_PRV_SECTION_LDR_DATA_RAM_START;
    size = (uint32_t) BSP_PRV_SECTION_LDR_DATA_RAM_END - (uint32_t) BSP_PRV_SECTION_LDR_DATA_RAM_START;
    bsp_copy_4byte(src, dst, size);
 #endif
}

/*******************************************************************************************************************//**
 * Clear the loader bss block in internal RAM.
 **********************************************************************************************************************/
void bsp_loader_bss_init (void)
{
    /* Define source address pointer and block size */
    uint32_t * src;
    uint32_t   size;

    /* Clear loader bss block. */
    src  = (uint32_t *) BSP_PRV_SECTION_LDR_DATA_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_LDR_DATA_BSS_END - (uint32_t) BSP_PRV_SECTION_LDR_DATA_BSS_START;
    bsp_bss_init_4byte(src, size);

 #if BSP_CFG_RAM_EXECUTION
  #if defined(__ICCARM__)

    /* Initialize the application data and clear the application bss.
     * This code is for RAM Execution. If you want to boot with ROM,
     * enable app_copy and app_bss_init, and disable this code.
     * Also need to change icf file. */
    __iar_data_init3();
  #elif defined(__GNUC__)

    /* Clear application bss block. */
    src  = (uint32_t *) BSP_PRV_SECTION_USER_DATA_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_USER_DATA_BSS_END - (uint32_t) BSP_PRV_SECTION_USER_DATA_BSS_START;
    bsp_bss_init_4byte(src, size);
  #endif

    /* Clear non-cache buffer block. */
    src  = (uint32_t *) BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_END - (uint32_t) BSP_PRV_SECTION_NONCACHE_BUFFER_BSS_START;
    bsp_bss_init_4byte(src, size);

    /* Clear shared non-cache buffer block. */
    src  = (uint32_t *) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_END -
           (uint32_t) BSP_PRV_SECTION_SHARED_NONCACHE_BUFFER_BSS_START;
    bsp_bss_init_4byte(src, size);

    /* Clear DMAC link mode data block. */
    src  = (uint32_t *) BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_END - (uint32_t) BSP_PRV_SECTION_DMAC_LINK_MODE_BSS_START;
    bsp_bss_init_4byte(src, size);
 #endif
}

#endif

/*******************************************************************************************************************//**
 * Copy the memory block from Source address to Destination address by the 4 byte unit.
 **********************************************************************************************************************/
void bsp_copy_4byte (uint32_t * src, uint32_t * dst, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;

    /* Copy Count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *dst++ = *src++;
    }

    /* Ensuring data-changing */
    __DSB();
}

/*******************************************************************************************************************//**
 * Clear the bss block by the 4 byte unit.
 **********************************************************************************************************************/
void bsp_bss_init_4byte (uint32_t * src, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;
    uint32_t zero = 0;

    /* Clear Count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *src++ = zero;
    }

    /* Ensuring data-changing */
    __DSB();
}

#if !(BSP_CFG_RAM_EXECUTION)

/*******************************************************************************************************************//**
 * Copy the application program block from external Flash to internal RAM.
 **********************************************************************************************************************/
void bsp_copy_to_ram (void)
{
    /* Define destination/source address pointer and block size */
    uint32_t * src;
    uint32_t * dst;
    uint32_t   size;

    /* Copy exception vector block */

    /* Casting the pointer and address to a (uint32_t *) and (uint32_t) are valid
     * because these will reference 32 bit address and address size */
    src  = (uint32_t *) BSP_PRV_SECTION_VECTOR_ROM_ADDRESS;
    dst  = (uint32_t *) BSP_PRV_SECTION_VECTOR_RAM_START;
    size = (uint32_t) BSP_PRV_SECTION_VECTOR_RAM_END - (uint32_t) BSP_PRV_SECTION_VECTOR_RAM_START;
    bsp_copy_4byte(src, dst, size);

    /* Copy user program block */

    /* Casting the pointer and address to a (uint32_t *) and (uint32_t) are valid
     * because these will reference 32 bit address and address size */
    src  = (uint32_t *) BSP_PRV_SECTION_USER_PRG_ROM_ADDRESS;
    dst  = (uint32_t *) BSP_PRV_SECTION_USER_PRG_RAM_START;
    size = (uint32_t) BSP_PRV_SECTION_USER_PRG_RAM_END - (uint32_t) BSP_PRV_SECTION_USER_PRG_RAM_START;
    bsp_copy_4byte(src, dst, size);

    /* Copy user data block */

    /* Casting the pointer and address to a (uint32_t *) and (uint32_t) are valid
     * because these will reference 32 bit address and address size */
    src  = (uint32_t *) BSP_PRV_SECTION_USER_DATA_ROM_ADDRESS;
    dst  = (uint32_t *) BSP_PRV_SECTION_USER_DATA_RAM_START;
    size = (uint32_t) BSP_PRV_SECTION_USER_DATA_RAM_END - (uint32_t) BSP_PRV_SECTION_USER_DATA_RAM_START;
    bsp_copy_4byte(src, dst, size);
}

/*******************************************************************************************************************//**
 * Clear the application bss block in internal RAM.
 **********************************************************************************************************************/
void bsp_application_bss_init (void)
{
    /* Define source address pointer and block size */
    uint32_t * src;
    uint32_t   size;

    /* Clear application bss block. */
    src  = (uint32_t *) BSP_PRV_SECTION_USER_DATA_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_USER_DATA_BSS_END - (uint32_t) BSP_PRV_SECTION_USER_DATA_BSS_START;
    bsp_bss_init_4byte(src, size);
}

#endif

#if BSP_FEATURE_TFU_SUPPORTED
void bsp_tfu_init (void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_TFU, 0U);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);
}

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
