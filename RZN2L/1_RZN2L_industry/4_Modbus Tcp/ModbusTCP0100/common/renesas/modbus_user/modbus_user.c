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
#include "hal_data.h"

/*============================================================================*/
/* D E F I N E                                                                */
/*============================================================================*/

/*============================================================================*/
/* E X T E R N                                                                */
/*============================================================================*/
/* definition in modbus_func.c */
extern int MB_Func_Exp_Func(uint8_t *req, uint8_t *res, uint16_t cnt);
extern int MB_Func_Exp_Addr(uint8_t *res);
extern int MB_Func_Exp_Data(uint8_t *res);
extern int MB_Func_Exp_Execution(uint8_t *res, int type);

/* definition in board_leds.c in FSP */
extern bsp_leds_t g_bsp_leds;

/*============================================================================*/
/* S T A T I C   F U N C T I O N   P R O T O T Y P E                          */
/*============================================================================*/
/* function for address error */
uint8_t MB_Reg_Exp_Addr(uint16_t data);

/************************************************
 READ_COILS(0x01) functions
 */
uint8_t MB_Coil_Read_00001(uint8_t *data);
uint8_t MB_Coil_Read_00002(uint8_t *data);
uint8_t MB_Coil_Read_00003(uint8_t *data);
uint8_t MB_Coil_Read_00004(uint8_t *data);
uint8_t MB_Coil_Read_00005(uint8_t *data);
uint8_t MB_Coil_Read_00006(uint8_t *data);
uint8_t MB_Coil_Read_00007(uint8_t *data);
uint8_t MB_Coil_Read_00008(uint8_t *data);

/************************************************
 WRITE_SINGLE_COIL(0x05) functions
 */
uint8_t MB_Coil_Write_00001(uint8_t data);
uint8_t MB_Coil_Write_00002(uint8_t data);
uint8_t MB_Coil_Write_00003(uint8_t data);
uint8_t MB_Coil_Write_00004(uint8_t data);
uint8_t MB_Coil_Write_00005(uint8_t data);
uint8_t MB_Coil_Write_00006(uint8_t data);
uint8_t MB_Coil_Write_00007(uint8_t data);
uint8_t MB_Coil_Write_00008(uint8_t data);

/************************************************
 READ DISCRETE INPUTS(0x02) functions
 */
uint8_t MB_D_Read_10001(uint8_t *data);
uint8_t MB_D_Read_10002(uint8_t *data);
uint8_t MB_D_Read_10003(uint8_t *data);
uint8_t MB_D_Read_10004(uint8_t *data);
uint8_t MB_D_Read_10005(uint8_t *data);
uint8_t MB_D_Read_10006(uint8_t *data);
uint8_t MB_D_Read_10007(uint8_t *data);
uint8_t MB_D_Read_10008(uint8_t *data);
uint8_t MB_D_Read_10009(uint8_t *data);
/* Generate exception at 10010 by MB_Reg_Exp_Addr() */
uint8_t MB_D_Read_10011(uint8_t *data);
uint8_t MB_D_Read_10012(uint8_t *data);

/************************************************
 READ_INPUT_REGISTERS(0x04) functions
 */
uint8_t MB_IReg_Read_30001(uint16_t *data);
uint8_t MB_IReg_Read_30002(uint16_t *data);
uint8_t MB_IReg_Read_30003(uint16_t *data);
/* Generate exception at 30004 by MB_Reg_Exp_Addr() */
/* Generate exception at 30005 by MB_Reg_Exp_Addr() */
/* Generate exception at 30006 by MB_Reg_Exp_Addr() */
/* Generate exception at 30007 by MB_Reg_Exp_Addr() */
uint8_t MB_IReg_Read_30008(uint16_t *data);

/************************************************
 READ_HOLDING_REGISTERS(0x03) functions
 */
uint8_t MB_Reg_Read_40001(uint16_t *data);
uint8_t MB_Reg_Read_40002(uint16_t *data);
uint8_t MB_Reg_Read_40003(uint16_t *data);
/* Generate exception at 40004 by MB_Reg_Exp_Addr() */
/* Generate exception at 40005 by MB_Reg_Exp_Addr() */
/* Generate exception at 40006 by MB_Reg_Exp_Addr() */
uint8_t MB_Reg_Read_40007(uint16_t *data);

/************************************************
 WRITE_SINGLE_REGISTER(0x06) functions
 */
