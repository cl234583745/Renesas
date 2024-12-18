/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_FEATURE_ADC_ADDITION_SUPPORTED                (1U)
#define BSP_FEATURE_ADC_CALIBRATION_REG_AVAILABLE         (0U)
#define BSP_FEATURE_ADC_CLOCK_SOURCE                      (FSP_PRIV_CLOCK_PCLKADC)
#define BSP_FEATURE_ADC_GROUP_B_SENSORS_ALLOWED           (1U)
#define BSP_FEATURE_ADC_HAS_ADCER_ADPRC                   (1U)
#define BSP_FEATURE_ADC_HAS_ADCER_ADRFMT                  (1U)
#define BSP_FEATURE_ADC_HAS_PGA                           (1U)
#define BSP_FEATURE_ADC_HAS_SAMPLE_HOLD_REG               (1U)
#define BSP_FEATURE_ADC_HAS_SAMPLE_HOLD_UNIT_NUM          (1U)
#define BSP_FEATURE_ADC_HAS_VREFAMPCNT                    (0U)
#define BSP_FEATURE_ADC_MAX_RESOLUTION_BITS               (12U)
#define BSP_FEATURE_ADC_REGISTER_MASK_TYPE                (1U)
#define BSP_FEATURE_ADC_SENSOR_MIN_SAMPLING_TIME          (4150U)
#define BSP_FEATURE_ADC_SENSORS_EXCLUSIVE                 (0U)
#define BSP_FEATURE_ADC_TSN_CALIBRATION_AVAILABLE         (0U)
#define BSP_FEATURE_ADC_TSN_CALIBRATION32_AVAILABLE       (1U)
#define BSP_FEATURE_ADC_TSN_CALIBRATION32_MASK            (0x00000FFFU)
#define BSP_FEATURE_ADC_TSN_CONTROL_AVAILABLE             (1U)
#define BSP_FEATURE_ADC_TSN_SLOPE                         (4000U)
#define BSP_FEATURE_ADC_UNIT_0_CHANNELS                   (0x00FF) // 0 to 7 in unit 0
#define BSP_FEATURE_ADC_UNIT_1_CHANNELS                   (0xFFFF) // 0 to 15 in unit 1
#define BSP_FEATURE_ADC_VALID_UNIT_MASK                   (3U)

#define BSP_FEATURE_BSC_32BIT_DATA_BUS_WIDTH_SUPPORTED    (1U)

#define BSP_FEATURE_BSP_AFMT_UNIT                         (0U)
#define BSP_FEATURE_BSP_BISS_UNIT                         (0U)
#define BSP_FEATURE_BSP_BOOT_PARAMETER                    (1U)
#define BSP_FEATURE_BSP_CR52_CORE_NUM                     (2U)
#define BSP_FEATURE_BSP_ENCOUT_SUPPORTED                  (0U)
#define BSP_FEATURE_BSP_ENDAT_UNIT                        (0U)
#define BSP_FEATURE_BSP_HDSL_UNIT                         (0U)
#define BSP_FEATURE_BSP_IRQ_PRIORITY_MASK                 (0xF8U)
#define BSP_FEATURE_BSP_IRQ_PRIORITY_POS_BIT              (3U)
#define BSP_FEATURE_BSP_MASTER_MPU0_SUPPORTED             (1U)
#define BSP_FEATURE_BSP_MASTER_MPU1_SUPPORTED             (1U)
#define BSP_FEATURE_BSP_MASTER_MPU2_SUPPORTED             (1U)
#define BSP_FEATURE_BSP_MASTER_MPU3_SUPPORTED             (1U)
#define BSP_FEATURE_BSP_MASTER_MPU4_SUPPORTED             (1U)
#define BSP_FEATURE_BSP_MASTER_MPU6_SUPPORTED             (1U)
#define BSP_FEATURE_BSP_MASTER_MPU7_SUPPORTED             (0U)
#define BSP_FEATURE_BSP_MSTP_CR52_CPU1_HAS_MSTPCRH        (1U)
#define BSP_FEATURE_BSP_SEMAPHORE_SUPPORTED               (1U)
#define BSP_FEATURE_BSP_SHOSTIF_SUPPORTED                 (0U)

#define BSP_FEATURE_CAN_CHECK_PCLKB_RATIO                 (0U)
#define BSP_FEATURE_CAN_CLOCK                             (0U)
#define BSP_FEATURE_CAN_MCLOCK_ONLY                       (0U)
#define BSP_FEATURE_CAN_NUM_CHANNELS                      (2U)

#define BSP_FEATURE_CANFD_NUM_CHANNELS                    (2U)
#define BSP_FEATURE_CANFD_NUM_INSTANCES                   (1U)

