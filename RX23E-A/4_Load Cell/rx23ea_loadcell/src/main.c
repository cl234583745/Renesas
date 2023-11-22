/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : main.c
 * Version      : 1.0
 * Device(s)    : R5F23E6AxFL
 * Tool-Chain   : e2studio, Renesas CCRX(v3.02.00)
 * H/W Platform : RSSKRX23E-A
 * Description  : This is the main tutorial code.
 *********************************************************************************************************************/
/*********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.08.2019 1.00     First Release
 *         : 20.07.2020 1.10     Update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "r_smc_entry.h"

#include "r_ring_buffer_control_api.h"
#include "r_communication_control_api.h"
#include "r_sensor_common_api.h"
#include "r_loadcell_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

#define D_PRV_SAMPLE_NUM (8)                            /** Number of moving average samples */

#ifndef  D_PRV_PC_TOOL_USE
    #define D_PRV_PC_TOOL_USE (1)                       /** 0:PC Tool not used, 1:PC Tool used              */
#endif

#if (D_PRV_PC_TOOL_USE == 1)
    #define D_PRV_SEND_ARY_LEN      (4096)              /** Transmission data storage array length          */
    #define D_PRV_RECV_ARY_LEN      (512)               /** Receive data storage array length               */
    #define D_PRV_SEND_PACKET_LEN   (257)               /** Transmission packet generation buffer length    */
    #define D_PRV_RECV_PACKET_LEN   (257)               /** Received packet analysis buffer length          */
#endif

#if (__BIG == 1)                                        /** Get endian                                      */
    #define     D_PRV_ENDIAN (D_BIG_ENDIANNESS)         /** Negotiation response                            */
#else
    #define     D_PRV_ENDIAN (D_LITTLE_ENDIANNESS)      /** Negotiation response                            */
