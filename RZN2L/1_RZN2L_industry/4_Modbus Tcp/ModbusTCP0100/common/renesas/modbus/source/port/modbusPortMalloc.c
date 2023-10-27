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

/*============================================================================*/
/* I N C L U D E                                                              */
/*============================================================================*/
#include <port/modbusPortMalloc.h>
#include "FreeRTOS.h"

/*============================================================================*/
/* P R O G R A M                                                              */
/*============================================================================*/
/** @brief Modbus memory allocation porting layer
 *
 *  This function is a porting layer that uses memory allocation function on modbus.
 *
 *  @param[in]          u32_wanted_size     Request size
 *
 *  @return             Address of the allocated area
 */
void * Modbus_malloc(uint32_t u32_wanted_size)
{
    return pvPortMalloc((size_t)u32_wanted_size);
}
/** @brief Modbus free porting layer
 *
 *  This function is a porting layer that uses free function on modbus.
 *
 *  @param[in, out]          * pv           Address of the area to be freed
 *
 */
void Modbus_free(void * pv)
{
	vPortFree(pv);
}

/* End of File */
