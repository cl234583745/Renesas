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
 * File Name    : r_sensor_common_api.c
 * Version      : 1.0
 * Description  : Source file of sensor common processing
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
#include <string.h>

#include "r_sensor_common_api.h"

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
 * Function Name: R_CALC_MovingAverage
 * Description  : This function takes AD values and returns a moving average result.
 * Arguments    : int32_t input -
 *                    input value.
 *              : float array[] -
 *                    previous values array.
 *              : size_t size -
 *                    Number of array elements
 *              : int32_t * w_index -
 *                    pointer to write index of array.
 * Return Value : float
 *                    Average value
 *********************************************************************************************************************/
float R_CALC_MovingAverage (float input, float array[], size_t size, int32_t *w_index)
{
    float   rslt      = 0.0F;
    float   divide;
    size_t  i;
    int32_t idx       = *w_index;


    /** Data update */
    array[(idx % size)] = input;

    /** Calculation of total value */
    for (i = 0U; i < size; i++)
    {
        rslt += array[i];
    }

    idx++;

    /** Select divide value. Both idx and size variables are within 24bit range */
    divide = (float)((idx <= size) ? idx : size);

    /** Wrapping and restore next index. */
    /** index value range is two times of array size */
    if( idx >= (size * 2))
    {
        idx = size;
    }
    *w_index = idx;
    
    /** Averaing */
    rslt /= divide;

    return rslt;
}

/**********************************************************************************************************************
 End of function R_CALC_MovingAverage
 *********************************************************************************************************************/
