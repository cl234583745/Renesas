/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <string.h>
#include "bsp_api.h"



#if defined(__GNUC__)
#if BSP_CFG_C_RUNTIME_INIT

extern void * _matcm_text;
extern void * _atcm_text_start;
extern void * _atcm_text_end;

extern void * _matcm_data;
extern void * _atcm_data_start;
extern void * _atcm_data_end;

extern void * __atcm_bss_start;
extern void * __atcm_bss_end;

#define BSP_PRV_SECTION_LDR_TEXT_ROM_ADDRESS                &_matcm_text
#define BSP_PRV_SECTION_LDR_TEXT_RAM_START                  &_atcm_text_start
#define BSP_PRV_SECTION_LDR_TEXT_RAM_END                    &_atcm_text_end

#define BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS                &_matcm_data
#define BSP_PRV_SECTION_LDR_DATA_RAM_START                  &_atcm_data_start
#define BSP_PRV_SECTION_LDR_DATA_RAM_END                    &_atcm_data_end

#define BSP_PRV_SECTION_LDR_DATA_BSS_START                  &__atcm_bss_start
#define BSP_PRV_SECTION_LDR_DATA_BSS_END                    &__atcm_bss_end

#endif//#if BSP_CFG_C_RUNTIME_INIT

static void bsp_copy_4byte(uint32_t * src, uint32_t * dst, uint32_t bytesize);
static void bsp_bss_init_4byte(uint32_t * src, uint32_t bytesize);
static void bsp_atcm_user_text_init();
static void bsp_atcm_user_data_init();
static void bsp_atcm_user_bss_init();
void atcm_user_init(void);

#endif//#if defined(__GNUC__)


/*******************************************************************************************************************//**
 * Copy the memory block from Source address to Destination address by the 4 byte unit.
 **********************************************************************************************************************/
void bsp_copy_4byte (uint32_t * src, uint32_t * dst, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;

    /* Copy Count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *dst++ = *src++;
    }

    /* Ensuring data-changing */
    __DSB();
}

/*******************************************************************************************************************//**
 * Clear the bss block by the 4 byte unit.
 **********************************************************************************************************************/
void bsp_bss_init_4byte (uint32_t * src, uint32_t bytesize)
{
    uint32_t i;
    uint32_t cnt;
    uint32_t zero = 0;

    /* Clear Count in 4 byte unit */
    cnt = (bytesize + 3) >> 2;

    for (i = 0; i < cnt; i++)
    {
        *src++ = zero;
    }

    /* Ensuring data-changing */
    __DSB();
}

/*******************************************************************************************************************//**
 * Copy the app text block from external Flash to internal RAM.
 **********************************************************************************************************************/
void bsp_atcm_user_text_init (void)
{
 #if (!defined(__GNUC__) || !(BSP_CFG_RAM_EXECUTION))

    /* Define destination/source address pointer and block size */
    uint32_t * src;
    uint32_t * dst;
    uint32_t   size;

    /* Copy loader data block */
    src  = (uint32_t *) BSP_PRV_SECTION_LDR_TEXT_ROM_ADDRESS;
    dst  = (uint32_t *) BSP_PRV_SECTION_LDR_TEXT_RAM_START;
    size = (uint32_t) BSP_PRV_SECTION_LDR_TEXT_RAM_END - (uint32_t) BSP_PRV_SECTION_LDR_TEXT_RAM_START;
    bsp_copy_4byte(src, dst, size);

 #endif
}
/*******************************************************************************************************************//**
 * Copy the app data block from external Flash to internal RAM.
 **********************************************************************************************************************/
void bsp_atcm_user_data_init (void)
{
 #if (!defined(__GNUC__) || !(BSP_CFG_RAM_EXECUTION))

    /* Define destination/source address pointer and block size */
    uint32_t * src;
    uint32_t * dst;
    uint32_t   size;

    /* Copy loader data block */
    src  = (uint32_t *) BSP_PRV_SECTION_LDR_DATA_ROM_ADDRESS;
    dst  = (uint32_t *) BSP_PRV_SECTION_LDR_DATA_RAM_START;
    size = (uint32_t) BSP_PRV_SECTION_LDR_DATA_RAM_END - (uint32_t) BSP_PRV_SECTION_LDR_DATA_RAM_START;
    bsp_copy_4byte(src, dst, size);

 #endif
}

/*******************************************************************************************************************//**
 * Clear the app bss block in internal RAM.
 **********************************************************************************************************************/
void bsp_atcm_user_bss_init (void)
{
    /* Define source address pointer and block size */
    uint32_t * src;
    uint32_t   size;

    /* Clear loader bss block. */
    src  = (uint32_t *) BSP_PRV_SECTION_LDR_DATA_BSS_START;
    size = (uint32_t) BSP_PRV_SECTION_LDR_DATA_BSS_END - (uint32_t) BSP_PRV_SECTION_LDR_DATA_BSS_START;
    bsp_bss_init_4byte(src, size);

}

#define SRAM_DATA_NUM     (4)
 /* Data with initial value and bss value. */
 static volatile uint32_t sram_data[SRAM_DATA_NUM]={0x0123, 0x4567, 0x89AB, 0xCDEF};
 static volatile uint32_t temp_sram_data[SRAM_DATA_NUM];
void atcm_user_init(void)
{

#if 0//must move to system.c  why?
    /* Copy the atcm_user text from external Flash to internal RAM. */
    bsp_atcm_user_text_init();
    /* Copy the atcm_user data from external Flash to internal RAM. */
    bsp_atcm_user_data_init();

    /* Clear atcm_user bss section in internal RAM. */
    bsp_atcm_user_bss_init();
#endif



    //test
    uint32_t data_num;
    for(data_num = 0; data_num < SRAM_DATA_NUM; data_num++)
    {
      temp_sram_data[data_num] = sram_data[data_num];
    }
}
