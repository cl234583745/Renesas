/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
                        [53] = cmt_cm_int_isr, /* CMT0_CMI (CMT0 Compare match) */
            [277] = ethercat_ssc_port_isr_esc_sync0, /* ESC_SYNC0 (EtherCAT Sync0 interrupt) */
            [278] = ethercat_ssc_port_isr_esc_sync1, /* ESC_SYNC1 (EtherCAT Sync1 interrupt) */
            [279] = ethercat_ssc_port_isr_esc_cat, /* ESC_CAT (EtherCAT interrupt) */
        };
        #endif
