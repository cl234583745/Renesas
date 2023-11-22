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
 * File Name    : r_ring_buffer_control_api.h
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

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef R_PRV_RING_BUFFER_CONTROL_API_H
    #define R_PRV_RING_BUFFER_CONTROL_API_H

    #define D_NO_UPDATE             (false)                  /** No index update  */
    #define D_UPDATE                (true)                   /** index update     */
    #define D_W_INDEX               (1U)                     /** Write index      */
    #define D_R_INDEX               (0U)                     /** Read index       */

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/** Structure declaration for ring buffer */
typedef struct
{
    uint8_t  *p_buf;                        /** Pointer of ring buffer                 */
    size_t   length;                        /** Length of ring buffer                  */
    uint32_t r_index;                       /** Read index of ring buffer              */
    uint32_t w_index;                       /** Write index of ring buffer             */
} st_ring_buf_t;

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
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
size_t R_RINGBUF_GetData (st_ring_buf_t *ary, uint8_t data[], size_t len, bool index_update);

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
size_t R_RINGBUF_SetData (st_ring_buf_t *ary, uint8_t data[], size_t len);

/**********************************************************************************************************************
 * Function Name: R_RINGBUF_GetDataLength
 * Description  : This function returns the number of data stored in the ring buffer.
 * Arguments    : ring_buf_t * ary
 *                    Ring buffer structure pointer
 * Return Value : uint32_t
 *                    Number of stored data
 *********************************************************************************************************************/
size_t R_RINGBUF_GetDataLength (st_ring_buf_t *ary);

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
uint32_t R_RINGBUF_SetDataIndex (st_ring_buf_t *ary, uint32_t value, uint8_t select);

#endif /* R_PRV_RING_BUFFER_CONTROL_API_H */
