/***********************************************************************************************************************
 * File Name    : flash_hp_ep.h
 * Description  : Contains macros, data structures and functions used in flash_hp_ep.h
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
#ifndef FLASH_HP_H_
#define FLASH_HP_H_

/*******************************************************************************************************************//**
 * @ingroup FLASH_HP
 * @{
 **********************************************************************************************************************/
/* Code Flash */
#define FLASH_HP_CF_BLOCK_SIZE_32KB       (32*1024)    /* Block Size 32 KB */
#define FLASH_HP_CF_BLOCK_SIZE_8KB        (8*1024)     /* Block Size 8KB */

#define FLASH_HP_CF_BLCOK_0               0x00000000U  /*    8 KB:  0x00000000 - 0x00001FFF */
#define FLASH_HP_CF_BLOCK_1               0x00002000U  /*    8 KB:  0x00002000 - 0x00003FFF */
#define FLASH_HP_CF_BLOCK_2               0x00004000U  /*    8 KB:  0x00004000 - 0x00005FFF */
#define FLASH_HP_CF_BLOCK_3               0x00006000U  /*    8 KB:  0x00006000 - 0x00007FFF */
#define FLASH_HP_CF_BLOCK_4				  0x00008000U  /*	 8 KB:  0x00008000 - 0x00009FFF */
#define FLASH_HP_CF_BLOCK_5				  0x0000A000U  /*    8 KB:  0x0000A000 - 0x0000BFFF */
#define FLASH_HP_CF_BLOCK_6				  0x0000C000U  /*    8 KB:  0x0000C000 - 0x0000DFFF */
#define FLASH_HP_CF_BLOCK_7				  0x0000E000U  /*    8 KB:  0x0000E000 - 0x0000FFFF */
#define FLASH_HP_CF_BLOCK_8               0x00010000U  /*   32 KB: 0x00010000 - 0x00017FFF */
#define FLASH_HP_CF_BLOCK_9               0x00018000U  /*   32 KB: 0x00018000 - 0x0001FFFF */
#define FLASH_HP_CF_BLCOK_10              0x00020000U  /*   32 KB: 0x00020000 - 0x0004FFFF */

#define FLASH_HP_CF_BLCOK_69              0x001F8000U  /*   32 KB: 0x001F8000 - 0x001FFFFF */

#define FLASH_HP_DF_BLOCK_SIZE            (64)
/* Data Flash */
#define FLASH_HP_DF_BLOCK_0               0x08000000U /*   64 B:  0x40100000 - 0x4010003F */
#define FLASH_HP_DF_BLOCK_1               0x08000040U /*   64 B:  0x40100040 - 0x4010007F */
#define FLASH_HP_DF_BLOCK_2               0x08000080U /*   64 B:  0x40100080 - 0x401000BF */
#define FLASH_HP_DF_BLOCK_3               0x080000C0U /*   64 B:  0x401000C0 - 0x401000FF */

#define BLOCK_SIZE                        (128)
#define BLOCK_NUM						  (2)


/* Switch Commands */
#define CODE_FLASH                     	(1U)
#define DATA_FLASH                     	(2U)
#define EXIT							(3U)
#define BUFF_SIZE						0x0F
#define BUFF_INDEX						0x00

void readCodeFlash(unsigned char * addr, unsigned char *data, unsigned long len);
/*******************************************************************************************************************//**
 * @} (end defgroup FLASH_HP)
 **********************************************************************************************************************/

#endif /* FLASH_HP_H_ */
