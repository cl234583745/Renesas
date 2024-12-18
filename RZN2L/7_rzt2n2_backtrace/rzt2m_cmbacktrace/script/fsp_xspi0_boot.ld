/*
                  Linker File for Renesas RZ/T2 FSP
*/

INCLUDE memory_regions.ld

/* The memory information for each device is done in memory regions file.
 * The starting address and length of memory not defined in memory regions file are defined as 0. */

ATCM_PRV_START = DEFINED(ATCM_START) ? ATCM_START : 0;
ATCM_PRV_LENGTH = DEFINED(ATCM_LENGTH) ? ATCM_LENGTH : 0;
BTCM_PRV_START = DEFINED(BTCM_START) ? BTCM_START : 0;
BTCM_PRV_LENGTH = DEFINED(BTCM_LENGTH) ? BTCM_LENGTH : 0;
SYSTEM_RAM_PRV_START = DEFINED(SYSTEM_RAM_START) ? SYSTEM_RAM_START : 0;
SYSTEM_RAM_PRV_LENGTH = DEFINED(SYSTEM_RAM_LENGTH) ? SYSTEM_RAM_LENGTH : 0;
SYSTEM_RAM_MIRROR_PRV_START = DEFINED(SYSTEM_RAM_MIRROR_START) ? SYSTEM_RAM_MIRROR_START : 0;
SYSTEM_RAM_MIRROR_PRV_LENGTH = DEFINED(SYSTEM_RAM_MIRROR_LENGTH) ? SYSTEM_RAM_MIRROR_LENGTH : 0;
xSPI0_CS0_SPACE_MIRROR_PRV_START = DEFINED(xSPI0_CS0_SPACE_MIRROR_START) ? xSPI0_CS0_SPACE_MIRROR_START : 0;
xSPI0_CS0_SPACE_MIRROR_PRV_LENGTH = DEFINED(xSPI0_CS0_SPACE_MIRROR_LENGTH) ? xSPI0_CS0_SPACE_MIRROR_LENGTH : 0;
xSPI0_CS1_SPACE_MIRROR_PRV_START = DEFINED(xSPI0_CS1_SPACE_MIRROR_START) ? xSPI0_CS1_SPACE_MIRROR_START : 0;
xSPI0_CS1_SPACE_MIRROR_PRV_LENGTH = DEFINED(xSPI0_CS1_SPACE_MIRROR_LENGTH) ? xSPI0_CS1_SPACE_MIRROR_LENGTH : 0;
xSPI1_CS0_SPACE_MIRROR_PRV_START = DEFINED(xSPI1_CS0_SPACE_MIRROR_START) ? xSPI1_CS0_SPACE_MIRROR_START : 0;
xSPI1_CS0_SPACE_MIRROR_PRV_LENGTH = DEFINED(xSPI1_CS0_SPACE_MIRROR_LENGTH) ? xSPI1_CS0_SPACE_MIRROR_LENGTH : 0;
xSPI1_CS1_SPACE_MIRROR_PRV_START = DEFINED(xSPI1_CS1_SPACE_MIRROR_START) ? xSPI1_CS1_SPACE_MIRROR_START : 0;
xSPI1_CS1_SPACE_MIRROR_PRV_LENGTH = DEFINED(xSPI1_CS1_SPACE_MIRROR_LENGTH) ? xSPI1_CS1_SPACE_MIRROR_LENGTH : 0;
CS0_SPACE_MIRROR_PRV_START = DEFINED(CS0_SPACE_MIRROR_START) ? CS0_SPACE_MIRROR_START : 0;
CS0_SPACE_MIRROR_PRV_LENGTH = DEFINED(CS0_SPACE_MIRROR_LENGTH) ? CS0_SPACE_MIRROR_LENGTH : 0;
CS2_SPACE_MIRROR_PRV_START = DEFINED(CS2_SPACE_MIRROR_START) ? CS2_SPACE_MIRROR_START : 0;
CS2_SPACE_MIRROR_PRV_LENGTH = DEFINED(CS2_SPACE_MIRROR_LENGTH) ? CS2_SPACE_MIRROR_LENGTH : 0;
CS3_SPACE_MIRROR_PRV_START = DEFINED(CS3_SPACE_MIRROR_START) ? CS3_SPACE_MIRROR_START : 0;
CS3_SPACE_MIRROR_PRV_LENGTH = DEFINED(CS3_SPACE_MIRROR_LENGTH) ? CS3_SPACE_MIRROR_LENGTH : 0;
CS5_SPACE_MIRROR_PRV_START = DEFINED(CS5_SPACE_MIRROR_START) ? CS5_SPACE_MIRROR_START : 0;
CS5_SPACE_MIRROR_PRV_LENGTH = DEFINED(CS5_SPACE_MIRROR_LENGTH) ? CS5_SPACE_MIRROR_LENGTH : 0;
xSPI0_CS0_SPACE_PRV_START = DEFINED(xSPI0_CS0_SPACE_START) ? xSPI0_CS0_SPACE_START : 0;
xSPI0_CS0_SPACE_PRV_LENGTH = DEFINED(xSPI0_CS0_SPACE_LENGTH) ? xSPI0_CS0_SPACE_LENGTH : 0;
xSPI0_CS1_SPACE_PRV_START = DEFINED(xSPI0_CS1_SPACE_START) ? xSPI0_CS1_SPACE_START : 0;
xSPI0_CS1_SPACE_PRV_LENGTH = DEFINED(xSPI0_CS1_SPACE_LENGTH) ? xSPI0_CS1_SPACE_LENGTH : 0;
xSPI1_CS0_SPACE_PRV_START = DEFINED(xSPI1_CS0_SPACE_START) ? xSPI1_CS0_SPACE_START : 0;
xSPI1_CS0_SPACE_PRV_LENGTH = DEFINED(xSPI1_CS0_SPACE_LENGTH) ? xSPI1_CS0_SPACE_LENGTH : 0;
xSPI1_CS1_SPACE_PRV_START = DEFINED(xSPI1_CS1_SPACE_START) ? xSPI1_CS1_SPACE_START : 0;
xSPI1_CS1_SPACE_PRV_LENGTH = DEFINED(xSPI1_CS1_SPACE_LENGTH) ? xSPI1_CS1_SPACE_LENGTH : 0;
CS0_SPACE_PRV_START = DEFINED(CS0_SPACE_START) ? CS0_SPACE_START : 0;
CS0_SPACE_PRV_LENGTH = DEFINED(CS0_SPACE_LENGTH) ? CS0_SPACE_LENGTH : 0;
CS2_SPACE_PRV_START = DEFINED(CS2_SPACE_START) ? CS2_SPACE_START : 0;
CS2_SPACE_PRV_LENGTH = DEFINED(CS2_SPACE_LENGTH) ? CS2_SPACE_LENGTH : 0;
CS3_SPACE_PRV_START = DEFINED(CS3_SPACE_START) ? CS3_SPACE_START : 0;
CS3_SPACE_PRV_LENGTH = DEFINED(CS3_SPACE_LENGTH) ? CS3_SPACE_LENGTH : 0;
CS5_SPACE_PRV_START = DEFINED(CS5_SPACE_START) ? CS5_SPACE_START : 0;
CS5_SPACE_PRV_LENGTH = DEFINED(CS5_SPACE_LENGTH) ? CS5_SPACE_LENGTH : 0;