uint8_t MB_Reg_Write_40001(uint16_t data);
uint8_t MB_Reg_Write_40002(uint16_t data);
uint8_t MB_Reg_Write_40003(uint16_t data);
/* Generate exception at 40004 by MB_Reg_Exp_Addr() */
/* Generate exception at 40005 by MB_Reg_Exp_Addr() */
/* Generate exception at 40006 by MB_Reg_Exp_Addr()*/
uint8_t MB_Reg_Write_40007(uint16_t data);

/*============================================================================*/
/* D E F I N E                                                                */
/*============================================================================*/

/* for holding register and input register */
#define MB_Reg_Exp_Addr_p8	((uint8_t (*)(uint8_t  *data))MB_Reg_Exp_Addr)
#define MB_Reg_Exp_Addr_p16	((uint8_t (*)(uint16_t *data))MB_Reg_Exp_Addr)
#define MB_Reg_Exp_Addr_8	((uint8_t (*)(uint8_t  data))MB_Reg_Exp_Addr)
#define MB_Reg_Exp_Addr_16	((uint8_t (*)(uint16_t data))MB_Reg_Exp_Addr)

/*============================================================================*/
/* V A R I A B L E                                                            */
/*============================================================================*/

/************************************************
 READ_COILS(0x01) functions
 */
uint8_t (*MB_Coils_Write[])(uint8_t data)=
{
    MB_Coil_Write_00001, /* 00001 */
    MB_Coil_Write_00002, /* 00002 */
    MB_Coil_Write_00003, /* 00003 */
    MB_Coil_Write_00004, /* 00004 */
    MB_Coil_Write_00005, /* 00005 */
    MB_Coil_Write_00006, /* 00006 */
    MB_Coil_Write_00007, /* 00007 */
    MB_Coil_Write_00008, /* 00008 */
};

/************************************************
 WRITE_SINGLE_COIL(0x05) functions
 */
uint8_t (*MB_Coils_Read[])(uint8_t *data)=
{
    MB_Coil_Read_00001, /* 00001 */
    MB_Coil_Read_00002, /* 00002 */
    MB_Coil_Read_00003, /* 00003 */
    MB_Coil_Read_00004, /* 00004 */
    MB_Coil_Read_00005, /* 00005 */
    MB_Coil_Read_00006, /* 00006 */
    MB_Coil_Read_00007, /* 00007 */
    MB_Coil_Read_00008, /* 00008 */
};

/************************************************
 READ DISCRETE INPUTS(0x02) functions
 */
uint8_t (*MB_Discretes_Input[])(uint8_t *data)=
{
    MB_D_Read_10001, /* 10001 */
    MB_D_Read_10002, /* 10002 */
    MB_D_Read_10003, /* 10003 */
    MB_D_Read_10004, /* 10004 */
    MB_D_Read_10005, /* 10005 */
    MB_D_Read_10006, /* 10006 */
    MB_D_Read_10007, /* 10007 */
    MB_D_Read_10008, /* 10008 */
    MB_D_Read_10009, /* 10009 */
    MB_Reg_Exp_Addr_p8, /* 10010 */
    MB_D_Read_10011, /* 10011 */
    MB_D_Read_10012, /* 10012 */
};

/************************************************
 READ_INPUT_REGISTERS(0x04) functions
 */
uint8_t (*MB_Input_Regs[])(uint16_t *data)=
{
    MB_IReg_Read_30001, /* 30001 */
    MB_IReg_Read_30002, /* 30002 */
    MB_IReg_Read_30003, /* 30003 */
    MB_Reg_Exp_Addr_p16, /* 30004 */
    MB_Reg_Exp_Addr_p16, /* 30005 */
    MB_Reg_Exp_Addr_p16, /* 30006 */
    MB_Reg_Exp_Addr_p16, /* 30007 */
    MB_IReg_Read_30008, /* 30008 */
};

/************************************************
 READ_HOLDING_REGISTERS(0x03) functions
 */
uint8_t (*MB_HoldingRegs_Read[])(uint16_t *data)=
{
    MB_Reg_Read_40001, /* 40001 */
    MB_Reg_Read_40002, /* 40002 */
    MB_Reg_Read_40003, /* 40003 */
    MB_Reg_Exp_Addr_p16, /* 40004 */
    MB_Reg_Exp_Addr_p16, /* 40005 */
    MB_Reg_Exp_Addr_p16, /* 40006 */
    MB_Reg_Read_40007, /* 40007 */
};

/************************************************
 WRITE_SINGLE_REGISTER(0x06) functions
 */
