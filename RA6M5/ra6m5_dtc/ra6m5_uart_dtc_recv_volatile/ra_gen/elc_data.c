/* generated ELC source file - do not edit */
#include "r_elc_api.h"
const elc_cfg_t g_elc_cfg =
{ .link[ELC_PERIPHERAL_GPT_A] = ELC_EVENT_SCI4_RXI, /* SCI4 RXI (Received data full) */
  .link[ELC_PERIPHERAL_GPT_B] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_GPT_C] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_GPT_D] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_GPT_E] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_GPT_F] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_GPT_G] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_GPT_H] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_ADC0] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_ADC0_B] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_ADC1] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_ADC1_B] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_DAC0] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_DAC1] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_IOPORT1] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_IOPORT2] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_IOPORT3] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_IOPORT4] = ELC_EVENT_NONE, /* No allocation */
  .link[ELC_PERIPHERAL_CTSU] = ELC_EVENT_NONE, /* No allocation */
};

#if BSP_TZ_SECURE_BUILD

        void R_BSP_ElcCfgSecurityInit(void);

        /* Configure ELC Security Attribution. */
        void R_BSP_ElcCfgSecurityInit(void)
        {
 #if (2U == BSP_FEATURE_ELC_VERSION)
            uint32_t elcsarbc = UINT32_MAX;

            elcsarbc &=  ~(1U << ELC_PERIPHERAL_GPT_A);

            /* Write the settings to ELCSARn Registers. */
            R_ELC->ELCSARA = 0xFFFFFFFEU;
            R_ELC->ELCSARB = elcsarbc;
 #else
            uint16_t elcsarbc[2] = {0xFFFFU, 0xFFFFU};
            elcsarbc[ELC_PERIPHERAL_GPT_A / 16U] &= (uint16_t) ~(1U << (ELC_PERIPHERAL_GPT_A % 16U));

            /* Write the settins to ELCSARn Registers. */
            R_ELC->ELCSARA = 0xFFFEU;
            R_ELC->ELCSARB = elcsarbc[0];
            R_ELC->ELCSARC = elcsarbc[1];
 #endif
        }
#endif
