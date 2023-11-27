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
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_communication_control_api.c
 * Version      : 1.0
 * Description  : communication control module
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 31.08.2019 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "r_ring_buffer_control_api.h"
#include "r_communication_control_api.h"

/**********************************************************************************************************************
 Includes
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Private (static) variables and functions
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
size_t R_COMM_GetPacket (st_ring_buf_t *r_buf, uint8_t r_packet[])
{
    /** Get length of unprocessed data in receive ring buffer  */
    size_t data_len = R_RINGBUF_GetData(r_buf, r_packet, D_HEADER_LENGTH, D_NO_UPDATE);

    /** When the unprocessed data is longer than the Header + Data Length */
    if (D_HEADER_LENGTH <= data_len)
    {
        /** Get one packet from the receive ring buffer and find its length */
        data_len = R_RINGBUF_GetData(r_buf, r_packet, (size_t) (D_HEADER_LENGTH + r_packet[D_DATA_LENGTH]), D_UPDATE);
    }
    return data_len;
}
/**********************************************************************************************************************
 End of function R_COMM_GetPacket
 *********************************************************************************************************************/