#endif

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables (to be accessed by other files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

static volatile float   s_weight_zero = 0.0F;           /** Zero weight [g]                                 */

static float    s_ad_array[D_PRV_SAMPLE_NUM];           /** Array for moving avarage calculation            */
static bool     s_zero_req = false;                     /** Zero reset request                              */
static float    s_coef_a   = D_LC_COEFFICIENT_A;        /** Weight conversion coefficient A                 */
static float    s_coef_b   = D_LC_COEFFICIENT_B;        /** Weight conversion coefficient B                 */

/** Monitor variable definition (reference only) */
static volatile float   s_weight = 0.0F;                /** Measured weight [g]                             */
static volatile int32_t s_dsad0_value;                  /** DSAD0 24bit A/D value storage variable          */

#if (D_PRV_PC_TOOL_USE == 1)
static uint8_t s_send_pkt[D_PRV_SEND_PACKET_LEN];       /** Buffer for generating transmission packet       */
static uint8_t s_recv_pkt[D_PRV_RECV_PACKET_LEN];       /** Buffer for analyzing received packet            */
#pragma section B B_DMAC_REPEAT_AREA                    /** Address alignment for DMAC repeat area          */
    static uint8_t s_send_ary[D_PRV_SEND_ARY_LEN];      /** DMAC3 transfer source                           */
    static uint8_t s_recv_ary[D_PRV_RECV_ARY_LEN];      /** DMAC0 transfer destination                      */
#pragma section
#endif

/* functions */
#if (D_PRV_PC_TOOL_USE == 1)
static size_t analysis_packet (uint8_t const rcv_pkt[], uint8_t send_pkt[], bool *p_tx_flag);
static void stop_operation (st_ring_buf_t *ary);
#endif

static bool sw_check (void);

void BSP_CFG_USER_CHARPUT_FUNCTION(char output_char)
{}
/**********************************************************************************************************************
 * Function Name: main
 * Description  : This function implements main function.
 * Arguments    : None
 * Return Value : None
 *********************************************************************************************************************/
void main (void)
{
    int32_t data_index = 0;                         /** Write index for array of moving average */

#if (D_PRV_PC_TOOL_USE == 1)
    bool  weight_update_flag = false;               /** Weight update flag.
                                                      * false: not updated, true: updated */
    bool weight_tx_flag = false;                    /** Transmission permission flag to PC tool.
                                                      * false: Prohibited, true: permitted */

    st_ring_buf_t recv_ring_buf =
    { s_recv_ary, D_PRV_RECV_ARY_LEN, 0, 0 };       /** Receive ring buffer     */
    st_ring_buf_t send_ring_buf =
    { s_send_ary, D_PRV_SEND_ARY_LEN, 0, 0 };       /** Transmit ring buffer    */
#endif

    /********************/
    /** Initial setting */
    /********************/
    memset(s_ad_array, 0, sizeof(s_ad_array));      /** filled moving average array by 0.0 */

#if (D_PRV_PC_TOOL_USE == 1)
    R_DMAC0_SetDestAddr(recv_ring_buf.p_buf);       /** Set receive ring buffer as DMAC0 transfer destination */
    R_DMAC3_SetSrcAddr(send_ring_buf.p_buf);        /** Set transmit ring buffer as DMAC3 transfer source */

    /** Receive start */
    R_Config_DMAC0_Start();
    R_SCI1_ReceiveStart();
#endif

    /** A/D conversion start */
    R_Config_DSAD0_Start();
    R_Config_DSAD0_Set_SoftwareTrigger();

    while (1)
    {
        /** Check SW input */
        if (false == s_zero_req)
        {
            s_zero_req = sw_check();
        }

        /***********************/
        /** Weight measurement */
        /***********************/
        /** A/D conversion complete for DSAD0? */
        if (true == R_DSAD0_IsConversionEnd())
        {
            int32_t dsad0_value;        /** Signed 24-bit DSAD0 value                                   */
            static float   dsad0_ave;          /** DSAD0 average value                                         */
            static float   weight_value;       /** Measured weight (after correction) [g]                      */

            R_DSAD0_ClearIrFlag();      /** clear DSAD0 IR flag                                         */

            /** Get A/D value */
            {
            	static uint32_t dsad0_reg;     /** DSAD0.DR register storage variable                          */

                R_Config_DSAD0_Get_ValueResult( &dsad0_reg);

                /** Flag mask, Sign extension */
                dsad0_value = (int32_t) ((dsad0_reg & 0x00FFFFFFU) << 8) >> 8;
            }

            /** Calculate moving average of Load cell A/D conversion value[LSB] */
            dsad0_ave = R_CALC_MovingAverage((float)dsad0_value, s_ad_array, (size_t) D_PRV_SAMPLE_NUM, &data_index);


            //s_coef_a = (M2-M1)/(dsad2-dsad1)
            //s_coef_b = M1-s_coef_a*dsad1 =  M2-s_coef_a*dsad2
            //s_coef_a = (900-10)/(0x8b950-0xffffddd0)
			//s_coef_b = 10-s_coef_a*dsad1 =  900-s_coef_a*dsad2
            s_coef_a = (900.0F-10.0F)/(0x8b950-0xffffddd0);
            //s_coef_b = 10.0F-s_coef_a * 0xffffddd0;
            s_coef_b = 900.0F-s_coef_a * 0x8b950;
            /** Calculate weight [g] from A/D conversion value[LSB] of Load cell */
            weight_value = R_LC_DsadToWeight(dsad0_ave, s_coef_a, s_coef_b);

            /** Zero reset */
            if (true == s_zero_req)
            {
                s_zero_req    = false;
                s_weight_zero = weight_value;
            }

            /** Calculate corrected weight from measured weight and zero weight */
            weight_value = weight_value - s_weight_zero;

            /** debug monitor */
            {
                s_dsad0_value = dsad0_value;
                s_weight      = weight_value;
            }
#if (D_PRV_PC_TOOL_USE == 1)
            weight_update_flag = true;  /** Weight data update flag set */
#endif
        }

#if (D_PRV_PC_TOOL_USE == 1)
        /**************************/
        /** Communication control */
        /**************************/
        {
            /******************************/
            /** Receive packet processing */
            /******************************/
            /** update the write index of receive buffer */
            {
                /** Cast address value to uint32_t. Calculate address difference. */
                uint32_t index = (uint32_t) R_DMAC0_GetDestAddr() - (uint32_t) recv_ring_buf.p_buf;
                R_RINGBUF_SetDataIndex( &recv_ring_buf, index, D_W_INDEX);
            }

            /** 1 packet acquisition? */
            if (0 < R_COMM_GetPacket( &recv_ring_buf, s_recv_pkt))
            {
                /** Analysis, execution, response packet generation */
                size_t packet_len = analysis_packet(s_recv_pkt, s_send_pkt, &weight_tx_flag);

                /** Set response in send ring buffer */
                if (0U == R_RINGBUF_SetData( &send_ring_buf, s_send_pkt, packet_len))
                {
                    /** ERROR:Stop transmission when cannot store paket */
                    stop_operation( &send_ring_buf);
                    weight_tx_flag = false;
                }
            }

            /***************************************/
            /** Measurement data packet generation */
            /***************************************/
            /** Weight transmission permitted and Weight updated */
            if ((true == weight_tx_flag) && (true == weight_update_flag))
            {
                size_t packet_len;

                /** Pointer variable for accessing send_pkt [D_DATA] as float type */
                float *p_send_pkt = (float *) &s_send_pkt[D_DATA];

                weight_update_flag = false; /** Weight data update flag clear */

                /** Weight packet generation */
                s_send_pkt[D_HEADER]        = D_RE_TRANSMISSION_CH0;
                s_send_pkt[D_DATA_LENGTH]   = D_WEIGHT_DATA_LENGTH;
                *p_send_pkt                 = s_weight;
                packet_len                  = D_HEADER_LENGTH + D_WEIGHT_DATA_LENGTH;

                /** Set data in send ring buffer */
                if (0U == R_RINGBUF_SetData( &send_ring_buf, s_send_pkt, packet_len))
                {
                    /** ERROR:Stop transmission when cannot store paket */
                    stop_operation( &send_ring_buf);
                    weight_tx_flag = false;
                }
            }

            /***********************************/
            /** Packet transmission processing */
            /***********************************/
            {
                /** Acquisition of transmission byte count */
                size_t data_len = R_RINGBUF_GetDataLength( &send_ring_buf);

                /** Not transmitting, and unsent data in transmit buffer */
                if ((true == R_SCI1_IsTransferEnd()) && (0U < data_len))
                {
                    /** Transmission start */
                    R_DMAC3_SetTxCnt((uint32_t) data_len);
                    R_Config_DMAC3_Start();
                    R_SCI1_SendStart();

                    /** Update the receive buffer read index */
                    R_RINGBUF_SetDataIndex( &send_ring_buf, send_ring_buf.w_index, D_R_INDEX);

                    /** Timeout detection timer start */
                    R_CMT0_CntClear();
                    R_Config_CMT0_Start();
                }
            }

            /*************************************/
            /** Communication timeout processing */
            /*************************************/
            /** Timeout occurred */
            if (true == R_CMT0_IsTimeout(R_SCI1_IsTransferEnd()))
            {
                /** ERROR:Stop transmission when transmit time out */
                stop_operation( &send_ring_buf);
                weight_tx_flag = false;
            }
        }
#endif /** D_PRV_PC_TOOL_USE == 1 */
    }
}
/**********************************************************************************************************************
 End of function main
 *********************************************************************************************************************/

#if (D_PRV_PC_TOOL_USE == 1)
/**********************************************************************************************************************
 * Function Name: stop_operation
 * Description  : This function Transmission error processing. Transmisson stop, Buffer initialization.
 * Arguments    : st_ring_buf_t
 *                    Ring buffer pointer to reset to DMAC3
 * Return Value : None
 *********************************************************************************************************************/
static void stop_operation (st_ring_buf_t *ary)
{
    R_Config_DMAC3_Stop();
    R_SCI1_SendStop();

    /** Buffer initialization */
    R_DMAC3_SetSrcAddr(ary->p_buf);
    R_RINGBUF_SetDataIndex(ary, 0, D_R_INDEX);
    R_RINGBUF_SetDataIndex(ary, 0, D_W_INDEX);

    R_LED1_Off();
}
/**********************************************************************************************************************
 End of function stop_operation
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: analysis_packet
 * Description  : This function analyzes the received data.
 * Arguments    : uint8_t const rcv_pkt[]
 *                    Receive packet pointer
 *              : uint8_t send_pkt[]
 *                    Response packet pointer
 *              : bool * p_tx_flag
 *                    DSAD data transmission permission flag pointer
 * Return Value : size_t
 *                    Response data length
 *********************************************************************************************************************/
static size_t analysis_packet (uint8_t const rcv_pkt[], uint8_t send_pkt[], bool *p_tx_flag)
{
    size_t len = 0;

    /** Command analysis */
    switch (rcv_pkt[D_HEADER])
    {
        case D_NEGOTIATION:
        {
            /** Response packet generation */
            send_pkt[D_HEADER]      = D_RE_NEGOTIATION;
            send_pkt[D_DATA_LENGTH] = D_RE_NEGOTIATION_DATA_LENGTH;
            send_pkt[D_DATA]        = D_PRV_ENDIAN | D_CH0_FLOAT | D_CH1_NONE | D_READ_SUPPORT | D_WRITE_NOT_SUPPORT;
            len                     = D_HEADER_LENGTH + D_RE_NEGOTIATION_DATA_LENGTH;
            break;
        }
        case D_READ:
        {
            uint32_t    read_num    = (rcv_pkt[D_READ_NUM] & 0x1F);             /** Get the number of Read.
                                                                                  * Mask with 0x1F of maximum value */
            uint32_t    read_byte   = read_num * 4;                             /** Number of read bytes.
                                                                                  * Maximum value 124               */
            uint32_t    *p_src_addr = (uint32_t *) *(uint32_t *) &rcv_pkt[D_START_ADDR];   /** Set Read address     */
            uint32_t    *p_dst_addr = (uint32_t *) &send_pkt[D_START_ADDR];     /** Pointer to store read data      */

            /** Response packet generation */
            send_pkt[D_HEADER]      = D_RE_READ;                                /** Header                          */

            send_pkt[D_DATA_LENGTH] = (uint8_t) (D_ADDR_LENGTH + read_byte);    /** Data Length. Maximum value 128  */
            *p_dst_addr             = (uint32_t) p_src_addr;                    /** read address                    */
            p_dst_addr++;

            for (uint8_t i = 0; i < read_num; i++)
            {
                *p_dst_addr++ = *p_src_addr++;                                  /** address read                    */
            }

            len = D_HEADER_LENGTH + D_ADDR_LENGTH + read_byte;                  /** Maximum value 130               */
            break;
        }
        case D_RUN:
        {
            /** Command execution  */
            *p_tx_flag = true;
            R_LED1_On();

            /** Response packet generation */
            send_pkt[D_HEADER]      = D_RE_RUN;                                 /** Header                          */
            send_pkt[D_DATA_LENGTH] = 0x00U;                                    /** Data Length                     */
            len                     = D_HEADER_LENGTH;
            break;
        }
        case D_STOP:
        {
            /** Command execution  */
            *p_tx_flag = false;
            R_LED1_Off();

            /** Response packet generation */
            send_pkt[D_HEADER]      = D_RE_STOP;
            send_pkt[D_DATA_LENGTH] = 0x00U;
            len                     = D_HEADER_LENGTH;
            break;
        }

        /** Not supported, undefined command */
        default:
        {
            /** Response packet generation */
            send_pkt[D_HEADER]      = (uint8_t) (rcv_pkt[D_HEADER] | D_NACK | D_RESPONSE); /** NACK response       */
            send_pkt[D_DATA_LENGTH] = 0x00U;
            len                     = D_HEADER_LENGTH;
            break;
        }
    }
    return len;
}
/**********************************************************************************************************************
 End of function analysis_packet
 *********************************************************************************************************************/
#endif /** D_PRV_PC_TOOL_USE == 1 */

/**********************************************************************************************************************
 * Function Name: sw_check
 * Description  : This function determines SW depression.
 * Arguments    : void
 * Return Value : bool
 *                    true:pressed, false:Not pressed
 *********************************************************************************************************************/
static bool sw_check (void)
{
    static uint8_t  s_sw_last       = 0U;
    static uint8_t  s_sw_last_fix   = 0U;
    uint8_t         sw_input        = 0U;
    bool            ret             = false;

    /** SW input */
    if (0U == D_SW_PORT)
    {
        /** SW ON */
        sw_input = 1U;
    }

    /** SW pressing judgment */
    if (sw_input == s_sw_last)
    {
        if ((0U == s_sw_last_fix) && (1U == sw_input))
        {
            /** SW press detection */
            ret = true;
        }
        s_sw_last_fix = sw_input;
    }
    s_sw_last = sw_input;

    return ret;
}
/**********************************************************************************************************************
 End of function sw_check
 *********************************************************************************************************************/

