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

/*===============================================================================================================================*/
/* I N C L U D E                                                                                                                 */
/*===============================================================================================================================*/
#include <modbusCommon.h>
#include <slave/modbusSlave.h>
#include <port/modbusPortMalloc.h>

/**
 * To store the MODBUS application APIs registerd by the user
 */
p_slave_map_init_t pt_slave_map_init = NULL;

/** @brief MODBUS function code mapping API
 *
 *  This API do the mapping of user defined functions for processing requests from clients depending on function code.
 *  When the MODBUS Slave stack receive a request, it invokes the corresponding handler function registered.   
 *
 *  This API is only valid when the MODBUS stack is configured as Slave mode
 *
 *  @param[in]	pt_slave_func_tbl	Structure pointer to function code mapping table
 * 
 *  @retval ERR_OK on success
 *  @retval ERR_INVALID_STACK_INIT_PARAMS if parameter is null  
 *  @retval ERR_MEM_ALLOC if memory allocation failed
 */
uint32_t Modbus_slave_map_init(p_slave_map_init_t pt_slave_func_tbl)
{
    uint32_t u32_status = ERR_OK;

    pt_slave_map_init = Modbus_malloc (sizeof(slave_map_init_t));
    if (NULL == pt_slave_map_init)
    {
        /* set MALLOC error */
        DEBUG_PRINT("\nMemory allocation failed\n");
        u32_status = ERR_MEM_ALLOC;
    }
    else
    {
        if (NULL == pt_slave_func_tbl)
        {
            u32_status = ERR_INVALID_STACK_INIT_PARAMS;
        }
        else
        {
            /**  Assigning the callback function to the function pointers */
            pt_slave_map_init->fp_function_code1 = pt_slave_func_tbl->fp_function_code1;
            pt_slave_map_init->fp_function_code2 = pt_slave_func_tbl->fp_function_code2;
            pt_slave_map_init->fp_function_code3 = pt_slave_func_tbl->fp_function_code3;
            pt_slave_map_init->fp_function_code4 = pt_slave_func_tbl->fp_function_code4;
            pt_slave_map_init->fp_function_code5 = pt_slave_func_tbl->fp_function_code5;
            pt_slave_map_init->fp_function_code6 = pt_slave_func_tbl->fp_function_code6;
            pt_slave_map_init->fp_function_code15 = pt_slave_func_tbl->fp_function_code15;
            pt_slave_map_init->fp_function_code16 = pt_slave_func_tbl->fp_function_code16;
            pt_slave_map_init->fp_function_code23 = pt_slave_func_tbl->fp_function_code23;
        }
    }

    return u32_status;
}

/* End of File */