uint8_t (*MB_HoldingRegs_Write[])(uint16_t data)=
{
    MB_Reg_Write_40001, /* 40001 */
    MB_Reg_Write_40002, /* 40002 */
    MB_Reg_Write_40003, /* 40003 */
    MB_Reg_Exp_Addr_16, /* 40004 */
    MB_Reg_Exp_Addr_16, /* 40005 */
    MB_Reg_Exp_Addr_16, /* 40006 */
    MB_Reg_Write_40007 /* 40007 */
};

uint8_t g_coils_area[] =
{ 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t g_discrete_input_area[] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
uint16_t g_input_reg_area[] =
{ 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x00008 };
uint16_t g_holding_reg_area[] =
{ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };

uint32_t g_coil_end_addr = sizeof(MB_Coils_Read) / sizeof(MB_Coils_Read[0]);
uint32_t g_discrete_input_end_addr = sizeof(MB_Discretes_Input) / sizeof(MB_Discretes_Input[0]);
uint32_t g_input_reg_end_addr = sizeof(MB_Input_Regs) / sizeof(MB_Input_Regs[0]);
uint32_t g_holding_reg_end_addr = sizeof(MB_HoldingRegs_Read) / sizeof(MB_HoldingRegs_Read[0]);

/*============================================================================*/
/* P R O G R A M                                                              */
/*============================================================================*/

/* ERR_ILLEGAL_DATA_ADDRESS is changed to ERR_SLAVE_DEVICE_FAILURE in MB_Func_Exp_Execution() */
uint8_t MB_Reg_Exp_Addr(uint16_t data)
{
    (void) data;
    return ERR_ILLEGAL_DATA_ADDRESS;
}

/************************************************
 WRITE_SINGLE_COIL (0x05) functions
 */
uint8_t MB_Coil_Write_00001(uint8_t data)
{
   g_coils_area[0] = data & 0x01;
   R_BSP_PinAccessEnable ();
   R_BSP_PinWrite ((bsp_io_port_pin_t) g_bsp_leds.p_leds[0], (bsp_io_level_t) g_coils_area[0]);
   R_BSP_PinAccessDisable ();
    return ERR_OK;
}
uint8_t MB_Coil_Write_00002(uint8_t data)
{
   g_coils_area[1] = data & 0x01;
   R_BSP_PinAccessEnable ();
   R_BSP_PinWrite ((bsp_io_port_pin_t) g_bsp_leds.p_leds[1], (bsp_io_level_t) g_coils_area[1]);
   R_BSP_PinAccessDisable ();
    return ERR_OK;
}
uint8_t MB_Coil_Write_00003(uint8_t data)
{
   g_coils_area[2] = data & 0x01;
   R_BSP_PinAccessEnable ();
   R_BSP_PinWrite ((bsp_io_port_pin_t) g_bsp_leds.p_leds[2], (bsp_io_level_t) g_coils_area[2]);
   R_BSP_PinAccessDisable ();
    return ERR_OK;
}
uint8_t MB_Coil_Write_00004(uint8_t data)
{
    g_coils_area[3] = data & 0x01;
#if defined (BOARD_RZT2M_RSK)
    R_BSP_PinAccessEnable ();
	R_BSP_PinWrite ((bsp_io_port_pin_t) g_bsp_leds.p_leds[3], (bsp_io_level_t) g_coils_area[3]);
	R_BSP_PinAccessDisable ();

#elif defined (BOARD_RZN2L_RSK)
    R_BSP_PinAccessEnable ();
    R_BSP_PinWrite ((bsp_io_port_pin_t) g_bsp_leds.p_leds[3], (bsp_io_level_t) g_coils_area[3]);
    R_BSP_PinAccessDisable ();

#else

#endif
    return ERR_OK;
}
uint8_t MB_Coil_Write_00005(uint8_t data)
{
    g_coils_area[4] = data & 0x01;
    return ERR_OK;
}
uint8_t MB_Coil_Write_00006(uint8_t data)
{
    g_coils_area[5] = data & 0x01;
    return ERR_OK;
}
uint8_t MB_Coil_Write_00007(uint8_t data)
{
    g_coils_area[6] = data & 0x01;
    return ERR_OK;
}
uint8_t MB_Coil_Write_00008(uint8_t data)
{
    g_coils_area[7] = data & 0x01;
    return ERR_OK;
}

/************************************************
 READ_COILS(0x01) functions
 */
uint8_t MB_Coil_Read_00001(uint8_t *coil)
{
    R_BSP_PinAccessEnable ();
    g_coils_area[0] = (uint8_t) R_BSP_PinRead ((bsp_io_port_pin_t) g_bsp_leds.p_leds[0]);
    R_BSP_PinAccessDisable ();
    *coil = g_coils_area[0];
    return ERR_OK;
}
uint8_t MB_Coil_Read_00002(uint8_t *coil)
{
    R_BSP_PinAccessEnable ();
    g_coils_area[1] = (uint8_t) R_BSP_PinRead ((bsp_io_port_pin_t) g_bsp_leds.p_leds[1]);
    R_BSP_PinAccessDisable ();
    *coil = g_coils_area[1];
    return ERR_OK;
}
uint8_t MB_Coil_Read_00003(uint8_t *coil)
{
    R_BSP_PinAccessEnable ();
    g_coils_area[2] = (uint8_t) R_BSP_PinRead ((bsp_io_port_pin_t) g_bsp_leds.p_leds[2]);
    R_BSP_PinAccessDisable ();
    *coil = g_coils_area[2];
    return ERR_OK;
}
uint8_t MB_Coil_Read_00004(uint8_t *coil)
{
    *coil = g_coils_area[3];
#if defined(BOARD_RZT2M_RSK)
    R_BSP_PinAccessEnable ();
    g_coils_area[3] = (uint8_t) R_BSP_PinRead ((bsp_io_port_pin_t) g_bsp_leds.p_leds[3]);
    R_BSP_PinAccessDisable ();
    *coil = g_coils_area[3];

#elif defined(BOARD_RZN2L_RSK)
   R_BSP_PinAccessEnable ();
   g_coils_area[3] = (uint8_t) R_BSP_PinRead ((bsp_io_port_pin_t) g_bsp_leds.p_leds[3]);
   R_BSP_PinAccessDisable ();
   *coil = g_coils_area[3];
#else

#endif
   return ERR_OK;
}
uint8_t MB_Coil_Read_00005(uint8_t *coil)
{
    *coil = g_coils_area[4];
    return ERR_OK;
}
uint8_t MB_Coil_Read_00006(uint8_t *coil)
{
    *coil = g_coils_area[5];
    return ERR_OK;
}
uint8_t MB_Coil_Read_00007(uint8_t *coil)
{
    *coil = g_coils_area[6];
    return ERR_OK;
}
uint8_t MB_Coil_Read_00008(uint8_t *coil)
{
    *coil = g_coils_area[7];
    return ERR_OK;
}

/************************************************
 READ_DISCRETE_INPUTS (0x03) functions
 */
uint8_t MB_D_Read_10001(uint8_t *state)
{
    *state = g_discrete_input_area[0];
    return ERR_OK;
}
uint8_t MB_D_Read_10002(uint8_t *state)
{
    *state = g_discrete_input_area[1];
    return ERR_OK;
}
uint8_t MB_D_Read_10003(uint8_t *state)
{
    *state = g_discrete_input_area[2];
    return ERR_OK;
}
uint8_t MB_D_Read_10004(uint8_t *state)
{
    *state = g_discrete_input_area[3];
    return ERR_OK;
}
uint8_t MB_D_Read_10005(uint8_t *state)
{
#if defined (BOARD_RA6M3_EK)

#elif defined (BOARD_RZT2M_RSK)
   uint32_t pin = 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_11_PIN_0);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[4] = (uint8_t) pin ^ 0x01;
#elif defined (BOARD_RZN2L_RSK)
   uint32_t pin = 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_13_PIN_6);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[4] = (uint8_t) pin ^ 0x01;
