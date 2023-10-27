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
#include "modbus.h"
#include "modbusCommon.h"

/*============================================================================*/
/* E X T E R N                                                                */
/*============================================================================*/
/* definitions in modbus_user.c */
extern uint32_t g_coil_end_addr;
extern uint8_t (*MB_Coils_Read[])(uint8_t *data);
extern uint8_t (*MB_Coils_Write[])(uint8_t data);
extern uint32_t g_discrete_input_end_addr;
extern uint8_t (*MB_Discretes_Input[])(uint8_t *data);
extern uint32_t g_holding_reg_end_addr;
extern uint8_t (*MB_HoldingRegs_Read[])(uint16_t *data);
extern uint8_t (*MB_HoldingRegs_Write[])(uint16_t data);
extern uint32_t g_input_reg_end_addr;
extern uint8_t (*MB_Input_Regs[])(uint16_t *data);

/*============================================================================*/
/* D E F I N E                                                                */
/*============================================================================*/
#define MODBUS_COIL_START_ADDR           (0)
#define MODBUS_DISCRETE_INPUT_START_ADDR (0)
#define MODBUS_HOLDING_REG_START_ADDR    (0)
#define MODBUS_INPUT_REG_START_ADDR      (0)
#define MODBUS_COIL_END_ADDR             (g_coil_end_addr)
#define MODBUS_DISCRETE_INPUT_END_ADDR   (g_discrete_input_end_addr)
#define MODBUS_HOLDING_REG_END_ADDR      (g_holding_reg_end_addr)
#define MODBUS_INPUT_REG_END_ADDR        (g_input_reg_end_addr)
#define MB_Reg_Exp_Addr0                 ((uint16_t (*)(uint8_t *))0)

/*============================================================================*/
/* V A R I A B L E                                                            */
/*============================================================================*/

/*============================================================================*/
/* S T A T I C   F U N C T I O N   P R O T O T Y P E                          */
/*============================================================================*/
int MB_Func_Exp_Addr(uint8_t *res);
int MB_Func_Exp_Data(uint8_t *res);
int MB_Func_Exp_Execution(uint8_t *res, int type);

/*============================================================================*/
/* P R O G R A M                                                              */
/*============================================================================*/
uint32_t cb_func_code01(p_req_read_coils_t, p_resp_read_coils_t);
uint32_t cb_func_code02(p_req_read_inputs_t, p_resp_read_inputs_t);
uint32_t cb_func_code03(p_req_read_holding_reg_t, p_resp_read_holding_reg_t);
uint32_t cb_func_code04(p_req_read_input_reg_t, p_resp_read_input_reg_t);
uint32_t cb_func_code05(p_req_write_single_coil_t, p_resp_write_single_coil_t);
uint32_t cb_func_code06(p_req_write_single_reg_t, p_resp_write_single_reg_t);
uint32_t cb_func_code15(p_req_write_multiple_coils_t, p_resp_write_multiple_coils_t);
uint32_t cb_func_code16(p_req_write_multiple_reg_t, p_resp_write_multiple_reg_t);
uint32_t cb_func_code23(p_req_read_write_multiple_reg_t, p_resp_read_write_multiple_reg_t);

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 1(Read Coils) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read coils(function code 1) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read coils support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request    structure pointer from stack to user with read coils request information
 * @param[out] pt_response   structure pointer to stack from user with read coils response data
 * @retval 0 on success of read coils operation
 * @retval 1 on failure of read coils operation
 ******************************************************************************
 */
