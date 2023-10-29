#include "hal_data.h"

#if defined(__ICCARM__)

 #pragma section="SYSTEM_RAM_DATA_RBLOCK"
 #pragma section="SYSTEM_RAM_DATA_WBLOCK"
 #pragma section="SYSTEM_RAM_DATA_ZBLOCK"

#elif defined(__GNUC__)

 extern uint32_t __atcm_bss_start;
 extern uint32_t __atcm_bss_end;

#endif



#define SRAM_DATA_NUM     (4)
 /* Data with initial value and bss value. */
 static volatile uint32_t sram_data[SRAM_DATA_NUM]={0x0123, 0x4567, 0x89AB, 0xCDEF};
 static volatile uint32_t temp_sram_data[SRAM_DATA_NUM];

//extern void coremain(void);
void func_atcm_bss_init (void);
/*******************************************************************************************************************//**
 *
 * Clear the System RAM bss.
 *
 **********************************************************************************************************************/
void func_atcm_bss_init (void)
{
    uint8_t * src_bss;
    uint8_t * dst_bss;
    uint8_t   zero = 0;

#if defined(__ICCARM__)
    src_bss = (uint8_t *) __section_begin("SYSTEM_RAM_DATA_ZBLOCK");
    dst_bss = (uint8_t *) __section_end("SYSTEM_RAM_DATA_ZBLOCK");
#elif defined(__GNUC__)
    src_bss = (uint8_t *) &__atcm_bss_start;
    dst_bss = (uint8_t *) &__atcm_bss_end;
#endif

    for (; src_bss < dst_bss; src_bss++)
    {
        *src_bss = zero;
    }

    /* Ensuring data-changing */
    __asm volatile ("dsb");



    //test
    uint32_t data_num;
    for(data_num = 0; data_num < SRAM_DATA_NUM; data_num++)
    {
      temp_sram_data[data_num] = sram_data[data_num];
    }


}