#else

#endif
    *state = g_discrete_input_area[4];
    return ERR_OK;
}
uint8_t MB_D_Read_10006(uint8_t *state)
{
#if defined (BOARD_RA6M3_EK)
    uint32_t pin = 0;
    R_BSP_PinAccessEnable ();
    pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_00_PIN_09);
    R_BSP_PinAccessDisable ();
    g_discrete_input_area[5] = (uint8_t) pin ^ 0x01;
#elif defined (BOARD_RZT2M_RSK)
   uint32_t pin = 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_11_PIN_3);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[5] = (uint8_t) pin ^ 0x01;
#elif defined (BOARD_RZN2L_RSK)
   uint32_t pin = 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_13_PIN_5);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[5] = (uint8_t) pin ^ 0x01;
#else

#endif
    *state = g_discrete_input_area[5];
    return ERR_OK;
}
uint8_t MB_D_Read_10007(uint8_t *state)
{
#if defined (BOARD_RA6M3_EK)
    uint32_t pin= 0;
    R_BSP_PinAccessEnable ();
    pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_00_PIN_08);
    R_BSP_PinAccessDisable ();
    g_discrete_input_area[6] = (uint8_t) pin ^ 0x01;
#elif defined (BOARD_RZT2M_RSK)
   uint32_t pin= 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_11_PIN_4);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[6] = (uint8_t) pin ^ 0x01;
