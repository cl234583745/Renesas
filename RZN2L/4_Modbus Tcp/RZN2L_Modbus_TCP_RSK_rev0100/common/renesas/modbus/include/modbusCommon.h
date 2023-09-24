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
 * Copyright (C) 2021 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#ifndef _MODBUSCOMMON_H_
#define _MODBUSCOMMON_H_

/*===============================================================================================================================*/
/* D E F I N E                                                                                                                   */
/*===============================================================================================================================*/
/**
 *  Define used to enable the debug print for stack debugging
 */
//#define ENABLE_DEBUG_PRINT
#define UINT8_TO_UINT16(data, index)         ((data[index] << 8) | data[index + 1])
#define GET_LSB(data)                        (data & 0xFF)
#define GET_MSB(data)                        ((data >> 8) & 0xFF) 

#ifdef ENABLE_DEBUG_PRINT
#define DEBUG_PRINT(...)                     printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) 
#endif

/**  
 * Maximum data size for the request packet (ASCII/RTU)
 */
#define MAX_DATA_SIZE                  256

/**  
 * Maximum data size of the received packet (ASCII/RTU)
 */
#define MAX_RECV_DATA_SIZE             512

/**  
 * Maximum data size of the send packet (ASCII/RTU)
 */
#define MAX_SND_DATA_SIZE              512

/**
 * Defines used to indicate the function codes
 */
#define FUNC_CODE_READ_COIL                      0x01
#define FUNC_CODE_READ_INPUT                     0x02
#define FUNC_CODE_READ_HOLD_REG                  0x03
#define FUNC_CODE_READ_INPUT_REG                 0x04
#define FUNC_CODE_WRITE_SINGLE_COIL              0x05
#define FUNC_CODE_WRITE_SINGLE_REG               0x06
#define FUNC_CODE_WRITE_MULTIPLE_COIL            0x0F
#define FUNC_CODE_WRITE_MULTIPLE_REG             0x10
#define FUNC_CODE_READ_WRITE_MULTIPLE_REG        0x17

/**
 * Defines used to indicate the exception function codes
 */
#define EXP_CODE_READ_COIL                        0x81
#define EXP_CODE_READ_INPUT                       0x82
#define EXP_CODE_READ_HOLD_REG                    0x83
#define EXP_CODE_READ_INPUT_REG                   0x84
#define EXP_CODE_WRITE_SINGLE_COIL                0x85
#define EXP_CODE_WRITE_SINGLE_REG                 0x86
#define EXP_CODE_WRITE_MULTIPLE_COIL              0x8F
#define EXP_CODE_WRITE_MULTIPLE_REG               0x90
#define EXP_CODE_READ_WRITE_MULTIPLE_REG          0x97

/**
 * Define used to indicate the the minimum and maximum data value to read/write to coils or registers
 */
#define MIN_READ_DISCRETE_CNT                 0x01
#define MAX_READ_DISCRETE_CNT                 0x7D0

#define MIN_READ_REG_CNT                      0x01
#define MAX_READ_REG_CNT                      0x7D

#define MIN_WRITE_COIL_VALUE                  0x0000
#define MAX_WRITE_COIL_VALUE                  0xFF00

#define MIN_WRITE_REG_VALUE                   0x0000
#define MAX_WRITE_REG_VALUE                   0xFFFF

#define MIN_MULTIPLE_DISCRETE_WRITE_CNT        0x01
#define MAX_MULTIPLE_DISCRETE_WRITE_CNT        0x07B0

#define MIN_MULTIPLE_REG_WRITE_CNT             0x01
#define MAX_MULTIPLE_REG_WRITE_CNT             0x7B

#define MIN_MULTIPLE_READ_WRITE_CNT            0x01
#define MAX_MULTIPLE_READ_WRITE_CNT            0x79

/** 
 * Define used to indicate the register size (in bytes)
 */
#define MB_REGISTER_SIZE                       (2) 

#endif  /* _MODBUSCOMMON_H_ */

/* End of file */