uint32_t cb_func_code01(p_req_read_coils_t pt_request, p_resp_read_coils_t pt_response)
{
    uint8_t u8_data;
    uint8_t u8_num_of_bytes;
    uint8_t u8_res;
    int i;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Starting Address */
    if ((pt_request->u16_start_addr < MODBUS_COIL_END_ADDR)
            && ((pt_request->u16_start_addr + pt_request->u16_num_of_coils) <= MODBUS_COIL_END_ADDR))
    {
        if ((pt_request->u16_num_of_coils % 8) == 0)
        {
            u8_num_of_bytes = (uint8_t) pt_request->u16_num_of_coils / 8U;
        }
        else
        {
            u8_num_of_bytes = (pt_request->u16_num_of_coils / 8) + 1;
        }
        pt_response->u8_num_of_bytes = u8_num_of_bytes;

        //scan_coil(&u8_data);
        for (i = 0; i < pt_request->u16_num_of_coils; i++)
        {
            if (i % 8 == 0)
            {
                pt_response->aru8_data[i / 8] = 0;
            }
            u8_res = (*MB_Coils_Read[pt_request->u16_start_addr + i]) (&u8_data);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
            pt_response->aru8_data[i / 8] |= u8_data << (i % 8);
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }
    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 2(Read Discrete Inputs) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read discrete inputs(function code 2) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read discrete inputs support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with read discrete inputs request information
 * @param[out] pt_response     structure pointer to stack from user with read discrete inputs response data
 * @retval 0 on success of read discrete inputs operation
 * @retval 1 on failure of read discrete inputs operation
 ******************************************************************************
 */
uint32_t cb_func_code02(p_req_read_inputs_t pt_request, p_resp_read_inputs_t pt_response)
{
    uint8_t u8_data;
    uint8_t u8_num_of_bytes;
    uint8_t u8_res;
    int i;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Starting Address */
    if ((pt_request->u16_start_addr < MODBUS_DISCRETE_INPUT_END_ADDR)
            && ((pt_request->u16_start_addr + pt_request->u16_num_of_inputs) <= MODBUS_DISCRETE_INPUT_END_ADDR))
    {
        if ((pt_request->u16_num_of_inputs % 8) == 0)
        {
            u8_num_of_bytes = pt_request->u16_num_of_inputs / 8;
        }
        else
        {
            u8_num_of_bytes = (pt_request->u16_num_of_inputs / 8) + 1;
        }
        pt_response->u8_num_of_bytes = u8_num_of_bytes;

        //scan_input_status(&u8_data);
        for (i = 0; i < pt_request->u16_num_of_inputs; i++)
        {
            if (i % 8 == 0)
            {
                pt_response->aru8_data[i / 8] = 0;
            }
            u8_res = (*MB_Discretes_Input[pt_request->u16_start_addr + i]) (&u8_data);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
            pt_response->aru8_data[i / 8] |= u8_data << (i % 8);
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }
    pt_response->u8_num_of_bytes = u8_num_of_bytes;
    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 3(Read Holding Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read holding registers(function code 3) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read holding registers support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with read holding registers request information
 * @param[out] pt_response     structure pointer to stack from user with read holding registers response data
 * @retval 0 on success of read holding registers operation
 * @retval 1 on failure of read holding registers operation
 ******************************************************************************
 */
uint32_t cb_func_code03(p_req_read_holding_reg_t pt_request, p_resp_read_holding_reg_t pt_response)
{
    uint16_t i;
    uint16_t u16_data;
    uint8_t u8_res;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Starting Address */
    if ((pt_request->u16_start_addr < MODBUS_HOLDING_REG_END_ADDR)
            && ((pt_request->u16_start_addr + pt_request->u16_num_of_reg) <= MODBUS_HOLDING_REG_END_ADDR))
    {
        pt_response->u8_num_of_bytes = pt_request->u16_num_of_reg * 2;

        for (i = 0; i < pt_request->u16_num_of_reg; i++)
        {
            u8_res = (*MB_HoldingRegs_Read[pt_request->u16_start_addr + i]) (&u16_data);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
            pt_response->aru16_data[i] = u16_data;
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }

    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 4(Read Input Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read input registers(function code 4) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read input registers support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with read input registers request information
 * @param[out] pt_response     structure pointer to stack from user with read input registers response data
 * @retval 0 on success of read input registers operation
 * @retval 1 on failure of read input registers operation
 ******************************************************************************
 */
uint32_t cb_func_code04(p_req_read_input_reg_t pt_request, p_resp_read_input_reg_t pt_response)
{
    uint16_t i;
    uint16_t u16_data;
    uint8_t u8_res;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Starting Address */
    if ((pt_request->u16_start_addr < MODBUS_INPUT_REG_END_ADDR)
            && (pt_request->u16_start_addr + pt_request->u16_num_of_reg <= MODBUS_INPUT_REG_END_ADDR))
    {
        pt_response->u8_num_of_bytes = pt_request->u16_num_of_reg * 2;

        for (i = 0; i < pt_request->u16_num_of_reg; i++)
        {
            u8_res = (*MB_Input_Regs[pt_request->u16_start_addr + i]) (&u16_data);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
            pt_response->aru16_data[i] = u16_data;
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }

    return ERR_OK;
}

/**
 ******************************************************************************
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
 ******************************************************************************
 */
uint32_t cb_func_code05(p_req_write_single_coil_t pt_request, p_resp_write_single_coil_t pt_response)
{
    uint8_t u8_data;
    uint8_t u8_res;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Register Value. Output Value == 0x0000 OR 0xFF00 */
    if (pt_request->u16_output_value == MAX_WRITE_COIL_VALUE)
    {
        u8_data = 0x01;
    }
    else if (pt_request->u16_output_value == MIN_WRITE_COIL_VALUE)
    {
        u8_data = 0x00;
    }
    else
    {
        return MB_Func_Exp_Data (&pt_response->u8_exception_code);
    }
    /* Check, Register Address */
    if (pt_request->u16_output_addr < MODBUS_COIL_END_ADDR)
    {
        pt_response->u16_output_addr = pt_request->u16_output_addr;
        pt_response->u16_output_value = pt_request->u16_output_value;

        u8_res = (*MB_Coils_Write[pt_request->u16_output_addr]) (u8_data);
        if (u8_res != ERR_OK)
        {
            return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }

    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 6(Write Single Register) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write single register(function code 6) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write single register support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with write single register request information
 * @param[out] pt_response     structure pointer to stack from user with write single register response
 * @retval 0 on success of write single register operation
 * @retval 1 on failure of write single register operation
 ******************************************************************************
 */
uint32_t cb_func_code06(p_req_write_single_reg_t pt_request, p_resp_write_single_reg_t pt_response)
{
    uint8_t u8_res;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Register Address */
    if ((pt_request->u16_register_addr < MODBUS_HOLDING_REG_END_ADDR))
    {
        pt_response->u16_register_addr = pt_request->u16_register_addr;
        pt_response->u16_register_value = pt_request->u16_register_value;

        u8_res = (*MB_HoldingRegs_Write[pt_request->u16_register_addr]) (pt_request->u16_register_value);
        if (u8_res != ERR_OK)
        {
            return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }

    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 15(Write Multiple Coils) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write multiple coils(function code 15) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write multiple coils support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with write multiple coils request information
 * @param[out] pt_response     structure pointer to stack from user with write multiple coils response
 * @retval 0 on success of write multiple coils operation
 * @retval 1 on failure of write multiple coils operation
 ******************************************************************************
 */
uint32_t cb_func_code15(p_req_write_multiple_coils_t pt_request, p_resp_write_multiple_coils_t pt_response)
{
    uint8_t u8_res;
    uint16_t i;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Starting Address */
    if ((pt_request->u16_start_addr < MODBUS_COIL_END_ADDR)
            && ((pt_request->u16_start_addr + pt_request->u16_num_of_outputs) <= MODBUS_COIL_END_ADDR))
    {
        pt_response->u16_start_addr = pt_request->u16_start_addr;
        pt_response->u16_num_of_outputs = pt_request->u16_num_of_outputs;

        for (i = 0; i < pt_request->u16_num_of_outputs; i++)
        {
            u8_res = (*MB_Coils_Write[pt_request->u16_start_addr + i]) (
                    (pt_request->aru8_data[(i / 8)] & (1 << (i % 8))) >> (i % 8));
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
        }

    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }

    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 16(Write Multiple Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a write multiple registers(function code 16) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means write multiple registers support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with write multiple registers request information
 * @param[out] pt_response     structure pointer to stack from user with write multiple registers response
 * @retval 0 on success of write multiple registers operation
 * @retval 1 on failure of write multiple registers operation
 ******************************************************************************
 */
uint32_t cb_func_code16(p_req_write_multiple_reg_t pt_request, p_resp_write_multiple_reg_t pt_response)
{
    uint8_t u8_res;
    uint16_t i;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    /* Check, Starting Address */
    if ((pt_request->u16_start_addr < MODBUS_HOLDING_REG_END_ADDR)
            && ((pt_request->u16_start_addr + pt_request->u16_num_of_reg) <= MODBUS_HOLDING_REG_END_ADDR))
    {
        pt_response->u16_start_addr = pt_request->u16_start_addr;
        pt_response->u16_num_of_reg = pt_request->u16_num_of_reg;

        for (i = 0; i < pt_request->u16_num_of_reg; i++)
        {
            u8_res = (*MB_HoldingRegs_Write[pt_request->u16_start_addr + i]) (pt_request->aru16_data[i]);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
        }
    }
    else
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }

    return ERR_OK;
}

/**
 ******************************************************************************
 * @brief Call back function pointer for MODBUS function code 23(Read/Write Multiple Registers) processing
 *
 * This call back function is only applicable in slave mode configuration of stack.
 * Application developer can assign a read/write multiple registers(function code 23) processing function to this function pointer and 
 * stack invokes user registered function when a request received from client side.
 * If this function pointer is set to NULL means read/write multiple registers support is not implemented or supported by application/device. 
 *
 * @param[in]  pt_request      structure pointer from stack to user with read/write multiple registers request information
 * @param[out] pt_response     structure pointer to stack from user with read/write multiple registers response
 * @retval 0 on success of read/write multiple registers operation
 * @retval 1 on failure of read/write multiple registers operation
 ******************************************************************************
 */
uint32_t cb_func_code23(p_req_read_write_multiple_reg_t pt_request, p_resp_read_write_multiple_reg_t pt_response)
{
    uint16_t i;
    uint16_t u16_data;
    uint8_t u8_res;

    pt_response->u16_transaction_id = pt_request->u16_transaction_id;
    pt_response->u16_protocol_id = pt_request->u16_protocol_id;
    pt_response->u8_slave_id = pt_request->u8_slave_id;
    pt_response->u8_exception_code = 0;

    if (!(pt_request->u16_write_start_addr < MODBUS_HOLDING_REG_END_ADDR)
            || !((pt_request->u16_write_start_addr + pt_request->u16_num_to_write) <= MODBUS_HOLDING_REG_END_ADDR)
            || !(pt_request->u16_read_start_addr < MODBUS_HOLDING_REG_END_ADDR)
            || !((pt_request->u16_read_start_addr + pt_request->u16_num_to_read) <= MODBUS_HOLDING_REG_END_ADDR))
    {
        return MB_Func_Exp_Addr (&pt_response->u8_exception_code);
    }
    /*  The write operation is performed before the read.  */
    {
        pt_response->u8_num_of_bytes = pt_request->u16_num_to_read * 2;

        //u16_base = pt_request->u16_write_start_addr;
        for (i = 0; i < pt_request->u16_num_to_write; i++)
        {
            //g_HoldingRegArea[u16_base + i] = pt_request->aru16_data[i];
            u8_res = (*MB_HoldingRegs_Write[pt_request->u16_write_start_addr + i]) (pt_request->aru16_data[i]);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
        }
    }
    {
        //u16_base = pt_request->u16_write_start_addr;
        for (i = 0; i < pt_request->u16_num_to_read; i++)
        {
            //pt_response->aru16_read_data[i] = g_HoldingRegArea[u16_base + i];
            u8_res = (*MB_HoldingRegs_Read[pt_request->u16_read_start_addr + i]) (&u16_data);
            if (u8_res != ERR_OK)
            {
                return MB_Func_Exp_Execution (&pt_response->u8_exception_code, u8_res);
            }
            pt_response->aru16_read_data[i] = u16_data;
        }
    }
    return ERR_OK;
}

/************************************************
 * ILLEGAL ADDRESS
 */
int MB_Func_Exp_Addr(uint8_t *res)
{
    *res = ERR_ILLEGAL_DATA_ADDRESS;
    return ERR_OK/*3byte*/;
}

/************************************************
 * ILLEGAL_VALUE
 */
int MB_Func_Exp_Data(uint8_t *res)
{
    *res = ERR_ILLEGAL_DATA_VALUE;
    return ERR_OK/*3*/;
}

/************************************************
 * ILLEGAL_CMD
 */
int MB_Func_Exp_Execution(uint8_t *res, int type)
{
    static uint16_t su2_com_flag_error;/* internal use */

    if (type == ERR_SLAVE_DEVICE_FAILURE) /* packet size error */
        su2_com_flag_error |= 0x8000;
    if (type == ERR_ILLEGAL_DATA_VALUE) /* write data error */
        su2_com_flag_error |= 0x4000;

    *res = ERR_SLAVE_DEVICE_FAILURE;
    return ERR_OK/*3*/;
}

/* End of File */
