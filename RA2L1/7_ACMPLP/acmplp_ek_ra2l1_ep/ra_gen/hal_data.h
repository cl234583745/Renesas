/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_dac.h"
#include "r_dac_api.h"
#include "r_acmplp.h"
#include "r_comparator_api.h"
FSP_HEADER
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_channel_cfg_t g_adc0_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  1
#endif
/** DAC on DAC Instance. */
extern const dac_instance_t g_dac;

/** Access the DAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dac_instance_ctrl_t g_dac_ctrl;
extern const dac_cfg_t g_dac_cfg;
/** Comparator Instance. */
extern const comparator_instance_t g_comparator;

/** Access the Comparator instance using these structures when calling API functions directly (::p_api is not used). */
extern acmplp_instance_ctrl_t g_comparator_ctrl;
extern const comparator_cfg_t g_comparator_cfg;

#ifndef NULL
void NULL(comparator_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
