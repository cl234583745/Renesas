/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sci.h
* Version      : 1.1.101
* Device(s)    : R5F523E6AxFL
* Description  : General header file for SCI peripheral.
* Creation Date: 2020-07-02
***********************************************************************************************************************/

#ifndef SCI_H
#define SCI_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Serial mode register (SMR)
*/
/* Clock select (CKS) */
#define _00_SCI_CLOCK_PCLK                        (0x00U) /* PCLK */
#define _01_SCI_CLOCK_PCLK_4                      (0x01U) /* PCLK/4 */
#define _02_SCI_CLOCK_PCLK_16                     (0x02U) /* PCLK/16 */
#define _03_SCI_CLOCK_PCLK_64                     (0x03U) /* PCLK/64 */
/* Multi-processor Mode (MP) */
#define _00_SCI_MULTI_PROCESSOR_DISABLE           (0x00U) /* Disable multiprocessor mode */
#define _04_SCI_MULTI_PROCESSOR_ENABLE            (0x04U) /* Enable multiprocessor mode */
/* Stop bit length (STOP) */
#define _00_SCI_STOP_1                            (0x00U) /* 1 stop bit length */
#define _08_SCI_STOP_2                            (0x08U) /* 2 stop bits length */
/* Parity mode (PM) */
#define _00_SCI_PARITY_EVEN                       (0x00U) /* Parity even */
#define _10_SCI_PARITY_ODD                        (0x10U) /* Parity odd */
/* Parity enable (PE) */
#define _00_SCI_PARITY_DISABLE                    (0x00U) /* Parity disable */
#define _20_SCI_PARITY_ENABLE                     (0x20U) /* Parity enable */
/* Character length (CHR) */
#define _00_SCI_DATA_LENGTH_8                     (0x00U) /* Data length 8 bits */
#define _40_SCI_DATA_LENGTH_7                     (0x40U) /* Data length 7 bits */
/* Communications mode (CM) */
#define _00_SCI_ASYNCHRONOUS_OR_I2C_MODE          (0x00U) /* Asynchronous mode or simple I2C mode */
#define _80_SCI_CLOCK_SYNCHRONOUS_OR_SPI_MODE     (0x80U) /* Clock synchronous mode or simple SPI mode */
/* Base clock pulse (BCP) */
#define _00_SCI_32_93_CLOCK_CYCLES                (0x00U) /* 32 or 93 clock cycles */
#define _04_SCI_64_128_CLOCK_CYCLES               (0x04U) /* 64 or 128 clock cycles */
#define _08_SCI_186_372_CLOCK_CYCLES              (0x08U) /* 186 or 372 clock cycles */
#define _0C_SCI_256_512_CLOCK_CYCLES              (0x0CU) /* 256 or 512 clock cycles */
/* Block transfer mode (BLK) */
#define _00_SCI_BLK_TRANSFER_DISABLE              (0x00U) /* Block transfer disable */
#define _40_SCI_BLK_TRANSFER_ENABLE               (0x40U) /* Block transfer enable */
/* GSM mode (GSM) */
#define _00_SCI_GSM_DISABLE                       (0x00U) /* Normal mode operation */
#define _80_SCI_GSM_ENABLE                        (0x80U) /* GSM mode operation */

/*
    Serial control register (SCR)
*/
/* Clock enable (CKE) */
#define _00_SCI_INTERNAL_SCK_UNUSED               (0x00U) /* Internal clock selected, SCK pin unused */
#define _00_SCI_INTERNAL_SCK_FIXED_LOW            (0x00U) /* GSM mode SCK fixed to low */
#define _01_SCI_INTERNAL_SCK_OUTPUT               (0x01U) /* Internal clock selected, SCK pin as clock output */
#define _02_SCI_INTERNAL_SCK_FIXED_HIGH           (0x02U) /* GSM mode SCK fixed to high */
#define _02_SCI_EXTERNAL                          (0x02U) /* External clock selected */
#define _02_SCI_EXTERNAL_TMR                      (0x02U) /* External or TMR clock selected */
#define _03_SCI_EXTERNAL                          (0x03U) /* External clock selected */
/* Transmit end interrupt enable (TEIE) */
#define _00_SCI_TEI_INTERRUPT_DISABLE             (0x00U) /* TEI interrupt request disable */
#define _04_SCI_TEI_INTERRUPT_ENABLE              (0x04U) /* TEI interrupt request enable */
/* Multi-processor interrupt enable (MPIE) */
#define _00_SCI_MP_INTERRUPT_NORMAL               (0x00U) /* Normal reception */
#define _08_SCI_MP_INTERRUPT_SPECIAL              (0x08U) /* Multi-processor ID reception */
/* Receive enable (RE) */
#define _00_SCI_RECEIVE_DISABLE                   (0x00U) /* Disable receive mode */
#define _10_SCI_RECEIVE_ENABLE                    (0x10U) /* Enable receive mode */
/* Transmit enable (TE) */
#define _00_SCI_TRANSMIT_DISABLE                  (0x00U) /* Disable transmit mode */
#define _20_SCI_TRANSMIT_ENABLE                   (0x20U) /* Enable transmit mode */
/* Receive interrupt enable (RIE) */
#define _00_SCI_RXI_ERI_DISABLE                   (0x00U) /* Disable RXI and ERI interrupt requests */
#define _40_SCI_RXI_ERI_ENABLE                    (0x40U) /* Enable RXI and ERI interrupt requests */
/* Transmit interrupt enable (TIE) */
#define _00_SCI_TXI_DISABLE                       (0x00U) /* Disable TXI interrupt requests */
#define _80_SCI_TXI_ENABLE                        (0x80U) /* Enable TXI interrupt requests */

