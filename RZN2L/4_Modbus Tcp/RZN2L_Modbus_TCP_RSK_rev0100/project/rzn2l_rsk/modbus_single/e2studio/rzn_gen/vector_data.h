/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#include "bsp_api.h"
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (3)
#endif
/* ISR prototypes */
void gmac_isr_pmt(void);
void gmac_isr_sbd(void);
void ethsw_isr_intr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_GMAC_PMT ((IRQn_Type) 251) /* GMAC_PMT (GMAC1 power management) */
#define VECTOR_NUMBER_GMAC_SBD ((IRQn_Type) 252) /* GMAC_SBD (GMAC1 general interrupt) */
#define VECTOR_NUMBER_ETHSW_INTR ((IRQn_Type) 253) /* ETHSW_INTR (Ethernet Switch interrupt) */
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
	GMAC_PMT_IRQn = 251, /* GMAC_PMT (GMAC1 power management) */
	GMAC_SBD_IRQn = 252, /* GMAC_SBD (GMAC1 general interrupt) */
	ETHSW_INTR_IRQn = 253, /* ETHSW_INTR (Ethernet Switch interrupt) */
	SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES = BSP_VECTOR_TABLE_MAX_ENTRIES
} IRQn_Type;
#endif /* VECTOR_DATA_H */