#elif defined (BOARD_RZN2L_RSK)
   uint32_t pin= 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_14_PIN_0);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[6] = (uint8_t) pin ^ 0x01;
#else

#endif
    *state = g_discrete_input_area[6];
    return ERR_OK;
}
uint8_t MB_D_Read_10008(uint8_t *state)
{
#if defined (BOARD_RA6M3_EK)

#elif defined (BOARD_RZT2M_RSK)
   uint32_t pin = 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_11_PIN_6);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[7] = (uint8_t) pin ^ 0x01;
#elif defined (BOARD_RZN2L_RSK)
   uint32_t pin = 0;
   R_BSP_PinAccessEnable ();
   pin = R_BSP_PinRead ((bsp_io_port_pin_t)BSP_IO_PORT_13_PIN_7);
   R_BSP_PinAccessDisable ();
   g_discrete_input_area[7] = (uint8_t) pin ^ 0x01;
#else

#endif
    *state = g_discrete_input_area[7];
    return ERR_OK;
}
uint8_t MB_D_Read_10009(uint8_t *state)
{
    *state = g_discrete_input_area[8];
    return ERR_OK;
}
/* Generate exception at 10010 by MB_Reg_Exp_Addr() */
uint8_t MB_D_Read_10011(uint8_t *state)
{
    *state = g_discrete_input_area[10];
    return ERR_OK;
}
uint8_t MB_D_Read_10012(uint8_t *state)
{
    *state = g_discrete_input_area[11];
    return ERR_OK;
}

/************************************************
 READ_INPUT_REGISTERS(0x04) functions
 */
uint8_t MB_IReg_Read_30001(uint16_t *data)
{
    *data = g_input_reg_area[0];
    return ERR_OK;
}
uint8_t MB_IReg_Read_30002(uint16_t *data)
{
    *data = g_input_reg_area[1];
    return ERR_OK;
}
uint8_t MB_IReg_Read_30003(uint16_t *data)
{
    *data = g_input_reg_area[2];
    return ERR_OK;
}
/* Generate exception at 30004 by MB_Reg_Exp_Addr() */
/* Generate exception at 30005 by MB_Reg_Exp_Addr() */
/* Generate exception at 30006 by MB_Reg_Exp_Addr() */
/* Generate exception at 30007 by MB_Reg_Exp_Addr() */
uint8_t MB_IReg_Read_30008(uint16_t *data)
{
    *data = g_input_reg_area[7];
    return ERR_OK;
}

/************************************************
 READ_HOLDING_REGISTERS(0x03) functions
 */
uint8_t MB_Reg_Read_40001(uint16_t *data)
{
    *data = g_holding_reg_area[0];
    return ERR_OK;
}
uint8_t MB_Reg_Read_40002(uint16_t *data)
{
    *data = g_holding_reg_area[1];
    return ERR_OK;
}
uint8_t MB_Reg_Read_40003(uint16_t *data)
{
    *data = g_holding_reg_area[2];
    return ERR_OK;
}
/* Generate exception at 40004 by MB_Reg_Exp_Addr() */
/* Generate exception at 40005 by MB_Reg_Exp_Addr() */
/* Generate exception at 40006 by MB_Reg_Exp_Addr() */
uint8_t MB_Reg_Read_40007(uint16_t *data)
{
    *data = g_holding_reg_area[6];
    return ERR_OK;
}

/************************************************
 WRITE_SINGLE_REGISTER(0x06) functions
 */
uint8_t MB_Reg_Write_40001(uint16_t data)
{
    g_holding_reg_area[0] = data;
    return ERR_OK;
}
uint8_t MB_Reg_Write_40002(uint16_t data)
{
    g_holding_reg_area[1] = data;
    return ERR_OK;
}
/* ERR_ILEEGAL_DATA_VALUE is changed to ERR_SLAVE_DEVICE_FAILURE in MB_Func_Exp_Execution() */
uint8_t MB_Reg_Write_40003(uint16_t data)
{
    if (data > 10)
    {
        return ERR_ILLEGAL_DATA_VALUE;
    }
    g_holding_reg_area[2] = data;
    return ERR_OK;
}
/* Generate exception at 40004 by MB_Reg_Exp_Addr() */
/* Generate exception at 40005 by MB_Reg_Exp_Addr() */
/* Generate exception at 40006 by MB_Reg_Exp_Addr() */
uint8_t MB_Reg_Write_40007(uint16_t data)
{
    g_holding_reg_area[6] = data;
    return ERR_OK;
}

/* End of File */
