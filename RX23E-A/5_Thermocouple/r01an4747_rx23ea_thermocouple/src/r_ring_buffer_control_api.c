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
 * File Name    : r_ring_buffer_control_api.c
 * Version      : 1.0
 * Description  : Ring buffer control module
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
#include <machine.h>

#include "r_ring_buffer_control_api.h"

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
 * Function Name: R_RINGBUF_GetData
 * Description  : This function retrieves the specified number of data from the ring buffer.
 * Arguments    : ring_buf_t * ary
 *                    Ring buffer structure pointer
 *              : uint8_t data[]
 *                    Storage pointer for read data
 *              : size_t len
 *                    Read data length
 *              : bool r_index_update
 *                    Ring buffer index update flag
 *                    false: Do not update, true: Update
 * Return Value : size_t
 *                    Read data length
 *********************************************************************************************************************/
size_t R_RINGBUF_GetData (st_ring_buf_t *ary, uint8_t data[], size_t len, bool index_update)
{
    size_t      ret                 = 0U;
    size_t      length              = ary->length;
    uint32_t    r_index             = ary->r_index;
    size_t      stored_data_length = R_RINGBUF_GetDataLength(ary);

    /** There is more data than the value specified in the argument */
    if (stored_data_length >= len)
    {
        /** Calculate the length to the end of the buffer */
        size_t end_length = length - (size_t) r_index;

        /** Data length does not exceed end of buffer */
        if (end_length >= len)
        {
            /** Copy to data[] from ary */
            memcpy( &data[0U], &ary->p_buf[r_index], len);
        }
        /** Data length exceeds end of buffer */
        else
        {
            /** Calculate the remaining data length */
            size_t remain_length = len - end_length;

            /** Copy considering end of buffer */
            memcpy( &data[0U], &ary->p_buf[r_index], end_length);
            memcpy( &data[end_length], &ary->p_buf[0U], remain_length);

        }

        /** Update index request */
        if (true == index_update)
        {
            /** Update read index */
            ary->r_index = (r_index + (uint32_t) len) % length;
        }

        /** Set return value */
        ret = len;
    }
    return ret;
}
/**********************************************************************************************************************
 End of function R_RINGBUF_GetData
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RINGBUF_SetData
 * Description  : This function writes the specified number of data to the ring buffer.
 * Arguments    : st_ring_buf_t * ary
 *                    Ring buffer structure pointer
 *              : uint8_t data[]
 *                    Storage pointer for read data
 *              : size_t len
 *                    Set data length
 * Return Value : size_t
 *                    Set data length
 *********************************************************************************************************************/
size_t R_RINGBUF_SetData (st_ring_buf_t *ary, uint8_t data[], size_t len)
{
    size_t      ret                = 0U;
    size_t      free_data_length;
    size_t      length             = ary->length;
    uint32_t    r_index            = ary->r_index;
    uint32_t    w_index            = ary->w_index;

    /** The value of w_index is greater than the value of r_index */
    if (w_index >= r_index)
    {
        /** Space available in buffer */
        free_data_length = (size_t) (((uint32_t) length - w_index) + r_index);
    }
    /** Other */
    else
    {
        /** Space available in buffer */
        free_data_length = (size_t) (r_index - w_index);
    }

    /** When the capacity to be used is smaller than the free capacity of the buffer */
    if (free_data_length > len)
    {
        /** Calculate the length to the end of the buffer */
        size_t end_length = (size_t) ((uint32_t) length - w_index);

        /** Data does not exist across the end of the buffer */
        if (end_length >= len)
        {
            /** Copy to data[] from ary */
            memcpy( &ary->p_buf[w_index], &data[0], len);
        }
        /** Data exists beyond the end of the buffer */
        else
        {
            /** Calculate the remaining data length */
            size_t remain_length = len - end_length;

            /** Copy to data[] from ary considering buffer end  */
            memcpy( &ary->p_buf[w_index], &data[0], end_length);
            memcpy( &ary->p_buf[0], &data[end_length], remain_length);
        }

        /** Update write index */
        ary->w_index = (w_index + (uint32_t) len) % length;

        ret = len;
    }
    return ret;
}
/**********************************************************************************************************************
 End of function R_RINGBUF_SetData
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RINGBUF_GetDataLength
 * Description  : This function returns the number of data stored in the ring buffer.
 * Arguments    : ring_buf_t * ary
 *                    Ring buffer structure pointer
 * Return Value : uint32_t
 *                    Number of stored data
 *********************************************************************************************************************/
size_t R_RINGBUF_GetDataLength (st_ring_buf_t *ary)
{
    size_t stored_data_length   = 0;
    size_t length               = ary->length;
    uint32_t r_index            = ary->r_index;
    uint32_t w_index            = ary->w_index;

    /** Data does not exist across the end of the buffer */
    if (w_index >= r_index)
    {
        /** Get stored data length from index difference */
        stored_data_length = (size_t) (w_index - r_index);
    }
    /** Data exists beyond the end of the buffer */
    else
    {
        /** Acquire stored data length from index difference and buffer length */
        stored_data_length = (size_t) (((uint32_t) length - r_index) + w_index);
    }

    return stored_data_length;
}
/**********************************************************************************************************************
 End of function R_RINGBUF_GetDataLength
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_RINGBUF_SetDataIndex
 * Description  : This function updates the ring buffer index
 * Arguments    : ring_buf_t * ary
 *                    Ring buffer structure pointer
 *              : uint32_t value
 *                    Set index value
 *              : uint8_t select
 *                    Target index
 *                    0: Read index, 1: write index
 * Return Value : uint32_t
 *                    Set index value
 *********************************************************************************************************************/
uint32_t R_RINGBUF_SetDataIndex (st_ring_buf_t *ary, uint32_t value, uint8_t select)
{
    uint32_t ret = 0;

    /** Update read index */
    if (0U == select)
    {
        ary->r_index = value;
        ret = value;
    }
    /** Update write index */
    else if (1U == select)
    {
        ary->w_index = value;
        ret = value;
    }
    /** other */
    else
    {
        /** Do Nothing */
        nop();
    }
    return ret;
}
/**********************************************************************************************************************
 End of function R_RINGBUF_SetDataIndex
 *********************************************************************************************************************/
