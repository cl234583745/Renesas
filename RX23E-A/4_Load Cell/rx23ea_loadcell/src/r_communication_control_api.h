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
 * File Name    : communication_control_api.h
 * Version      : 1.0
 * Description  : Ring buffer control module header file
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.08.2019 1.00     First Release
 *         : 20.07.2020 1.10     Update
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "r_ring_buffer_control_api.h"

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_PRV_COMMUNICATION_CONTROL_API_H
    #define R_PRV_COMMUNICATION_CONTROL_API_H

/** for index */
    #define D_HEADER                            (0x00U)
    #define D_DATA_LENGTH                       (0x01U)
    #define D_DATA                              (0x02U)
    #define D_START_ADDR                        (0x02U)
    #define D_READ_NUM                          (0x06U)
    #define D_READ_DATA                         (0x06U)

/** for Header */
    #define D_ACK                               (0x00U)
    #define D_NACK                              (0x10U)
    #define D_COMMAND                           (0x00U)
    #define D_RESPONSE                          (0x20U)

/** command */
    #define D_NEGOTIATION                       (0x80U)
    #define D_READ                              (0x81U)
    #define D_WRITE                             (0x82U)
    #define D_RUN                               (0x83U)
    #define D_STOP                              (0x84U)

/** response(ACK) */
    #define D_RE_NEGOTIATION                    (D_NEGOTIATION | D_RESPONSE)
    #define D_RE_READ                           (D_READ        | D_RESPONSE)
    #define D_RE_WRITE                          (D_WRITE       | D_RESPONSE)
    #define D_RE_RUN                            (D_RUN         | D_RESPONSE)
    #define D_RE_STOP                           (D_STOP        | D_RESPONSE)
    #define D_RE_TRANSMISSION_CH0               (0xA5U)
    #define D_RE_TRANSMISSION_CH1               (0xA6U)

/** for Negotiation */
    #define D_LITTLE_ENDIANNESS                 (0x00U)
    #define D_BIG_ENDIANNESS                    (0x80U)
    #define D_CH0_NONE                          (0x00U)
    #define D_CH0_DSAD                          (0x40U)
    #define D_CH0_FLOAT                         (0x60U)
    #define D_CH1_NONE                          (0x00U)
    #define D_CH1_DSAD                          (0x10U)
    #define D_CH1_FLOAT                         (0x18U)
    #define D_READ_SUPPORT                      (0x04U)
    #define D_READ_NOT_SUPPORT                  (0x00U)
    #define D_WRITE_SUPPORT                     (0x02U)
    #define D_WRITE_NOT_SUPPORT                 (0x00U)

/** for Length */
    #define D_HEADER_LENGTH                     (0x02U)
    #define D_ADDR_LENGTH                       (0x04U)
    #define D_RE_NEGOTIATION_DATA_LENGTH        (0x01U)
    #define D_TEMP_DATA_LENGTH                  (0x04U)
    #define D_WEIGHT_DATA_LENGTH                (0x04U)

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions (to be accessed by other files)
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function Name: R_COMM_GetPacket
 * Description  : This function takes a packet from the ring buffer.
 * Arguments    : st_ringbuf_t * r_buf
 *                    Ring buffer structure pointer
 *              : uint8_t * p_packet[]
 *                    Storage pointer for p_packet
 * Return Value : size_t
 *                    packet length
 *********************************************************************************************************************/
size_t R_COMM_GetPacket (st_ring_buf_t *r_buf, uint8_t r_packet[]);

#endif /* R_PRV_COMMUNICATION_CONTROL_API_H */