/*
    Serial status register (SSR)
*/
/* Multi-Processor bit transfer */
#define _00_SCI_SET_DATA_TRANSFER                 (0x00U) /* Set data transmission cycles */
#define _01_SCI_SET_ID_TRANSFER                   (0x01U) /* Set ID transmission cycles */
/* Multi-Processor */
#define _00_SCI_DATA_TRANSFER                     (0x00U) /* In data transmission cycles */
#define _02_SCI_ID_TRANSFER                       (0x02U) /* In ID transmission cycles */
/* Transmit end flag (TEND) */
#define _00_SCI_TRANSMITTING                      (0x00U) /* A character is being transmitted */
#define _04_SCI_TRANSMIT_COMPLETE                 (0x04U) /* Character transfer has been completed */
/* Parity error flag (PER) */
#define _08_SCI_PARITY_ERROR                      (0x08U) /* A parity error has occurred */
/* Framing error flag (FER) */
#define _10_SCI_FRAME_ERROR                       (0x10U) /* A framing error has occurred */
/* Error signal status flag (ERS) */
#define _10_SCI_LOW_ERROR_DETECTED                (0x10U) /* A low error signal responded */
/* Overrun error flag (ORER) */
#define _20_SCI_OVERRUN_ERROR                     (0x20U) /* An overrun error has occurred */
/* Receive Data Full Flag (RDRF) */
#define _40_SCI_RECEIVE_DATAFULL                  (0x40U) /* Data has been received normally, and transferred from 
                                                             RSR to RDR */
/* Transmit Data Empty Flag (TDRE) */
#define _80_SCI_TRANSMIT_DATAEMPTY                (0x80U) /* Data is transferred from TDR to TSR */

/*
    Smart card mode register (SCMR)
*/
/* Smart card interface mode select (SMIF) */
#define _00_SCI_SERIAL_MODE                       (0x00U) /* Serial communications interface mode */
#define _01_SCI_SMART_CARD_MODE                   (0x01U) /* Smart card interface mode */
/* Transmitted / received data invert (SINV) */
#define _00_SCI_DATA_INVERT_NONE                  (0x00U) /* Data is not inverted */
#define _04_SCI_DATA_INVERTED                     (0x04U) /* Data is inverted */
/* Transmitted / received data transfer direction (SDIR) */
#define _00_SCI_DATA_LSB_FIRST                    (0x00U) /* Transfer data LSB first */
#define _08_SCI_DATA_MSB_FIRST                    (0x08U) /* Transfer data MSB first */
/* Character length 1 (CHR1)*/
#define _00_SCI_DATA_LENGTH_9                     (0x00U) /* Transmit/receive in 9-bit data length */
#define _10_SCI_DATA_LENGTH_8_OR_7                (0x10U) /* Transmit/receive in 8-bit or 7-bit data length */
/* Base clock pulse 2 (BCP2) */
#define _00_SCI_93_128_186_512_CLK                (0x00U) /* 93, 128, 186, or 512 clock cycles */
#define _80_SCI_32_64_256_372_CLK                 (0x80U) /* 32, 64, 256, or 372 clock cycles */
#define _62_SCI_SCMR_DEFAULT                      (0x62U) /* Write default value of SCMR */

