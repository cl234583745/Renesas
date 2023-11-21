/***********************************************************************
*
*  FILE        : rx23ea_local_board_lab.c
*  DATE        : 2023-09-16
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "r_smc_entry.h"

#include "r_sensor_common_api.h"
#include "r_rtd_api.h"
#include "r_dsp_types.h"
#include "r_dsp_iirbiquad.h"

bool timer_flag = false;
static volatile int32_t s_dsad0_value;                  /** DSAD0 24bit A/D value storage variable          */
static volatile float   s_temp      = 0;                /** Measurement temperature storage variable        */

/* Filter Characteristic definition */
#define IIRBIQUAD_COEF_FLAT (0)    /* Pass-through characteristics */
#define IIRBIQUAD_COEF_HPF  (1)    /* High pass filter characteristics */
#define IIRBIQUAD_COEF_LPF  (2)    /* Low pass filter characteristics */

static volatile int32_t gs_intermediate_buffer[2][1024]; /* buffer to interface DSP module */
static volatile int32_t gs_output_buffer[1024/2];          /* Output Buffer for DSP to store the result */
static uint8_t gs_coef_select;           /* Number of filter Characteristic */
static const int32_t gs_sample_wave_data[1024] =
{
    #include "wave_sample2.h"
};

void main(void);

void main(void)
{
	uint32_t dsad0_reg;     						/** DSAD0.DR register storage variable                          */
	float rtd_temp = 0;                             /** Measurement temperature */

    memset((void *)gs_intermediate_buffer, 0, sizeof(gs_intermediate_buffer));  /* Clears Intermediate Buffer to zero */
    memset((void *)gs_output_buffer, 0, sizeof(gs_output_buffer));  /* Clears Output Buffer to zero */

    /*** Initializes IIR filter & FFT processing ***/
    gs_coef_select = IIRBIQUAD_COEF_HPF;

    R_DSP_IIRBiquad_Init();
    R_DSP_IIRBiquad_UpdateCoef(gs_coef_select);

	R_Config_TMR0_TMR1_Start();

    /** A/D conversion start */
    R_Config_DSAD0_Start();
    R_Config_DSAD0_Set_SoftwareTrigger();

    while (1)
    {
        if(true == timer_flag){
        	timer_flag = false;

			/****************************/
			/** Temperature measurement */
			/****************************/
			/** A/D conversion complete for DSAD0? */
			if (true == r_dsad0_IsConversionEnd())
			{
	            int32_t dsad0_value;        /** Signed 24-bit DSAD0 value                                   */

	            r_dsad0_ClearIrFlag();      /** clear DSAD0 IR flag                                         */

	            /** Get A/D value */
	            {
	                R_Config_DSAD0_Get_ValueResult( &dsad0_reg);

	                /** Flag mask, Sign extension */
	                dsad0_value = (int32_t) ((dsad0_reg & 0x00FFFFFFU) << 8) >> 8;
	            }

	            /** Calculate the temperature [degree] of the reference junction */
	            rtd_temp = R_RTD_DsadToTemp((float) dsad0_value);

	            /** debug monitor */
	            {
	                s_dsad0_value   = dsad0_value;
	                s_temp          = rtd_temp;
	            }

			}
        }

	    /* Executes IIRbiquad processing */
	    R_DSP_IIRBiquad_Operation((int32_t *)gs_sample_wave_data,
	                              (int32_t *)gs_intermediate_buffer[1]);
    }
}
