fsp_xspi0_boot.ld				配置xspi0启动后自动生成的，app均使用atcm
fsp_xspi0_boot_sram_all.ld		整体修改app atcm为sram(sram mirror),需要注意dmac usb等不能使用sram，需要使用noncache_buffer
fsp_xspi0_boot_sram_text.ld		只修改text到sram，其余app仍在atcm
fsp_xspi0_boot_sram_atcm.ld		同时使用sram和atcm。优先使用sram，次要使用atcm