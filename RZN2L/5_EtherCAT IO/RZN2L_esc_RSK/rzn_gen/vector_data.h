/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#include "bsp_api.h"
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (4)
#endif
/* ISR prototypes */
void cmt_cm_int_isr(void);
void ethercat_ssc_port_isr_esc_sync0(void);
void ethercat_ssc_port_isr_esc_sync1(void);
void ethercat_ssc_port_isr_esc_cat(void);

/* Vector table allocations */
#define VECTOR_NUMBER_CMT0_CMI ((IRQn_Type) 53) /* CMT0_CMI (CMT0 Compare match) */
#define VECTOR_NUMBER_ESC_SYNC0 ((IRQn_Type) 277) /* ESC_SYNC0 (EtherCAT Sync0 interrupt) */
#define VECTOR_NUMBER_ESC_SYNC1 ((IRQn_Type) 278) /* ESC_SYNC1 (EtherCAT Sync1 interrupt) */
#define VECTOR_NUMBER_ESC_CAT ((IRQn_Type) 279) /* ESC_CAT (EtherCAT interrupt) */
typedef enum IRQn {
	SoftwareGeneratedInt0 = -32,
	SoftwareGeneratedInt1 = -31,
	SoftwareGeneratedInt2 = -30,
	SoftwareGeneratedInt3 = -29,
	SoftwareGeneratedInt4 = -28,
	SoftwareGeneratedInt5 = -27,
	SoftwareGeneratedInt6 = -26,
	SoftwareGeneratedInt7 = -25,
	SoftwareGeneratedInt8 = -24,
	SoftwareGeneratedInt9 = -23,
	SoftwareGeneratedInt10 = -22,
	SoftwareGeneratedInt11 = -21,
	SoftwareGeneratedInt12 = -20,
	SoftwareGeneratedInt13 = -19,
	SoftwareGeneratedInt14 = -18,
	SoftwareGeneratedInt15 = -17,
	DebugCommunicationsChannelInt = -10,
	PerformanceMonitorCounterOverflowInt = -9,
	CrossTriggerInterfaceInt = -8,
	VritualCPUInterfaceMaintenanceInt = -7,
	HypervisorTimerInt = -6,
	VirtualTimerInt = -5,
	NonSecurePhysicalTimerInt = -2,
	CMT0_CMI_IRQn = 53, /* CMT0_CMI (CMT0 Compare match) */
	ESC_SYNC0_IRQn = 277, /* ESC_SYNC0 (EtherCAT Sync0 interrupt) */
	ESC_SYNC1_IRQn = 278, /* ESC_SYNC1 (EtherCAT Sync1 interrupt) */
	ESC_CAT_IRQn = 279, /* ESC_CAT (EtherCAT interrupt) */
	SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES = BSP_VECTOR_TABLE_MAX_ENTRIES
} IRQn_Type;
#endif /* VECTOR_DATA_H */
