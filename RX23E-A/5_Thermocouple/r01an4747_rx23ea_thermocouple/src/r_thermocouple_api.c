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
 * File Name    : r_thermocouple_api.c
 * Version      : 1.0
 * Description  : thermocouple control source file
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
#include "r_thermocouple_api.h"

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

/** Refers to the IEC60584-1 type K standard thermoelectromotive force[uV], a table of thermoelectromotive force[uV]
  * in 1 degree Celsius steps in the range of -75 degree Celsius to 251 degree Celsius */
static const float s_tc_table[] =
{
    -2755,-2721,-2688,-2654,-2620,-2587,-2553,-2519,-2485,-2450,
    -2416,-2382,-2347,-2312,-2278,-2243,-2208,-2173,-2138,-2103,
    -2067,-2032,-1996,-1961,-1925,-1889,-1854,-1818,-1782,-1745,
    -1709,-1673,-1637,-1600,-1564,-1527,-1490,-1453,-1417,-1380,
    -1343,-1305,-1268,-1231,-1194,-1156,-1119,-1081,-1043,-1006,
    -968,  -930, -892, -854, -816, -778, -739, -701, -663, -624,
    -586,  -547, -508, -470, -431, -392, -353, -314, -275, -236,
    -197,  -157, -118,  -79,  -39,    0,   39,   79,  119,  158,
    198,    238,  277,  317,  357,  397,  437,  477,  517,  557,
    597,    637,  677,  718,  758,  798,  838,  879,  919,  960,
    1000,  1041, 1081, 1122, 1163, 1203, 1244, 1285, 1326, 1366,
    1407,  1448, 1489, 1530, 1571, 1612, 1653, 1694, 1735, 1776,
    1817,  1858, 1899, 1941, 1982, 2023, 2064, 2106, 2147, 2188,
    2230,  2271, 2312, 2354, 2395, 2436, 2478, 2519, 2561, 2602,
    2644,  2685, 2727, 2768, 2810, 2851, 2893, 2934, 2976, 3017,
    3059,  3100, 3142, 3184, 3225, 3267, 3308, 3350, 3391, 3433,
    3474,  3516, 3557, 3599, 3640, 3682, 3723, 3765, 3806, 3848,
    3889,  3931, 3972, 4013, 4055, 4096, 4138, 4179, 4220, 4262,
    4303,  4344, 4385, 4427, 4468, 4509, 4550, 4591, 4633, 4674,
    4715,  4756, 4797, 4838, 4879, 4920, 4961, 5002, 5043, 5084,
    5124,  5165, 5206, 5247, 5288, 5328, 5369, 5410, 5450, 5491,
    5532,  5572, 5613, 5653, 5694, 5735, 5775, 5815, 5856, 5896,
    5937,  5977, 6017, 6058, 6098, 6138, 6179, 6219, 6259, 6299,
    6339,  6380, 6420, 6460, 6500, 6540, 6580, 6620, 6660, 6701,
    6741,  6781, 6821, 6861, 6901, 6941, 6981, 7021, 7060, 7100,
    7140,  7180, 7220, 7260, 7300, 7340, 7380, 7420, 7460, 7500,
    7540,  7579, 7619, 7659, 7699, 7739, 7779, 7819, 7859, 7899,
    7939,  7979, 8019, 8059, 8099, 8138, 8178, 8218, 8258, 8298,
    8338,  8378, 8418, 8458, 8499, 8539, 8579, 8619, 8659, 8699,
    8739,  8779, 8819, 8860, 8900, 8940, 8980, 9020, 9061, 9101,
    9141,  9181, 9222, 9262, 9302, 9343, 9383, 9423, 9464, 9504,
    9545,  9585, 9626, 9666, 9707, 9747, 9788, 9828, 9869, 9909,
    9950,  9991,10031,10072,10113,10153,10194,
};

/*********************************************************************************************************************
 * Function Name: R_TC_TempToEmf
 * Description  : Convert temperature to Thermal electromotive force
 * Arguments    : float temp -
 *                    temperature
 * Return Value : float
 *                    Thermal electromotive
 ********************************************************************************************************************/
float R_TC_TempToEmf (float temp)
{
    float emf   = 0;                                            /** thermoelectric power[uV]                        */

    /** Set a temperature range that includes the value of the argument */
    int32_t x0  = (int32_t) temp;                               /** x0:Truncate the decimal part of temp[degree]    */
    int32_t x1  = x0 + ((temp >= 0) ? 1 : (-1));                /** Calculate the index between temp                */

    /** Calculate index corresponding to temperature range and refer to table data */
    float y0    = s_tc_table[x0 - (int32_t) D_TC_TABLE_TOP_TEMPARATURE]; /** y0:Thermoelectric power of x0 degrees  */
    float y1    = s_tc_table[x1 - (int32_t) D_TC_TABLE_TOP_TEMPARATURE]; /** y1:Thermoelectric power of x1 degrees  */

    emf = R_CALC_Lerp((float) x0, y0, (float) x1, y1, temp);    /** Linear interpolation                            */

    return emf;
}
/**********************************************************************************************************************
 End of function R_TC_TempToEmf()
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_TC_DsadToEmf
 * Description  : Convert dsad to Thermal electromotive force
 * Arguments    : float dsad -
 *                    24bit DSAD value
 * Return Value : float
 *                    thermal electromotive
 *********************************************************************************************************************/
float R_TC_DsadToEmf (float dsad)
{
    return ((D_TC_GAIN * dsad) + D_TC_OFFSET);
}
/**********************************************************************************************************************
 End of function R_TC_DsadToEmf()
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Name: R_TC_EmfToTemp
 * Description  : Convert Thermal electromotive force to temperature
 * Arguments    : float emf -
 *                    Thermal electromotive force
 * Return Value : float
 *                    Temperature
 *********************************************************************************************************************/
float R_TC_EmfToTemp (float emf)
{
    float f_temp = 0;

    /** Finds the index of the data range that contains the value of the argument by binary search  */
    uint16_t idx = R_CALC_BinarySearch(s_tc_table, D_TC_TABLE_SIZE, emf);

    /** Refer table data using the determined index */
    float x0 = s_tc_table[idx];                 /** x0:Temperature range lower limit voltage [uV]   */
    float x1 = s_tc_table[idx + 1];             /** x1:Temperature range upper limit voltage [uV]   */

    /** convert idx to temperature */
    float y0 = (float) idx + D_TC_TABLE_TOP_TEMPARATURE;

    /** convert idx + 1 to temperature */
    float y1 = (float) (idx + 1) + D_TC_TABLE_TOP_TEMPARATURE;

    f_temp = R_CALC_Lerp(x0, y0, x1, y1, emf);  /** Linear interpolation                            */

    return f_temp;
}
/**********************************************************************************************************************
 End of function R_TC_EmfToTemp()
 *********************************************************************************************************************/