/*
    Serial extended mode register (SEMR)
*/
/* Asynchronous Mode Clock Source Select (ACS0) */
#define _00_SCI_ASYNC_SOURCE_EXTERNAL             (0x00U) /* External clock input */
#define _01_SCI_ASYNC_SOURCE_TMR                  (0x01U) /* Logical AND of two clock cycles output from TMR */
/* Bit Modulation Enable (BRME) */
#define _00_SCI_BIT_MODULATION_DISABLE            (0x00U) /* Bit rate modulation function is disabled */
#define _04_SCI_BIT_MODULATION_ENABLE             (0x04U) /* Bit rate modulation function is enabled */
/* Asynchronous mode base clock select (ABCS) */
#define _00_SCI_16_BASE_CLOCK                     (0x00U) /* Selects 16 base clock cycles for 1 bit period */
#define _10_SCI_8_BASE_CLOCK                      (0x10U) /* Selects 8 base clock cycles for 1 bit period */
/* Digital noise filter function enable (NFEN) */
#define _00_SCI_NOISE_FILTER_DISABLE              (0x00U) /* Noise filter is disabled */
#define _20_SCI_NOISE_FILTER_ENABLE               (0x20U) /* Noise filter is enabled */
/* Baud Rate Generator Double-Speed Mode Select (BGDM) */
#define _00_SCI_BAUDRATE_SINGLE                   (0x00U) /* Baud rate generator outputs normal frequency */
#define _40_SCI_BAUDRATE_DOUBLE                   (0x40U) /* Baud rate generator doubles output frequency */
/* Asynchronous start bit edge detections select (RXDESEL) */
#define _00_SCI_LOW_LEVEL_START_BIT               (0x00U) /* Low level on RXDn pin selected as start bit */
#define _80_SCI_FALLING_EDGE_START_BIT            (0x80U) /* Falling edge on RXDn pin selected as start bit */

/*
    Noise filter setting register (SNFR)
*/
/* Noise filter clock select (NFCS) */
#define _00_SCI_ASYNC_DIV_1                       (0x00U) /* Clock signal divided by 1 is used with the noise filter */
#define _01_SCI_IIC_DIV_1                         (0x01U) /* Clock signal divided by 1 is used with the noise filter */
#define _02_SCI_IIC_DIV_2                         (0x02U) /* Clock signal divided by 2 is used with the noise filter */
#define _03_SCI_IIC_DIV_4                         (0x03U) /* Clock signal divided by 4 is used with the noise filter */
#define _04_SCI_IIC_DIV_8                         (0x04U) /* Clock signal divided by 8 is used with the noise filter */

/*
    I2C mode register 1 (SIMR1)
*/
/* Simple IIC mode select (IICM) */
#define _00_SCI_SERIAL_SMART_CARD_MODE            (0x00U) /* Serial or smart card mode */
#define _01_SCI_IIC_MODE                          (0x01U) /* Simple IIC mode */
/* SSDA Output Delay Select (IICDL)*/
#define _00_SCI_NONE                              (0x00U) /* No output delay */
#define _08_SCI_0_TO_1_CYCLE                      (0x08U) /* 0 to 1 cycle */
#define _10_SCI_1_TO_2_CYCLE                      (0x10U) /* 1 to 2 cycles */
#define _18_SCI_2_TO_3_CYCLE                      (0x18U) /* 2 to 3 cycles */
#define _20_SCI_3_TO_4_CYCLE                      (0x20U) /* 3 to 4 cycles */
#define _28_SCI_4_TO_5_CYCLE                      (0x28U) /* 4 to 5 cycles */
#define _30_SCI_5_TO_6_CYCLE                      (0x30U) /* 5 to 6 cycles */
#define _38_SCI_6_TO_7_CYCLE                      (0x38U) /* 6 to 7 cycles */
#define _40_SCI_7_TO_8_CYCLE                      (0x40U) /* 7 to 8 cycles */
#define _48_SCI_8_TO_9_CYCLE                      (0x48U) /* 8 to 9 cycles */
#define _50_SCI_9_TO_10_CYCLE                     (0x50U) /* 9 to 10 cycles */
#define _58_SCI_10_TO_11_CYCLE                    (0x58U) /* 10 to 11 cycles */
#define _60_SCI_11_TO_12_CYCLE                    (0x60U) /* 11 to 12 cycles */
#define _68_SCI_12_TO_13_CYCLE                    (0x68U) /* 12 to 13 cycles */
#define _70_SCI_13_TO_14_CYCLE                    (0x70U) /* 13 to 14 cycles */
#define _78_SCI_14_TO_15_CYCLE                    (0x78U) /* 14 to 15 cycles */
#define _80_SCI_15_TO_16_CYCLE                    (0x80U) /* 15 to 16 cycles */
#define _88_SCI_16_TO_17_CYCLE                    (0x88U) /* 16 to 17 cycles */
#define _90_SCI_17_TO_18_CYCLE                    (0x90U) /* 17 to 18 cycles */
#define _98_SCI_18_TO_19_CYCLE                    (0x98U) /* 18 to 19 cycles */
#define _A0_SCI_19_TO_20_CYCLE                    (0xA0U) /* 19 to 20 cycles */
#define _A8_SCI_20_TO_21_CYCLE                    (0xA8U) /* 20 to 21 cycles */
#define _B0_SCI_21_TO_22_CYCLE                    (0xB0U) /* 21 to 22 cycles */
#define _B8_SCI_22_TO_23_CYCLE                    (0xB8U) /* 22 to 23 cycles */
#define _C0_SCI_23_TO_24_CYCLE                    (0xC0U) /* 23 to 24 cycles */
#define _C8_SCI_24_TO_25_CYCLE                    (0xC8U) /* 24 to 25 cycles */
#define _D0_SCI_25_TO_26_CYCLE                    (0xD0U) /* 25 to 26 cycles */
#define _D8_SCI_26_TO_27_CYCLE                    (0xD8U) /* 26 to 27 cycles */
#define _E0_SCI_27_TO_28_CYCLE                    (0xE0U) /* 27 to 28 cycles */
#define _E8_SCI_28_TO_29_CYCLE                    (0xE8U) /* 28 to 29 cycles */
#define _F0_SCI_29_TO_30_CYCLE                    (0xF0U) /* 29 to 30 cycles */
#define _F8_SCI_30_TO_31_CYCLE                    (0xF8U) /* 30 to 31 cycles */