#define BSP_FEATURE_CGC_HAS_BCLK                          (1U)
#define BSP_FEATURE_CGC_HAS_FCLK                          (1U)
#define BSP_FEATURE_CGC_HAS_FLDWAITR                      (0U)
#define BSP_FEATURE_CGC_HAS_FLWT                          (1U)
#define BSP_FEATURE_CGC_HAS_HOCOWTCR                      (1U)
#define BSP_FEATURE_CGC_HAS_MEMWAIT                       (0U)
#define BSP_FEATURE_CGC_HAS_PCLKA                         (1U)
#define BSP_FEATURE_CGC_HAS_PCLKB                         (1U)
#define BSP_FEATURE_CGC_HAS_PCLKC                         (1U)
#define BSP_FEATURE_CGC_HAS_PCLKD                         (1U)
#define BSP_FEATURE_CGC_HAS_PLL                           (1U)
#define BSP_FEATURE_CGC_HAS_PLL2                          (0U)
#define BSP_FEATURE_CGC_HAS_SRAMPRCR2                     (0U)
#define BSP_FEATURE_CGC_HAS_SRAMWTSC                      (1U)
#define BSP_FEATURE_CGC_HOCOSF_BEFORE_OPCCR               (0U)
#define BSP_FEATURE_CGC_HOCOWTCR_64MHZ_ONLY               (0U)
#define BSP_FEATURE_CGC_ICLK_DIV_RESET                    (BSP_CLOCKS_SYS_CLOCK_DIV_4)
#define BSP_FEATURE_CGC_LOCO_CONTROL_ADDRESS              (0x81280070U)
#define BSP_FEATURE_CGC_LOCO_STABILIZATION_MAX_US         (61U)
#define BSP_FEATURE_CGC_LOW_SPEED_MAX_FREQ_HZ             (1000000U) // This MCU does have Low Speed Mode, up to 1MHz
#define BSP_FEATURE_CGC_LOW_VOLTAGE_MAX_FREQ_HZ           (0U)       // This MCU does not have Low Voltage Mode
#define BSP_FEATURE_CGC_MIDDLE_SPEED_MAX_FREQ_HZ          (0U)       // This MCU does not have Middle Speed Mode
#define BSP_FEATURE_CGC_MOCO_STABILIZATION_MAX_US         (15U)
#define BSP_FEATURE_CGC_MODRV_MASK                        (0x30U)
#define BSP_FEATURE_CGC_MODRV_SHIFT                       (0x4U)
#define BSP_FEATURE_CGC_PLL1_CONTROL_ADDRESS              (0x81280050U)
#define BSP_FEATURE_CGC_PLLCCR_MAX_HZ                     (240000000U)
#define BSP_FEATURE_CGC_PLLCCR_TYPE                       (1U)
#define BSP_FEATURE_CGC_PLLCCR_WAIT_US                    (0U) // No wait between setting PLLCCR and clearing PLLSTP

#define BSP_FEATURE_CGC_SCKDIVCR_BCLK_MATCHES_PCLKB       (0U)
#define BSP_FEATURE_CGC_SODRV_MASK                        (0x02U)
#define BSP_FEATURE_CGC_SODRV_SHIFT                       (0x1U)
#define BSP_FEATURE_CGC_STARTUP_OPCCR_MODE                (0U)

#define BSP_FEATURE_CMT_VALID_CHANNEL_MASK                (0x3FU)

#define BSP_FEATURE_CMTW_VALID_CHANNEL_MASK               (0x3U)

#define BSP_FEATURE_CRC_VALID_CHANNEL_MASK                (0x3U)

#define BSP_FEATURE_DMAC_MAX_CHANNEL                      (16U)
#define BSP_FEATURE_DMAC_MAX_UNIT                         (2U)
#define BSP_FEATURE_DMAC_UNIT0_ERROR_NUM                  (5U)

#define BSP_FEATURE_DSMIF_CHANNEL_STATUS                  (1U)
#define BSP_FEATURE_DSMIF_DATA_FORMAT_SEL                 (0U)
#define BSP_FEATURE_DSMIF_ERROR_STATUS_CLR                (5U)
#define BSP_FEATURE_DSMIF_OVERCURRENT_DETECT_CONTROL      (1U)
#define BSP_FEATURE_DSMIF_OVERCURRENT_DETECT_ISR          (1U)
#define BSP_FEATURE_DSMIF_OVERCURRENT_DETECT_NOTIFY       (0U)
#define BSP_FEATURE_DSMIF_OVERCURRENT_ERROR_STATUS        (1U)
#define BSP_FEATURE_DSMIF_OVERCURRENT_NOTIFY_STATUS       (0U)
#define BSP_FEATURE_DSMIF_VERSION                         (1U)