LOADER_PARAM_ADDRESS            = DEFINED(CR52_0) ? ALIGN(xSPI0_CS0_SPACE_PRV_START, 0x00020000) : _loader_text_start;
FLASH_CONTENTS_ADDRESS          = DEFINED(CR52_0) ? LOADER_PARAM_ADDRESS + 0x0000004C : _loader_text_start;
NONCACHE_FLASH_CONTENTS_ADDRESS = DEFINED(CR52_0) ? flash_contents_end : _fvector_start + NONCACHE_OFFSET;
LOADER_TEXT_ADDRESS             = DEFINED(CR52_0) ? 0x00102000 : 0x10001000;
INTVEC_ADDRESS                  = DEFINED(CR52_0) ? 0x00000000 : 0x10000000;
TEXT_ADDRESS                    = DEFINED(CR52_0) ? 0x00000100 : noncache_flash_contents_end;
RAM_START                       = DEFINED(CR52_0) ? ATCM_PRV_START : SYSTEM_RAM_PRV_START;
RAM_LENGTH                      = DEFINED(CR52_0) ? ATCM_PRV_LENGTH : SYSTEM_RAM_PRV_LENGTH;
LOADER_START                    = DEFINED(CR52_0) ? BTCM_PRV_START : SYSTEM_RAM_PRV_START;
LOADER_LENGTH                   = DEFINED(CR52_0) ? BTCM_PRV_LENGTH : SYSTEM_RAM_PRV_LENGTH;
ROM_START                       = DEFINED(CR52_0) ? xSPI0_CS0_SPACE_PRV_START : SYSTEM_RAM_PRV_START;
ROM_LENGTH                      = DEFINED(CR52_0) ? xSPI0_CS0_SPACE_PRV_START : SYSTEM_RAM_PRV_LENGTH;

