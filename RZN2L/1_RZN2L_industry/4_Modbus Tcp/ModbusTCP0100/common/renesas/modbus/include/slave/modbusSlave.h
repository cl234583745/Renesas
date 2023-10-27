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

#ifndef _MODBUSSLAVE_H_
#define _MODBUSSLAVE_H_

/*===============================================================================================================================*/
/* I N C L U D E                                                                                                                 */
/*===============================================================================================================================*/
#include <modbusTypedef.h>

/*===============================================================================================================================*/
/* T Y P E D E F                                                                                                                 */
/*===============================================================================================================================*/

/**
 * @brief Call back function pointer for MODBUS function code 1(Read Coils) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read coils(function code 1) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read coils support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_read_coils	structure pointer from stack to user with read coils request information
 * @param[out] pt_resp_read_coils   	structure pointer to stack from user with read coils response data
 * @retval 0 on success of read coils operation
 * @retval 1 on failure of read coils operation
 */
typedef uint32_t (*fp_function_code1_t)(p_req_read_coils_t pt_req_read_coils, p_resp_read_coils_t pt_resp_read_coils);
/**
 * @brief Call back function pointer for MODBUS function code 2(Read Discrete Inputs) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read discrete inputs(function code 2) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read discrete inputs support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_read_inputs      structure pointer from stack to user with read discrete inputs request information
 * @param[out] pt_resp_read_inputs     structure pointer to stack from user with read discrete inputs response data
 * @retval 0 on success of read discrete inputs operation
 * @retval 1 on failure of read discrete inputs operation
 */
typedef uint32_t (*fp_function_code2_t)(p_req_read_inputs_t pt_req_read_inputs,
        p_resp_read_inputs_t pt_resp_read_inputs);
/**
 * @brief Call back function pointer for MODBUS function code 3(Read Holding Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read holding registers(function code 3) processing function to this function pointer and 
 * Stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read holding registers support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_read_holding_reg      structure pointer from stack to user with read holding registers request information
 * @param[out] pt_resp_read_holding_reg     structure pointer to stack from user with read holding registers response data
 * @retval 0 on success of read holding registers operation
 * @retval 1 on failure of read holding registers operation
 */
typedef uint32_t (*fp_function_code3_t)(p_req_read_holding_reg_t pt_req_read_holding_reg,
        p_resp_read_holding_reg_t pt_resp_read_holding_reg);
/**
 * @brief Call back function pointer for MODBUS function code 4(Read Input Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read input registers(function code 4) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read input registers support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_read_input_reg      structure pointer from stack to user with read input registers request information
 * @param[out] pt_resp_read_input_reg     structure pointer to stack from user with read input registers response data
 * @retval 0 on success of read input registers operation
 * @retval 1 on failure of read input registers operation
 */
typedef uint32_t (*fp_function_code4_t)(p_req_read_input_reg_t pt_req_read_input_reg,
        p_resp_read_input_reg_t pt_resp_read_input_reg);
/**
 * @brief Call back function pointer for MODBUS function code 5(Write Single Coil) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write single coil(function code 5) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write single coil support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_write_single_coil      structure pointer from stack to user with write single coil request information
 * @param[out] pt_resp_write_single_coil     structure pointer to stack from user with write single coil response
 * @retval 0 on success of write single coil operation
 * @retval 1 on failure of write single coil operation
 */
typedef uint32_t (*fp_function_code5_t)(p_req_write_single_coil_t pt_req_write_single_coil,
        p_resp_write_single_coil_t pt_resp_write_single_coil);
/**
 * @brief Call back function pointer for MODBUS function code 6(Write Single Register) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write single register(function code 6) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write single register support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_write_single_reg      structure pointer from stack to user with write single register request information
 * @param[out] pt_resp_write_single_reg     structure pointer to stack from user with write single register response
 * @retval 0 on success of write single register operation
 * @retval 1 on failure of write single register operation
 */
typedef uint32_t (*fp_function_code6_t)(p_req_write_single_reg_t pt_req_write_single_reg,
        p_resp_write_single_reg_t pt_resp_write_single_reg);
/**
 * @brief Call back function pointer for MODBUS function code 15(Write Multiple Coils) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write multiple coils(function code 15) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write multiple coils support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_write_multiple_coils      structure pointer from stack to user with write multiple coils request information
 * @param[out] pt_resp_write_multiple_coils     structure pointer to stack from user with write multiple coils response
 * @retval 0 on success of write multiple coils operation
 * @retval 1 on failure of write multiple coils operation
 */
typedef uint32_t (*fp_function_code15_t)(p_req_write_multiple_coils_t pt_req_write_multiple_coils,
        p_resp_write_multiple_coils_t pt_resp_write_multiple_coils);
