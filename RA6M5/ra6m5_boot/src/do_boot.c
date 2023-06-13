/*
 * do_boot.c
 *
 *  Created on: 2021年11月7日
 *      Author: Ryan.Lan
 */

#include "hal_data.h"

#include "do_boot.h"

struct arm_vector_table {
    uint32_t msp;
    uint32_t reset;
};

void do_boot(unsigned long Boot_Addr)
{

    static struct arm_vector_table *vt;
    uintptr_t flash_base;
    uint32_t vt_msp, vt_reset;


    flash_base = Boot_Addr;

    vt = (struct arm_vector_table *)(flash_base);


    vt->msp = *(uint32_t *)flash_base;
    vt->reset = *(uint32_t *)(flash_base+4);

    SCB->VTOR = ((int)(&(vt->msp)) & 0x1FFFFF80);
    __DSB();

    /* Disable MSP monitoring  */
#if BSP_FEATURE_BSP_HAS_SP_MON
    R_MPU_SPMON->SP[0].CTL = 0;
    while(R_MPU_SPMON->SP[0].CTL != 0);
#endif

    __set_MSP(vt->msp);
    //if boot enable freertos,you must set psp in boot before jump to app
    __set_PSP(*(uint32_t *)flash_base);

    __set_PRIMASK(1);

    ((void (*)()) vt->reset)();

    __set_PRIMASK(0);
}