LOADER_TEXT_IMAGE               = DEFINED(CR52_0) ? _mloader_text : _loader_text_start;
LOADER_DATA_IMAGE               = DEFINED(CR52_0) ? _mloader_data : _loader_data_start;
INTVEC_IMAGE                    = DEFINED(CR52_0) ? _mfvector : _fvector_start;
TEXT_IMAGE                      = DEFINED(CR52_0) ? _mtext : _text_start;
DATA_IMAGE                      = DEFINED(CR52_0) ? _mdata : _data_start;
SECONDARY_IMAGE                 = ALIGN(noncache_flash_contents_end, 0x20000);
SECONDARY_START                 = SYSTEM_RAM_PRV_START;
IMAGE_INFO_ADDRESS              = _fvector_start + 0x100;

/* Define starting addresses and length for data_noncache, DMAC link mode data, CPU-shared non-cache, and CPU-specific non-cache areas. */
NONCACHE_OFFSET                 = DEFINED(SYSTEM_RAM_MIRROR_START) ? 0x00010000 : 0;
SYSTEM_RAM_MIRROR_OFFSET        = DEFINED(SYSTEM_RAM_MIRROR_START) ? 0x20000000 : 0;
DATA_NONCACHE_OFFSET            = DEFINED(SYSTEM_RAM_MIRROR_START) ? 0x00068000 : 0;
DMAC_LINK_MODE_OFFSET           = DEFINED(SYSTEM_RAM_MIRROR_START) ? 0x00064000 : 0;
SHARED_NONCACHE_BUFFER_OFFSET   = DEFINED(SYSTEM_RAM_MIRROR_START) ? 0x00060000 : 0;
NONCACHE_BUFFER_OFFSET          = DEFINED(SYSTEM_RAM_MIRROR_START) ? 0x00020000 : 0;
DATA_NONCACHE_START             = DEFINED(CR52_0) ? SYSTEM_RAM_MIRROR_PRV_START + SYSTEM_RAM_MIRROR_PRV_LENGTH - DATA_NONCACHE_OFFSET : _mdata_noncache + SYSTEM_RAM_MIRROR_OFFSET;
DMAC_LINK_MODE_START            = DEFINED(CR52_0) ? SYSTEM_RAM_MIRROR_PRV_START + SYSTEM_RAM_MIRROR_PRV_LENGTH - DMAC_LINK_MODE_OFFSET : _mdmac_link_mode + SYSTEM_RAM_MIRROR_OFFSET;
SHARED_NONCACHE_BUFFER_START    = SYSTEM_RAM_MIRROR_PRV_START + SYSTEM_RAM_MIRROR_PRV_LENGTH - SHARED_NONCACHE_BUFFER_OFFSET;
NONCACHE_BUFFER_START           = DEFINED(CR52_0) ? SYSTEM_RAM_MIRROR_PRV_START + SYSTEM_RAM_MIRROR_PRV_LENGTH - NONCACHE_BUFFER_OFFSET : _mncbuffer + SYSTEM_RAM_MIRROR_OFFSET;