/**
 * @brief Call back function pointer for MODBUS function code 16(Write Multiple Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write multiple registers(function code 16) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write multiple registers support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_write_multiple_reg      structure pointer from stack to user with write multiple registers request information
 * @param[out] pt_resp_write_multiple_reg     structure pointer to stack from user with write multiple registers response
 * @retval 0 on success of write multiple registers operation
 * @retval 1 on failure of write multiple registers operation
 */
typedef uint32_t (*fp_function_code16_t)(p_req_write_multiple_reg_t pt_req_write_multiple_reg,
        p_resp_write_multiple_reg_t pt_resp_write_multiple_reg);
/**
 * @brief Call back function pointer for MODBUS function code 23(Read/Write Multiple Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read/write multiple registers(function code 23) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read/write multiple registers support is not implemented or supported by application/device. 
 *		
 * @param[in]  pt_req_read_write_multiple_reg      structure pointer from stack to user with read/write multiple registers request information
 * @param[out] pt_resp_read_write_multiple_reg     structure pointer to stack from user with read/write multiple registers response
 * @retval 0 on success of read/write multiple registers operation
 * @retval 1 on failure of read/write multiple registers operation
 */
typedef uint32_t (*fp_function_code23_t)(p_req_read_write_multiple_reg_t pt_req_read_write_multiple_reg,
        p_resp_read_write_multiple_reg_t pt_resp_read_write_multiple_reg);

/*===============================================================================================================================*/
/* S T R U C T U R E                                                                                                             */
/*===============================================================================================================================*/

/**
 * @brief Structure for function code operation mapping in slave mode configuration of stack
 *
 * Application programmer should develop functions for processing the request from MODBUS client side   
 * and assign those functions address to corresponding function pointers in this structure.   
 *
 * For Eg: To process read coils request from client, application programmer should develop read coils processing function 
 * and assign to function pointer "fpt_function_code1". 
 * Stack invokes user registered function in "fpt_function_code1" when read coils request received from client.
 *
 * If user set any call back function pointer to NULL, stack consider as particular MODBUS function code is not implemented/supported 
 * and client request for particular function code operation get rejected with corresponding exception code.
 */
typedef struct _slave_map_init
{
    fp_function_code1_t fp_function_code1; /**< Call back function pointer for MODBUS function code 1(Read Coils) operation */
    fp_function_code2_t fp_function_code2; /**< Call back function pointer for MODBUS function code 2(Read Discrete Inputs) operation */
    fp_function_code3_t fp_function_code3; /**< Call back function pointer for MODBUS function code 3(Read Holding Registers) operation */
    fp_function_code4_t fp_function_code4; /**< Call back function pointer for MODBUS function code 4(Read Input Registers) operation */
    fp_function_code5_t fp_function_code5; /**< Call back function pointer for MODBUS function code 5(Write Single Coil) operation */
    fp_function_code6_t fp_function_code6; /**< Call back function pointer for MODBUS function code 6(Write Single Register) operation */
    fp_function_code15_t fp_function_code15; /**< Call back function pointer for MODBUS function code 15(Write Multiple Coils) operation */
    fp_function_code16_t fp_function_code16; /**< Call back function pointer for MODBUS function code 16(Write Multiple Registers) operation */
    fp_function_code23_t fp_function_code23; /**< Call back function pointer for MODBUS function code 23(Read/Write Multiple Registers) operation */
} slave_map_init_t;

typedef struct _slave_map_init *p_slave_map_init_t;

/*============================================================================*/
/* E X T E R N                                                                */
/*============================================================================*/
/**
 * To store the MODBUS application APIs registered by the user
 */
extern p_slave_map_init_t pt_slave_map_init;

/*===============================================================================================================================*/
/* P R O T O T Y P E S                                                                                                           */
/*===============================================================================================================================*/

/** @brief MODBUS function code mapping API
 *
 *  This API does the mapping of user defined functions for processing requests from clients depending on function code.
 *  When the MODBUS Slave stack receive a request, it invokes the corresponding handler function registered.   
 *
 * This API is only valid when the MODBUS stack is configured as Slave mode
 *
 *  @param[in]	pt_slave_func_tbl	structure pointer to function code mapping table
 * 
 *  @retval ERR_OK on success
 *  @retval ERR_INVALID_STACK_INIT_PARAMS if parameter is null  
 *  @retval ERR_MEM_ALLOC if memory allocation failed
 */
uint32_t Modbus_slave_map_init(p_slave_map_init_t pt_slave_func_tbl);

#endif /* _MODBUSSLAVE_H_ */
/* End of File */