#define BSP_FEATURE_ELC_ELC_SSEL_NUM                      (19)
#define BSP_FEATURE_ELC_EVENT_MASK_NUM                    (4U)
#define BSP_FEATURE_ELC_PERIPHERAL_0_MASK                 (0xFFFFFFFFU) // ELC event source no.0 to 31 available on this MCU
#define BSP_FEATURE_ELC_PERIPHERAL_1_MASK                 (0x007FFFFFU) // ELC event source no.32 to 63 available on this MCU.
#define BSP_FEATURE_ELC_PERIPHERAL_2_MASK                 (0x00000000U) // ELC event source no.64 to 95 available on this MCU.
#define BSP_FEATURE_ELC_PERIPHERAL_3_MASK                 (0x00000000U) // ELC event source no.96 to 127 available on this MCU.
#define BSP_FEATURE_ELC_PERIPHERAL_TYPE                   (1U)

#define BSP_FEATURE_ESC_MAX_PORTS                         (3U)
#define BSP_FEATURE_ETHER_FIFO_DEPTH                      (0x0000070FU)
#define BSP_FEATURE_ETHER_PHY_MAX_CHANNELS                (3U)
#define BSP_FEATURE_ETHSS_SWITCH_MODE_BIT_MASK            (3U)
#define BSP_FEATURE_ETHSS_MAX_PORTS                       (3U)
#define BSP_FEATURE_ETHSW_MAX_CHANNELS                    (1U)
#define BSP_FEATURE_ETHSW_SUPPORTED                       (1U)
#define BSP_FEATURE_GMAC_MAX_CHANNELS                     (1U)
#define BSP_FEATURE_GMAC_MAX_PORTS                        (3U)

#define BSP_FEATURE_GPT_32BIT_CHANNEL_MASK                (0x3FFFF)
#define BSP_FEATURE_GPT_LLPP_BASE_CHANNEL                 (0)  // LLPP channel: ch0-6
#define BSP_FEATURE_GPT_LLPP_CHANNEL_MASK                 (0x0007F)
#define BSP_FEATURE_GPT_NONSAFETY_BASE_CHANNEL            (7)  // Non-safety channel: ch7-13
#define BSP_FEATURE_GPT_NONSAFETY_CHANNEL_MASK            (0x0007F)
#define BSP_FEATURE_GPT_SAFETY_BASE_CHANNEL               (14) // safety channel: ch14-17
#define BSP_FEATURE_GPT_SAFETY_CHANNEL_MASK               (0x0000F)
#define BSP_FEATURE_GPT_VALID_CHANNEL_MASK                (0x3FFFF)
#define BSP_FEATURE_GPTE_CHANNEL_MASK                     (0xF0)
#define BSP_FEATURE_GPTEH_CHANNEL_MASK                    (0xF)

#define BSP_FEATURE_ICU_ERROR_CR52_CPU1_SUPPORTED         (1U)
#define BSP_FEATURE_ICU_ERROR_PERI_ERR_REG_NUM            (2U)
#define BSP_FEATURE_ICU_ERROR_PERI_ERR0_REG_MASK          (0xFFFFFFFFU)
#define BSP_FEATURE_ICU_ERROR_PERI_ERR1_REG_MASK          (0x19FFFFFFU)
#define BSP_FEATURE_ICU_ERROR_PERI_ERR2_REG_MASK          (0x00000000U)
#define BSP_FEATURE_ICU_ERROR_PERI_ERR3_REG_MASK          (0x00000000U)
#define BSP_FEATURE_ICU_ERROR_PERIPHERAL_TYPE             (1U)
#define BSP_FEATURE_ICU_HAS_WUPEN1                        (0U)
#define BSP_FEATURE_ICU_INTER_CPU_IRQ_CHANNEL             (1U)
#define BSP_FEATURE_ICU_INTER_CPU_IRQ_CHANNELS_MASK       (0xFFU)
#define BSP_FEATURE_ICU_INTER_CPU_IRQ_NS_SWINT_MASK       (0x0000003FU) // Non-safety channel: ch0-5 (bit0-5)
#define BSP_FEATURE_ICU_INTER_CPU_IRQ_S_SWINT_MASK        (0x000000C0U) // Safety channel: ch6-7 (bit6-7)
#define BSP_FEATURE_ICU_INTER_CPU_IRQ_S_SWINT_SHIFT       (6U)
#define BSP_FEATURE_ICU_IRQ_CHANNELS_MASK                 (0xFFFFU)
#define BSP_FEATURE_ICU_WUPEN_MASK                        (0xFF4FFFFFU)

#define BSP_FEATURE_IIC_FAST_MODE_PLUS                    (0U)
#define BSP_FEATURE_IIC_SAFETY_CHANNEL                    (2U)
#define BSP_FEATURE_IIC_SAFETY_CHANNEL_BASE_ADDRESS       (R_IIC2_BASE)
#define BSP_FEATURE_IIC_VALID_CHANNEL_MASK                (0x07)

