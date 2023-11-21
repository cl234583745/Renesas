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
 * File Name    : r_sensor_common_api.h
 * Version      : 1.0
 * Description  : Sensor common processing header file
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
#ifndef R_PRV_SENSOR_COMMON_API_H
    #define R_PRV_SENSOR_COMMON_API_H

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Function Name: R_CALC_BinarySearch
 * Description  : binary search
 * Arguments    : const float * p_data_table -
 *                    Pointer of table to search
 *              : uint16_t table_size -
 *                    Number of elements of table to search
 *              : float data -
 *                    Data to search
 * Return Value : uint16_t
 *                    index
 *********************************************************************************************************************/
uint16_t R_CALC_BinarySearch (const float *p_data_table, uint16_t table_size, float data);

/**********************************************************************************************************************
 * Function Name: R_CALC_Lerp
 * Description  : Linear interpolation function
 * Arguments    : float x0 -
 *                    the value of x0
 *              : float y0 -
 *                    the value of y0
 *              : float x1 -
 *                    the value of x1
 *              : float y1 -
 *                    the value of y1
 *              : float x -
 *                    the value of x
 * Return Value : float
 *                    y
 *********************************************************************************************************************/
float R_CALC_Lerp (float x0, float y0, float x1, float y1, float x);

#endif /* R_PRV_SENSOR_COMMON_API_H */