MEMORY
{
    ATCM : ORIGIN = ATCM_PRV_START, LENGTH = ATCM_PRV_LENGTH
    BTCM : ORIGIN = BTCM_PRV_START, LENGTH = BTCM_PRV_LENGTH
    SYSTEM_RAM : ORIGIN = SYSTEM_RAM_PRV_START, LENGTH = SYSTEM_RAM_PRV_LENGTH
    SYSTEM_RAM_MIRROR : ORIGIN = SYSTEM_RAM_MIRROR_PRV_START, LENGTH = SYSTEM_RAM_MIRROR_PRV_LENGTH
    xSPI0_CS0_SPACE_MIRROR : ORIGIN = xSPI0_CS0_SPACE_MIRROR_PRV_START, LENGTH = xSPI0_CS0_SPACE_MIRROR_PRV_LENGTH
    xSPI0_CS1_SPACE_MIRROR : ORIGIN = xSPI0_CS1_SPACE_MIRROR_PRV_START, LENGTH = xSPI0_CS1_SPACE_MIRROR_PRV_LENGTH
    xSPI1_CS0_SPACE_MIRROR : ORIGIN = xSPI1_CS0_SPACE_MIRROR_PRV_START, LENGTH = xSPI1_CS0_SPACE_MIRROR_PRV_LENGTH
    xSPI1_CS1_SPACE_MIRROR : ORIGIN = xSPI1_CS1_SPACE_MIRROR_PRV_START, LENGTH = xSPI1_CS1_SPACE_MIRROR_PRV_LENGTH
    CS0_SPACE_MIRROR : ORIGIN = CS0_SPACE_MIRROR_PRV_START, LENGTH = CS0_SPACE_MIRROR_PRV_LENGTH
    CS2_SPACE_MIRROR : ORIGIN = CS2_SPACE_MIRROR_PRV_START, LENGTH = CS2_SPACE_MIRROR_PRV_LENGTH
    CS3_SPACE_MIRROR : ORIGIN = CS3_SPACE_MIRROR_PRV_START, LENGTH = CS3_SPACE_MIRROR_PRV_LENGTH
    CS5_SPACE_MIRROR : ORIGIN = CS5_SPACE_MIRROR_PRV_START, LENGTH = CS5_SPACE_MIRROR_PRV_LENGTH
    xSPI0_CS0_SPACE : ORIGIN = xSPI0_CS0_SPACE_PRV_START, LENGTH = xSPI0_CS0_SPACE_PRV_LENGTH
    xSPI0_CS1_SPACE : ORIGIN = xSPI0_CS1_SPACE_PRV_START, LENGTH = xSPI0_CS1_SPACE_PRV_LENGTH
    xSPI1_CS0_SPACE : ORIGIN = xSPI1_CS0_SPACE_PRV_START, LENGTH = xSPI1_CS0_SPACE_PRV_LENGTH
    xSPI1_CS1_SPACE : ORIGIN = xSPI1_CS1_SPACE_PRV_START, LENGTH = xSPI1_CS1_SPACE_PRV_LENGTH
    CS0_SPACE : ORIGIN = CS0_SPACE_PRV_START, LENGTH = CS0_SPACE_PRV_LENGTH
    CS2_SPACE : ORIGIN = CS2_SPACE_PRV_START, LENGTH = CS2_SPACE_PRV_LENGTH
    CS3_SPACE : ORIGIN = CS3_SPACE_PRV_START, LENGTH = CS3_SPACE_PRV_LENGTH
    CS5_SPACE : ORIGIN = CS5_SPACE_PRV_START, LENGTH = CS5_SPACE_PRV_LENGTH
    RAM_TEXT : ORIGIN = RAM_START, LENGTH = RAM_LENGTH
    RAM_DATA : ORIGIN = RAM_START, LENGTH = RAM_LENGTH
    LOADER_TEXT_STACK : ORIGIN = LOADER_START, LENGTH = LOADER_LENGTH
    LOADER_DATA_STACK : ORIGIN = LOADER_START, LENGTH = LOADER_LENGTH
    FLASH_CONTENTS : ORIGIN = ROM_START, LENGTH = ROM_LENGTH
}

