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
#include "loader_table.h"

#if defined(__ICCARM__)
 #pragma section="APPLICATION_PRG_RBLOCK"
 #pragma section="APPLICATION_PRG_WBLOCK"

 #define application_prg_flash_addr (__section_begin("APPLICATION_PRG_RBLOCK"))
 #define application_prg_start_addr (__section_begin("APPLICATION_PRG_WBLOCK"))
 #define application_prg_size       (__section_size("APPLICATION_PRG_RBLOCK"))

#elif defined(__GNUC__)
extern uint32_t IMAGE_APP_RAM_start;
extern uint32_t IMAGE_APP_FLASH_section_start;
extern uint32_t IMAGE_APP_FLASH_section_size;

 #define application_prg_flash_addr (&IMAGE_APP_FLASH_section_start)
 #define application_prg_start_addr (&IMAGE_APP_RAM_start)
 #define application_prg_size       (&IMAGE_APP_FLASH_section_size)

#endif
/*
 * table = { Src, Dst, Size, Enable flag(enable:1 disable:0) }
 * table0: Load Application program to System SRAM from flash memory
 * table1: Disable table
 * table2: Disable table
 * table3: Disable table
 */
const loader_table table[TABLE_ENTRY_NUM] BSP_PLACE_IN_SECTION("CPU0_LOADER_TABLE") =
{
  { (uint32_t *)application_prg_flash_addr, (uint32_t *)application_prg_start_addr, (uint32_t)application_prg_size, (uint32_t)TABLE_ENABLE },
  { (uint32_t *)TABLE_INVALID_VALUE, (uint32_t *)TABLE_INVALID_VALUE, (uint32_t)TABLE_INVALID_VALUE, (uint32_t)TABLE_DISABLE },
  { (uint32_t *)TABLE_INVALID_VALUE, (uint32_t *)TABLE_INVALID_VALUE, (uint32_t)TABLE_INVALID_VALUE, (uint32_t)TABLE_DISABLE },
  { (uint32_t *)TABLE_INVALID_VALUE, (uint32_t *)TABLE_INVALID_VALUE, (uint32_t)TABLE_INVALID_VALUE, (uint32_t)TABLE_DISABLE }
};