#define BSP_FEATURE_IOPORT_ELC_PORTS                      (4U)
#define BSP_FEATURE_IOPORT_HAS_ETHERNET                   (1U)
#define BSP_FEATURE_IOPORT_PIN_PFC_TYPE                   (1U)

#define BSP_FEATURE_LPM_CHANGE_MSTP_ARRAY                 {{0, 15}, {0, 13}, {1, 31}, {1, 6}, {1, 5}, {1, 4}, {2, 5}}
#define BSP_FEATURE_LPM_CHANGE_MSTP_REQUIRED              (1U)
#define BSP_FEATURE_LPM_DPSIEGR_MASK                      (0x00137FFFU)
#define BSP_FEATURE_LPM_DPSIER_MASK                       (0x071F7FFFU)
#define BSP_FEATURE_LPM_HAS_DEEP_STANDBY                  (1U)
#define BSP_FEATURE_LPM_HAS_SBYCR_OPE                     (1U)
#define BSP_FEATURE_LPM_HAS_SNZEDCR1                      (0U)
#define BSP_FEATURE_LPM_HAS_SNZREQCR1                     (0U)
#define BSP_FEATURE_LPM_HAS_STCONR                        (1U)
#define BSP_FEATURE_LPM_SBYCR_WRITE1_B14                  (0U)
#define BSP_FEATURE_LPM_SNZEDCR_MASK                      (0x000000FFU)
#define BSP_FEATURE_LPM_SNZREQCR_MASK                     (0x7342FFFFU)

#define BSP_FEATURE_MAILBOX_SEM_SUPPORTED                 (0U)

#define BSP_FEATURE_MTU3_MAX_CHANNELS                     (9U)
#define BSP_FEATURE_MTU3_UVW_MAX_CHANNELS                 (3U)
#define BSP_FEATURE_MTU3_VALID_CHANNEL_MASK               (0x01FF)

#define BSP_FEATURE_OSPI_DEVICE_0_START_ADDRESS           (0x0U)
#define BSP_FEATURE_OSPI_DEVICE_1_START_ADDRESS           (0x0U)

#define BSP_FEATURE_POE3_ERROR_SIGNAL_TYPE                (1U)
#define BSP_FEATURE_POE3_PIN_SELECT_TYPE                  (1U)

#define BSP_FEATURE_POEG_CHANNEL_MASK                     (0xFU)
#define BSP_FEATURE_POEG_ERROR_SIGNAL_TYPE                (1U)
#define BSP_FEATURE_POEG_GROUP_OFSSET_ADDRESS             (0x400)
#define BSP_FEATURE_POEG_LLPP_UNIT                        (0U)
#define BSP_FEATURE_POEG_MAX_UNIT                         (2U)
#define BSP_FEATURE_POEG_NONSAFETY_UNIT                   (1U)
#define BSP_FEATURE_POEG_SAFETY_UNIT                      (2U)

#define BSP_FEATURE_SEM_SUPPORTED                         (1U)

#define BSP_FEATURE_SCI_ADDRESS_MATCH_CHANNELS            (BSP_FEATURE_SCI_CHANNELS)
#define BSP_FEATURE_SCI_CHANNELS                          (0x3FU)
#define BSP_FEATURE_SCI_UART_CSTPEN_CHANNELS              (0x03FU)
#define BSP_FEATURE_SCI_UART_FIFO_CHANNELS                (0x3FFU)
#define BSP_FEATURE_SCI_UART_FIFO_DEPTH                   (16U)
#define BSP_FEATURE_SCI_SAFETY_CHANNEL                    (5U)
#define BSP_FEATURE_SCI_SAFETY_CHANNEL_BASE_ADDRESS       (R_SCI5_BASE)

#define BSP_FEATURE_SPI_HAS_BYTE_SWAP                     (1U)
#define BSP_FEATURE_SPI_HAS_SPCR3                         (0U)
#define BSP_FEATURE_SPI_HAS_SSL_LEVEL_KEEP                (1U)
#define BSP_FEATURE_SPI_MAX_CHANNEL                       (4U)
#define BSP_FEATURE_SPI_SAFETY_CHANNEL                    (3U)
#define BSP_FEATURE_SPI_SAFETY_CHANNEL_BASE_ADDRESS       (R_SPI3_BASE)

#define BSP_FEATURE_TFU_SUPPORTED                         (1U)

#define BSP_FEATURE_XSPI_CHANNELS                         (0x03U)
#define BSP_FEATURE_XSPI_CS_ADDRESS_SPACE_SETTING_TYPE    (1U)
#define BSP_FEATURE_XSPI_NUM_CHIP_SELECT                  (2U)
#define BSP_FEATURE_XSPI_OTFD_SUPPORTED                   (0U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