SECTIONS
{
    .loader_param LOADER_PARAM_ADDRESS : AT (LOADER_PARAM_ADDRESS)
    {
        KEEP(*(.loader_param))
    } > FLASH_CONTENTS
    .flash_contents FLASH_CONTENTS_ADDRESS : AT (FLASH_CONTENTS_ADDRESS)
    {
        _mloader_text = .;
        . = DEFINED(CR52_0) ? . + (_loader_text_end - _loader_text_start) : .;
        _mloader_data = .;
        . = DEFINED(CR52_0) ? . + (_loader_data_end - _loader_data_start) : .;
        _mfvector = .;
        . = DEFINED(CR52_0) ? . + (_fvector_end - _fvector_start) : .;
        _mtext = .;
        . = DEFINED(CR52_0) ? . + (_text_end - _text_start) : .;
        _mdata = .;
        . = DEFINED(CR52_0) ? . + (_data_end - _data_start) : .;
        flash_contents_end = .;
    } > FLASH_CONTENTS
    .noncache_flash_contents NONCACHE_FLASH_CONTENTS_ADDRESS : AT (NONCACHE_FLASH_CONTENTS_ADDRESS)
    {
        . = ALIGN(4);
        _mdata_noncache = .;
        . = . + (_data_noncache_end - _data_noncache_start);
        . = ALIGN(4);
        _mdmac_link_mode = .;
        . = . + (_dmac_link_mode_end - _dmac_link_mode_start);
        . = ALIGN(32);
        _msncbuffer = .;
        . = . + (_sncbuffer_end - _sncbuffer_start);
        . = ALIGN(32);
        _mncbuffer = .;
        . = . + (_ncbuffer_end - _ncbuffer_start);
        noncache_flash_contents_end = .;
    } > FLASH_CONTENTS
    .secondary SECONDARY_START : AT (SECONDARY_IMAGE)
    {
        _secondary_start = .;
        KEEP(*(.secondary))
        _secondary_end = .;
    } > SYSTEM_RAM
    .loader_text LOADER_TEXT_ADDRESS : AT (LOADER_TEXT_IMAGE)
    {
        _loader_text_start = .;
        *(.loader_text)
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(.text*)
        */fsp/src/bsp/mcu/all/*/bsp_irq_core.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_irq.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_semaphore.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.text*)
        */fsp/src/bsp/mcu/all/bsp_cache.o(.text*)
        */fsp/src/r_ioport/r_ioport.o(.text*)
        KEEP(*(.warm_start))
        KEEP(*(.loader_user_data*))
        . = ALIGN(0x200);
        _loader_text_end = .;
    } > LOADER_TEXT_STACK
    .loader_data _loader_text_end : AT (LOADER_DATA_IMAGE)
    {
        _loader_data_start = .;
        __loader_data_start = .;
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(.data*)
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*/system_core.o(.rodata*)
        */fsp/src/bsp/mcu/all/*/bsp_irq_core.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_irq.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_semaphore.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.data*)
        */fsp/src/bsp/mcu/all/bsp_cache.o(.data*)
        */fsp/src/r_ioport/r_ioport.o(.data*)
        . = ALIGN(4);
        __loader_data_end = .;
        __loader_bss_start = .;
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(.bss*)
        */fsp/src/bsp/mcu/all/*/bsp_irq_core.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_irq.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_semaphore.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.bss*)
        */fsp/src/bsp/mcu/all/bsp_cache.o(.bss*)
        */fsp/src/r_ioport/r_ioport.o(.bss*)
        */fsp/src/bsp/cmsis/Device/RENESAS/Source/*.o(COMMON)
        */fsp/src/bsp/mcu/all/*/bsp_irq_core.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_clocks.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_irq.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_semaphore.o(COMMON)
        */fsp/src/bsp/mcu/all/bsp_register_protection.o(.COMMON)
        */fsp/src/bsp/mcu/all/bsp_cache.o(COMMON)
        */fsp/src/r_ioport/r_ioport.o(.COMMON)
        . = ALIGN(4);
        __loader_bss_end = . ;
        _loader_data_end = .;
    } > LOADER_DATA_STACK
    .intvec INTVEC_ADDRESS : AT (INTVEC_IMAGE)
    {
        _fvector_start = .;
        KEEP(*(.intvec))
        _fvector_end = .;
    } > RAM_TEXT
    .image_info IMAGE_INFO_ADDRESS : AT (IMAGE_INFO_ADDRESS)
    {
        _image_info_start = .;
        KEEP(*(.image_info))
        _image_info_end = .;
    } > RAM_DATA
    .text TEXT_ADDRESS : AT (TEXT_IMAGE)
    {
        _text_start = .;
        *(.text*)

        KEEP(*(.reset_handler))

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)
        _ctor_end = .;

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        _dtor_end = .;

        KEEP(*(.eh_frame*))
    } > RAM_TEXT
    .rvectors :
    {
        _rvectors_start = .;
        KEEP(*(.rvectors))
        _rvectors_end = .;
    } > RAM_TEXT
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > RAM_TEXT
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
        _text_end = .;
    } > RAM_TEXT
    .data _text_end :  AT (DATA_IMAGE)
    {
        _data_start = .;

        *(vtable)
        *(.data.*)
        *(.data)

        *(.rodata*)
        _erodata = .;

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))

        . = ALIGN(4);

    } > RAM_DATA
    .got :
    {
        *(.got)
        *(.got.plt)

        /* All data end */
        _data_end = .;
    } > RAM_DATA
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        _ebss = .;
        _end = .;
    } > RAM_DATA
    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        __HeapBase = .;
        /* Place the STD heap here. */
        KEEP(*(.heap))
        __HeapLimit = .;
    } > RAM_DATA
    .thread_stack (NOLOAD):
    {
        . = ALIGN(8);
        __ThreadStackBase = .;
        /* Place the Thread stacks here. */
        KEEP(*(.stack*))
        __ThreadStackLimit = .;
    } > RAM_DATA
    .sys_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __SysStackBase = .;
        /* Place the sys_stack here. */
        KEEP(*(.sys_stack))
        __SysStackLimit = .;
    } > LOADER_DATA_STACK
    .svc_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __SvcStackBase = .;
        /* Place the svc_stack here. */
        KEEP(*(.svc_stack))
        __SvcStackLimit = .;
    } > LOADER_DATA_STACK
    .irq_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __IrqStackBase = .;
        /* Place the irq_stack here. */
        KEEP(*(.irq_stack))
        __IrqStackLimit = .;
    } > LOADER_DATA_STACK
    .fiq_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __FiqStackBase = .;
        /* Place the fiq_stack here. */
        KEEP(*(.fiq_stack))
        __FiqStackLimit = .;
    } > LOADER_DATA_STACK
    .und_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __UndStackBase = .;
        /* Place the und_stack here. */
        KEEP(*(.und_stack))
        __UndStackLimit = .;
    } > LOADER_DATA_STACK
    .abt_stack (NOLOAD) :
    {
        . = ALIGN(8);
        __AbtStackBase = .;
        /* Place the abt_stack here. */
        KEEP(*(.abt_stack))
        __AbtStackLimit = .;
    } > LOADER_DATA_STACK
    .data_noncache DATA_NONCACHE_START : AT (_mdata_noncache)
    {
        . = ALIGN(4);
        _data_noncache_start = .;
        KEEP(*(.data_noncache*))
        _data_noncache_end = .;
    } > SYSTEM_RAM_MIRROR
    .dmac_link_mode DMAC_LINK_MODE_START : AT (_mdmac_link_mode)
    {
        . = ALIGN(4);
        _dmac_link_mode_start = .;
        KEEP(*(.dmac_link_mode*))
        _dmac_link_mode_end = .;
    } > SYSTEM_RAM_MIRROR
    .shared_noncache_buffer SHARED_NONCACHE_BUFFER_START : AT (_msncbuffer)
    {
        . = ALIGN(32);
        _sncbuffer_start = .;
        KEEP(*(.shared_noncache_buffer*))
        _sncbuffer_end = .;
    } > SYSTEM_RAM_MIRROR
    .noncache_buffer NONCACHE_BUFFER_START : AT (_mncbuffer)
    {
        . = ALIGN(32);
        _ncbuffer_start = .;
        KEEP(*(.noncache_buffer*))
        _ncbuffer_end = .;
    } > SYSTEM_RAM_MIRROR
}

__ddsc_xSPI0_CS0_SPACE_START = DEFINED(CR52_0) ? LOADER_PARAM_ADDRESS : 0x60020000;
__ddsc_xSPI0_CS0_SPACE_END = DEFINED(CR52_0) ? noncache_flash_contents_end : 0x60040000 ;