/*
    I2C mode register 2 (SIMR2)
*/
/* IIC interrupt mode select (IICINTM) */
#define _00_SCI_ACK_NACK_INTERRUPTS               (0x00U) /* Use ACK/NACK interrupts */
#define _01_SCI_RX_TX_INTERRUPTS                  (0x01U) /* Use reception/transmission interrupts */
/* Clock synchronization (IICCSC) */
#define _00_SCI_NO_SYNCHRONIZATION                (0x00U) /* No synchronization with the clock signal */
#define _02_SCI_SYNCHRONIZATION                   (0x02U) /* Synchronization with the clock signal */
/* ACK transmission data (IICACKT) */
#define _00_SCI_ACK_TRANSMISSION                  (0x00U) /* ACK transmission */
#define _20_SCI_NACK_TRANSMISSION                 (0x20U) /* NACK transmission and reception of ACK/NACK */

/*
    I2C mode register 3 (SIMR3)
*/
/* Start condition generation (IICSTAREQ) */
#define _00_SCI_START_CONDITION_OFF               (0x00U) /* Start condition is not generated */
#define _01_SCI_START_CONDITION_ON                (0x01U) /* Start condition is generated */
/* Restart condition generation (IICRSTAREQ) */
#define _00_SCI_RESTART_CONDITION_OFF             (0x00U) /* Restart condition is not generated */
#define _02_SCI_RESTART_CONDITION_ON              (0x02U) /* Restart condition is generated */
/* Stop condition generation (IICSTPREQ) */
#define _00_SCI_STOP_CONDITION_OFF                (0x00U) /* Stop condition is not generated */
#define _04_SCI_STOP_CONDITION_ON                 (0x04U) /* Stop condition is generated */
/* Issuing of start, restart, or stop condition completed flag (IICSTIF) */
#define _00_SCI_CONDITION_GENERATED               (0x00U) /* No requests to generate conditions/conditions generated */
#define _08_SCI_GENERATION_COMPLETED              (0x08U) /* All request generation has been completed */
/* SSDA output select (IICSDAS) */
#define _00_SCI_SSDA_DATA_OUTPUT                  (0x00U) /* SSDA output is serial data output */
#define _10_SCI_SSDA_START_RESTART_STOP_CONDITION (0x10U) /* SSDA output generates start, restart or stop condition */
#define _20_SCI_SSDA_LOW_LEVEL                    (0x20U) /* SSDA output low level */
#define _30_SCI_SSDA_HIGH_IMPEDANCE               (0x30U) /* SSDA output high impedance */
/* SSCL output select (IICSCLS) */
#define _00_SCI_SSCL_CLOCK_OUTPUT                 (0x00U) /* SSCL output is serial clock output */
#define _40_SCI_SSCL_START_RESTART_STOP_CONDITION (0x40U) /* SSCL output generates start, restart or stop condition */
#define _80_SCI_SSCL_LOW_LEVEL                    (0x80U) /* SSCL output low level */
#define _C0_SCI_SSCL_HIGH_IMPEDANCE               (0xC0U) /* SSCL output high impedance */

