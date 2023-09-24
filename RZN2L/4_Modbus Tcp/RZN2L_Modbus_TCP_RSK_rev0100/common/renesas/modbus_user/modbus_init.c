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

/*============================================================================*/
/* D E F I N E                                                                */
/*============================================================================*/

/*============================================================================*/
/* V A R I A B L E                                                            */
/*============================================================================*/

/*============================================================================*/
/* E X T E R N                                                                */
/*============================================================================*/

/* defined in modbus_func.c */
extern uint32_t cb_func_code01(p_req_read_coils_t, p_resp_read_coils_t);
extern uint32_t cb_func_code02(p_req_read_inputs_t, p_resp_read_inputs_t);
extern uint32_t cb_func_code03(p_req_read_holding_reg_t, p_resp_read_holding_reg_t);
extern uint32_t cb_func_code04(p_req_read_input_reg_t, p_resp_read_input_reg_t);
extern uint32_t cb_func_code05(p_req_write_single_coil_t, p_resp_write_single_coil_t);
extern uint32_t cb_func_code06(p_req_write_single_reg_t, p_resp_write_single_reg_t);
extern uint32_t cb_func_code15(p_req_write_multiple_coils_t, p_resp_write_multiple_coils_t);
extern uint32_t cb_func_code16(p_req_write_multiple_reg_t, p_resp_write_multiple_reg_t);
extern uint32_t cb_func_code23(p_req_read_write_multiple_reg_t, p_resp_read_write_multiple_reg_t);

/*============================================================================*/
/* S T A T I C   F U N C T I O N   P R O T O T Y P E                          */
/*============================================================================*/

/*============================================================================*/
/* P R O G R A M                                                              */
/*============================================================================*/
uint32_t modbus_init(void);

/**
 ******************************************************************************
 @brief  Initialize MODBUS protocol stack
 @param  none
 @retval error code
 ******************************************************************************
 */
uint32_t modbus_init(void)
{
    uint32_t ercd;
    slave_map_init_t st_slave_map;

    /* Enable IP table */
    Modbus_tcp_init_ip_table (ENABLE, ACCEPT);

    /* register IP address */
    ercd = Modbus_tcp_add_ip_addr ("192.168.1.101");
    if (ercd != ERR_OK)
    {
        return ercd;
    }

    /* register callback functions */
    st_slave_map.fp_function_code1 = cb_func_code01; /* Read Coils operation */
    st_slave_map.fp_function_code2 = cb_func_code02; /* Read Discrete Inputs operation */
    st_slave_map.fp_function_code3 = cb_func_code03; /* Read Holding Registers operation */
    st_slave_map.fp_function_code4 = cb_func_code04; /* Read Input Registers operation */
    st_slave_map.fp_function_code5 = cb_func_code05; /* Write Single Coil operation */
    st_slave_map.fp_function_code6 = cb_func_code06; /* Write Single Register operation */
    st_slave_map.fp_function_code15 = cb_func_code15; /* Write Multiple Coils operation */
    st_slave_map.fp_function_code16 = cb_func_code16; /* Write Multiple Registers operation */
    st_slave_map.fp_function_code23 = cb_func_code23; /* Read/Write Multiple Registers operation */
    ercd = Modbus_slave_map_init (&st_slave_map);
    if (ercd != ERR_OK)
    {
        return ercd;
    }


    ercd = Modbus_tcp_server_init_stack (0,
                                         ENABLE_MULTIPLE_CLIENT_CONNECTION);
    return ercd;
}

/* End of file */
