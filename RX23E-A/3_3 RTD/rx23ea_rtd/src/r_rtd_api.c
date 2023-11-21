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
 * from -50 degree Celsius to 251 degree Celsius.  */

static const float s_rtd_table[] =
{
    80.306,  80.703,  81.100,  81.497,  81.894,  82.290,  82.687,  83.083,  83.479,  83.875,
    84.271,  84.666,  85.062,  85.457,  85.853,  86.248,  86.643,  87.038,  87.432,  87.827,
    88.222,  88.616,  89.010,  89.404,  89.798,  90.192,  90.586,  90.980,  91.373,  91.767,
    92.160,  92.553,  92.946,  93.339,  93.732,  94.124,  94.517,  94.909,  95.302,  95.694,
    96.086,  96.478,  96.870,  97.261,  97.653,  98.044,  98.436,  98.827,  99.218,  99.609,
    100.000, 100.391, 100.781, 101.172, 101.562, 101.953, 102.343, 102.733, 103.123, 103.513,
    103.903, 104.292, 104.682, 105.071, 105.460, 105.849, 106.238, 106.627, 107.016, 107.405,
    107.794, 108.182, 108.570, 108.959, 109.347, 109.735, 110.123, 110.510, 110.898, 111.286,
    111.673, 112.060, 112.447, 112.835, 113.221, 113.608, 113.995, 114.382, 114.768, 115.155,
    115.541, 115.927, 116.313, 116.699, 117.085, 117.470, 117.856, 118.241, 118.627, 119.012,
    119.397, 119.782, 120.167, 120.552, 120.936, 121.321, 121.705, 122.090, 122.474, 122.858,
    123.242, 123.626, 124.009, 124.393, 124.777, 125.160, 125.543, 125.926, 126.309, 126.692,
    127.075, 127.458, 127.840, 128.223, 128.605, 128.987, 129.370, 129.752, 130.133, 130.515,
    130.897, 131.278, 131.660, 132.041, 132.422, 132.803, 133.184, 133.565, 133.946, 134.326,
    134.707, 135.087, 135.468, 135.848, 136.228, 136.608, 136.987, 137.367, 137.747, 138.126,
    138.506, 138.885, 139.264, 139.643, 140.022, 140.400, 140.779, 141.158, 141.536, 141.914,
    142.293, 142.671, 143.049, 143.426, 143.804, 144.182, 144.559, 144.937, 145.314, 145.691,
    146.068, 146.445, 146.822, 147.198, 147.575, 147.951, 148.328, 148.704, 149.080, 149.456,
    149.832, 150.208, 150.583, 150.959, 151.334, 151.710, 152.085, 152.460, 152.835, 153.210,
    153.584, 153.959, 154.333, 154.708, 155.082, 155.456, 155.830, 156.204, 156.578, 156.952,
    157.325, 157.699, 158.072, 158.445, 158.818, 159.191, 159.564, 159.937, 160.309, 160.682,
    161.054, 161.427, 161.799, 162.171, 162.543, 162.915, 163.286, 163.658, 164.030, 164.401,
    164.772, 165.143, 165.514, 165.885, 166.256, 166.627, 166.997, 167.368, 167.738, 168.108,
    168.478, 168.848, 169.218, 169.588, 169.958, 170.327, 170.696, 171.066, 171.435, 171.804,
    172.173, 172.542, 172.910, 173.279, 173.648, 174.016, 174.384, 174.752, 175.120, 175.488,
    175.856, 176.224, 176.591, 176.959, 177.326, 177.693, 178.060, 178.427, 178.794, 179.161,
    179.528, 179.894, 180.260, 180.627, 180.993, 181.359, 181.725, 182.091, 182.456, 182.822,
    183.188, 183.553, 183.918, 184.283, 184.648, 185.013, 185.378, 185.743, 186.107, 186.472,
    186.836, 187.200, 187.564, 187.928, 188.292, 188.656, 189.019, 189.383, 189.746, 190.110,
    190.473, 190.836, 191.199, 191.562, 191.924, 192.287, 192.649, 193.012, 193.374, 193.736,
    194.098, 194.460,
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

