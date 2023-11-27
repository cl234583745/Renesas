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
 * File Name    : r_rtd_api.c
 * Version      : 1.0
 * Description  : RTD control source file
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

#include "r_sensor_common_api.h"
#include "r_rtd_api.h"

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

/** Refer to the IEC60751 Pt100 standard resistance table, and use the resistance [ohm] table in 1 degree Celsius steps
  * from -40 degree Celsius to 86 degree Celsius.  */

static const float s_rtd_table[] =
{
    84.271, 84.666, 85.062, 85.457, 85.853, 86.248, 86.643, 87.038, 87.432, 87.827,
    88.222, 88.616, 89.010, 89.404, 89.798, 90.192, 90.586, 90.980, 91.373, 91.767,
    92.160, 92.553, 92.946, 93.339, 93.732, 94.124, 94.517, 94.909, 95.302, 95.694,
    96.086, 96.478, 96.870, 97.261, 97.653, 98.044, 98.436, 98.827, 99.218, 99.609,
    100.000,100.391,100.781,101.172,101.562,101.953,102.343,102.733,103.123,103.513,
    103.903,104.292,104.682,105.071,105.460,105.849,106.238,106.627,107.016,107.405,
    107.794,108.182,108.570,108.959,109.347,109.735,110.123,110.510,110.898,111.286,
    111.673,112.060,112.447,112.835,113.221,113.608,113.995,114.382,114.768,115.155,
    115.541,115.927,116.313,116.699,117.085,117.470,117.856,118.241,118.627,119.012,
    119.397,119.782,120.167,120.552,120.936,121.321,121.705,122.090,122.474,122.858,
    123.242,123.626,124.009,124.393,124.777,125.160,125.543,125.926,126.309,126.692,
    127.075,127.458,127.840,128.223,128.605,128.987,129.370,129.752,130.133,130.515,
    130.897,131.278,131.660,132.041,132.422,132.803,133.184,
};

/**********************************************************************************************************************
 * Function Name: R_RTD_DsadToTemp
 * Description  : RTD physical quantity calculation function. Convert DSAD value to temperature.
 * Arguments    : float dsad -
 *                    24bit dsad value
 * Return Value : float
 *                    temperature[degree]
 *********************************************************************************************************************/
float R_RTD_DsadToTemp (float dsad)
{
    float rrtd = (D_RTD_GAIN * dsad) + D_RTD_OFFSET;        /** Convert DSAD Value to RTD resistance value [ohm]   */
    uint16_t idx = R_CALC_BinarySearch(s_rtd_table, D_RTD_TABLE_SIZE, rrtd);

    /** Set a temperature range that includes the value of the argument */
    float x0 = s_rtd_table[idx];                            /** x0:Temperature range lower limit resistance [ohm]  */
    float x1 = s_rtd_table[idx + 1];                        /** x1:Temperature range upper limit resistance [ohm]  */

    /** convert idx to temperature */
    float y0 = (float) idx + D_RTD_TABLE_TOP_TEMPARATURE;

    /** convert idx + 1 to temperature */
    float y1 = (float) (idx + 1) + D_RTD_TABLE_TOP_TEMPARATURE;

    float f_temp = R_CALC_Lerp(x0, y0, x1, y1, rrtd);       /** Calculated temperature [degree]                     */

    return f_temp;
}
/**********************************************************************************************************************
 End of function R_RTD_DsadToTemp()
 *********************************************************************************************************************/

