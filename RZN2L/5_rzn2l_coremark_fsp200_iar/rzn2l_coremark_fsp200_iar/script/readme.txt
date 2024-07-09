fsp_xspi0_boot.icf			配置xspi0启动后自动生成的，app均使用atcm
fsp_xspi0_boot_sram_all.icf		整体修改app atcm为sram(sram mirror),需要注意dmac usb等不能使用sram，需要使用noncache_buffer
fsp_xspi0_boot_sram_text.icf		只修改text到sram，其余app仍在atcm
fsp_xspi0_boot_sram_atcm.icf		同时使用sram和atcm。优先使用sram，次要使用atcm。FSP_XSPI0_BOOT_SRAM_ATCM=1（c/c++ 和 assembly）【iar单独优化文件，则预编译也是单独的】