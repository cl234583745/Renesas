/* This file is used to include the application image into the loader project. */

.section .IMAGE_APP_FLASH_section, "ax", %progbits
.incbin "../../RZN2L_bsp_xspi0bootx1_app/Debug/RZN2L_bsp_xspi0bootx1_app.bin"