/*
    I2C status register (SISR)
*/
/* ACK reception data flag (IICACKR) */
#define _00_SCI_ACK_RECEIVED                      (0x00U) /* ACK received */
#define _01_SCI_NACK_RECEIVED                     (0x01U) /* NACK received */

/*
    SPI mode register (SPMR)
*/
/* SS pin function enable (SSE) */
#define _00_SCI_SS_PIN_DISABLE                    (0x00U) /* SS pin function disabled */
#define _01_SCI_SS_PIN_ENABLE                     (0x01U) /* SS pin function enabled */
/* CTS enable (CTSE) */
#define _00_SCI_RTS                               (0x00U) /* RTS function is enabled */
#define _02_SCI_CTS                               (0x02U) /* CTS function is enabled */
/* Master slave select (MSS) */
#define _00_SCI_SPI_MASTER                        (0x00U) /* Master mode */
#define _04_SCI_SPI_SLAVE                         (0x04U) /* Slave mode */
/* Mode fault flag (MFF) */
#define _00_SCI_NO_MODE_FAULT                     (0x00U) /* No mode fault */
#define _10_SCI_MODE_FAULT                        (0x10U) /* Mode fault */
/* Clock polarity select (CKPOL) */
#define _00_SCI_CLOCK_NOT_INVERTED                (0x00U) /* Clock polarity is not inverted */
#define _40_SCI_CLOCK_INVERTED                    (0x40U) /* Clock polarity is inverted */
/* Clock phase select (CKPH) */
#define _00_SCI_CLOCK_NOT_DELAYED                 (0x00U) /* Clock is not delayed */
#define _80_SCI_CLOCK_DELAYED                     (0x80U) /* Clock is delayed */

/*
    Interrupt Source Priority Register n (IPRn)
*/
/* Interrupt Priority Level Select (IPR[3:0]) */
#define _00_SCI_PRIORITY_LEVEL0                   (0x00U) /* Level 0 (interrupt disabled) */
#define _01_SCI_PRIORITY_LEVEL1                   (0x01U) /* Level 1 */
#define _02_SCI_PRIORITY_LEVEL2                   (0x02U) /* Level 2 */
#define _03_SCI_PRIORITY_LEVEL3                   (0x03U) /* Level 3 */
#define _04_SCI_PRIORITY_LEVEL4                   (0x04U) /* Level 4 */
#define _05_SCI_PRIORITY_LEVEL5                   (0x05U) /* Level 5 */
#define _06_SCI_PRIORITY_LEVEL6                   (0x06U) /* Level 6 */
#define _07_SCI_PRIORITY_LEVEL7                   (0x07U) /* Level 7 */
#define _08_SCI_PRIORITY_LEVEL8                   (0x08U) /* Level 8 */
#define _09_SCI_PRIORITY_LEVEL9                   (0x09U) /* Level 9 */
#define _0A_SCI_PRIORITY_LEVEL10                  (0x0AU) /* Level 10 */
#define _0B_SCI_PRIORITY_LEVEL11                  (0x0BU) /* Level 11 */
#define _0C_SCI_PRIORITY_LEVEL12                  (0x0CU) /* Level 12 */
#define _0D_SCI_PRIORITY_LEVEL13                  (0x0DU) /* Level 13 */
#define _0E_SCI_PRIORITY_LEVEL14                  (0x0EU) /* Level 14 */
#define _0F_SCI_PRIORITY_LEVEL15                  (0x0FU) /* Level 15 (highest) */

/*
    Transfer status control value
*/
/* Simple IIC Transmit Receive Flag */
#define _80_SCI_IIC_TRANSMISSION                  (0x80U) /* Simple IIC Transmission State */
#define _00_SCI_IIC_RECEPTION                     (0x00U) /* Simple IIC Reception State */
/* Simple IIC Start Stop Flag */
#define _80_SCI_IIC_START_CYCLE                   (0x80U) /* Simple IIC Start Cycle */
#define _00_SCI_IIC_STOP_CYCLE                    (0x00U) /* Simple IIC Stop Cycle */
/* Multiprocessor Asynchronous Communication Flag */
#define _80_SCI_ID_TRANSMISSION_CYCLE             (0x80U) /* Multiprocessor Asynchronous ID Transmission Cycle  */
#define _00_SCI_DATA_TRANSMISSION_CYCLE           (0x00U) /* Multiprocessor Asynchronous Data Transmission Cycle */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
