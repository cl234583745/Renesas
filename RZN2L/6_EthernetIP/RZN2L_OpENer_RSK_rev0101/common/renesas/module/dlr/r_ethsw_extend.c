/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

#include <string.h>

#include "bsp_api.h"
#include "r_ethsw.h"
#include <math.h>

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

#define ETHSW_EXTENDED_API_FOR_FUTURE

#ifndef ETHSW_ERROR_RETURN

/*LDRA_INSPECTED 77 S This macro does not work when surrounded by parentheses. */
 #define ETHSW_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif                                 /* !ETHSW_ERROR_RETURN */

/** "SWIT" in ASCII.  Used to determine if the control block is open. */
#define ETHSW_OPEN                                       (0x53574954U)

/* Initial value of down counter for timeout detection */
#define ETHSW_TIMEOUT_COUNT                              (1000000000U)

/* Maximum Frame Length */
#define ETHSW_FRAME_SIZE                                 (0x2408U)

/* ENABLE PORT Bit Definitions(PORT_ENA,UCAST_DEFAULT_MASK,BCAST_DEFAULT_MASK,MCAST_DEFAULT_MASK Register) */
#define ETHSW_ENABLE_PORT0                               (1UL << 0)
#define ETHSW_ENABLE_PORT1                               (1UL << 1)
#define ETHSW_ENABLE_PORT2                               (1UL << 2)
#define ETHSW_ENABLE_PORT3                               (1UL << 3)

/* COMMAND_CONFIG Register Bit Definitions */
#define ETHSW_COMMAND_CONFIG_ENA_10                      (1UL << 25)
#define ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK               (1UL << 24)
#define ETHSW_COMMAND_CONFIG_CNTL_FRM_ENA                (1UL << 23)
#define ETHSW_COMMAND_CONFIG_LOOP_ENA                    (1UL << 15)
#define ETHSW_COMMAND_CONFIG_SW_RESET                    (1UL << 13)
#define ETHSW_COMMAND_CONFIG_TX_CRC_APPEND               (1UL << 11)
#define ETHSW_COMMAND_CONFIG_HD_ENA                      (1UL << 10)
#define ETHSW_COMMAND_CONFIG_TX_ADDR_INS                 (1UL << 9)
#define ETHSW_COMMAND_CONFIG_PAUSE_IGNORE                (1UL << 8)
#define ETHSW_COMMAND_CONFIG_PAUSE_FWD                   (1UL << 7)
#define ETHSW_COMMAND_CONFIG_CRC_FWD                     (1UL << 6)
#define ETHSW_COMMAND_CONFIG_PAD_EN                      (1UL << 5)
#define ETHSW_COMMAND_CONFIG_PROMIS_EN                   (1UL << 4)
#define ETHSW_COMMAND_CONFIG_ETH_SPEED                   (1UL << 3)
#define ETHSW_COMMAND_CONFIG_RX_ENA                      (1UL << 1)
#define ETHSW_COMMAND_CONFIG_TX_ENA                      (1UL << 0)

/* INT_CONFIG Register Bit Number Definitions */
#define ETHSW_INT_CONFIG_PATTERN_INT                     (1UL << 31)
#define ETHSW_INT_CONFIG_TDMA_INT                        (1UL << 30)
#define ETHSW_INT_CONFIG_TSM_INT                         (1UL << 29)
#define ETHSW_INT_CONFIG_IRQ_LNK_SHIFT                   (8U)
#define ETHSW_INT_CONFIG_IRQ_LNK_MASK                    (0x7 << ETHSW_INT_CONFIG_IRQ_LNK_SHIFT)
#define ETHSW_INT_CONFIG_DLR_INT                         (1UL << 5)
#define ETHSW_INT_CONFIG_IRQ_EN                          (1UL << 0)

#define ETHSW_HEX_000000FF                               (0x000000ffU)
#define ETHSW_HEX_0000FF00                               (0x0000ff00U)
#define ETHSW_HEX_00FF0000                               (0x00ff0000U)
#define ETHSW_HEX_FF000000                               (0xff000000U)
#define ETHSW_HEX_0000FFFF                               (0x0000ffffU)
#define ETHSW_DEC_100                                    (100U)
#define ETHSW_DEC_512                                    (512U)
#define ETHSW_DEC_1000                                   (1000U)

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

 #define ETHSW_1BIT_MASK                                 (0x1U)
 #define ETHSW_2BITS_MASK                                (0x3U)
 #define ETHSW_3BITS_MASK                                (0x7U)
 #define ETHSW_4BITS_MASK                                (0xfU)
 #define ETHSW_9BITS_MASK                                (0x1ffU)
 #define ETHSW_12BITS_MASK                               (0xfffU)

/* MMCTL_QTRIG Register Bit Definitions */
 #define ETHSW_MMCTL_QTRIG_QUEUE_TRIG_SHIFT              (16U)
 #define ETHSW_MMCTL_QTRIG_QUEUE_TRIG_MASK               (0xffffU << ETHSW_MMCTL_QTRIG_QUEUE_TRIG_SHIFT)
 #define ETHSW_MMCTL_QTRIG_PORT_MASK_SHIFT               (0U)
 #define ETHSW_MMCTL_QTRIG_PORT_MASK_MASK                (0xfU << ETHSW_MMCTL_QTRIG_PORT_MASK_SHIFT)

/* TCV_SEQ_CTRL Register Bit Definitions */
 #define ETHSW_TDMA_TCV_SEQ_START                        (1UL << 0)
 #define ETHSW_TDMA_TCV_SEQ_INT                          (1UL << 1)
 #define ETHSW_TDMA_TCV_SEQ_D_IDX_SHIFT                  2
 #define ETHSW_TDMA_TCV_SEQ_D_IDX_MASK                   (0x1ff << ETHSW_TDMA_TCV_SEQ_D_IDX_SHIFT)
 #define ETHSW_TDMA_TCV_SEQ_GPIO_SHIFT                   22
 #define ETHSW_TDMA_TCV_SEQ_GPIO_MASK                    (0xf << ETHSW_TDMA_TCV_SEQ_GPIO_SHIFT)
 #define ETHSW_TDMA_TCV_SEQ_READ_MODE                    (1UL << 31)

/* TCV_SEQ_ADDR Register Bit Definitions */
 #define ETHSW_TDMA_TCV_SEQ_S_ADDR_SHIFT                 0
 #define ETHSW_TDMA_TCV_SEQ_S_ADDR_MASK                  (0xfff << ETHSW_TDMA_TCV_SEQ_S_ADDR_SHIFT)

/* TS_FIFO_READ_CTRL Register Bit Definitions */
 #define ETHSW_TS_FIFO_READ_CTRL_PORT_NUM_SHIFT          0
 #define ETHSW_TS_FIFO_READ_CTRL_PORT_NUM_MASK           (0xf << ETHSW_TS_FIFO_READ_CTRL_PORT_NUM_SHIFT)
 #define ETHSW_TS_FIFO_READ_CTRL_TS_VALID                1 << 4
 #define ETHSW_TS_FIFO_READ_CTRL_TS_SEL_SHIFT            6
 #define ETHSW_TS_FIFO_READ_CTRL_TS_SEL_MASK             (0x3 << ETHSW_TS_FIFO_READ_CTRL_TS_SEL_SHIFT)
 #define ETHSW_TS_FIFO_READ_CTRL_TS_ID_SHIFT             8
 #define ETHSW_TS_FIFO_READ_CTRL_TS_ID_MASK              (0x7f << ETHSW_TS_FIFO_READ_CTRL_TS_ID_SHIFT)

/* TCV_DATA_ADDR Register Bit Definitions */
 #define ETHSW_TCV_DATA_ADDR_MASK                        (0x1FFU)

/* TCV_D_CTRL Register Bit Definitions */
 #define ETHSW_TCV_D_CTRL_QGATE_MASK                     (0xFFU)
 #define ETHSW_TCV_D_CTRL_PMASK_MASK                     (0x7U)

/* TDMA_CONFIG Register Bit Definitions */
 #define ETHSW_TDMA_CONFIG_TIMER_SEL_MASK                (0x1U)

/* TDMA_ENA_CTRL Register Bit Definitions */
 #define ETHSW_TDMA_ENA_CTRL_PORT_ENA_MASK               (0x7U)

/* TDMA_IRQ_CONTRO and TDMA_IRQ_STAT_ACK Register Bit Definitions */
 #define ETHSW_TDMA_IRQ_CONTROL_TCV_INT_EN               (1UL << 0)
 #define ETHSW_TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_SHIFT    1
 #define ETHSW_TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_MASK     (0xfff << ETHSW_TDMA_IRQ_CONTROL_TCV_IDX_INT_ENA_SHIFT)
 #define ETHSW_TDMA_IRQ_CONTROL_CTR1_INT_EN              (1UL << 13)

/* CONFIG Register Bit Definitions */
 #define ETHSW_CONFIG_IRQ_TX_EN_SHIFT                    16
 #define ETHSW_CONFIG_IRQ_TX_EN_MASK                     (7UL << ETHSW_CONFIG_IRQ_TX_EN_SHIFT)
 #define ETHSW_CONFIG_IRQ_TSFIFO_OVR                     (1UL << 2)
 #define ETHSW_CONFIG_IRQ_EN                             (1UL << 0)

/* ATIME_CTRL Register Bit Definitions */
 #define ETHSW_ATIME_CTRL_ENABLE                         (1UL << 0)
 #define ETHSW_ATIME_CTRL_CAPTURE                        (1UL << 11)

/* ATIME_INC Register Bit Definitions */
 #define ETHSW_ATIME_INC_CLOCK_PERIOD_SHIFT              (0U)
 #define ETHSW_ATIME_INC_CLOCK_PERIOD_MASK               (0x7FU << ETHSW_ATIME_INC_CLOCK_PERIOD_SHIFT)

/* PTP_DOMAIN Register Bit Definitions */
 #define ETHSW_PTP_DOMAIN_SHIFT                          (8U)
 #define ETHSW_PTP_DOMAIN_MASK                           (0xFFU)

/* FIFO_READ_CTRL Register Bit Definitions */
 #define ETHSW_FIFO_READ_CTRL_TS_VALID                   (1UL << 4)
 #define ETHSW_FIFO_READ_CTRL_TS_SEL_SHIFT               6
 #define ETHSW_FIFO_READ_CTRL_TS_SEL_MASK                (0x3 << ETHSW_FIFO_READ_CTRL_TS_SEL_SHIFT)
 #define ETHSW_FIFO_READ_CTRL_TS_ID_SHIFT                8
 #define ETHSW_FIFO_READ_CTRL_TS_ID_MASK                 (0xf << ETHSW_FIFO_READ_CTRL_TS_ID_SHIFT)

/* GPARSER Register Bit Definitions */
 #define ETHSW_GPARSER_PARSER_MAX                        (7)
 #define ETHSW_GPARSER_ARITH_MAX                         (7)
 #define ETHSW_GPARSER_SNOOP_STAGE1                      (3)
 #define ETHSW_GPARSER_SNOOP_STAGE2                      (7)
 #define ETHSW_GPARSER_PARSER_VALID                      (1 << 24)
 #define ETHSW_GPARSER_CMPMASK_OFFSET                    (30)
 #define ETHSW_GPARSER_CMP_OFFSET                        (8)
 #define ETHSW_GPARSER_MODE_OFFSET_DA                    (1 << 23)
 #define ETHSW_GPARSER_MODE_SKIPVLAN                     (1 << 25)
 #define ETHSW_GPARSER_MODE_IPDATA                       (1 << 26)
 #define ETHSW_GPARSER_MODE_IPPROTO                      (1 << 27)
 #define ETHSW_GPARSER_MODE_COMPARE16                    (1 << 28)
 #define ETHSW_GPARSER_MODE_OFFSETPLUS2                  (1 << 29)
 #define ETHSW_GPARSER_MODE_CMP_OR                       (1 << 30)

 #define ETHSW_GPARSER_MODE_NONIPPROT                                \
    (ETHSW_GPARSER_MODE_SKIPVLAN | ETHSW_GPARSER_MODE_CMP_OR |       \
     ETHSW_GPARSER_MODE_COMPARE16 | ETHSW_GPARSER_MODE_OFFSETPLUS2 | \
     ETHSW_GPARSER_MODE_IPDATA)

 #define ETHSW_GPARSER_MODE_VALID \
    (ETHSW_GPARSER_MODE_NONIPPROT | ETHSW_GPARSER_MODE_IPPROTO)

 #define ETHSW_GPARSER_MODE_INVALID \
    ~((uint32_t) ETHSW_GPARSER_MODE_VALID)

 #define ETHSW_GPARSER_OFFSET_MASK            (0x3F)
 #define ETHSW_GPARSER_OFFSET_OFFSET          (16)
 #define ETHSW_GPARSER_ARITH_BLOCK1           (3)
 #define ETHSW_GPARSER_ARITH_BLOCK2           (7)
 #define ETHSW_GPARSER_GPARSER_BLOCK1         (3)
 #define ETHSW_GPARSER_GPARSER_BLOCK2         (7)
 #define ETHSW_GPARSER_INVERTMAP              (0xF)
 #define ETHSW_GPARSER_SELMATCH               (0xF)
 #define ETHSW_GPARSER_SELMATCH_OFFSET        (8)
 #define ETHSW_GPARSER_OPERATION_OFFSET       (16)
 #define ETHSW_GPARSER_RESINVERT_OFFSET       (17)
 #define ETHSW_GPARSER_SNOOPMODE_OFFSET       (20)
 #define ETHSW_GPARSER_SNOOPMODE_OFF          (0)
 #define ETHSW_GPARSER_SNOOPMODE_FWD_MGT      (1)
 #define ETHSW_GPARSER_SNOOPMODE_FWD_CPY      (2)
 #define ETHSW_GPARSER_SNOOPMODE_DISCARD      (3)
 #define ETHSW_GPARSER_ARITHSEL_OFFSET        (12)
 #define ETHSW_GPARSER_RESINVERT              (1 << 17)
 #define ETHSW_GPARSER_BLOCK_CNT              (2)
 #define ETHSW_GPARSER_PARSER_PER_BLOCK       (4)
 #define ETHSW_GPARSER_PARSER_BLOCK_ID1       (1)
 #define ETHSW_GPARSER_PARSER_BLOCK_ID2       (2)
 #define ETHSW_GPARSER_PARSER_OPER_OR         (1 << 16)
 #define ETHSW_GPARSER_ARTIH_ALL              (0x7)

/* EEE_CTL_STAT_P[n] Register Bit Definitions */
 #define ETHSW_EEE_AUTO                       (1)

/* AUTH_PORT[n] Register Bit Definitions */
 #define ETHWE_SWITCH_PORT_AUTHORIZED         (1 << 0)
 #define ETHWE_SWITCH_PORT_CONTROLLED_BOTH    (1 << 1)
 #define ETHWE_SWITCH_PORT_EAPOL_ENABLE       (1 << 2)

/* MGMT_CONFIG Register Bit Definitions */
 #define ETHSW_MGMT_BPDU_FWD_ENABLE           (1 << 6)
 #define ETHSW_MGMT_BPDU_DISCARD              (1 << 7)
 #define ETHSW_MGMT_BPDU_MGMT_ENABLE          (1 << 8)
 #define ETHSW_MGMT_BPDU_MGMT_DISCARD         (1 << 9)

/* DLR_IRQ_CONTROL Register Bit Definitions */
 #define ETHSW_TGT_DLR_INT_NEWSTATUS          (1 << 0)  ///< new ring status
 #define ETHSW_TGT_DLR_INT_BEACONTMO_0        (1 << 4)  ///< beacon timeout port 0
 #define ETHSW_TGT_DLR_INT_BEACONTMO_1        (1 << 5)  ///< beacon timeout port 1
 #define ETHSW_TGT_DLR_INT_SUVICHNG           (1 << 6)  ///< Supervisor changed
 #define ETHSW_TGT_DLR_INT_LNKCHNG_0          (1 << 7)  ///< link status changed port 0
 #define ETHSW_TGT_DLR_INT_LNKCHNG_1          (1 << 8)  ///< link status changed port 1
 #define ETHSW_TGT_DLR_INT_BECONRCV_0         (1 << 12) ///< beacon frame received port 0
 #define ETHSW_TGT_DLR_INT_BECONRCV_1         (1 << 13) ///< beacon frame received port 1
 #define ETHSW_TGT_DLR_INT_LOCALLOOP_0        (1 << 14) ///< loop detected port 0
 #define ETHSW_TGT_DLR_INT_LOCALLOOP_1        (1 << 15) ///< loop detected port 1

/* FRM_LENGTH_P[n] Register Bit Definitions */
 #define ETHSW_FRAMESIZE_MASK                 0x3FFF    ///< maximum frame size mask

/* MGMT_TAG_CONFIG Register Tagfield set value */
 #define ETHSW_TAGFIELD_DEFAULT               0xE001

/* STATUS_P[n] Register PHYSPEED value Definitions */
 #define ETHSW_PHYSPEED_10MBPS                0
 #define ETHSW_PHYSPEED_100MBPS               1
 #define ETHSW_PHYSPEED_1GBPS                 2

/* Register offset for Port-n */
 #define ETHSW_PORT_OFFSET                    (0x400)
 #define ETHSW_PORT_OFFSET_4BYTES             (0x04)
 #define ETHSW_PORT_OFFSET_8BYTES             (0x08)
 #define ETHSW_PORT_OFFSET_16BYTES            (0x10)
 #define ETHSW_PORT_OFFSET_32BYTES            (0x20)

 #define ETHSW_PORT_TX_MSK                    (1) ///< COMMAND_CONFIG_P[n] - mask for tx_ena
 #define ETHSW_PORT_RX_MSK                    (2) ///< COMMAND_CONFIG_P[n] - mask for rx_ena
 #define ETHSW_PORT_RXTX                      (ETHSW_PORT_TX_MSK | ETHSW_PORT_RX_MSK)

 #define ETHSW_VLANID_MASK                    (0xFFF0)
 #define ETHSW_VLANID_OFFSET                  (4)
 #define ETHSW_VLAN_PORT_MASK                 (0xF)
 #define ETHSW_VLAN_INVALID_ENTRY             (0x0FFFF)
 #define ETHSW_VLAN_INOUT_MODE_MASK           (0x3)
 #define ETHSW_VLAN_DEFID_MASK                (0xFFFF)
 #define ETHSW_VLAN_MODE_BITS                 (0x2)

 #define ETHSW_LK_MASK                        (0x9E)
 #define ETHSW_LK_PERVLAN                     (7)
 #define ETHSW_LK_DISCUNKN                    (4)
 #define ETHSW_LK_MIGRAT                      (3)
 #define ETHSW_LK_AGING                       (2)
 #define ETHSW_LK_LEARN                       (1)
 #define ETHSW_LK_ADDR_CLEAR                  (1 << 29)
 #define ETHSW_LK_ADDR_LOOKUP                 (1 << 28)
 #define ETHSW_LK_ADDR_VALID                  (1)
 #define ETHSW_LK_ADDR_WRITE                  (1 << 25)
 #define ETHSW_LK_ADDR_REG_VALID              (1 << 16)
 #define ETHSW_LK_ADDR_REG_TYPE               (1 << 17)
 #define ETHSW_LK_ADDR_PORTMASK               (0xF << 21)
 #define ETHSW_LK_ADDR_PRIOMASK               (0x7 << 18)
 #define ETHSW_LK_ADDR_PORT_OFFSET            (5)
 #define ETHSW_LK_ADDR_PORT_REG_OFFSET        (21)
 #define ETHSW_LK_ADDR_PRIO_REG_OFFSET        (18)
 #define ETHSW_LK_ADDR_MASKANDFLAGS_MASK      (0xFFFFU << 16)

 #define ETHSW_COUNTER_OFFSET                 (0x400)

 #define ETHSW_PHY_LINK                       (4)

 #define ETHSW_QOS_VLANPRIO_MASK              (0xFFFFFF)
 #define ETHSW_QOS_OUTQUEUES                  (8)

 #define ETHSW_QOS_MODE_MASK                  (0xF)
 #define ETHSW_QOS_DEFPRIO_OFFSET             (4)
 #define ETHSW_QOS_DEFPRIO_MASK               (7)
 #define ETHSW_QOS_IPPRIO_READ                (1U << 31)
 #define ETHSW_QOS_IPPRIO_WRITE               (0U << 31)
 #define ETHSW_QOS_IPPRIO_OFFSET              (9)
 #define ETHSW_QOS_IPPRIO_DSMASK              (0xFF)
 #define ETHSW_QOS_IPPRIO_MASK                (7)

 #define ETHSW_HZ                             (200000000)
 #define ETHSW_DOS_TIMEFACTOR                 (16385) ///< The time factor is calculated in 16385 switch system clock cycles

 #define ETHSW_RATE_IS_MASK                   (0x7FF)
 #define ETHSW_RATE_IS_MAX                    (80)

/* driver local define */
 #define ETHSW_PORT_COUNT_ALL                 (4U)      ///< Number of All port (include managemant port)
 #define ETHSW_PORT_COUNT                     (3U)      ///< Number of External port
 #define ETHSW_TIME_COUNT                     (2U)      ///< Number of timer
 #define ETHSW_QUEUE_COUNT                    (8U)      ///< Number of Queue
 #define ETHSW_PULSE_GENERATOR_COUNT          (4U)      ///< Number of Pulse Gennrator
 #define ETHSW_RX_PATTERN_MATCHER_COUNT       (12U)     ///< Number of RX Pattern Matcher

 #define ETHSW_RN_ADVSW_VLAN_TABLE_ENTRIES    (32U)     ///< Number of VLAN table entries

 #define ETHSW_RN_ADVSW_DLR_ETHTYPE           (0x80E1U) ///< DLR EtherType for DLR module
 #define ETHSW_RN_ADVSW_DLR_TICKS             (200U)    ///< ticks for 1 us

 #define ETHSW_TCV_S_ADDR_MASK                (0xfffU)

/* Multicast MAC Address used by DLR */
 #define ETHSW_MULTICAST_BEACON               {0x01, 0x21, 0x6c, 0x00, 0x00, 0x01}
 #define ETHSW_MULTICAST_DLR                  {0x01, 0x21, 0x6c, 0x00, 0x00, 0x00}

#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

/** MAC table entry(for register access) */
typedef struct st_ethsw_mactab_entry_reg
{
    ethsw_mac_addr_t mac;              ///< MAC address
    uint16_t         mask_and_flags;   ///< Port bit mask and flags
} ethsw_mactab_entry_reg_t;

typedef union
{
    ethsw_mac_addr_t    mac_addr;       /* MAC address                                  */
    uint32_t            reg_data[2];    /* datas for LK_DATA_LO and LK_DATA_HI register */
} ethsw_mac_addr_reg_t;

#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/
#ifdef ETHSW_EXTENDED_API_FOR_FUTURE
extern ethsw_portentry_t switch_config[];
#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void ethsw_enable_icu(ethsw_instance_ctrl_t * const p_instance_ctrl);

static void ethsw_disable_icu(ethsw_instance_ctrl_t * const p_instance_ctrl);

void ethsw_isr_intr(void);

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

static void ethsw_isr_tsm(ethsw_instance_ctrl_t * p_instance_ctrl);

static void ethsw_isr_tdma(ethsw_instance_ctrl_t * p_instance_ctrl);

static void ethsw_dlr_callback(ethsw_dlr_event_t event, uint32_t port);

static void ethsw_isr_dlr(ethsw_instance_ctrl_t * p_instance_ctrl);

static void ethsw_isr_rx_pattern_matcher(ethsw_instance_ctrl_t * p_instance_ctrl);

static fsp_err_t ethsw_snoop_init(R_ETHSW_Type volatile * p_switch_reg,
                                  ethsw_snoop_action_t    mode1,
                                  ethsw_snoop_action_t    mode2);

static fsp_err_t ethsw_gparser_set(R_ETHSW_Type volatile * p_switch_reg,
                                   uint8_t                 parserid,
                                   bool                    enable,
                                   uint32_t                mode,
                                   uint16_t                compval,
                                   uint8_t                 mask,
                                   uint16_t                offset);

static fsp_err_t ethsw_snoop_free_gparser_get(R_ETHSW_Type volatile * p_switch_reg, uint32_t block, uint32_t * p_id);

static void ethsw_memcpy_volatile(volatile void * p_dst, volatile const void * p_src, uint32_t cnt);

static void r_ethsw_reg_protection_disable(void * const p_reg_base);
static void r_ethsw_reg_protection_enable(void * const p_reg_base);

#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

#if defined(__GNUC__)

/* This structure is affected by warnings from a GCC compiler bug. This pragma suppresses the warnings in this
 * structure only.*/

/*LDRA_INSPECTED 69 S */
 #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/** ETHER SWITCH HAL module version data structure */
const fsp_version_t module_version =
{
    .api_version_minor  = ETHSW_API_VERSION_MINOR,
    .api_version_major  = ETHSW_API_VERSION_MAJOR,
    .code_version_major = ETHSW_CODE_VERSION_MAJOR,
    .code_version_minor = ETHSW_CODE_VERSION_MINOR
};

/** ETHER SWITCH HAL API mapping for Ethernet Controller interface */
/*LDRA_INSPECTED 27 D This structure must be accessible in user code. It cannot be static. */
const ethsw_api_t g_ethsw_on_ethsw =
{
    .open       = R_ETHSW_Open,
    .close      = R_ETHSW_Close,
    .speedCfg   = R_ETHSW_SpeedCfg,
    .versionGet = R_ETHSW_VersionGet,

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

    /* Forwarding */
    .portCount         = R_ETHSW_PortCount,
    .macTableSet       = R_ETHSW_MacTableSet,
    .macTableGet       = R_ETHSW_MacTableGet,
    .macTableConfSet   = R_ETHSW_MacTableConfSet,
    .macTableConfGet   = R_ETHSW_MacTableConfGet,
    .macTableClear     = R_ETHSW_MacTableClear,
    .learningSet       = R_ETHSW_LearningSet,
    .learningGet       = R_ETHSW_LearningGet,
    .portForwardAdd    = R_ETHSW_PortForwardAdd,
    .portForwardDel    = R_ETHSW_PortForwardDel,
    .discardUnknownSet = R_ETHSW_DiscardUnknownSet,
    .discardUnknownGet = R_ETHSW_DiscardUnknownGet,
    .floodUnknownSet   = R_ETHSW_FloodUnknownSet,
    .floodUnknownGet   = R_ETHSW_FloodUnknownGet,

    /* MAC */
    .linkStateGet      = R_ETHSW_LinkStateGet,
    .portAdminStateSet = R_ETHSW_PortAdminStateSet,
    .portAdminStateGet = R_ETHSW_PortAdminStateGet,
    .frameSizeMaxSet   = R_ETHSW_FrameSizeMaxSet,
    .frameSizeMaxGet   = R_ETHSW_FrameSizeMaxGet,

    /* VLAN management */
    .vlanDefaultSet        = R_ETHSW_VlanDefaultSet,
    .vlanDefaultGet        = R_ETHSW_VlanDefaultGet,
    .vlanPortAdd           = R_ETHSW_VlanPortAdd,
    .vlanPortRemove        = R_ETHSW_VlanPortRemove,
    .vlanInModeSet         = R_ETHSW_VlanInModeSet,
    .vlanInModeGet         = R_ETHSW_VlanInModeGet,
    .vlanOutModeSet        = R_ETHSW_VlanOutModeSet,
    .vlanOutModeGet        = R_ETHSW_VlanOutModeGet,
    .vlanTableCount        = R_ETHSW_VlanTableCount,
    .vlanTableGet          = R_ETHSW_VlanTableGet,
    .vlanVerifySet         = R_ETHSW_VlanVerifySet,
    .vlanVerifyGet         = R_ETHSW_VlanVerifyGet,
    .vlanDiscardUnknownSet = R_ETHSW_VlanDiscardUnknownSet,
    .vlanDiscardUnknownGet = R_ETHSW_VlanDiscardUnknownGet,

    /* Timestamp */
    .timeStamp             = R_ETHSW_TimeStamp,
    .tsLatencyEgressGet    = R_ETHSW_TsLatencyEgressGet,
    .tsLatencyIngressGet   = R_ETHSW_TsLatencyIngressGet,
    .tsActive              = R_ETHSW_TsActive,
    .tsSet                 = R_ETHSW_TsSet,
    .tsGet                 = R_ETHSW_TsGet,
    .tsDomainSet           = R_ETHSW_TsDomainSet,
    .tsDomainGet           = R_ETHSW_TsDomainGet,
    .tsPeerDelaySet        = R_ETHSW_TsPeerDelaySet,
    .tsPeerDelayGet        = R_ETHSW_TsPeerDelayGet,
    .tsCallbackSet         = R_ETHSW_TsCallbackSet,
    .tsOffsetCorrectionSet = R_ETHSW_TsOffsetCorrectionSet,
    .tsRateCorrectionSet   = R_ETHSW_TsRateCorrectionSet,
    .tsRateCorrectionGet   = R_ETHSW_TsRateCorrectionGet,

    /* TDMA */
    .tdmaEnableSet      = R_ETHSW_TdmaEnableSet,
    .tdmaEnableGet      = R_ETHSW_TdmaEnableGet,
    .tdmaTcvSeqEntrySet = R_ETHSW_TdmaTcvSeqEntrySet,
    .tdmaTcvSeqEntryGet = R_ETHSW_TdmaTcvSeqEntryGet,
    .tdmaTcvDatEntrySet = R_ETHSW_TdmaTcvDatEntrySet,
    .tdmaTcvDatEntryGet = R_ETHSW_TdmaTcvDatEntryGet,
    .tdmaTcvSeqRangeSet = R_ETHSW_TdmaTcvSeqRangeSet,
    .tdmaTcvSeqRangeGet = R_ETHSW_TdmaTcvSeqRangeGet,
    .tdmaGpioModeSet    = R_ETHSW_TdmaGpioModeSet,
    .tdmaGpioModeGet    = R_ETHSW_TdmaGpioModeGet,
    .tdmaCounter0Set    = R_ETHSW_TdmaCounter0Set,
    .tdmaCounter0Get    = R_ETHSW_TdmaCounter0Get,
    .tdmaCounter1Set    = R_ETHSW_TdmaCounter1Set,
    .tdmaCounter1Get    = R_ETHSW_TdmaCounter1Get,

    /* MMCTL */
    .mmctlQgateSet        = R_ETHSW_MmctlQgateSet,
    .mmctlPoolSizeSet     = R_ETHSW_MmctlPoolSizeSet,
    .mmctlPoolSizeGet     = R_ETHSW_MmctlPoolSizeGet,
    .mmctlQueueAssignSet  = R_ETHSW_MmctlQueueAssignSet,
    .mmctlQueueAssignGet  = R_ETHSW_MmctlQueueAssignGet,
    .mmctlYellowLengthSet = R_ETHSW_MmctlYellowLengthSet,
    .mmctlYellowLengthGet = R_ETHSW_MmctlYellowLengthGet,
    .queueFlushEventSet   = R_ETHSW_QueueFlushEventSet,

    /* Mirroring */
    .mirrorSet = R_ETHSW_MirrorSet,
    .mirrorGet = R_ETHSW_MirrorGet,

    /* QoS */
    .qosModeSet       = R_ETHSW_QosModeSet,
    .qosModeGet       = R_ETHSW_QosModeGet,
    .qosPrioValnSet   = R_ETHSW_QosPrioValnSet,
    .qosPrioValnGet   = R_ETHSW_QosPrioValnGet,
    .qosPrioIpSet     = R_ETHSW_QosPrioIpSet,
    .qosPrioIpGet     = R_ETHSW_QosPrioIpGet,
    .qosPrioTypeSet   = R_ETHSW_QosPrioTypeSet,
    .qosPrioTypeGet   = R_ETHSW_QosPrioTypeGet,
    .queuesPerPortGet = R_ETHSW_QueuesPerPortGet,

    /* Storm protection */
    .stormTimeSet  = R_ETHSW_StormTimeSet,
    .stormTimeGet  = R_ETHSW_StormTimeGet,
    .bcastLimitSet = R_ETHSW_BcastLimitSet,
    .bcastLimitGet = R_ETHSW_BcastLimitGet,
    .mcastLimitSet = R_ETHSW_McastLimitSet,
    .mcastLimitGet = R_ETHSW_McastLimitGet,
    .txRateSet     = R_ETHSW_TxRateSet,
    .txRateGet     = R_ETHSW_TxRateGet,

    /* EEE (Energy Efficient Ethernet) */
    .eeeSet = R_ETHSW_EeeSet,
    .eeeGet = R_ETHSW_EeeGet,

    /* Snooping */
    .snoopAdd    = R_ETHSW_SnoopAdd,
    .snoopRemove = R_ETHSW_SnoopRemove,

    /* Bridge management */
    .portAuhSet     = R_ETHSW_PortAuhSet,
    .portAuhGet     = R_ETHSW_PortAuhGet,
    .portCtrlDirSet = R_ETHSW_PortCtrlDirSet,
    .portCtrlDirGet = R_ETHSW_PortCtrlDirGet,
    .portEapolSet   = R_ETHSW_PortEapolSet,
    .portEapolGet   = R_ETHSW_PortEapolGet,
    .bpduSet        = R_ETHSW_BpduSet,
    .bpduGet        = R_ETHSW_BpduGet,

    /* DLR */
    .dlrInitSet       = R_ETHSW_DlrInitSet,
    .dlrEnableSet     = R_ETHSW_DlrEnableSet,
    .dlrShutdownSet   = R_ETHSW_DlrShutdownSet,
    .dlrLastBeaconGet = R_ETHSW_DlrLastBeaconGet,
    .dlrNodeStateGet  = R_ETHSW_DlrNodeStateGet,
    .dlrRingStateGet  = R_ETHSW_DlrRingStateGet,
    .dlrSvIpGet       = R_ETHSW_DlrSvIpGet,
    .dlrSvPrecGet     = R_ETHSW_DlrSvPrecGet,
    .dlrVlanGet       = R_ETHSW_DlrVlanGet,
    .dlrSvMacGet      = R_ETHSW_DlrSvMacGet,
    .dlrMacEntryDel   = R_ETHSW_DlrMacEntryDel,

    /* Rx Pattern Matcher */
    .rxPatternMatcherSet      = R_ETHSW_RxPatternMatcherSet,
    .rxPatternMatcherGet      = R_ETHSW_RxPatternMatcherGet,
    .rxPatternMatcherCallback = R_ETHSW_RxPatternMatcherCallback,

    /* Pulse Generato */
    .pulseGeneratorInit = R_ETHSW_PulseGeneratorInit,
    .pulseGeneratorSet  = R_ETHSW_PulseGeneratorSet,
    .pulseGeneratorGet  = R_ETHSW_PulseGeneratorGet,

    /* Cut Through */
    .cutThroughSet = R_ETHSW_CutThroughSet,
    .cutThroughGet = R_ETHSW_CutThroughGet,
    .ctDelaySet    = R_ETHSW_CtDelaySet,
    .ctDelayGet    = R_ETHSW_CtDelayGet,
#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */
};

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

 #define ETHSW_HOST_PORT_NUMBER    3

void (* gp_ethsw_ptp_callback)(        ///< pointer of callback function called when an PTP Interupt occurs
    ethsw_ptp_event_t event,
    uint8_t port,
    ethsw_timestamp_t * p_timestamp);

void (* gp_ethsw_tdma_callback)(        ///< pointer of callback function called when an TDMA Interupt occurs
    ethsw_tdma_event_t           event,
    ethsw_tdma_callback_data_t * p_tdma_data);

void (* gp_ethsw_dlr_callback)(         ///< pointer of callback function called when an DLR Interupt occurs
    ethsw_dlr_event_t event,
    uint32_t          port);

void (* gp_ethsw_rx_pattern_callback)(  ///< pointer of callback function called when an Rx pattern matcher interupt occurs
    ethsw_rx_pattern_event_t        event,
    ethsw_rx_pattern_event_data_t * p_data);

static ethsw_snoop_action_t block1Mode; ///< Snoop mode of parser block 1
static ethsw_snoop_action_t block2Mode; ///< Snoop mode of parser block 2

#endif /* ETHSW_EXTENDED_API_FOR_FUTURE */

/*******************************************************************************************************************//**
 * @addtogroup ETHER_SWITCH
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/********************************************************************************************************************//**
 * Open the switch ports and gets it ready to pass through the frames.
 *
 * @retval  FSP_SUCCESS                 Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER SWITCH control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN        Control block has already been opened or channel is being used by another
 *                                      instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_INVALID_CHANNEL     Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Interrupt is not enabled.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_Open (ethsw_ctrl_t * const p_ctrl, ethsw_cfg_t const * const p_cfg)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ethsw_instance_ctrl_t * p_instance_ctrl =
        (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type * p_reg_switch;

#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN((NULL != p_cfg), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((BSP_FEATURE_ETHSW_MAX_CHANNELS > p_cfg->channel), FSP_ERR_INVALID_CHANNEL);
    ETHSW_ERROR_RETURN((0 <= p_cfg->irq), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((ETHSW_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);

    if (ETHSW_PHYLINK_ENABLE == p_cfg->phylink)
    {
        ETHSW_ERROR_RETURN((NULL != p_cfg->p_callback), FSP_ERR_INVALID_POINTER);
        ETHSW_ERROR_RETURN((NULL != p_cfg->p_context), FSP_ERR_INVALID_POINTER);
    }
#endif

    p_instance_ctrl->p_reg_switch    = (void *) R_ETHSW;
    p_instance_ctrl->p_reg_ethsw_ptp = (void *) R_ETHSW_PTP;
    p_instance_ctrl->p_reg_ethss     = (void *) R_ETHSS;

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    /* Initialize configuration of Ethernet switch module. */
    p_instance_ctrl->p_switch_cfg = p_cfg;

    /* Module start */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_ETHSW, p_cfg->channel);
    R_BSP_ModuleResetDisable(BSP_MODULE_RESET_ETHSW);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    /* set Maximum Frame Length size */
    p_reg_switch->FRM_LENGTH_P0_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P1_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P2_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P3_b.FRM_LENGTH = ETHSW_FRAME_SIZE;

    /* Initialize COMMAND_CONFIG_p[0] Reg */
    p_reg_switch->COMMAND_CONFIG_P0 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[1] Reg */
    p_reg_switch->COMMAND_CONFIG_P1 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[2] Reg */
    p_reg_switch->COMMAND_CONFIG_P2 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize COMMAND_CONFIG_p[3] Reg */
    p_reg_switch->COMMAND_CONFIG_P3 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND |
                                       ETHSW_COMMAND_CONFIG_ETH_SPEED | ETHSW_COMMAND_CONFIG_RX_ENA |
                                       ETHSW_COMMAND_CONFIG_TX_ENA);

    /* Initialize PORT ENABLE Reg */
    p_reg_switch->PORT_ENA_b.RXENA =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);
    p_reg_switch->PORT_ENA_b.TXENA =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* Initialize Learning/Lookup function global configuration Reg */
    p_reg_switch->LK_CTRL            = 0;
    p_reg_switch->LK_CTRL_b.ALW_MGRT = 1;
    p_reg_switch->LK_CTRL_b.AGING_EN = 1;
    p_reg_switch->LK_CTRL_b.LEARN_EN = 1;
    p_reg_switch->LK_CTRL_b.LKUP_EN  = 1;

    /* enable Default unicast resolution */
    p_reg_switch->UCAST_DEFAULT_MASK0_b.UCASTDM =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* enable Default broadcast resolution */
    p_reg_switch->BCAST_DEFAULT_MASK0_b.BCASTDM =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* enable Default multicast resolution */
    p_reg_switch->MCAST_DEFAULT_MASK0_b.MCASTDM =
        (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2 | ETHSW_ENABLE_PORT3);

    /* set port in authorized state(Port 0 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT0_b.AUTH = 1;

    /* set port in authorized state(Port 1 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT1_b.AUTH = 1;

    /* set port in authorized state(Port 2 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT2_b.AUTH = 1;

    /* set port in authorized state(Port 3 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT3_b.AUTH = 1;

    /* Initialize MGMT_TAG_CONFIG Reg */
    if (ETHSW_SPECIFIC_TAG_ENABLE == p_cfg->specific_tag)
    {
        p_reg_switch->MGMT_TAG_CONFIG_b.TAGFIELD  = (uint16_t) p_cfg->specific_tag_id;
        p_reg_switch->MGMT_TAG_CONFIG_b.TYPE2_EN  = 0;
        p_reg_switch->MGMT_TAG_CONFIG_b.TYPE1_EN  = 0;
        p_reg_switch->MGMT_TAG_CONFIG_b.AL_FRAMES = 1;
        p_reg_switch->MGMT_TAG_CONFIG_b.ENABLE    = 1;
    }
    else
    {
        p_reg_switch->MGMT_TAG_CONFIG_b.ENABLE = 0;
    }

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE
    err = ethsw_snoop_init(p_reg_switch, ETHSW_SNOOP_ACTION_CAPTURE, ETHSW_SNOOP_ACTION_CPY_FW);
    if (err != FSP_SUCCESS)
    {
        return err;
    }
#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

    /* Set Ethernet Switch interrupt level and enable */
    ethsw_enable_icu(p_instance_ctrl);

    if (ETHSW_PHYLINK_ENABLE == p_instance_ctrl->p_switch_cfg->phylink)
    {
        /* Set line port phy link change interrupt enable Bit (PORT0 - 2) */
        p_reg_switch->INT_CONFIG_b.IRQ_LINK = (ETHSW_ENABLE_PORT0 | ETHSW_ENABLE_PORT1 | ETHSW_ENABLE_PORT2);

        /* Set Interrupt global enable */
        p_reg_switch->INT_CONFIG_b.IRQ_EN = 1;
    }

    p_instance_ctrl->open = ETHSW_OPEN;

    return err;
}                                      /* End of function R_ETHSW_Open() */

/********************************************************************************************************************//**
 * Close the switch ports
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_Close (ethsw_ctrl_t * const p_ctrl)
{
    fsp_err_t               err             = FSP_SUCCESS;
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

    R_ETHSW_Type * p_reg_switch;

    p_reg_switch = p_instance_ctrl->p_reg_switch;

#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set line port phy link change interrupt disable Bit (PORT0 - 2) */
    p_reg_switch->INT_CONFIG_b.IRQ_LINK = 0;

    /* Set Interrupt global disable */
    p_reg_switch->INT_CONFIG_b.IRQ_EN = 0;

    /* set port in unauthorized state(Port 0 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT0_b.AUTH = 0;

    /* set port in unauthorized state(Port 1 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT1_b.AUTH = 0;

    /* set port in unauthorized state(Port 2 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT2_b.AUTH = 0;

    /* set port in unauthorized state(Port 3 authentication control and configuration Reg) */
    p_reg_switch->AUTH_PORT3_b.AUTH = 0;

    /* clear Maximum Frame Length Value */
    p_reg_switch->FRM_LENGTH_P0_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P1_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P2_b.FRM_LENGTH = ETHSW_FRAME_SIZE;
    p_reg_switch->FRM_LENGTH_P3_b.FRM_LENGTH = ETHSW_FRAME_SIZE;

    /* Disable PORT Rx Tx */
    p_reg_switch->PORT_ENA_b.RXENA = 0U;
    p_reg_switch->PORT_ENA_b.TXENA = 0U;

    /* Disable the MAC reception function(COMMAND_CONFIG_p[1] Reg) */
    p_reg_switch->COMMAND_CONFIG_P0 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[2] Reg) */
    p_reg_switch->COMMAND_CONFIG_P1 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[2] Reg) */
    p_reg_switch->COMMAND_CONFIG_P2 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable the MAC reception function(COMMAND_CONFIG_p[3] Reg) */
    p_reg_switch->COMMAND_CONFIG_P3 = (ETHSW_COMMAND_CONFIG_NO_LGTH_CHECK | ETHSW_COMMAND_CONFIG_TX_CRC_APPEND);

    /* Disable Ethernet interrupt. */
    ethsw_disable_icu(p_instance_ctrl);

    /** Clear configure block parameters. */
    p_instance_ctrl->p_switch_cfg = NULL;

    /** Mark the channel not open so other APIs cannot use it. */
    p_instance_ctrl->open = 0U;

    return err;
}                                      /* End of function R_ETHER_Close() */

/********************************************************************************************************************//**
 * Configs speed and duplex of the port
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Invalid input parameter.
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_SpeedCfg (ethsw_ctrl_t * const p_ctrl, uint8_t const port, ethsw_link_speed_t const speed)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_reg_command_config;
    uint32_t                command_config;

#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    switch (port)
    {
        case 0:
        {
            p_reg_command_config = &p_reg_switch->COMMAND_CONFIG_P0;
            break;
        }

        case 1:
        {
            p_reg_command_config = &p_reg_switch->COMMAND_CONFIG_P1;
            break;
        }

        case 2:
        {
            p_reg_command_config = &p_reg_switch->COMMAND_CONFIG_P2;
            break;
        }

        default:
        {
            err = FSP_ERR_INVALID_ARGUMENT;
            break;
        }
    }

    if (err == FSP_SUCCESS)
    {
        command_config = *p_reg_command_config;

        command_config &= ~(ETHSW_COMMAND_CONFIG_ENA_10 |
                            ETHSW_COMMAND_CONFIG_HD_ENA |
                            ETHSW_COMMAND_CONFIG_ETH_SPEED);
        switch (speed)
        {
            case ETHSW_LINK_SPEED_10H:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_ENA_10 |
                                   ETHSW_COMMAND_CONFIG_HD_ENA);
                break;
            }

            case ETHSW_LINK_SPEED_10F:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_ENA_10);
                break;
            }

            case ETHSW_LINK_SPEED_100H:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_HD_ENA);
                break;
            }

            case ETHSW_LINK_SPEED_100F:
            {
                break;
            }

            case ETHSW_LINK_SPEED_1000H:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_HD_ENA |
                                   ETHSW_COMMAND_CONFIG_ETH_SPEED);
                break;
            }

            case ETHSW_LINK_SPEED_1000F:
            {
                command_config |= (ETHSW_COMMAND_CONFIG_ETH_SPEED);
                break;
            }

            default:
            {
                err = FSP_ERR_INVALID_ARGUMENT;
                break;
            }
        }

        *p_reg_command_config = command_config;
    }

    return err;
}                                      /* End of function R_ETHSW_ConfSpeed() */

/********************************************************************************************************************//**
 * Provides API and code version in the user provided pointer. Implements @ref ethsw_api_t::versionGet.
 *
 * @retval  FSP_SUCCESS                  Version information stored in provided p_version.
 * @retval  FSP_ERR_ASSERTION            p_version is NULL.
 ***********************************************************************************************************************/
__INLINE fsp_err_t R_ETHSW_VersionGet (fsp_version_t * const p_version)
{
#if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_version);
#endif

    *p_version = module_version;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VersionGet() */

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

/********************************************************************************************************************//**
 * Gets the number of available external Ethernet ports.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_PortCount (ethsw_ctrl_t * const p_ctrl, uint32_t * p_cnt_ports)
{
 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_cnt_ports), FSP_ERR_INVALID_POINTER);
 #endif

    *p_cnt_ports = ETHSW_PORT_COUNT;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortCount() */

/*******************************************************************************************************************//**
 * Sets the static MAC address entry for the given MAC address.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_TIMEOUT             Timeout error
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MacTableSet (ethsw_ctrl_t * const p_ctrl, ethsw_mactab_entry_t * p_mac_tab)
{
    ethsw_mac_addr_reg_t    entry = {0};
    uint64_t                timeout;        /* timeout */
    ethsw_port_mask_t       port_mask;      /* converted portmask */
    uint32_t                priority;       /* switching priority for entry */
    volatile uint32_t       dammy_read;

    R_ETHSW_Type volatile * p_switch_reg;
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_mac_tab), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((NULL != p_mac_tab->addr), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((NULL != *p_mac_tab->addr), FSP_ERR_INVALID_POINTER);

    ETHSW_ERROR_RETURN((p_mac_tab->portmask & ETHSW_PORT_MASK), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* remove portmask bit and translate host port if set */
    port_mask = p_mac_tab->portmask & ETHSW_PORT_BITS;
    if (p_mac_tab->portmask & ETHSW_PORT_HOST)
    {
        port_mask |= (1U << ETHSW_HOST_PORT_NUMBER);
    }

    /* copy mac address to get valid alignment */
    memcpy(entry.mac_addr, *p_mac_tab->addr, ETHSW_MAC_ADDR_LEN);

    p_switch_reg->LK_DATA_LO = entry.reg_data[0];
    p_switch_reg->LK_DATA_HI = entry.reg_data[1];

    /* damy read */
    dammy_read = p_switch_reg->LK_DATA_LO;
    dammy_read = p_switch_reg->LK_DATA_HI;
    dammy_read = dammy_read;

    /* Delete entry? */
    if (0 == port_mask)
    {
        p_switch_reg->LK_ADDR_CTRL |= (ETHSW_LK_ADDR_CLEAR | ETHSW_LK_ADDR_LOOKUP);
    }
    else
    {
        timeout = ETHSW_TIMEOUT_COUNT;

        /* Perform lookup to get address entry */
        p_switch_reg->LK_ADDR_CTRL |= ETHSW_LK_ADDR_LOOKUP;
        while (true == p_switch_reg->LK_ADDR_CTRL_b.BUSY)
        {
            ETHSW_ERROR_RETURN((0 != timeout), FSP_ERR_TIMEOUT);
            timeout--;
        }

        /* extract requested priority if given by command data */
        priority = p_mac_tab->priority;

        /* Address is already there. Just update the port mask and priority */
        if (p_switch_reg->LK_DATA_HI & ETHSW_LK_ADDR_REG_VALID)
        {
            /* update port bit mask  */
            p_switch_reg->LK_DATA_HI &= ~(uint32_t) ETHSW_LK_ADDR_PORTMASK;
            p_switch_reg->LK_DATA_HI |= (port_mask << ETHSW_LK_ADDR_PORT_REG_OFFSET);

            /* updated priority */
            p_switch_reg->LK_DATA_HI &= ~(uint32_t) ETHSW_LK_ADDR_PRIOMASK;
            p_switch_reg->LK_DATA_HI |= (priority << ETHSW_LK_ADDR_PRIO_REG_OFFSET) & ETHSW_LK_ADDR_PRIOMASK;
        }
        else
        {
            /* New static entry to be created */
            p_switch_reg->LK_DATA_HI &= ~ETHSW_LK_ADDR_MASKANDFLAGS_MASK;
            p_switch_reg->LK_DATA_HI |= ETHSW_LK_ADDR_REG_VALID;
            p_switch_reg->LK_DATA_HI |= ETHSW_LK_ADDR_REG_TYPE;
            p_switch_reg->LK_DATA_HI |= port_mask << ETHSW_LK_ADDR_PORT_REG_OFFSET;
            p_switch_reg->LK_DATA_HI |= (priority << ETHSW_LK_ADDR_PRIO_REG_OFFSET) & ETHSW_LK_ADDR_PRIOMASK;
        }

        /* trigger MAC table write */
        p_switch_reg->LK_ADDR_CTRL |= ETHSW_LK_ADDR_WRITE;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MacTableSet() */

/*******************************************************************************************************************//**
 * Retrieves the port mask for the given MAC address from the static MAC table.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_TIMEOUT             Timeout error
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MacTableGet (ethsw_ctrl_t * const p_ctrl, ethsw_mactab_entry_t * p_mac_tab)
{
    uint64_t timeout;                            /* timeout */
    volatile ethsw_mactab_entry_reg_t * p_entry; /* MAC table entry(for register access) */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_mac_tab), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((NULL != p_mac_tab->addr), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((NULL != *p_mac_tab->addr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* set Entry-pointer to "Lower 32-Bit Data of Lookup Memory Entry" register */
    p_entry = (volatile ethsw_mactab_entry_reg_t *) &(p_switch_reg->LK_DATA_LO);

    ethsw_memcpy_volatile(p_entry->mac, *p_mac_tab->addr, ETHSW_MAC_ADDR_LEN);

    timeout = ETHSW_TIMEOUT_COUNT;

    /* Perform lookup to get address entry */
    p_switch_reg->LK_ADDR_CTRL |= ETHSW_LK_ADDR_LOOKUP;
    while (true == p_switch_reg->LK_ADDR_CTRL_b.BUSY)
    {
        ETHSW_ERROR_RETURN((0 != timeout), FSP_ERR_TIMEOUT);
        timeout--;
    }

    /* Entry found? */
    if (p_entry->mask_and_flags & ETHSW_LK_ADDR_VALID)
    {
        p_mac_tab->portmask = p_entry->mask_and_flags >> ETHSW_LK_ADDR_PORT_OFFSET;

        /* if host port is set convert to host port */
        if (p_mac_tab->portmask & (1U << ETHSW_HOST_PORT_NUMBER))
        {
            p_mac_tab->portmask = (p_mac_tab->portmask & ~(1U << ETHSW_HOST_PORT_NUMBER)) | ETHSW_PORT_HOST;
        }

        /* set port mask flag */
        p_mac_tab->portmask |= ETHSW_PORT_MASK;
    }
    else
    {
        p_mac_tab->portmask = 0;

        return FSP_ERR_INVALID_ARGUMENT;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MacTableGet() */

/*******************************************************************************************************************//**
 *  Configures the MAC learning and aging parameters of MAC table.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MacTableConfSet (ethsw_ctrl_t * const p_ctrl, ethsw_mactab_conf_t * p_mac_tab_cnf)
{
    uint32_t reg_val;                  /* Register value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_mac_tab_cnf), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    reg_val  = p_switch_reg->LK_CTRL & ~((uint32_t) ETHSW_LK_MASK);
    reg_val |= ((uint32_t) p_mac_tab_cnf->learn << ETHSW_LK_LEARN) |
               ((uint32_t) p_mac_tab_cnf->aging << ETHSW_LK_AGING) |
               ((uint32_t) p_mac_tab_cnf->migration << ETHSW_LK_MIGRAT) |
               ((uint32_t) p_mac_tab_cnf->discunknown << ETHSW_LK_DISCUNKN) |
               ((uint32_t) p_mac_tab_cnf->pervlan << ETHSW_LK_PERVLAN);

    p_switch_reg->LK_CTRL = reg_val;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MacTableConfSet() */

/*******************************************************************************************************************//**
 * Retrieves the MAC learning and aging parameters of MAC table.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MacTableConfGet (ethsw_ctrl_t * const p_ctrl, ethsw_mactab_conf_t * p_mac_tab_cnf)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_mac_tab_cnf), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_mac_tab_cnf->learn       = (p_switch_reg->LK_CTRL & (1U << ETHSW_LK_LEARN)) ? true : false;
    p_mac_tab_cnf->aging       = (p_switch_reg->LK_CTRL & (1U << ETHSW_LK_AGING)) ? true : false;
    p_mac_tab_cnf->migration   = (p_switch_reg->LK_CTRL & (1U << ETHSW_LK_MIGRAT)) ? true : false;
    p_mac_tab_cnf->discunknown = (p_switch_reg->LK_CTRL & (1U << ETHSW_LK_DISCUNKN)) ? true : false;
    p_mac_tab_cnf->pervlan     = (p_switch_reg->LK_CTRL & (1U << ETHSW_LK_PERVLAN)) ? true : false;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MacTableConfGet() */

/*******************************************************************************************************************//**
 * Clears specific types of entries from the MAC table or clears the whole table.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_TIMEOUT             Timeout error
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MacTableClear (ethsw_ctrl_t * const p_ctrl, ethsw_mactab_clr_modes_t * p_mac_tab_clr)
{
    fsp_err_t res = FSP_ERR_TIMEOUT;   /* result */
    uint64_t  timeout;                 /* timeout */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_mac_tab_clr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    timeout = ETHSW_TIMEOUT_COUNT;

    switch (*p_mac_tab_clr)
    {
        case ETHSW_MACTAB_CLR_ALL:
        {
            p_switch_reg->LK_CTRL_b.CLRTBL = true;

            while (true == p_switch_reg->LK_CTRL_b.CLRTBL)
            {
                ETHSW_ERROR_RETURN((0 != timeout), FSP_ERR_TIMEOUT);
                timeout--;
            }

            res = FSP_SUCCESS;
            break;
        }

        case ETHSW_MACTAB_CLR_DYNAMIC:
        {
            p_switch_reg->LK_ADDR_CTRL_b.CLR_DYNAMIC = true;

            while (true == p_switch_reg->LK_ADDR_CTRL_b.CLR_DYNAMIC)
            {
                ETHSW_ERROR_RETURN((0 != timeout), FSP_ERR_TIMEOUT);
                timeout--;
            }

            res = FSP_SUCCESS;
            break;
        }

        case ETHSW_MACTAB_CLR_STATIC:
        {
            p_switch_reg->LK_ADDR_CTRL_b.CLR_STATIC = true;

            while (true == p_switch_reg->LK_ADDR_CTRL_b.CLR_STATIC)
            {
                ETHSW_ERROR_RETURN((0 != timeout), FSP_ERR_TIMEOUT);
                timeout--;
            }

            res = FSP_SUCCESS;
            break;
        }

        default:
        {
            res = FSP_ERR_INVALID_ARGUMENT;
            break;
        }
    }

    return res;
}                                      /* End of function R_ETHSW_MacTableClear() */

/*******************************************************************************************************************//**
 * Sets learning state for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_LearningSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_dlr_learn)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type          * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_dlr_learn), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    /* set learning state */
    if (*p_flag_dlr_learn)
    {
        p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS &= ~(1U << port_num) & ETHSW_4BITS_MASK;
    }
    else
    {
        p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS = (p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS | (1U << port_num)) &
                                                     ETHSW_4BITS_MASK;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_LearningSet() */

/*******************************************************************************************************************//**
 * Gets learning state for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_LearningGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_dlr_learn)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_dlr_learn), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    *p_flag_dlr_learn = (bool) !(p_switch_reg->INPUT_LEARN_BLOCK_b.LEARNDIS & (1U << port_num));

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_LearningGet() */

/*******************************************************************************************************************//**
 * Enables the forwarding of incoming frames on a port for RSTP.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortForwardAdd (ethsw_ctrl_t * const p_ctrl, uint32_t port)
{
    uint32_t block;                    /* register value */
    uint32_t tx;                       /* register value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    block  = p_switch_reg->INPUT_LEARN_BLOCK_b.BLOCKEN;
    block &= ~(1U << port_num);

    tx  = p_switch_reg->PORT_ENA_b.TXENA;
    tx |= (1U << port_num);

    p_switch_reg->INPUT_LEARN_BLOCK_b.BLOCKEN = block & ETHSW_4BITS_MASK;

    p_switch_reg->PORT_ENA_b.TXENA = tx & ETHSW_4BITS_MASK;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disables the forwarding of incoming frames on a port for RSTP
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortForwardDel (ethsw_ctrl_t * const p_ctrl, uint32_t port)
{
    uint32_t block;                    /* register value */
    uint32_t tx;                       /* register value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    block  = p_switch_reg->INPUT_LEARN_BLOCK_b.BLOCKEN;
    block |= (1U << port_num);

    tx  = p_switch_reg->PORT_ENA_b.TXENA;
    tx &= ~(1U << port_num);

    p_switch_reg->INPUT_LEARN_BLOCK_b.BLOCKEN = block & ETHSW_4BITS_MASK;

    p_switch_reg->PORT_ENA_b.TXENA = tx & ETHSW_4BITS_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortForwardAdd() */

/*******************************************************************************************************************//**
 * Set the Discard Unknown Destination Bit in Control register.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DiscardUnknownSet (ethsw_ctrl_t * const p_ctrl, bool * p_flag_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (true == *p_flag_enable)
    {
        p_switch_reg->LK_CTRL_b.DISC_UNK_DEST = 1;
    }
    else
    {
        p_switch_reg->LK_CTRL_b.DISC_UNK_DEST = 0;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DiscardUnknownSet() */

/*******************************************************************************************************************//**
 * Get the Discard Unknown Destination Bit in Control register.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DiscardUnknownGet (ethsw_ctrl_t * const p_ctrl, bool * p_flag_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (p_switch_reg->LK_CTRL_b.DISC_UNK_DEST)
    {
        *p_flag_enable = true;
    }
    else
    {
        *p_flag_enable = false;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DiscardUnknownGet() */

/*******************************************************************************************************************//**
 * Set the flood domain port masks for frames with unknown destinations
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_FloodUnknownSet (ethsw_ctrl_t * const p_ctrl, ethsw_flood_unk_conf_t * p_flood_unk_conf)
{
    uint32_t reg;                      /* value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_flood_unk_conf), FSP_ERR_INVALID_POINTER);

    /* this function only works with a portmask */
    ETHSW_ERROR_RETURN((p_flood_unk_conf->port_mask_bcast & ETHSW_PORT_MASK), FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN((p_flood_unk_conf->port_mask_mcast & ETHSW_PORT_MASK), FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN((p_flood_unk_conf->port_mask_ucast & ETHSW_PORT_MASK), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    reg = p_flood_unk_conf->port_mask_bcast;
    if (reg & ETHSW_PORT_HOST)
    {
        reg &= ~ETHSW_PORT_HOST;
        reg |= ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
    }

    reg &= ~ETHSW_PORT_MASK;
    p_switch_reg->BCAST_DEFAULT_MASK0_b.BCASTDM = reg & ETHSW_4BITS_MASK;

    reg = p_flood_unk_conf->port_mask_mcast;
    if (reg & ETHSW_PORT_HOST)
    {
        reg &= ~ETHSW_PORT_HOST;
        reg |= ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
    }

    reg &= ~ETHSW_PORT_MASK;
    p_switch_reg->MCAST_DEFAULT_MASK0_b.MCASTDM = reg & ETHSW_4BITS_MASK;

    reg = p_flood_unk_conf->port_mask_ucast;
    if (reg & ETHSW_PORT_HOST)
    {
        reg &= ~ETHSW_PORT_HOST;
        reg |= ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
    }

    reg &= ~ETHSW_PORT_MASK;
    p_switch_reg->UCAST_DEFAULT_MASK0_b.UCASTDM = reg & ETHSW_4BITS_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_FloodUnknownSet() */

/*******************************************************************************************************************//**
 * Get the flood domain port masks for frames with unknown destinations.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_FloodUnknownGet (ethsw_ctrl_t * const p_ctrl, ethsw_flood_unk_conf_t * p_flood_unk_conf)
{
    ethsw_flood_unk_conf_t * p_config; /* config data */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_flood_unk_conf), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_config = p_flood_unk_conf;

    p_config->port_mask_bcast = p_switch_reg->BCAST_DEFAULT_MASK0_b.BCASTDM;
    p_config->port_mask_mcast = p_switch_reg->MCAST_DEFAULT_MASK0_b.MCASTDM;
    p_config->port_mask_ucast = p_switch_reg->UCAST_DEFAULT_MASK0_b.UCASTDM;

    if (p_config->port_mask_bcast & (~ETHSW_PORT_MASK & ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER)))
    {
        p_config->port_mask_bcast &= ~ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
        p_config->port_mask_bcast |= ETHSW_PORT_HOST;
    }

    p_config->port_mask_bcast |= ETHSW_PORT_MASK;

    if (p_config->port_mask_mcast & (~ETHSW_PORT_MASK & ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER)))
    {
        p_config->port_mask_mcast &= ~ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
        p_config->port_mask_mcast |= ETHSW_PORT_HOST;
    }

    p_config->port_mask_mcast |= ETHSW_PORT_MASK;

    if (p_config->port_mask_ucast & (~ETHSW_PORT_MASK & ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER)))
    {
        p_config->port_mask_ucast &= ~ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
        p_config->port_mask_ucast |= ETHSW_PORT_HOST;
    }

    p_config->port_mask_ucast |= ETHSW_PORT_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_FloodUnknownGet() */

/*******************************************************************************************************************//**
 * Return link state for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_LinkStateGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_state_link)
{
    const volatile uint32_t * p_reg;   /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_link), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (const volatile uint32_t *) ((uint8_t *) &p_switch_reg->STATUS_P0 +
                                         ETHSW_COUNTER_OFFSET * port_num);

    *p_state_link = (*p_reg & ETHSW_PHY_LINK) ?
                    ETHSW_STATE_UP : ETHSW_STATE_DOWN;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_LinkStateGet() */

/*******************************************************************************************************************//**
 * Sets admin state for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortAdminStateSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_state_port)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_port), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &(p_switch_reg->COMMAND_CONFIG_P0) + (ETHSW_PORT_OFFSET * port_num));

    if (true == *p_state_port)
    {
        *p_reg |= ETHSW_PORT_RXTX;
    }
    else
    {
        *p_reg &= ~((uint32_t) ETHSW_PORT_RXTX);
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortAdminStateSet() */

/*******************************************************************************************************************//**
 * Gets admin state for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortAdminStateGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_state_port)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_port), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &(p_switch_reg->COMMAND_CONFIG_P0) + (ETHSW_PORT_OFFSET * port_num));

    *p_state_port = ((*p_reg & ETHSW_PORT_RXTX) == ETHSW_PORT_RXTX) ? true : false;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortAdminStateGet() */

/*******************************************************************************************************************//**
 * Sets maximum frame size of given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_FrameSizeMaxSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_frame_size_max)
{
    volatile uint32_t * p_reg;         /* Register */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_frame_size_max), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* calculate per port address of the frame size register */
    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->FRM_LENGTH_P0 + (ETHSW_PORT_OFFSET * port_num));

    /* assign masked command data to register */
    *p_reg = *p_frame_size_max & ETHSW_FRAMESIZE_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_FrameSizeMaxSet() */

/*******************************************************************************************************************//**
 * Gets maximum frame size of given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_FrameSizeMaxGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_frame_size_max)
{
    volatile uint32_t * p_reg;         /* Register */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_frame_size_max), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* calculate per port address of the frame size register */
    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->FRM_LENGTH_P0 + (ETHSW_PORT_OFFSET * port_num));

    /* assign masked register value to command data */
    *p_frame_size_max = ((*p_reg) & ETHSW_FRAMESIZE_MASK);

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_FrameSizeMaxGet() */

/*******************************************************************************************************************//**
 * Sets the default VLAN for input/output processing for the specified port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanDefaultSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint16_t * p_id_vlan)
{
    volatile uint32_t * p_def_tag;     /* default tag */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_id_vlan), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_def_tag = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->SYSTEM_TAGINFO0 +
                                       (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_def_tag = *p_id_vlan & ETHSW_VLAN_DEFID_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanDefaultSet() */

/*******************************************************************************************************************//**
 * Gets the default VLAN for input/output processing for the specified port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanDefaultGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint16_t * p_id_vlan)
{
    volatile uint32_t * p_def_tag;     /* default tag */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_id_vlan), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_def_tag = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->SYSTEM_TAGINFO0 +
                                       (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_id_vlan = *p_def_tag & ETHSW_VLAN_DEFID_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanDefaultGet() */

/*******************************************************************************************************************//**
 * Adds a VLAN for a given port to the VLAN domain table.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanPortAdd (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint16_t * p_id_vlan)
{
    volatile uint32_t * p_vlan_entry;  /* Pointer VLAN table entry */
    fsp_err_t           err = FSP_ERR_INVALID_ARGUMENT;
    uint32_t            port_num;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_id_vlan), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_vlan_entry = &(p_switch_reg->VLAN_RES_TABLE0);

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    /* First run: check whether the vlan id entry is already in the table */
    while (p_vlan_entry <= &p_switch_reg->VLAN_RES_TABLE31)
    {
        if ((uint32_t) (*p_vlan_entry & ETHSW_VLANID_MASK) ==
            (uint32_t) (*p_id_vlan << ETHSW_VLANID_OFFSET))
        {
            /* Update port info and return */
            *p_vlan_entry = *p_vlan_entry | (1U << port_num);

            err = FSP_SUCCESS;
            break;
        }

        p_vlan_entry = (volatile uint32_t *) ((uint8_t *) p_vlan_entry + ETHSW_PORT_OFFSET_4BYTES);
    }

    if (FSP_SUCCESS != err)
    {
        /* Second run: find free entry and add VLAN id and port */
        p_vlan_entry = &(p_switch_reg->VLAN_RES_TABLE0);
        while (p_vlan_entry <= &p_switch_reg->VLAN_RES_TABLE31)
        {
            /* Default VLANid is all bit set */
            if (ETHSW_VLANID_MASK == (*p_vlan_entry & ETHSW_VLANID_MASK))
            {
                *p_vlan_entry = (1U << port_num) | (((uint32_t) *p_id_vlan) << ETHSW_VLANID_OFFSET);

                err = FSP_SUCCESS;
                break;
            }

            p_vlan_entry = (volatile uint32_t *) ((uint8_t *) p_vlan_entry + ETHSW_PORT_OFFSET_4BYTES);
        }
    }

    /* If not FSP_SUCCESS, not VLAN not in table and no free entries left */

    return err;
}                                      /* End of function R_ETHSW_VlanPortAdd() */

/*******************************************************************************************************************//**
 * Removes a VLAN for a given port to the VLAN domain table.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanPortRemove (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint16_t * p_id_vlan)
{
    volatile uint32_t * p_vlan_entry;  /* Pointer VLAN table entry */
    fsp_err_t           err = FSP_ERR_INVALID_ARGUMENT;
    uint32_t            port_num;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_id_vlan), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_vlan_entry = &(p_switch_reg->VLAN_RES_TABLE0);

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    /* First run: check whether the vlan id entry is already in the table */
    while (p_vlan_entry <= &p_switch_reg->VLAN_RES_TABLE31)
    {
        if ((uint32_t) (*p_vlan_entry & ETHSW_VLANID_MASK) ==
            (uint32_t) (*p_id_vlan << ETHSW_VLANID_OFFSET))
        {
            /* Remove port from port mask */
            *p_vlan_entry &= ~(1U << port_num);

            /* Reset to default if no more ports are mapped
             * so the entry is free again for new VLAN ids
             */
            if (0 == ((*p_vlan_entry) & ETHSW_VLAN_PORT_MASK))
            {
                *p_vlan_entry = ETHSW_VLAN_INVALID_ENTRY;
            }

            err = FSP_SUCCESS;
            break;
        }

        p_vlan_entry = (volatile uint32_t *) ((uint8_t *) p_vlan_entry + ETHSW_PORT_OFFSET_4BYTES);
    }

    /* If not FSP_SUCCESS, The VLAN id was not in the table so nothing to remove */

    return err;
}                                      /* End of function R_ETHSW_VlanPortRemove() */

/*******************************************************************************************************************//**
 * Sets the mode of VLAN input operations for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanInModeSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_vlan_in_mode_t * p_mode_vlan_in)
{
    uint32_t reg_val;                  /* switch reg value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_vlan_in), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_VLANIN_PASSTHROUGH_OVERRIDE == *p_mode_vlan_in) ||
                        (ETHSW_VLANIN_REPLACE == *p_mode_vlan_in) ||
                        (ETHSW_VLANIN_TAG_ALWAYS == *p_mode_vlan_in) ||
                        (ETHSW_VLANIN_DISABLE == *p_mode_vlan_in)),
                       FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    /* Disable VLAN input processing ? */
    if (ETHSW_VLANIN_DISABLE == *p_mode_vlan_in)
    {
        /* Disable input processing and reset mode */
        p_switch_reg->VLAN_IN_MODE_ENA_b.VLANINMDEN &= ~(1U << port_num) & ETHSW_4BITS_MASK;

        p_switch_reg->VLAN_IN_MODE &= ~((uint32_t) ETHSW_VLAN_INOUT_MODE_MASK << (port_num * ETHSW_VLAN_MODE_BITS));
    }
    else
    {
        reg_val = p_switch_reg->VLAN_IN_MODE;

        /* Set new mode */
        reg_val &= ~(((uint32_t) ETHSW_VLAN_INOUT_MODE_MASK) << (port_num * ETHSW_VLAN_MODE_BITS));
        reg_val |= ((uint32_t) *p_mode_vlan_in << (port_num * 2));

        p_switch_reg->VLAN_IN_MODE = reg_val;

        /* Enable input processing */
        p_switch_reg->VLAN_IN_MODE_ENA_b.VLANINMDEN = (p_switch_reg->VLAN_IN_MODE_ENA_b.VLANINMDEN | (1U << port_num)) &
                                                      ETHSW_4BITS_MASK;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanInModeSet() */

/*******************************************************************************************************************//**
 * Gets the mode of VLAN input operations for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanInModeGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_vlan_in_mode_t * p_mode_vlan_in)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_vlan_in), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    /* VLAN input processing enabled? */
    if (0 == (p_switch_reg->VLAN_IN_MODE_ENA & (1U << port_num)))
    {
        *p_mode_vlan_in = ETHSW_VLANIN_DISABLE;
    }
    else
    {
        *p_mode_vlan_in = (ethsw_vlan_in_mode_t) ((p_switch_reg->VLAN_IN_MODE >>
                                                   (port_num * ETHSW_VLAN_MODE_BITS)) &
                                                  ETHSW_VLAN_INOUT_MODE_MASK);
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanInModeGet() */

/*******************************************************************************************************************//**
 * Sets the mode of VLAN output operations for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanOutModeSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_vlan_out_mode_t * p_mode_vlan_out)

{
    uint32_t reg_val;                  /* register value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_vlan_out), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_VLANOUT_DISABLE == *p_mode_vlan_out) ||
                        (ETHSW_VLANOUT_STRIP == *p_mode_vlan_out) ||
                        (ETHSW_VLANOUT_TAGTHROUGH == *p_mode_vlan_out) ||
                        (ETHSW_VLANOUT_DOMAINTRANSP == *p_mode_vlan_out)),
                       FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    reg_val = p_switch_reg->VLAN_OUT_MODE;

    /* Set new mode */
    reg_val &= ~((uint32_t) ETHSW_VLAN_INOUT_MODE_MASK << (port_num * ETHSW_VLAN_MODE_BITS));
    reg_val |= ((uint32_t) *p_mode_vlan_out << (port_num * ETHSW_VLAN_MODE_BITS));

    p_switch_reg->VLAN_OUT_MODE = reg_val;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanOutModeSet() */

/*******************************************************************************************************************//**
 * Gets the mode of VLAN output operations for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanOutModeGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_vlan_out_mode_t * p_mode_vlan_out)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_vlan_out), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    *p_mode_vlan_out = (ethsw_vlan_out_mode_t) ((p_switch_reg->VLAN_OUT_MODE >>
                                                 (port_num * ETHSW_VLAN_MODE_BITS)) &
                                                ETHSW_VLAN_INOUT_MODE_MASK);

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanOutModeGet() */

/*******************************************************************************************************************//**
 * Gets count of VLAN table entries.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanTableCount (ethsw_ctrl_t * const p_ctrl, uint32_t * p_cnt_tab_entries)

{
 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_cnt_tab_entries), FSP_ERR_INVALID_POINTER);
 #endif

    *p_cnt_tab_entries = ETHSW_RN_ADVSW_VLAN_TABLE_ENTRIES;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanTableCount() */

/*******************************************************************************************************************//**
 * Gets information of VLAN table entries.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanTableGet (ethsw_ctrl_t * const p_ctrl, ethsw_vlan_entry_t * p_vlan)

{
    volatile uint32_t * p_vlan_entry;  /* VLAN entry */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_vlan), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_RN_ADVSW_VLAN_TABLE_ENTRIES > p_vlan->index), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Pointer VLAN table entry */
    p_vlan_entry = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->VLAN_RES_TABLE0 +
                                          (ETHSW_PORT_OFFSET_4BYTES * p_vlan->index));

    /* Entry valid ? */
    if (ETHSW_VLAN_INVALID_ENTRY == (*p_vlan_entry & ETHSW_VLAN_INVALID_ENTRY))
    {
        /* Invalid */
        p_vlan->valid    = false;
        p_vlan->vlan     = 0;
        p_vlan->portmask = 0;
    }
    else
    {
        /* Entry is valid */
        p_vlan->valid    = true;
        p_vlan->vlan     = (*p_vlan_entry & ETHSW_VLANID_MASK) >> ETHSW_VLANID_OFFSET;
        p_vlan->portmask = (*p_vlan_entry) & ETHSW_VLAN_PORT_MASK;

        if (p_vlan->portmask & (~ETHSW_PORT_MASK & ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER)))
        {
            p_vlan->portmask &= ~ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
            p_vlan->portmask |= ETHSW_PORT_HOST;
        }

        p_vlan->portmask |= ETHSW_PORT_MASK;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanTableGet() */

/*******************************************************************************************************************//**
 * Set VLAN verification for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanVerifySet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    if (*p_flag_enable)
    {
        p_switch_reg->VLAN_VERIFY_b.VLANVERI = (p_switch_reg->VLAN_VERIFY_b.VLANVERI | (1U << port_num)) &
                                               ETHSW_4BITS_MASK;
    }
    else
    {
        p_switch_reg->VLAN_VERIFY_b.VLANVERI &= ~(1U << port_num) & ETHSW_4BITS_MASK;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanVerifySet() */

/*******************************************************************************************************************//**
 * Get VLAN verification for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanVerifyGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    if (p_switch_reg->VLAN_VERIFY_b.VLANVERI & (1U << port_num))
    {
        *p_flag_enable = true;
    }
    else
    {
        *p_flag_enable = false;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanVerifyGet() */

/*******************************************************************************************************************//**
 * Set VLAN discartion of unknown frames for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanDiscardUnknownSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    if (*p_flag_enable)
    {
        p_switch_reg->VLAN_VERIFY_b.VLANDISC = (p_switch_reg->VLAN_VERIFY_b.VLANDISC | (1U << port_num)) &
                                               ETHSW_4BITS_MASK;
    }
    else
    {
        p_switch_reg->VLAN_VERIFY_b.VLANDISC &= ~(1U << port_num) & ETHSW_4BITS_MASK;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanDiscardUnknownSet() */

/*******************************************************************************************************************//**
 * Get VLAN discartion of unknown frames for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_VlanDiscardUnknownGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;
    uint32_t                port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    if (p_switch_reg->VLAN_VERIFY_b.VLANDISC & (1U << port_num))
    {
        *p_flag_enable = true;
    }
    else
    {
        *p_flag_enable = false;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_VlanDiscardUnknownGet() */

/*******************************************************************************************************************//**
 * Enable Tx timestamping for an external port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TimeStamp (ethsw_ctrl_t * const p_ctrl, uint32_t port, bool * p_flag_enable)
{
    uint32_t reg_config;               /* CONFIG */
    uint32_t reg_int_config;           /* INT_CONFIG */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT > port), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_flag_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    reg_config     = p_switch_reg->TSM_CONFIG; /* CONFIG register */
    reg_int_config = p_switch_reg->INT_CONFIG; /* INT_CONFIG register */

    if (false != *p_flag_enable)
    {
        /* enable options */
        reg_config |=
            (ETHSW_CONFIG_IRQ_EN | ETHSW_CONFIG_IRQ_TSFIFO_OVR | (1U << (port + ETHSW_CONFIG_IRQ_TX_EN_SHIFT)));
        p_switch_reg->TSM_CONFIG = reg_config;

        reg_int_config          |= (ETHSW_INT_CONFIG_IRQ_EN | ETHSW_INT_CONFIG_TSM_INT);
        p_switch_reg->INT_CONFIG = reg_int_config;
    }
    else
    {
        /* disable options */
        reg_config &= (~(1U << (port + ETHSW_CONFIG_IRQ_TX_EN_SHIFT)));

        if (0 == (reg_config & ETHSW_CONFIG_IRQ_TX_EN_MASK))
        {
            reg_config &= (~ETHSW_CONFIG_IRQ_TSFIFO_OVR);

            reg_int_config          &= (~ETHSW_INT_CONFIG_TSM_INT);
            p_switch_reg->INT_CONFIG = reg_int_config;
        }

        p_switch_reg->TSM_CONFIG = reg_config;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TimeStamp() */

/*******************************************************************************************************************//**
 * Get the latancy between leaving PHY and timestamping of outgoing frames.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsLatencyEgressGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_latency_egress)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT > port), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_latency_egress), FSP_ERR_INVALID_POINTER);
 #endif

    /* store egress latency */
 #if 0
    *p_latency_egress = switch_config[port].latency_egress;
 #else
    *p_latency_egress = 0;             /* This is a temporary!!! */
 #endif

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsLatencyEgressGet() */

/*******************************************************************************************************************//**
 * Get the latency between receiving in PHY and timestamping of incoming frames.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsLatencyIngressGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_latency_ingress)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT > port), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_latency_ingress), FSP_ERR_INVALID_POINTER);
 #endif

    /* store ingress latency */
 #if 0
    *p_latency_ingress = switch_config[port].latency_ingress;
 #else
    *p_latency_ingress = 0;            /* This is a temporary!!! */
 #endif

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsLatencyIngressGet() */

/*******************************************************************************************************************//**
 * Enables / disables the timer with the specified timer number.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsActive (ethsw_ctrl_t * const p_ctrl, ethsw_time_enable_t * p_time_enable)
{
    volatile uint32_t * p_atime_ctrl;  /* time control for time */
    volatile uint32_t * p_atime_inc;   /* Clock period of the timestamping clock */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_time_enable), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_time_enable->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Pointer ATIME_CTRL[n] register */
    p_atime_ctrl = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_CTRL0 +
                                          (ETHSW_PORT_OFFSET_32BYTES * p_time_enable->tim_num));

    /* Pointer ATIME_INC[n] register */
    p_atime_inc = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_INC0 +
                                         (ETHSW_PORT_OFFSET_32BYTES * p_time_enable->tim_num));

    if (true == p_time_enable->flag_enable)
    {
        /* enable operation */
        *p_atime_ctrl |= ETHSW_ATIME_CTRL_ENABLE;

        *p_atime_inc &= (~ETHSW_ATIME_INC_CLOCK_PERIOD_MASK);
        *p_atime_inc |= (uint32_t) (p_time_enable->clock_period << ETHSW_ATIME_INC_CLOCK_PERIOD_SHIFT) &
                        ETHSW_ATIME_INC_CLOCK_PERIOD_MASK;
    }
    else
    {
        /* disable operation */
        *p_atime_ctrl &= (~ETHSW_ATIME_CTRL_ENABLE);
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsActive() */

/*******************************************************************************************************************//**
 * Sets current timer.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsSet (ethsw_ctrl_t * const p_ctrl, ethsw_timestamp_t * p_timestamp)
{
    volatile uint32_t * p_atime;       /* nanoseconds time value */
    volatile uint32_t * p_atime_sec;   /* seconds time value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_timestamp), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_timestamp->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Pointer ATIME[n] register */
    p_atime = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME0 +
                                     (ETHSW_PORT_OFFSET_32BYTES * p_timestamp->tim_num));

    /* Pointer ATIME_SEC[n] register */
    p_atime_sec = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_SEC0 +
                                         (ETHSW_PORT_OFFSET_32BYTES * p_timestamp->tim_num));

    *p_atime_sec = p_timestamp->tim_sec;
    *p_atime     = p_timestamp->tim_nsec;

    return FSP_SUCCESS;
}                                      /* End of ethsw_timer_set() */

/*******************************************************************************************************************//**
 * Gets current timer
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_TIMEOUT             Timeout error
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsGet (ethsw_ctrl_t * const p_ctrl, ethsw_timestamp_t * p_timestamp)
{
    volatile uint32_t * p_atime_ctrl;  /* time control for time */
    volatile uint32_t * p_atime;       /* nanoseconds time value */
    volatile uint32_t * p_atime_sec;   /* seconds time value */
    uint64_t            timeout;       /* timeout */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_timestamp), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_timestamp->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Pointer ATIME_CTRL[n] register */
    p_atime_ctrl = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_CTRL0 +
                                          (ETHSW_PORT_OFFSET_32BYTES * p_timestamp->tim_num));

    /* Pointer ATIME[n] register */
    p_atime = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME0 +
                                     (ETHSW_PORT_OFFSET_32BYTES * p_timestamp->tim_num));

    /* Pointer ATIME_SEC[n] register */
    p_atime_sec = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->ATIME_SEC0 +
                                         (ETHSW_PORT_OFFSET_32BYTES * p_timestamp->tim_num));

    *p_atime_ctrl |= ETHSW_ATIME_CTRL_CAPTURE;

    timeout = ETHSW_TIMEOUT_COUNT;

    while (ETHSW_ATIME_CTRL_CAPTURE == (*p_atime_ctrl & ETHSW_ATIME_CTRL_CAPTURE))
    {
        ETHSW_ERROR_RETURN((0 != timeout), FSP_ERR_TIMEOUT);
        timeout--;
    }

    p_timestamp->tim_sec  = *p_atime_sec;
    p_timestamp->tim_nsec = *p_atime;

    p_timestamp->timestamp_id = 0;

    return FSP_SUCCESS;
}                                      /* End of ethsw_timer_get() */

/*******************************************************************************************************************//**
 * Sets the domain number in the specified timer.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsDomainSet (ethsw_ctrl_t * const p_ctrl, ethsw_ptp_domain_t * p_domain)
{
    uint32_t domain_mask;
    uint32_t domain;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_domain), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_domain->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    domain_mask = ETHSW_PTP_DOMAIN_MASK << (p_domain->tim_num * ETHSW_PTP_DOMAIN_SHIFT);

    domain  = p_switch_reg->PTP_DOMAIN;
    domain &= (~domain_mask);
    domain |= (uint32_t) (p_domain->domain_num << (p_domain->tim_num * ETHSW_PTP_DOMAIN_SHIFT));

    p_switch_reg->PTP_DOMAIN = domain;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsDomainSet() */

/*******************************************************************************************************************//**
 * Gets the domain number in the specified timer.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsDomainGet (ethsw_ctrl_t * const p_ctrl, ethsw_ptp_domain_t * p_domain)
{
    uint32_t domain_mask;
    uint32_t domain;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_domain), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_domain->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    domain_mask = ETHSW_PTP_DOMAIN_MASK << (p_domain->tim_num * ETHSW_PTP_DOMAIN_SHIFT);

    domain  = p_switch_reg->PTP_DOMAIN;
    domain &= domain_mask;

    p_domain->domain_num = (uint8_t) (domain >> (p_domain->tim_num * ETHSW_PTP_DOMAIN_SHIFT));

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsDomainGet() */

/*******************************************************************************************************************//**
 * Sets the peer delay value for the specified timer number of the specified port number.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsPeerDelaySet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_ptp_peerdelay_t * p_peerdelay)
{
    volatile uint32_t * p_peerdelay_reg;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT > port), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_peerdelay), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_peerdelay->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_peerdelay_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PEERDELAY_P0_T0 +
                                             (ETHSW_PORT_OFFSET_16BYTES * port) +
                                             (ETHSW_PORT_OFFSET_4BYTES * p_peerdelay->tim_num));

    *p_peerdelay_reg = p_peerdelay->peerdelay;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsPeerDelaySet() */

/*******************************************************************************************************************//**
 * Gets the peer delay value for the specified timer number of the specified port number.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsPeerDelayGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_ptp_peerdelay_t * p_peerdelay)
{
    volatile uint32_t * p_peerdelay_reg;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT > port), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_peerdelay), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_peerdelay->tim_num), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_peerdelay_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PEERDELAY_P0_T0 +
                                             (ETHSW_PORT_OFFSET_16BYTES * port) +
                                             (ETHSW_PORT_OFFSET_4BYTES * p_peerdelay->tim_num));

    p_peerdelay->peerdelay = *p_peerdelay_reg;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsPeerDelayGet() */

/*******************************************************************************************************************//**
 * Register a callback function that receives a send timestamp event.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TsCallbackSet (ethsw_ctrl_t * const    p_ctrl,
                                 void (                * p_ptp_callback)(
                                     ethsw_ptp_event_t   event,
                                     uint8_t             port,
                                     ethsw_timestamp_t * p_timestamp))
{
 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_ptp_callback), FSP_ERR_INVALID_POINTER);
 #endif

    gp_ethsw_ptp_callback = p_ptp_callback;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TsCallbackSet() */

/********************************************************************************************************************//**
 * Sets offset corection for the specified timer.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TsOffsetCorrectionSet (ethsw_ctrl_t * const p_ctrl, ethsw_ts_offset_correction_t * p_offset)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_offset), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_TIME_COUNT > p_offset->time_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    if (0 == p_offset->time_num)       // timer 0
    {
        p_reg_switch->ATIME_INC0_b.OFFS_CORR_INC = p_offset->offs_inc &
                                                   (R_ETHSW_ATIME_INC0_OFFS_CORR_INC_Msk >>
                                                    R_ETHSW_ATIME_INC0_OFFS_CORR_INC_Pos);

        p_reg_switch->ATIME_OFFS_CORR0 = p_offset->offs_corr;

        p_reg_switch->ATIME_OFFSET0 = p_offset->offset;
    }
    else                               // timer 1
    {
        p_reg_switch->ATIME_INC1_b.OFFS_CORR_INC = p_offset->offs_inc &
                                                   (R_ETHSW_ATIME_INC1_OFFS_CORR_INC_Msk >>
                                                    R_ETHSW_ATIME_INC1_OFFS_CORR_INC_Pos);

        p_reg_switch->ATIME_OFFS_CORR1 = p_offset->offs_corr;

        p_reg_switch->ATIME_OFFSET1 = p_offset->offset;
    }

    return err;
}

/********************************************************************************************************************//**
 * Sets clock rate corection for the specified timer.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TsRateCorrectionSet (ethsw_ctrl_t * const p_ctrl, ethsw_ts_rate_correction_t * p_rate)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_rate), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_TIME_COUNT > p_rate->time_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    if (0 == p_rate->time_num)         // timer 0
    {
        p_reg_switch->ATIME_INC0_b.CORR_INC = p_rate->corr_inc &
                                              (R_ETHSW_ATIME_INC0_CORR_INC_Msk >> R_ETHSW_ATIME_INC0_CORR_INC_Pos);

        p_reg_switch->ATIME_CORR0 = p_rate->corr;
    }
    else                               // timer 1
    {
        p_reg_switch->ATIME_INC1_b.CORR_INC = p_rate->corr_inc &
                                              (R_ETHSW_ATIME_INC1_CORR_INC_Msk >> R_ETHSW_ATIME_INC1_CORR_INC_Pos);

        p_reg_switch->ATIME_CORR1 = p_rate->corr;
    }

    return err;
}

/********************************************************************************************************************//**
 * Gets clock rate corection for the specified timer.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TsRateCorrectionGet (ethsw_ctrl_t * const p_ctrl, ethsw_ts_rate_correction_t * p_rate)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_rate), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_TIME_COUNT > p_rate->time_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    if (0 == p_rate->time_num)         // timer 0
    {
        p_rate->corr_inc = p_reg_switch->ATIME_INC0_b.CORR_INC;
        p_rate->corr     = p_reg_switch->ATIME_CORR0;
    }
    else                               // timer 1
    {
        p_rate->corr_inc = p_reg_switch->ATIME_INC1_b.CORR_INC;
        p_rate->corr     = p_reg_switch->ATIME_CORR1;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Sets enable/disable status of the TDMA scheduler.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaEnableSet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_enable_t * p_tdma_enable)
{
    uint32_t portmask;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_enable), FSP_ERR_INVALID_POINTER);

    ETHSW_ERROR_RETURN((ETHSW_TIME_COUNT > p_tdma_enable->tim_num), FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN((ETHSW_PORT_MASK & p_tdma_enable->portmask), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (true == p_tdma_enable->flg_enable)
    {
        /* enable operation */

        gp_ethsw_tdma_callback = p_tdma_enable->p_tdma_callback;

        p_switch_reg->TDMA_START  = p_tdma_enable->tdma_start;
        p_switch_reg->TDMA_MODULO = p_tdma_enable->tdma_modulo;
        p_switch_reg->TDMA_CYCLE  = p_tdma_enable->tdma_cycle;

        portmask = p_tdma_enable->portmask & (~ETHSW_PORT_MASK);
        if (portmask & ETHSW_PORT_HOST)
        {
            portmask &= ~ETHSW_PORT_HOST;
            portmask |= ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
        }

        p_switch_reg->TDMA_ENA_CTRL_b.PORT_ENA = portmask & R_ETHSW_TDMA_ENA_CTRL_PORT_ENA_Msk;

        p_switch_reg->TDMA_CONFIG_b.TIMER_SEL = p_tdma_enable->tim_num & ETHSW_1BIT_MASK;

        p_switch_reg->TDMA_IRQ_CONTROL_b.TCV_INT_EN = 1;
        p_switch_reg->INT_CONFIG_b.TDMA_INT         = 1;
        p_switch_reg->INT_CONFIG_b.IRQ_EN           = 1;
        p_switch_reg->TDMA_CONFIG_b.TDMA_ENA        = 1;
    }
    else
    {
        /* disable operation */
        p_switch_reg->TDMA_CONFIG_b.TDMA_ENA        = 0;
        p_switch_reg->TDMA_IRQ_CONTROL_b.TCV_INT_EN = 0;
        p_switch_reg->INT_CONFIG_b.TDMA_INT         = 0;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaEnableSet() */

/*******************************************************************************************************************//**
 * Gets enable/disable status of the TDMA scheduler.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaEnableGet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_enable_t * p_tdma_enable)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_enable), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (p_switch_reg->TDMA_CONFIG_b.TDMA_ENA == 1)
    {
        /* enable operation */
        p_tdma_enable->tdma_start  = p_switch_reg->TDMA_START;
        p_tdma_enable->tdma_modulo = p_switch_reg->TDMA_MODULO;
        p_tdma_enable->tdma_cycle  = p_switch_reg->TDMA_CYCLE;

        p_tdma_enable->portmask = p_switch_reg->TDMA_ENA_CTRL_b.PORT_ENA;
        if (p_tdma_enable->portmask & (~ETHSW_PORT_MASK & ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER)))
        {
            p_tdma_enable->portmask &= ~ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
            p_tdma_enable->portmask |= ETHSW_PORT_HOST;
        }

        p_tdma_enable->portmask |= ETHSW_PORT_MASK;

        p_tdma_enable->tim_num    = p_switch_reg->TDMA_CONFIG_b.TIMER_SEL_ACTIVE;
        p_tdma_enable->flg_enable = true;
    }
    else
    {
        /* disable operation */
        p_tdma_enable->tdma_start  = 0;
        p_tdma_enable->tdma_modulo = 0;
        p_tdma_enable->tdma_cycle  = 0;
        p_tdma_enable->portmask    = ETHSW_PORT_MASK;
        p_tdma_enable->tim_num     = 0;
        p_tdma_enable->flg_enable  = false;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaEnableGet() */

/*******************************************************************************************************************//**
 * Sets TCV sequence entry.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaTcvSeqEntrySet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_seq_entry_t * p_tdma_seq_entry)
{
    uint32_t tcv_seq = 0;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_seq_entry), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((((ETHSW_TDMA_TCV_SEQ_D_IDX_MASK >> ETHSW_TDMA_TCV_SEQ_D_IDX_SHIFT) >=
                         p_tdma_seq_entry->tcv_d_index) &&
                        ((ETHSW_TDMA_TCV_SEQ_S_ADDR_MASK >> ETHSW_TDMA_TCV_SEQ_S_ADDR_SHIFT) >=
                         p_tdma_seq_entry->tcv_s_addr)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (ETHSW_TDMA_TCV_START == (p_tdma_seq_entry->tcv_func & ETHSW_TDMA_TCV_START))
    {
        tcv_seq |= ETHSW_TDMA_TCV_SEQ_START;
    }

    if (ETHSW_TDMA_TCV_INT == (p_tdma_seq_entry->tcv_func & ETHSW_TDMA_TCV_INT))
    {
        tcv_seq |= ETHSW_TDMA_TCV_SEQ_INT;
    }

    tcv_seq |= (p_tdma_seq_entry->tcv_d_index << ETHSW_TDMA_TCV_SEQ_D_IDX_SHIFT) & ETHSW_TDMA_TCV_SEQ_D_IDX_MASK;

    tcv_seq |= (p_tdma_seq_entry->tcv_gpio << ETHSW_TDMA_TCV_SEQ_GPIO_SHIFT) & ETHSW_TDMA_TCV_SEQ_GPIO_MASK;

    p_switch_reg->TCV_SEQ_ADDR = (p_tdma_seq_entry->tcv_s_addr << ETHSW_TDMA_TCV_SEQ_S_ADDR_SHIFT) &
                                 ETHSW_TDMA_TCV_SEQ_S_ADDR_MASK;
    p_switch_reg->TCV_SEQ_CTRL = tcv_seq;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaTcvSeqEntrySet() */

/*******************************************************************************************************************//**
 * Gets TCV sequence entry.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaTcvSeqEntryGet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_seq_entry_t * p_tdma_seq_entry)
{
    uint32_t tcv_seq;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_seq_entry), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_TDMA_TCV_SEQ_S_ADDR_MASK >> ETHSW_TDMA_TCV_SEQ_S_ADDR_SHIFT) >=
                        p_tdma_seq_entry->tcv_s_addr),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->TCV_SEQ_ADDR = (p_tdma_seq_entry->tcv_s_addr << ETHSW_TDMA_TCV_SEQ_S_ADDR_SHIFT) &
                                 ETHSW_TDMA_TCV_SEQ_S_ADDR_MASK;
    p_switch_reg->TCV_SEQ_CTRL = ETHSW_TDMA_TCV_SEQ_READ_MODE;

    tcv_seq = p_switch_reg->TCV_SEQ_CTRL;

    p_tdma_seq_entry->tcv_func = ETHSW_TDMA_TCV_NON;

    if (tcv_seq & ETHSW_TDMA_TCV_SEQ_START)
    {
        p_tdma_seq_entry->tcv_func |= ETHSW_TDMA_TCV_START;
    }

    if (tcv_seq & ETHSW_TDMA_TCV_SEQ_INT)
    {
        p_tdma_seq_entry->tcv_func |= ETHSW_TDMA_TCV_INT;
    }

    p_tdma_seq_entry->tcv_d_index = (tcv_seq & ETHSW_TDMA_TCV_SEQ_D_IDX_MASK) >> ETHSW_TDMA_TCV_SEQ_D_IDX_SHIFT;

    p_tdma_seq_entry->tcv_gpio = (tcv_seq & ETHSW_TDMA_TCV_SEQ_GPIO_MASK) >> ETHSW_TDMA_TCV_SEQ_GPIO_SHIFT;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaTcvSeqEntryGet() */

/*******************************************************************************************************************//**
 * Sets TCV data entry.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaTcvDatEntrySet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_dat_entry_t * p_tdma_dat_entry)
{
    uint32_t pmask;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_dat_entry), FSP_ERR_INVALID_POINTER);

    ETHSW_ERROR_RETURN(((R_ETHSW_TCV_D_ADDR_ADDR_Msk >> R_ETHSW_TCV_D_ADDR_ADDR_Pos) >= p_tdma_dat_entry->tcv_d_addr),
                       FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN(
        ((R_ETHSW_TCV_D_CTRL_QGATE_Msk >> R_ETHSW_TCV_D_CTRL_QGATE_Pos) >= p_tdma_dat_entry->tcv_d_qgate),
        FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN((p_tdma_dat_entry->tcv_d_pmask & ETHSW_PORT_MASK), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->TCV_D_ADDR_b.ADDR = p_tdma_dat_entry->tcv_d_addr &
                                      (R_ETHSW_TCV_D_ADDR_ADDR_Msk >> R_ETHSW_TCV_D_ADDR_ADDR_Pos);
    p_switch_reg->TCV_D_OFFSET       = p_tdma_dat_entry->tcv_d_offset;
    p_switch_reg->TCV_D_CTRL_b.QGATE = p_tdma_dat_entry->tcv_d_qgate &
                                       (R_ETHSW_TCV_D_CTRL_QGATE_Msk >> R_ETHSW_TCV_D_CTRL_QGATE_Pos);

    pmask = p_tdma_dat_entry->tcv_d_pmask & (~ETHSW_PORT_MASK);
    if (pmask & ETHSW_PORT_HOST)
    {
        pmask &= ~ETHSW_PORT_HOST;
        pmask |= ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
    }

    p_switch_reg->TCV_D_CTRL_b.PMASK = pmask & (R_ETHSW_TCV_D_CTRL_PMASK_Msk >> R_ETHSW_TCV_D_CTRL_PMASK_Pos);

    p_switch_reg->TCV_D_CTRL_b.HOLD_REQ     = p_tdma_dat_entry->hold_req;
    p_switch_reg->TCV_D_CTRL_b.GATE_MODE    = p_tdma_dat_entry->gate_mode;
    p_switch_reg->TCV_D_CTRL_b.TRIGGER_MODE = p_tdma_dat_entry->trigger_mode;
    p_switch_reg->TCV_D_CTRL_b.IN_CT_ENA    = p_tdma_dat_entry->in_ct_ena;
    p_switch_reg->TCV_D_CTRL_b.OUT_CT_ENA   = p_tdma_dat_entry->out_ct_ena;
    p_switch_reg->TCV_D_CTRL_b.RED_PERIOD   = p_tdma_dat_entry->red_period;
    p_switch_reg->TCV_D_CTRL_b.INC_CTR1     = p_tdma_dat_entry->inc_crt1;
    p_switch_reg->TCV_D_CTRL_b.INC_CTR0     = p_tdma_dat_entry->inc_crt0;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaTcvDatEntrySet() */

/*******************************************************************************************************************//**
 * Gets TCV data entry.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaTcvDatEntryGet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_dat_entry_t * p_tdma_dat_entry)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_dat_entry), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((R_ETHSW_TCV_D_ADDR_ADDR_Msk >> R_ETHSW_TCV_D_ADDR_ADDR_Pos) >= p_tdma_dat_entry->tcv_d_addr),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->TCV_D_ADDR_b.ADDR = p_tdma_dat_entry->tcv_d_addr & ETHSW_9BITS_MASK;

    p_tdma_dat_entry->tcv_d_offset = p_switch_reg->TCV_D_OFFSET;
    p_tdma_dat_entry->tcv_d_qgate  = p_switch_reg->TCV_D_CTRL_b.QGATE;

    p_tdma_dat_entry->tcv_d_pmask = p_switch_reg->TCV_D_CTRL_b.PMASK;
    if (p_tdma_dat_entry->tcv_d_pmask & (~ETHSW_PORT_MASK & ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER)))
    {
        p_tdma_dat_entry->tcv_d_pmask &= ~ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
        p_tdma_dat_entry->tcv_d_pmask |= ETHSW_PORT_HOST;
    }

    p_tdma_dat_entry->tcv_d_pmask |= ETHSW_PORT_MASK;

    p_tdma_dat_entry->hold_req     = p_switch_reg->TCV_D_CTRL_b.HOLD_REQ;
    p_tdma_dat_entry->gate_mode    = p_switch_reg->TCV_D_CTRL_b.GATE_MODE;
    p_tdma_dat_entry->trigger_mode = p_switch_reg->TCV_D_CTRL_b.TRIGGER_MODE;
    p_tdma_dat_entry->in_ct_ena    = p_switch_reg->TCV_D_CTRL_b.IN_CT_ENA;
    p_tdma_dat_entry->out_ct_ena   = p_switch_reg->TCV_D_CTRL_b.OUT_CT_ENA;
    p_tdma_dat_entry->red_period   = p_switch_reg->TCV_D_CTRL_b.RED_PERIOD;
    p_tdma_dat_entry->inc_crt1     = p_switch_reg->TCV_D_CTRL_b.INC_CTR1;
    p_tdma_dat_entry->inc_crt0     = p_switch_reg->TCV_D_CTRL_b.INC_CTR0;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaTcvDatEntryGet() */

/*******************************************************************************************************************//**
 * Sets start and last of TCV sequence entry.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaTcvSeqRangeSet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_seq_range_t * p_tdma_seq_range)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_seq_range), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_TCV_S_ADDR_MASK >= p_tdma_seq_range->tcv_start) &&
                        (ETHSW_TCV_S_ADDR_MASK >= p_tdma_seq_range->tcv_last)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->TDMA_TCV_START      = p_tdma_seq_range->tcv_start;
    p_switch_reg->TCV_SEQ_LAST_b.LAST = p_tdma_seq_range->tcv_last & ETHSW_12BITS_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaTcvSeqRangeSet() */

/*******************************************************************************************************************//**
 * Gets start and last of TCV sequence entry.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaTcvSeqRangeGet (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_seq_range_t * p_tdma_seq_range)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_seq_range), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_tdma_seq_range->tcv_start = p_switch_reg->TDMA_TCV_START;
    p_tdma_seq_range->tcv_last  = p_switch_reg->TCV_SEQ_LAST_b.LAST;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaTcvSeqRangeGet() */

/*******************************************************************************************************************//**
 * Sets TDMA GPIO mode.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaGpioModeSet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_tdma_gpio_mode)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_gpio_mode), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->TDMA_GPIO_b.GPIO_MODE = *p_tdma_gpio_mode;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaGpioModeSet() */

/*******************************************************************************************************************//**
 * Gets TDMA GPIO mode.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaGpioModeGet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_tdma_gpio_mode)

{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_gpio_mode), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_tdma_gpio_mode = p_switch_reg->TDMA_GPIO_b.GPIO_MODE;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TdmaGpioModeGet() */

/********************************************************************************************************************//**
 * Sets TDMA counter 0
 * The Counter 0 is 32-bit counter that is incremented when the TCV field INC_CTR0 is set to 1.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaCounter0Set (ethsw_ctrl_t * const p_ctrl, uint32_t * p_tdma_counter0)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_counter0), FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_reg_switch->TDMA_CTR0 = *p_tdma_counter0;

    return err;
}

/********************************************************************************************************************//**
 * Gets TDMA counter 0
 * The Counter 0 is 32-bit counter that is incremented when the TCV field INC_CTR0 is set to 1.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaCounter0Get (ethsw_ctrl_t * const p_ctrl, uint32_t * p_tdma_counter0)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_counter0), FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    *p_tdma_counter0 = p_reg_switch->TDMA_CTR0;

    return err;
}

/********************************************************************************************************************//**
 * Sets TDMA counter 1
 * The counter 1 is an 8-bit counter that is incremented when the TCV field INC_CTR1 is set to 1.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaCounter1Set (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_counter1_t * p_tdma_counter1)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t                counter1;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_counter1), FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    counter1 = 0;

    if (true == p_tdma_counter1->write_value)
    {
        counter1  = (uint32_t) (p_tdma_counter1->value << R_ETHSW_TDMA_CTR1_VALUE_Pos) & R_ETHSW_TDMA_CTR1_VALUE_Msk;
        counter1 |= (uint32_t) (p_tdma_counter1->write_value << R_ETHSW_TDMA_CTR1_WRITE_ENA_Pos) &
                    R_ETHSW_TDMA_CTR1_WRITE_ENA_Msk;
    }

    counter1 |= (uint32_t) (p_tdma_counter1->max_value << R_ETHSW_TDMA_CTR1_MAX_Pos) & R_ETHSW_TDMA_CTR1_MAX_Msk;
    counter1 |= (uint32_t) (p_tdma_counter1->int_value << R_ETHSW_TDMA_CTR1_INT_VALUE_Pos) &
                R_ETHSW_TDMA_CTR1_INT_VALUE_Msk;

    p_reg_switch->TDMA_CTR1 = counter1;

    p_reg_switch->TDMA_IRQ_CONTROL_b.CTR1_INT_EN = p_tdma_counter1->int_enable;

    return err;
}

/********************************************************************************************************************//**
 * Gets TDMA counter 1
 * The counter 1 is an 8-bit counter that is incremented when the TCV field INC_CTR1 is set to 1.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_TdmaCounter1Get (ethsw_ctrl_t * const p_ctrl, ethsw_tdma_counter1_t * p_tdma_counter1)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_tdma_counter1), FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_tdma_counter1->value      = p_reg_switch->TDMA_CTR1_b.VALUE;
    p_tdma_counter1->max_value  = p_reg_switch->TDMA_CTR1_b.MAX;
    p_tdma_counter1->int_value  = p_reg_switch->TDMA_CTR1_b.INT_VALUE;
    p_tdma_counter1->int_enable = (bool) p_reg_switch->TDMA_IRQ_CONTROL_b.CTR1_INT_EN;

    return err;
}

/*******************************************************************************************************************//**
 * Sets queue gate.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlQgateSet (ethsw_ctrl_t * const p_ctrl, ethsw_mmclt_qgate_t * p_mmctl_qgate)
{
    uint32_t mmctl_qgate;
    uint32_t portmask;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_mmctl_qgate), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_PORT_MASK & p_mmctl_qgate->portmask), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    portmask = p_mmctl_qgate->portmask & (~ETHSW_PORT_MASK);
    if (portmask & ETHSW_PORT_HOST)
    {
        portmask &= ~ETHSW_PORT_HOST;
        portmask |= ETHSW_PORT_BIT(ETHSW_HOST_PORT_NUMBER);
    }

    mmctl_qgate = (uint32_t) (p_mmctl_qgate->qgate << ETHSW_MMCTL_QTRIG_QUEUE_TRIG_SHIFT) &
                  ETHSW_MMCTL_QTRIG_QUEUE_TRIG_MASK;

    mmctl_qgate |= (portmask << ETHSW_MMCTL_QTRIG_PORT_MASK_SHIFT) & ETHSW_MMCTL_QTRIG_PORT_MASK_MASK;

    p_switch_reg->MMCTL_QGATE = mmctl_qgate;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MmctlQgateSet() */

/********************************************************************************************************************//**
 * Sets memory pool size, in sells
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlPoolSizeSet (ethsw_ctrl_t * const p_ctrl, ethsw_mmctl_pool_size_t * p_pool_size)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_pool_size), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_MMCTL_POOL_ID_0 == p_pool_size->pool_id) ||
                        (ETHSW_MMCTL_POOL_ID_1 == p_pool_size->pool_id) ||
                        (ETHSW_MMCTL_POOL_ID_GLOBAL == p_pool_size->pool_id)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    switch (p_pool_size->pool_id)
    {
        case ETHSW_MMCTL_POOL_ID_0:
        {
            p_reg_switch->MMCTL_POOL0_CTR_b.CELLS = p_pool_size->cells & R_ETHSW_MMCTL_POOL0_CTR_CELLS_Msk;
            break;
        }

        case ETHSW_MMCTL_POOL_ID_1:
        {
            p_reg_switch->MMCTL_POOL1_CTR_b.CELLS = p_pool_size->cells & R_ETHSW_MMCTL_POOL1_CTR_CELLS_Msk;
            break;
        }

        case ETHSW_MMCTL_POOL_ID_GLOBAL:
        {
            p_reg_switch->MMCTL_POOL_GLOBAL_b.CELLS = p_pool_size->cells & R_ETHSW_MMCTL_POOL_GLOBAL_CELLS_Msk;
            break;
        }

        default:
        {
            break;
        }
    }

    return err;
}

/********************************************************************************************************************//**
 * Gets memory pool size, in sells
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlPoolSizeGet (ethsw_ctrl_t * const p_ctrl, ethsw_mmctl_pool_size_t * p_pool_size)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_pool_size), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_MMCTL_POOL_ID_0 == p_pool_size->pool_id) ||
                        (ETHSW_MMCTL_POOL_ID_1 == p_pool_size->pool_id) ||
                        (ETHSW_MMCTL_POOL_ID_GLOBAL == p_pool_size->pool_id)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    switch (p_pool_size->pool_id)
    {
        case ETHSW_MMCTL_POOL_ID_0:
        {
            p_pool_size->cells = p_reg_switch->MMCTL_POOL0_CTR_b.CELLS;
            break;
        }

        case ETHSW_MMCTL_POOL_ID_1:
        {
            p_pool_size->cells = p_reg_switch->MMCTL_POOL1_CTR_b.CELLS;
            break;
        }

        case ETHSW_MMCTL_POOL_ID_GLOBAL:
        {
            p_pool_size->cells = p_reg_switch->MMCTL_POOL_GLOBAL_b.CELLS;
            break;
        }

        default:
        {
            break;
        }
    }

    return err;
}

/********************************************************************************************************************//**
 * Assigns a pool to the specified queue.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlQueueAssignSet (ethsw_ctrl_t * const p_ctrl, ethsw_mmctl_queue_assign_t * p_queue_assign)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t                qmap;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_queue_assign), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_MMCTL_POOL_ID_0 == p_queue_assign->pool_id) ||
                        (ETHSW_MMCTL_POOL_ID_1 == p_queue_assign->pool_id) ||
                        (ETHSW_MMCTL_POOL_ID_GLOBAL == p_queue_assign->pool_id)),
                       FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN(ETHSW_QUEUE_COUNT > p_queue_assign->queue_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    qmap  = p_reg_switch->MMCTL_POOL_QMAP;
    qmap &=
        ~((R_ETHSW_MMCTL_POOL_QMAP_Q0_MAP_Msk | R_ETHSW_MMCTL_POOL_QMAP_Q0_ENA_Msk) << (p_queue_assign->queue_num * 4));

    if (ETHSW_MMCTL_POOL_ID_GLOBAL != p_queue_assign->pool_id)
    {
        qmap |= (1U << R_ETHSW_MMCTL_POOL_QMAP_Q0_ENA_Pos) << (p_queue_assign->queue_num * 4);
        qmap |=
            ((uint32_t) (p_queue_assign->pool_id << R_ETHSW_MMCTL_POOL_QMAP_Q0_MAP_Pos) &
             R_ETHSW_MMCTL_POOL_QMAP_Q0_MAP_Msk) << (p_queue_assign->queue_num * 4);
    }

    p_reg_switch->MMCTL_POOL_QMAP = qmap;

    return err;
}

/********************************************************************************************************************//**
 * Gets the pool assigned to the specified queue
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlQueueAssignGet (ethsw_ctrl_t * const p_ctrl, ethsw_mmctl_queue_assign_t * p_queue_assign)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t                qmap;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_queue_assign), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_QUEUE_COUNT > p_queue_assign->queue_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;
    qmap         = p_reg_switch->MMCTL_POOL_QMAP;

    if (qmap & (R_ETHSW_MMCTL_POOL_QMAP_Q0_ENA_Msk << (p_queue_assign->queue_num * 4)))
    {
        qmap = qmap >> (p_queue_assign->queue_num * 4);
        qmap = qmap >> R_ETHSW_MMCTL_POOL_QMAP_Q0_MAP_Pos;

        p_queue_assign->pool_id = (ethsw_mmctl_pool_id_t) (qmap & R_ETHSW_MMCTL_POOL_QMAP_Q0_MAP_Msk);
    }
    else
    {
        p_queue_assign->pool_id = ETHSW_MMCTL_POOL_ID_GLOBAL;
    }

    return err;
}

/********************************************************************************************************************//**
 * Sets length in bytes of the YELLOW period for specified port.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlYellowLengthSet (ethsw_ctrl_t * const    p_ctrl,
                                        uint32_t                port,
                                        ethsw_yellow_length_t * p_yellow_length)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_mmctl_yellow_byte_length;
    uint32_t                mmctl_yellow_byte_length;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_yellow_length), FSP_ERR_INVALID_POINTER);
 #endif

    mmctl_yellow_byte_length =
        (uint32_t) (p_yellow_length->enable << R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YLEN_EN_Pos) &
        R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YLEN_EN_Msk;
    mmctl_yellow_byte_length |=
        (uint32_t) (p_yellow_length->length << R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YELLOW_LEN_Pos) &
        R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YELLOW_LEN_Msk;

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_mmctl_yellow_byte_length = &p_reg_switch->MMCTL_YELLOW_BYTE_LENGTH_P0;

    p_mmctl_yellow_byte_length[port] = mmctl_yellow_byte_length;

    return err;
}

/********************************************************************************************************************//**
 * Gets length in bytes of the YELLOW period for specified port.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_MmctlYellowLengthGet (ethsw_ctrl_t * const    p_ctrl,
                                        uint32_t                port,
                                        ethsw_yellow_length_t * p_yellow_length)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_mmctl_yellow_byte_length;
    uint32_t                mmctl_yellow_byte_length;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_yellow_length), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_mmctl_yellow_byte_length = &p_reg_switch->MMCTL_YELLOW_BYTE_LENGTH_P0;

    mmctl_yellow_byte_length = p_mmctl_yellow_byte_length[port];

    p_yellow_length->length = (mmctl_yellow_byte_length & R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YELLOW_LEN_Msk) >>
                              R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YELLOW_LEN_Pos;
    p_yellow_length->enable = (mmctl_yellow_byte_length & R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YLEN_EN_Msk) >>
                              R_ETHSW_MMCTL_YELLOW_BYTE_LENGTH_P0_YLEN_EN_Pos;

    return err;
}

/********************************************************************************************************************//**
 * Sets Queue Flush Event.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_QueueFlushEventSet (ethsw_ctrl_t * const p_ctrl, ethsw_queue_flush_event_t * p_queue_flush_event)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t                mmctl_qflush;
    uint32_t                port_mask;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_queue_flush_event), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PORT_MASK & p_queue_flush_event->port_mask, FSP_ERR_INVALID_ARGUMENT);
 #endif

    /* remove portmask bit and translate host port if set */
    port_mask = p_queue_flush_event->port_mask & ETHSW_PORT_BITS;
    if (p_queue_flush_event->port_mask & ETHSW_PORT_HOST)
    {
        port_mask |= (1U << ETHSW_HOST_PORT_NUMBER);
    }

    mmctl_qflush = (port_mask << R_ETHSW_MMCTL_QFLUSH_PORT_MASK_Pos) & R_ETHSW_MMCTL_QFLUSH_PORT_MASK_Msk;

    mmctl_qflush |= (p_queue_flush_event->queue_mask << R_ETHSW_MMCTL_QFLUSH_QUEUE_MASK_Pos) &
                    R_ETHSW_MMCTL_QFLUSH_QUEUE_MASK_Msk;
    mmctl_qflush |= (uint32_t) (p_queue_flush_event->action << R_ETHSW_MMCTL_QFLUSH_ACTION_Pos) &
                    R_ETHSW_MMCTL_QFLUSH_ACTION_Msk;

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_reg_switch->MMCTL_QFLUSH = mmctl_qflush;

    return err;
}

/*******************************************************************************************************************//**
 * Sets the dedicated port that will received all frames that are mirrored as well as the type of mirrored frames.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MirrorSet (ethsw_ctrl_t * const p_ctrl, ethsw_mirror_conf_t * p_cnf_mirror)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_cnf_mirror), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Disable port mirroring for reconfig */
    p_switch_reg->MIRROR_CONTROL = 0;

    /* Configure specific mode */
    switch (p_cnf_mirror->mirrortype)
    {
        /* Match egress dest. address */
        case ETHSW_MIRR_MODE_EGRESS_DA:
        {
            p_switch_reg->MIRROR_CONTROL_b.EG_DA_MATCH = 1;
            ethsw_memcpy_volatile(&p_switch_reg->MIRROR_EDST_0, *p_cnf_mirror->macaddr, ETHSW_MAC_ADDR_LEN);
            break;
        }

        /* Match egress src. address */
        case ETHSW_MIRR_MODE_EGRESS_SA:
        {
            p_switch_reg->MIRROR_CONTROL_b.EG_SA_MATCH = 1;
            ethsw_memcpy_volatile(&p_switch_reg->MIRROR_ESRC_0, *p_cnf_mirror->macaddr, ETHSW_MAC_ADDR_LEN);
            break;
        }

        /* Match ingress dest. address */
        case ETHSW_MIRR_MODE_INGRESS_DA:
        {
            p_switch_reg->MIRROR_CONTROL_b.ING_DA_MATCH = 1;
            ethsw_memcpy_volatile(&p_switch_reg->MIRROR_IDST_0, *p_cnf_mirror->macaddr, ETHSW_MAC_ADDR_LEN);
            break;
        }

        /* Match ingress src. address */
        case ETHSW_MIRR_MODE_INGRESS_SA:
        {
            p_switch_reg->MIRROR_CONTROL_b.ING_SA_MATCH = 1;
            ethsw_memcpy_volatile(&p_switch_reg->MIRROR_ISRC_0, *p_cnf_mirror->macaddr, ETHSW_MAC_ADDR_LEN);
            break;
        }

        /* Match egress port */
        case ETHSW_MIRR_MODE_EGRESS_PORT:
        {
            p_switch_reg->MIRROR_CONTROL_b.EG_MAP_EN = 1;
            p_switch_reg->MIRROR_EG_MAP_b.EMAP       = (uint32_t) p_cnf_mirror->portmap & ETHSW_4BITS_MASK;
            break;
        }

        /* Match ingress port */
        case ETHSW_MIRR_MODE_INGRESS_PORT:
        {
            p_switch_reg->MIRROR_CONTROL_b.ING_MAP_EN = 1;
            p_switch_reg->MIRROR_ING_MAP_b.IMAP       = (uint32_t) p_cnf_mirror->portmap & ETHSW_4BITS_MASK;
            break;
        }

        default:
        {
            break;
        }
    }

    /* Set mirror port and enable mirroring */
    p_switch_reg->MIRROR_CONTROL_b.PORT      = p_cnf_mirror->port & ETHSW_2BITS_MASK;
    p_switch_reg->MIRROR_CONTROL_b.MIRROR_EN = 1;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MirrorSet() */

/*******************************************************************************************************************//**
 * Gets the current port mirroring configuration.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_MirrorGet (ethsw_ctrl_t * const p_ctrl, ethsw_mirror_conf_t * p_cnf_mirror)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_cnf_mirror), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_cnf_mirror->port = p_switch_reg->MIRROR_CONTROL_b.PORT;

    /* Mode dependent settings */

    if (p_switch_reg->MIRROR_CONTROL_b.EG_DA_MATCH)
    {
        /* Match egress dest. address */

        p_cnf_mirror->mirrortype = ETHSW_MIRR_MODE_EGRESS_DA;
        ethsw_memcpy_volatile(*p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_EDST_0, ETHSW_MAC_ADDR_LEN);
    }

    if (p_switch_reg->MIRROR_CONTROL_b.EG_SA_MATCH)
    {
        /* Match egress src. address */

        p_cnf_mirror->mirrortype = ETHSW_MIRR_MODE_EGRESS_SA;
        ethsw_memcpy_volatile(*p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_ESRC_0, ETHSW_MAC_ADDR_LEN);
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.ING_DA_MATCH)
    {
        /* Match ingress dest. address */

        p_cnf_mirror->mirrortype = ETHSW_MIRR_MODE_INGRESS_DA;
        ethsw_memcpy_volatile(*p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_IDST_0, ETHSW_MAC_ADDR_LEN);
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.ING_SA_MATCH)
    {
        /* Match ingress src. address */

        p_cnf_mirror->mirrortype = ETHSW_MIRR_MODE_INGRESS_SA;
        ethsw_memcpy_volatile(*p_cnf_mirror->macaddr, &p_switch_reg->MIRROR_ISRC_0, ETHSW_MAC_ADDR_LEN);
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.EG_MAP_EN)
    {
        /* Match egress port */

        p_cnf_mirror->mirrortype = ETHSW_MIRR_MODE_EGRESS_PORT;
        p_cnf_mirror->portmap    = (uint8_t) (p_switch_reg->MIRROR_EG_MAP_b.EMAP & (~ETHSW_PORT_MASK));
    }
    else if (p_switch_reg->MIRROR_CONTROL_b.ING_MAP_EN)
    {
        /* Match ingress port */

        p_cnf_mirror->mirrortype = ETHSW_MIRR_MODE_INGRESS_PORT;
        p_cnf_mirror->portmap    = (uint8_t) (p_switch_reg->MIRROR_ING_MAP_b.IMAP & (~ETHSW_PORT_MASK));
    }
    else
    {
        ;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_MirrorGet() */

/*******************************************************************************************************************//**
 * Sets the priority resolution mode to use for a specific port.
 * It also defines the default priority if no priority resolution produced a result.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosModeSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_qos_mode_t * p_mode_qos)
{
    volatile uint32_t * p_reg;         /* register value */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_qos), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PRIORITY_CFG0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_reg &= (uint32_t) ~((ETHSW_QOS_DEFPRIO_MASK << ETHSW_QOS_DEFPRIO_OFFSET) | ETHSW_QOS_MODE_MASK);
    *p_reg |= (uint32_t) (((p_mode_qos->defprio & ETHSW_QOS_DEFPRIO_MASK) << ETHSW_QOS_DEFPRIO_OFFSET) |
                          (p_mode_qos->mode & ETHSW_QOS_MODE_MASK));

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosModeSet() */

/*******************************************************************************************************************//**
 * Gets the priority resolution mode to use for a specific port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosModeGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_qos_mode_t * p_mode_qos)
{
    volatile uint32_t * p_reg;         /* register value */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_qos), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->PRIORITY_CFG0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port_num));

    /* Get mode and mask */
    p_mode_qos->mode    = *p_reg & ETHSW_QOS_MODE_MASK;
    p_mode_qos->defprio = (*p_reg >> ETHSW_QOS_DEFPRIO_OFFSET) & ETHSW_QOS_DEFPRIO_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosModeGet() */

/*******************************************************************************************************************//**
 * Sets the priority mapping between VLAN tag priorities and the switch core priorities.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosPrioValnSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_bm_prio_vlan)
{
    volatile uint32_t * p_reg;         /* register value */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_bm_prio_vlan), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->VLAN_PRIORITY0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_reg = *p_bm_prio_vlan & ETHSW_QOS_VLANPRIO_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosPrioValnSet() */

/*******************************************************************************************************************//**
 * Gets the priority mapping between VLAN tag priorities and the switch core priorities.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosPrioValnGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_bm_prio_vlan)
{
    volatile uint32_t * p_reg;         /* register value */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_bm_prio_vlan), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->VLAN_PRIORITY0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_bm_prio_vlan = *p_reg & ETHSW_QOS_VLANPRIO_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosPrioValnGet() */

/*******************************************************************************************************************//**
 * Sets the priority for the given DiffServ field of an IP packet for the given port.
 * Up to 64 different DiffServ entries may be defined per port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosPrioIpSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_qos_prio_ip_t * p_prio_qos_ip)
{
    volatile uint32_t * p_reg;         /* register value */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_prio_qos_ip), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IP_PRIORITY0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_reg = (ETHSW_QOS_IPPRIO_WRITE |
              ((uint32_t) p_prio_qos_ip->priority << ETHSW_QOS_IPPRIO_OFFSET) |
              ((uint32_t) p_prio_qos_ip->diffserv & ETHSW_QOS_IPPRIO_DSMASK));

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosPrioIpSet() */

/*******************************************************************************************************************//**
 * Gets the priority for the given DiffServ field of an IP packet for the given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosPrioIpGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_qos_prio_ip_t * p_prio_qos_ip)
{
    volatile uint32_t * p_reg;         /* register value */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_prio_qos_ip), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IP_PRIORITY0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port_num));

    *p_reg = ETHSW_QOS_IPPRIO_READ | (p_prio_qos_ip->diffserv & ETHSW_QOS_IPPRIO_DSMASK);

    p_prio_qos_ip->priority = (*p_reg >> ETHSW_QOS_IPPRIO_OFFSET) & ETHSW_QOS_IPPRIO_MASK;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosPrioIpGet() */

/*******************************************************************************************************************//**
 * Sets the priority for the given Ethertypes to the given values.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosPrioTypeSet (ethsw_ctrl_t * const p_ctrl, ethsw_qos_prio_type_t * p_prio_qos_ethtype)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_prio_qos_ethtype), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (!p_prio_qos_ethtype->type1)
    {
        p_switch_reg->PRIORITY_TYPE1_b.VALID    = 0;
        p_switch_reg->PRIORITY_TYPE1_b.PRIORITY = 0;
        p_switch_reg->PRIORITY_TYPE1_b.TYPEVAL  = 0;
    }
    else
    {
        p_switch_reg->PRIORITY_TYPE1_b.PRIORITY = (uint32_t) p_prio_qos_ethtype->prio1 & ETHSW_3BITS_MASK;
        p_switch_reg->PRIORITY_TYPE1_b.TYPEVAL  = p_prio_qos_ethtype->type1;
        p_switch_reg->PRIORITY_TYPE1_b.VALID    = 1;
    }

    if (!p_prio_qos_ethtype->type2)
    {
        p_switch_reg->PRIORITY_TYPE2_b.VALID    = 0;
        p_switch_reg->PRIORITY_TYPE2_b.PRIORITY = 0;
        p_switch_reg->PRIORITY_TYPE2_b.TYPEVAL  = 0;
    }
    else
    {
        p_switch_reg->PRIORITY_TYPE2_b.PRIORITY = (uint32_t) p_prio_qos_ethtype->prio2 & ETHSW_3BITS_MASK;
        p_switch_reg->PRIORITY_TYPE2_b.TYPEVAL  = p_prio_qos_ethtype->type2;
        p_switch_reg->PRIORITY_TYPE2_b.VALID    = 1;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosPrioTypeSet() */

/*******************************************************************************************************************//**
 * Gets the priority settings for type based priorities.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QosPrioTypeGet (ethsw_ctrl_t * const p_ctrl, ethsw_qos_prio_type_t * p_prio_qos_ethtype)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_prio_qos_ethtype), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_prio_qos_ethtype->type1 = (p_switch_reg->PRIORITY_TYPE1_b.VALID) ?
                                p_switch_reg->PRIORITY_TYPE1_b.TYPEVAL : 0;

    p_prio_qos_ethtype->prio1 = p_switch_reg->PRIORITY_TYPE1_b.PRIORITY;

    p_prio_qos_ethtype->type2 = (p_switch_reg->PRIORITY_TYPE2_b.VALID) ?
                                p_switch_reg->PRIORITY_TYPE2_b.TYPEVAL : 0;

    p_prio_qos_ethtype->prio2 = p_switch_reg->PRIORITY_TYPE2_b.PRIORITY;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QosPrioTypeGet() */

/*******************************************************************************************************************//**
 * Gets the number of output queues per port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_QueuesPerPortGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_out_queues)
{
 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;

    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_out_queues), FSP_ERR_INVALID_POINTER);
 #endif

    *p_out_queues = ETHSW_QOS_OUTQUEUES;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_QueuesPerPortGet() */

/*******************************************************************************************************************//**
 * Sets the timeframe for the broadcast/multicast rate limiting in ms.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_StormTimeSet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_time_storm)
{
    double val;                        /* value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_time_storm), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    val = round((((double) *p_time_storm) *
                 (ETHSW_HZ / ETHSW_DEC_1000)) / ETHSW_DOS_TIMEFACTOR);

    p_switch_reg->BCAST_STORM_LIMIT_b.TMOUT = (uint16_t) val;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_StormTimeSet() */

/*******************************************************************************************************************//**
 * Gets the timeframe for the broadcast/multicast rate limiting in ms.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_StormTimeGet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_time_storm)
{
    double val;                        /* value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_time_storm), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    val = round((((double) p_switch_reg->BCAST_STORM_LIMIT_b.TMOUT) *
                 ETHSW_DOS_TIMEFACTOR) / (ETHSW_HZ / ETHSW_DEC_1000));

    *p_time_storm = (uint16_t) val;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_StormTimeGet() */

/*******************************************************************************************************************//**
 * Sets the max. allowed broadcast frames that can be received in a given time.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_BcastLimitSet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_num_storm_frames)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_num_storm_frames), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->BCAST_STORM_LIMIT_b.BCASTLIMIT = *p_num_storm_frames;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_BcastLimitSet() */

/*******************************************************************************************************************//**
 * Gets the max. allowed broadcast frames that can be received in a given time.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_BcastLimitGet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_num_storm_frames)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_num_storm_frames), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_num_storm_frames = p_switch_reg->BCAST_STORM_LIMIT_b.BCASTLIMIT;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_BcastLimitGet() */

/*******************************************************************************************************************//**
 * Sets the max. allowed multicast frames that can be received in a given time.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_McastLimitSet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_num_storm_frames)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_num_storm_frames), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->MCAST_STORM_LIMIT_b.MCASTLIMIT = *p_num_storm_frames;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_McastLimitSet() */

/*******************************************************************************************************************//**
 * Gets the max. allowed multicast frames that can be received in a given time.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_McastLimitGet (ethsw_ctrl_t * const p_ctrl, uint16_t * p_num_storm_frames)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_num_storm_frames), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_num_storm_frames = p_switch_reg->MCAST_STORM_LIMIT_b.MCASTLIMIT;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_McastLimitGet() */

/*******************************************************************************************************************//**
 * Sets the max. allowed bandwidth for the given port in percent.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TxRateSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, float * p_rate)
{
    float               idleslope;     /* Idle Slope value */
    volatile uint32_t * p_reg;         /* register */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_rate), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((ETHSW_RATE_IS_MAX >= *p_rate), FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    /* calculate Traffic Shaper bandwidth control register addresse */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IDLE_SLOPE_P0 +
                                   (ETHSW_PORT_OFFSET * port_num));

    /* Calc idle slope and set it */
    idleslope = (ETHSW_DEC_512 * (*p_rate / ETHSW_DEC_100)) / (1 - (*p_rate / ETHSW_DEC_100));

    /* check if calculated slope rate exceeds maximum */
    if (ETHSW_RATE_IS_MASK < idleslope)
    {
        /* limit slope rate to maximum value */
        idleslope = ETHSW_RATE_IS_MASK;
    }

    /* write slope rate to port specific register (IDLE_SLOPE_P[n]) */
    *p_reg = (uint16_t) idleslope;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TxRateSet() */

/*******************************************************************************************************************//**
 * Gets the max. allowed bandwidth for the given port in percent.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_TxRateGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, float * p_rate)
{
    volatile uint32_t * p_reg;         /* register */
    uint32_t            port_num;      /* port */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((ETHSW_PORT_COUNT_ALL > (port & ~ETHSW_PORT_HOST)), FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_rate), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    port_num = (port & ETHSW_PORT_HOST) ? ETHSW_HOST_PORT_NUMBER : port;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->IDLE_SLOPE_P0 +
                                   (ETHSW_PORT_OFFSET * port_num));

    *p_rate = (1 / (1 + (ETHSW_DEC_512 / (float) (*p_reg & ETHSW_RATE_IS_MASK)))) * ETHSW_DEC_100;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_TxRateGet() */

/*******************************************************************************************************************//**
 * Enables/disables and configure the Energy Efficient Ethernet Mode of the switch.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_EeeSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_eee_t * p_cnf_eee)
{
    volatile uint32_t * p_reg;         /* Register */
    volatile uint32_t * pIdlereg;      /* Idle Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_cnf_eee), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_CTL_STAT_P0 +
                                   ETHSW_COUNTER_OFFSET * port);

    if (true == p_cnf_eee->state)
    {
        /* Disable EEE, set idle time and enable again */
        *p_reg &= ~(uint32_t) ETHSW_EEE_AUTO;

        pIdlereg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_IDLE_TIME_P0 +
                                          ETHSW_COUNTER_OFFSET * port);

        *pIdlereg = p_cnf_eee->idletime;

        *p_reg |= ETHSW_EEE_AUTO;
    }
    else
    {
        /* Disable EEE */
        *p_reg &= ~((uint32_t) ETHSW_EEE_AUTO);
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_EeeSet() */

/*******************************************************************************************************************//**
 * Gets the Energy Efficient Ethernet configuration of the switch.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_EeeGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, ethsw_eee_t * p_cnf_eee)
{
    volatile uint32_t * p_reg;         /* Register */
    volatile uint32_t * pIdlereg;      /* Idle Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_cnf_eee), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_CTL_STAT_P0 +
                                   (ETHSW_COUNTER_OFFSET * port));

    pIdlereg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->EEE_IDLE_TIME_P0 +
                                      (ETHSW_COUNTER_OFFSET * port));

    p_cnf_eee->state = (*p_reg & ETHSW_EEE_AUTO) ? true : false;

    p_cnf_eee->idletime = *pIdlereg;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_EeeGet() */

/*******************************************************************************************************************//**
 * Add a snooping configuration
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_OUT_OF_MEMORY       Not found paeser
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_SnoopAdd (ethsw_ctrl_t * const p_ctrl, ethsw_snoop_config_t * p_cnf_snoop)
{
    fsp_err_t res    = FSP_SUCCESS;    /* Result */
    uint32_t  id     = 0;              /* Parser id */
    bool      found  = false;          /* found flag */
    uint32_t  mode   = 0;              /* mode */
    uint16_t  offset = 0;              /* offset */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_cnf_snoop), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Check whether a block has the requested mode */
    if (block1Mode == p_cnf_snoop->action)
    {
        res = ethsw_snoop_free_gparser_get(p_switch_reg, ETHSW_GPARSER_PARSER_BLOCK_ID1, &id);
        if (FSP_SUCCESS == res)
        {
            found = true;
        }
    }

    if ((block2Mode == p_cnf_snoop->action) && (false == found))
    {
        res = ethsw_snoop_free_gparser_get(p_switch_reg, ETHSW_GPARSER_PARSER_BLOCK_ID2, &id);
    }

    if (FSP_SUCCESS == res)
    {
        /* Set params based on mode */
        switch (p_cnf_snoop->mode)
        {
            /* IP data mode */
            case ETHSW_SNOOP_MODE_IPDATA:
            {
                mode  |= ETHSW_GPARSER_MODE_IPDATA;
                offset = (uint16_t) p_cnf_snoop->offset;
                break;
            }

            /* IP proto mode */
            case ETHSW_SNOOP_MODE_IPPROT:
            {
                mode |= ETHSW_GPARSER_MODE_IPPROTO;
                break;
            }

            /* Ethertype mode */
            case ETHSW_SNOOP_MODE_ETYPE:
            {
                /* Ethertype consists of data mode, compare16 and skip vlan */
                mode |= ETHSW_GPARSER_MODE_COMPARE16;
                mode |= ETHSW_GPARSER_MODE_SKIPVLAN;
                break;
            }

            /* Free data mode */
            case ETHSW_SNOOP_MODE_DATA:
            {
                mode  |= p_cnf_snoop->flags;
                offset = (uint16_t) p_cnf_snoop->offset;
                break;
            }
        }

        p_cnf_snoop->id = id;
        res             = ethsw_gparser_set(p_switch_reg,
                                            (uint8_t) id,
                                            true,
                                            mode,
                                            p_cnf_snoop->match,
                                            p_cnf_snoop->mask,
                                            offset);
    }

    return res;
}                                      /* End of function R_ETHSW_SnoopAdd() */

/*******************************************************************************************************************//**
 * Removes a snooping configuration.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_SnoopRemove (ethsw_ctrl_t * const p_ctrl, uint32_t * p_id_cnf_snoop)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_id_cnf_snoop), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    return ethsw_gparser_set(p_switch_reg, (uint8_t) *p_id_cnf_snoop, false, 0, 0, 0, 0);
}                                      /* End of function R_ETHSW_SnoopRemove() */

/*******************************************************************************************************************//**
 * Sets authorization state for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortAuhSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_state_auth)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_auth), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_AUTH_AUTHORIZED == *p_state_auth) || (ETHSW_AUTH_UNAUTHORIZED == *p_state_auth)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port));

    /* set authentication */
    if (ETHSW_AUTH_AUTHORIZED == *p_state_auth)
    {
        *p_reg |= ETHWE_SWITCH_PORT_AUTHORIZED;
    }
    else
    {
        *p_reg &= ~(uint32_t) ETHWE_SWITCH_PORT_AUTHORIZED;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortAuhSet() */

/*******************************************************************************************************************//**
 * Gets authorization state for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortAuhGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_state_auth)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_auth), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port));

    *p_state_auth = (*p_reg & ETHWE_SWITCH_PORT_AUTHORIZED) ?
                    ETHSW_AUTH_AUTHORIZED : ETHSW_AUTH_UNAUTHORIZED;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortAuhGet() */

/*******************************************************************************************************************//**
 * Sets controlled direction for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortCtrlDirSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_state_dir_port_ctrl)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_dir_port_ctrl), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_CTRL_DIR_BOTH == *p_state_dir_port_ctrl) ||
                        (ETHSW_CTRL_DIR_INPUT == *p_state_dir_port_ctrl)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port));

    /* check given direction value */
    if (ETHSW_CTRL_DIR_BOTH == *p_state_dir_port_ctrl)
    {
        /* set port to controll input and output */
        *p_reg |= ETHWE_SWITCH_PORT_CONTROLLED_BOTH;
    }
    else
    {
        /* set port to control only input */
        *p_reg &= ~(uint32_t) ETHWE_SWITCH_PORT_CONTROLLED_BOTH;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortCtrlDirSet() */

/*******************************************************************************************************************//**
 * Gets controlled direction for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortCtrlDirGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_state_dir_port_ctrl)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_state_dir_port_ctrl), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port));

    /* get controlled direction from register */
    *p_state_dir_port_ctrl = (*p_reg & ETHWE_SWITCH_PORT_CONTROLLED_BOTH) ?
                             ETHSW_CTRL_DIR_BOTH : ETHSW_CTRL_DIR_INPUT;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortCtrlDirGet() */

/*******************************************************************************************************************//**
 * Sets EAPOL reception mode for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortEapolSet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_mode_eapol_recv)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_eapol_recv), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_EAPOL_ENABLE == *p_mode_eapol_recv) || (ETHSW_EAPOL_DISABLE == *p_mode_eapol_recv)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port));

    /* set EAPOL reception mode */
    /* check given reception mode value */
    if (ETHSW_EAPOL_ENABLE == *p_mode_eapol_recv)
    {
        /* enable reception of EAPOL frames */
        *p_reg |= ETHWE_SWITCH_PORT_EAPOL_ENABLE;
    }
    else
    {
        /* disable reception of EAPOL frames */
        *p_reg &= ~(uint32_t) ETHWE_SWITCH_PORT_EAPOL_ENABLE;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortEapolSet() */

/*******************************************************************************************************************//**
 * Gets EAPOL reception mode for given port.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_PortEapolGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_mode_eapol_recv)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);

    ETHSW_ERROR_RETURN((NULL != p_mode_eapol_recv), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to authentication register */
    p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->AUTH_PORT0 +
                                   (ETHSW_PORT_OFFSET_4BYTES * port));

    /* get EAPOL reception mode from register */
    *p_mode_eapol_recv = (*p_reg & ETHWE_SWITCH_PORT_EAPOL_ENABLE) ?
                         ETHSW_EAPOL_ENABLE : ETHSW_EAPOL_DISABLE;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_PortEapolGet() */

/*******************************************************************************************************************//**
 * Sest Bridge Protocol Frame (BPDU) forwarding mode.
 * Enabling the BPDU forwarding mode this function will also disable the unchecked dropping of BPDU frames.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_BpduSet (ethsw_ctrl_t * const p_ctrl, ethsw_bpdu_ctrl_t * p_state_bpdu_ctrl)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_bpdu_ctrl), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(((ETHSW_BPDU_CTRL_MGMT_FWD == *p_state_bpdu_ctrl) ||
                        (ETHSW_BPDU_CTRL_DISCARD == *p_state_bpdu_ctrl)),
                       FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to Management configuration register (0x44050020) */
    p_reg = &(p_switch_reg->MGMT_CONFIG);

    /* set BPDU forwarding mode */
    /* check given forwarding mode value */
    if (ETHSW_BPDU_CTRL_MGMT_FWD == *p_state_bpdu_ctrl)
    {
        /* Enable BPDU forwarding */
        *p_reg |= ETHSW_MGMT_BPDU_FWD_ENABLE;

        /* Disable discarding BPDU frames */
        *p_reg &= ~((uint32_t) (ETHSW_MGMT_BPDU_DISCARD | ETHSW_MGMT_BPDU_MGMT_DISCARD));
    }
    else
    {
        /* Disable BPDU forwarding */
        *p_reg &= ~(uint32_t) ETHSW_MGMT_BPDU_FWD_ENABLE;

        /* Enable discarding BPDU frames */
        *p_reg |= ETHSW_MGMT_BPDU_DISCARD;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_BpduSet() */

/*******************************************************************************************************************//**
 * Get Bridge Protocol Frame (BPDU) forwarding mode
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_BpduGet (ethsw_ctrl_t * const p_ctrl, ethsw_bpdu_ctrl_t * p_state_bpdu_ctrl)
{
    volatile uint32_t * p_reg;         /* Register */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_bpdu_ctrl), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* get pointer to Management configuration register (0x44050020) */
    p_reg = &(p_switch_reg->MGMT_CONFIG);

    /* Check if discarding and forwarding are enabled at the same time */
    if (((*p_reg & ETHSW_MGMT_BPDU_FWD_ENABLE) && (*p_reg & ETHSW_MGMT_BPDU_DISCARD)) ||
        (!(*p_reg & ETHSW_MGMT_BPDU_FWD_ENABLE) && !(*p_reg & ETHSW_MGMT_BPDU_DISCARD)))
    {
        *p_state_bpdu_ctrl = ETHSW_BPDU_CTRL_UNKNOWN;
    }
    else
    {
        /* get BPDU forwarding mode from register */
        *p_state_bpdu_ctrl = (*p_reg & ETHSW_MGMT_BPDU_FWD_ENABLE) ?
                             ETHSW_BPDU_CTRL_MGMT_FWD : ETHSW_BPDU_CTRL_DISCARD;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_BpduGet() */

/*******************************************************************************************************************//**
 * Initialize DLR module
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_TIMEOUT             Timeout error
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrInitSet (ethsw_ctrl_t * const p_ctrl, ethsw_dlr_init_t * p_dlr_init)
{
    ethsw_mactab_entry_t mac_entry = {0}; // MAC table entry
    fsp_err_t            res;             // result
    uint8_t              cnt;             // loop counter

    /* Multicast MAC for Beacon Frames */
    uint8_t dlr_mac[ETHSW_MAC_ADDR_LEN] = ETHSW_MULTICAST_BEACON;

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_dlr_init), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    /* Forced Forwarding via Static MAC Table entries */
    mac_entry.addr = (ethsw_mac_addr_t *) &dlr_mac;

    /* --> Beacon Frames (01:21:6c:00:00:01) ports 0, 1 */
    dlr_mac[5]         = 0x01;
    mac_entry.portmask = (1U << ETHSW_PORT(0)) | (1U << ETHSW_PORT(1)) |
                         ETHSW_PORT_MASK;
    res = R_ETHSW_MacTableSet(p_ctrl, &mac_entry);
    if (FSP_SUCCESS != res)
    {
        return res;
    }

    /* --> Neighbor_Check/SignOn Frames (01:21:6c:00:00:02) port 3 */
    dlr_mac[5]         = 0x02;
    mac_entry.portmask = ETHSW_PORT_HOST | ETHSW_PORT_MASK;

    res = R_ETHSW_MacTableSet(p_ctrl, &mac_entry);
    if (FSP_SUCCESS != res)
    {
        return res;
    }

    /* --> other Frames (01:21:6c:00:00:{03..05}) ports 0, 1, 3 */
    mac_entry.portmask = (1U << ETHSW_PORT(0)) | (1U << ETHSW_PORT(1)) |
                         ETHSW_PORT_HOST | ETHSW_PORT_MASK;
    for (cnt = 3; cnt < 6; cnt++)
    {
        dlr_mac[5] = cnt;
        res        = R_ETHSW_MacTableSet(p_ctrl, &mac_entry);
        if (FSP_SUCCESS != res)
        {
            return res;
        }
    }

    /* write local MAC for Loop Filter */
    p_switch_reg->DLR_LOC_MAClo = ((((uint32_t *) p_dlr_init->p_host_addr)[0] << 0) |
                                   (((uint32_t *) p_dlr_init->p_host_addr)[1] << 8) |
                                   (((uint32_t *) p_dlr_init->p_host_addr)[2] << 16) |
                                   (((uint32_t *) p_dlr_init->p_host_addr)[3] << 24));

    p_switch_reg->DLR_LOC_MAChi = ((((uint32_t *) p_dlr_init->p_host_addr)[4] << 0) |
                                   (((uint32_t *) p_dlr_init->p_host_addr)[5] << 8));

    /* callback function pointer  */
    gp_ethsw_dlr_callback = p_dlr_init->p_dlr_callback;

    /* set DLR EtherType */
    p_switch_reg->DLR_ETH_TYP_b.DLR_ETH_TYP = ETHSW_RN_ADVSW_DLR_ETHTYPE;

    /* set number of cycles for 1 us */
    /* 200 MHz -> 200 Ticks = 1 us */
    p_switch_reg->DLR_CONTROL_b.US_TIME = ETHSW_RN_ADVSW_DLR_TICKS;

    /* discard Bacon frames with invalid Timeout values  */
#if 0
    p_switch_reg->DLR_CONTROL_b.IGNORE_INVTM = 1;
#else
    p_switch_reg->DLR_CONTROL_b.IGNORE_INVTM = 0; //Receive Beacon frames with invalid Timeout values
#endif

    /* interrupts */
    p_switch_reg->DLR_IRQ_STAT_ACK = ETHSW_HEX_0000FFFF;

    p_switch_reg->INT_CONFIG_b.IRQ_EN  = 1;
    p_switch_reg->INT_CONFIG_b.DLR_INT = 1;

    p_switch_reg->DLR_IRQ_CONTROL = ETHSW_TGT_DLR_INT_BEACONTMO_0 |
                                    ETHSW_TGT_DLR_INT_BEACONTMO_1 |
                                    ETHSW_TGT_DLR_INT_SUVICHNG |
                                    ETHSW_TGT_DLR_INT_LNKCHNG_0 |
                                    ETHSW_TGT_DLR_INT_LNKCHNG_1 |
                                    ETHSW_TGT_DLR_INT_NEWSTATUS |
                                    ETHSW_TGT_DLR_INT_LOCALLOOP_0 |
                                    ETHSW_TGT_DLR_INT_LOCALLOOP_1;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrInit() */

/*******************************************************************************************************************//**
 * Enable DLR module
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrEnableSet (ethsw_ctrl_t * const p_ctrl)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->DLR_CONTROL_b.ENABLE          = 1;
    p_switch_reg->DLR_CONTROL_b.LOOP_FILTER_ENA = 1;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrEnable() */

/*******************************************************************************************************************//**
 * Shutdown DLR module.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrShutdownSet (ethsw_ctrl_t * const p_ctrl)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    p_switch_reg->DLR_CONTROL_b.ENABLE          = 0;
    p_switch_reg->DLR_CONTROL_b.LOOP_FILTER_ENA = 0;

    p_switch_reg->INT_CONFIG_b.DLR_INT = 0;
    p_switch_reg->DLR_IRQ_CONTROL      = 0x00000000;
    p_switch_reg->DLR_IRQ_STAT_ACK     = ETHSW_HEX_0000FFFF;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrShutdown() */

/*******************************************************************************************************************//**
 * Gets DLR last beacon status.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrLastBeaconGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_state_dlr)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_dlr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    if (((ETHSW_PORT(0) == port) && (0 != (p_switch_reg->DLR_STATUS_b.LastBcnRcvPort & 0x01))) ||
        ((ETHSW_PORT(1) == port) && (0 != (p_switch_reg->DLR_STATUS_b.LastBcnRcvPort & 0x02))))
    {
        *p_state_dlr = 1;
    }
    else
    {
        *p_state_dlr = 0;
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrLastBeacon() */

/*******************************************************************************************************************//**
 * Gets DLR node status.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrNodeStateGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_state_dlr)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_dlr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_state_dlr = p_switch_reg->DLR_STATUS_b.NODE_STATE;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrNodeState() */

/*******************************************************************************************************************//**
 * Gets DLR ring status.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrRingStateGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_state_dlr)
{
    return R_ETHSW_DlrNodeStateGet(p_ctrl, p_state_dlr);
}                                      /* End of function R_ETHSW_DlrRingState() */

/*******************************************************************************************************************//**
 * Gets IP address of DLR supervisor.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrSvIpGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_state_dlr)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_dlr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_state_dlr = p_switch_reg->DLR_SUPR_IPADR;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrSvIp() */

/*******************************************************************************************************************//**
 * Gets preference of DLR supervisor.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrSvPrecGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_state_dlr)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_dlr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_state_dlr = p_switch_reg->DLR_SUPR_MAChi_b.PRECE;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrSvPrec() */

/*******************************************************************************************************************//**
 * Gets VLAN ID of DLR beacon frame.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrVlanGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_state_dlr)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_state_dlr), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    *p_state_dlr = p_switch_reg->DLR_STATE_VLAN_b.VLANINFO;

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrVlan() */

/*******************************************************************************************************************//**
 * Gets MAC address of DLR beacon frame.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrSvMacGet (ethsw_ctrl_t * const p_ctrl, ethsw_mac_addr_t * pp_addr_mac)
{
    uint32_t regVal;                   /* register value */

    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    R_ETHSW_Type volatile * p_switch_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != pp_addr_mac), FSP_ERR_INVALID_POINTER);
 #endif

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    regVal            = p_switch_reg->DLR_SUPR_MAClo;
    (*pp_addr_mac)[0] = (uint8_t) (regVal & ETHSW_HEX_000000FF) >> 0;
    (*pp_addr_mac)[1] = (uint8_t) ((regVal & ETHSW_HEX_0000FF00) >> 8);
    (*pp_addr_mac)[2] = (uint8_t) ((regVal & ETHSW_HEX_00FF0000) >> 16);
    (*pp_addr_mac)[3] = (uint8_t) ((regVal & ETHSW_HEX_FF000000) >> 24);

    regVal            = p_switch_reg->DLR_SUPR_MAChi;
    (*pp_addr_mac)[4] = (uint8_t) (regVal & ETHSW_HEX_000000FF);
    (*pp_addr_mac)[5] = (uint8_t) ((regVal & ETHSW_HEX_0000FF00) >> 8);

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrSvMac() */

/*******************************************************************************************************************//**
 * Delete the static MAC address entry used by DLR.
 *
 * @retval  FSP_SUCCESS                 Command successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval  FSP_ERR_TIMEOUT             Timeout error
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_DlrMacEntryDel (ethsw_ctrl_t * const p_ctrl)
{
    uint8_t              dlr_mac[ETHSW_MAC_ADDR_LEN] = ETHSW_MULTICAST_DLR; /* Multicast MAC for DLR */
    ethsw_mactab_entry_t mac_entry = {0};                                   /* Mac table entry */
    fsp_err_t            res;                                               /* result */
    uint32_t             cnt;                                               /* count */

    /* initialize MAC Table entries */
    mac_entry.addr = (ethsw_mac_addr_t *) &dlr_mac;

    for (cnt = 1; cnt < 6; cnt++)
    {
        /* set DLR Multicast MAC address */
        dlr_mac[5] = (uint8_t) cnt;

        /* delete address entry from MAC Table */
        mac_entry.portmask = ETHSW_PORT_MASK;

        res = R_ETHSW_MacTableSet(p_ctrl, &mac_entry);
        if (FSP_SUCCESS != res)
        {
            return res;
        }
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHSW_DlrForward() */

/********************************************************************************************************************//**
 * Sets Rx pattern matcher
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_RxPatternMatcherSet (ethsw_ctrl_t * const p_ctrl, ethsw_rx_pattern_matcher_t * p_rx_pattern_matcher)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;

    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_rxmatch_config;
    uint32_t volatile     * p_pattern_ctrl;

    uint32_t pattern_mask;
    uint32_t pattern_ctrl;
    uint32_t port_num;

    uint32_t pattern_port_mask;
    uint32_t port_mask;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_rx_pattern_matcher), FSP_ERR_INVALID_POINTER);

    ETHSW_ERROR_RETURN(ETHSW_RX_PATTERN_MATCHER_COUNT > p_rx_pattern_matcher->pattern_sel, FSP_ERR_INVALID_ARGUMENT)
    ETHSW_ERROR_RETURN(ETHSW_PORT_MASK & p_rx_pattern_matcher->pattern_port_mask, FSP_ERR_INVALID_ARGUMENT);
    ETHSW_ERROR_RETURN((NULL != p_rx_pattern_matcher->p_pattern_ctrl), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((NULL != p_rx_pattern_matcher->p_pattern_data), FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;
    pattern_mask = 1U << p_rx_pattern_matcher->pattern_sel;

    // Disable Rx Patter Mattcher
    p_reg_switch->RXMATCH_CONFIG0_b.PATTERN_EN &= ~pattern_mask & R_ETHSW_RXMATCH_CONFIG0_PATTERN_EN_Msk;
    p_reg_switch->RXMATCH_CONFIG1_b.PATTERN_EN &= ~pattern_mask & R_ETHSW_RXMATCH_CONFIG1_PATTERN_EN_Msk;
    p_reg_switch->RXMATCH_CONFIG2_b.PATTERN_EN &= ~pattern_mask & R_ETHSW_RXMATCH_CONFIG2_PATTERN_EN_Msk;
    p_reg_switch->RXMATCH_CONFIG3_b.PATTERN_EN &= ~pattern_mask & R_ETHSW_RXMATCH_CONFIG3_PATTERN_EN_Msk;

    /* remove portmask bit and translate host port if set */
    pattern_port_mask = p_rx_pattern_matcher->pattern_port_mask & ETHSW_PORT_BITS;
    if (p_rx_pattern_matcher->pattern_port_mask & ETHSW_PORT_HOST)
    {
        pattern_port_mask |= (1U << ETHSW_HOST_PORT_NUMBER);
    }

    if (0 != pattern_port_mask)        // Enable
    {
        // Set Rx patter mattcher control
        pattern_ctrl  = 0;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->match_not << R_ETHSW_PATTERN_CTRL0_MATCH_NOT_Pos) &
            R_ETHSW_PATTERN_CTRL0_MATCH_NOT_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->mgmt_fwd << R_ETHSW_PATTERN_CTRL0_MGMTFWD_Pos) &
            R_ETHSW_PATTERN_CTRL0_MGMTFWD_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->discard << R_ETHSW_PATTERN_CTRL0_DISCARD_Pos) &
            R_ETHSW_PATTERN_CTRL0_DISCARD_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->set_prio << R_ETHSW_PATTERN_CTRL0_SET_PRIO_Pos) &
            R_ETHSW_PATTERN_CTRL0_SET_PRIO_Msk;
        pattern_ctrl |= (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->mode << R_ETHSW_PATTERN_CTRL0_MODE_Pos) &
                        R_ETHSW_PATTERN_CTRL0_MODE_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->timer_sel_ovr <<
                        R_ETHSW_PATTERN_CTRL0_TIMER_SEL_OVR_Pos) &
            R_ETHSW_PATTERN_CTRL0_TIMER_SEL_OVR_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->force_forward <<
                        R_ETHSW_PATTERN_CTRL0_FORCE_FORWARD_Pos) &
            R_ETHSW_PATTERN_CTRL0_FORCE_FORWARD_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->hub_trigger << R_ETHSW_PATTERN_CTRL0_HUBTRIGGER_Pos) &
            R_ETHSW_PATTERN_CTRL0_HUBTRIGGER_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->match_red << R_ETHSW_PATTERN_CTRL0_MATCH_RED_Pos) &
            R_ETHSW_PATTERN_CTRL0_MATCH_RED_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->match_not_red <<
                        R_ETHSW_PATTERN_CTRL0_MATCH_NOT_RED_Pos) &
            R_ETHSW_PATTERN_CTRL0_MATCH_NOT_RED_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->vlan_skip << R_ETHSW_PATTERN_CTRL0_VLAN_SKIP_Pos) &
            R_ETHSW_PATTERN_CTRL0_VLAN_SKIP_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->priority << R_ETHSW_PATTERN_CTRL0_PRIORITY_Pos) &
            R_ETHSW_PATTERN_CTRL0_PRIORITY_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->learning_dis << R_ETHSW_PATTERN_CTRL0_LEARNING_DIS_Pos) &
            R_ETHSW_PATTERN_CTRL0_LEARNING_DIS_Msk;

        port_mask = p_rx_pattern_matcher->p_pattern_ctrl->port_mask & ETHSW_PORT_BITS;
        if (p_rx_pattern_matcher->p_pattern_ctrl->port_mask & ETHSW_PORT_HOST)
        {
            port_mask |= (1U << ETHSW_HOST_PORT_NUMBER);
        }

        pattern_ctrl |= (port_mask << R_ETHSW_PATTERN_CTRL0_PORTMASK_Pos) & R_ETHSW_PATTERN_CTRL0_PORTMASK_Msk;

        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->imc_trigger << R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_Pos) &
            R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->imc_trigger_dely <<
                        R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_DLY_Pos) & R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_DLY_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->swap_bytes << R_ETHSW_PATTERN_CTRL0_SWAP_BYTES_Pos) &
            R_ETHSW_PATTERN_CTRL0_SWAP_BYTES_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->match_lt << R_ETHSW_PATTERN_CTRL0_MATCH_LT_Pos) &
            R_ETHSW_PATTERN_CTRL0_MATCH_LT_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->timer_sel << R_ETHSW_PATTERN_CTRL0_TIMER_SEL_Pos) &
            R_ETHSW_PATTERN_CTRL0_TIMER_SEL_Msk;
        pattern_ctrl |=
            (uint32_t) (p_rx_pattern_matcher->p_pattern_ctrl->queue_sel << R_ETHSW_PATTERN_CTRL0_QUEUESEL_Pos) &
            R_ETHSW_PATTERN_CTRL0_QUEUESEL_Msk;

        p_pattern_ctrl = &p_reg_switch->PATTERN_CTRL0;

        p_pattern_ctrl[p_rx_pattern_matcher->pattern_sel] = pattern_ctrl;

        // Set Rx patter mattcher data
        p_reg_switch->PATTERN_SEL  = p_rx_pattern_matcher->pattern_sel;
        p_reg_switch->PTRN_CMP_30  = p_rx_pattern_matcher->p_pattern_data->reg.cmp_30;
        p_reg_switch->PTRN_CMP_74  = p_rx_pattern_matcher->p_pattern_data->reg.cmp_74;
        p_reg_switch->PTRN_CMP_118 = p_rx_pattern_matcher->p_pattern_data->reg.cmp_118;
        p_reg_switch->PTRN_MSK_30  = p_rx_pattern_matcher->p_pattern_data->reg.msk_30;
        p_reg_switch->PTRN_MSK_74  = p_rx_pattern_matcher->p_pattern_data->reg.msk_74;
        p_reg_switch->PTRN_MSK_118 = p_rx_pattern_matcher->p_pattern_data->reg.msk_118;

        if (true == p_rx_pattern_matcher->pattern_int) // IRQ enable
        {
            p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT =
                (p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT | pattern_mask) &
                R_ETHSW_PATTERN_IRQ_CONTROL_MATCHINT_Msk;
            p_reg_switch->PATTERN_IRQ_CONTROL_b.ERROR_INT = 0xF;
            p_reg_switch->INT_CONFIG_b.PATTERN_INT        = 1;
        }
        else                           // IRQ disabe
        {
            p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT &= ~pattern_mask & R_ETHSW_PATTERN_IRQ_CONTROL_MATCHINT_Msk;
            if (0 == p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT)
            {
                p_reg_switch->PATTERN_IRQ_CONTROL_b.ERROR_INT = 0;
                p_reg_switch->INT_CONFIG_b.PATTERN_INT        = 0;
            }
        }

        // Enable Rx Patter Mattcher
        p_rxmatch_config = &p_reg_switch->RXMATCH_CONFIG0;
        for (port_num = 0; port_num <= ETHSW_HOST_PORT_NUMBER; port_num++)
        {
            if (pattern_port_mask & (1U << port_num))
            {
                p_rxmatch_config[port_num] |= pattern_mask;
            }
        }
    }
    else                               // Disable
    {
        p_pattern_ctrl = &p_reg_switch->PATTERN_CTRL0;
        p_pattern_ctrl[p_rx_pattern_matcher->pattern_sel] = 0;

        p_reg_switch->PATTERN_SEL  = p_rx_pattern_matcher->pattern_sel;
        p_reg_switch->PTRN_CMP_30  = 0;
        p_reg_switch->PTRN_CMP_74  = 0;
        p_reg_switch->PTRN_CMP_118 = 0;
        p_reg_switch->PTRN_MSK_30  = 0;
        p_reg_switch->PTRN_MSK_74  = 0;
        p_reg_switch->PTRN_MSK_118 = 0;

        // IRQ disabe
        p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT &= ~pattern_mask & R_ETHSW_PATTERN_IRQ_CONTROL_MATCHINT_Msk;
        if (0 == p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT)
        {
            p_reg_switch->PATTERN_IRQ_CONTROL_b.ERROR_INT = 0;
            p_reg_switch->INT_CONFIG_b.PATTERN_INT        = 0;
        }
    }

    return err;
}

/********************************************************************************************************************//**
 * Gets Rx pattern matcher
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_RxPatternMatcherGet (ethsw_ctrl_t * const p_ctrl, ethsw_rx_pattern_matcher_t * p_rx_pattern_matcher)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;

    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_pattern_ctrl;
    uint32_t volatile     * p_rxmatch_config;

    uint32_t pattern_mask;
    uint32_t pattern_ctrl;
    uint32_t port_num;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_rx_pattern_matcher), FSP_ERR_INVALID_POINTER);

    ETHSW_ERROR_RETURN(ETHSW_RX_PATTERN_MATCHER_COUNT > p_rx_pattern_matcher->pattern_sel, FSP_ERR_INVALID_ARGUMENT)
    ETHSW_ERROR_RETURN((NULL != p_rx_pattern_matcher->p_pattern_ctrl), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN((NULL != p_rx_pattern_matcher->p_pattern_data),
                       FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;
    pattern_mask = 1U << p_rx_pattern_matcher->pattern_sel;

    // Set Rx patter mattcher control
    p_pattern_ctrl = &p_reg_switch->PATTERN_CTRL0;
    pattern_ctrl   = p_pattern_ctrl[p_rx_pattern_matcher->pattern_sel];

    p_rx_pattern_matcher->p_pattern_ctrl->match_not =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_MATCH_NOT_Msk) >> R_ETHSW_PATTERN_CTRL0_MATCH_NOT_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->mgmt_fwd =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_MGMTFWD_Msk) >> R_ETHSW_PATTERN_CTRL0_MGMTFWD_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->discard =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_DISCARD_Msk) >> R_ETHSW_PATTERN_CTRL0_DISCARD_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->set_prio =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_SET_PRIO_Msk) >> R_ETHSW_PATTERN_CTRL0_SET_PRIO_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->mode =
        (ethsw_rx_pattern_mode_t) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_MODE_Msk) >> R_ETHSW_PATTERN_CTRL0_MODE_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->timer_sel_ovr =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_TIMER_SEL_OVR_Msk) >> R_ETHSW_PATTERN_CTRL0_TIMER_SEL_OVR_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->force_forward =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_FORCE_FORWARD_Msk) >> R_ETHSW_PATTERN_CTRL0_FORCE_FORWARD_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->hub_trigger =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_HUBTRIGGER_Msk) >> R_ETHSW_PATTERN_CTRL0_HUBTRIGGER_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->match_red =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_MATCH_RED_Msk) >> R_ETHSW_PATTERN_CTRL0_MATCH_RED_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->match_not_red =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_MATCH_NOT_RED_Msk) >> R_ETHSW_PATTERN_CTRL0_MATCH_NOT_RED_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->vlan_skip =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_VLAN_SKIP_Msk) >> R_ETHSW_PATTERN_CTRL0_VLAN_SKIP_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->priority =
        (uint8_t) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_PRIORITY_Msk) >> R_ETHSW_PATTERN_CTRL0_PRIORITY_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->learning_dis =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_LEARNING_DIS_Msk) >> R_ETHSW_PATTERN_CTRL0_LEARNING_DIS_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->port_mask =
        (uint8_t) (pattern_ctrl & R_ETHSW_PATTERN_CTRL0_PORTMASK_Msk) >> R_ETHSW_PATTERN_CTRL0_PORTMASK_Pos;
    p_rx_pattern_matcher->p_pattern_ctrl->imc_trigger =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_Msk) >> R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->imc_trigger_dely =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_DLY_Msk) >>
                R_ETHSW_PATTERN_CTRL0_IMC_TRIGGER_DLY_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->swap_bytes =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_SWAP_BYTES_Msk) >> R_ETHSW_PATTERN_CTRL0_SWAP_BYTES_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->match_lt =
        (bool) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_MATCH_LT_Msk) >> R_ETHSW_PATTERN_CTRL0_MATCH_LT_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->timer_sel =
        (uint8_t) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_TIMER_SEL_Msk) >> R_ETHSW_PATTERN_CTRL0_TIMER_SEL_Pos);
    p_rx_pattern_matcher->p_pattern_ctrl->queue_sel =
        (uint8_t) ((pattern_ctrl & R_ETHSW_PATTERN_CTRL0_QUEUESEL_Msk) >> R_ETHSW_PATTERN_CTRL0_QUEUESEL_Pos);

    /* if host port is set convert to host port */
    if (p_rx_pattern_matcher->p_pattern_ctrl->port_mask & (1U << ETHSW_HOST_PORT_NUMBER))
    {
        p_rx_pattern_matcher->p_pattern_ctrl->port_mask &= ~(1U << ETHSW_HOST_PORT_NUMBER);
        p_rx_pattern_matcher->p_pattern_ctrl->port_mask |= ETHSW_PORT_HOST;
    }

    /* set port mask flag */
    p_rx_pattern_matcher->p_pattern_ctrl->port_mask |= ETHSW_PORT_MASK;

    // Set Rx patter mattcher data
    p_reg_switch->PATTERN_SEL = p_rx_pattern_matcher->pattern_sel;

    p_rx_pattern_matcher->p_pattern_data->reg.cmp_30  = p_reg_switch->PTRN_CMP_30;
    p_rx_pattern_matcher->p_pattern_data->reg.cmp_74  = p_reg_switch->PTRN_CMP_74;
    p_rx_pattern_matcher->p_pattern_data->reg.cmp_118 = p_reg_switch->PTRN_CMP_118;
    p_rx_pattern_matcher->p_pattern_data->reg.msk_30  = p_reg_switch->PTRN_MSK_30;
    p_rx_pattern_matcher->p_pattern_data->reg.msk_74  = p_reg_switch->PTRN_MSK_74;
    p_rx_pattern_matcher->p_pattern_data->reg.msk_118 = p_reg_switch->PTRN_MSK_118;

    // Set Rx patter mattcher int
    if (p_reg_switch->PATTERN_IRQ_CONTROL_b.MATCHINT & pattern_mask)
    {
        p_rx_pattern_matcher->pattern_int = true;
    }
    else
    {
        p_rx_pattern_matcher->pattern_int = false;
    }

    // Set Rx patter mattcher enable
    p_rxmatch_config = &p_reg_switch->RXMATCH_CONFIG0;
    p_rx_pattern_matcher->pattern_port_mask = 0;
    for (port_num = 0; port_num <= ETHSW_HOST_PORT_NUMBER; port_num++)
    {
        if (p_rxmatch_config[port_num] & pattern_mask)
        {
            p_rx_pattern_matcher->pattern_port_mask |= (uint32_t) (1U << port_num);
        }
    }

    /* if host port is set convert to host port */
    if (p_rx_pattern_matcher->pattern_port_mask & (1U << ETHSW_HOST_PORT_NUMBER))
    {
        p_rx_pattern_matcher->pattern_port_mask &= ~(1U << ETHSW_HOST_PORT_NUMBER);
        p_rx_pattern_matcher->pattern_port_mask |= ETHSW_PORT_HOST;
    }

    /* set port mask flag */
    p_rx_pattern_matcher->pattern_port_mask |= ETHSW_PORT_MASK;

    return err;
}

/********************************************************************************************************************//**
 * Register the callback function for getting events from Rx pattern matcher.
 * Unregister if NULL is specified in the callback function.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_RxPatternMatcherCallback (
    ethsw_ctrl_t * const                p_ctrl,
    void (                            * p_rx_pattern_callback_func)(
        ethsw_rx_pattern_event_t        event,
        ethsw_rx_pattern_event_data_t * p_data))
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    gp_ethsw_rx_pattern_callback = p_rx_pattern_callback_func;

    return err;
}

/********************************************************************************************************************//**
 * Initilize pulse generator.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_PulseGeneratorInit (ethsw_ctrl_t * const p_ctrl, uint32_t * p_time_num)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSS_Type volatile * p_reg_ethss;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_time_num), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_TIME_COUNT > *p_time_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_ethss = p_instance_ctrl->p_reg_ethss;

    r_ethsw_reg_protection_disable(p_instance_ctrl->p_reg_ethss);

    p_reg_ethss->PTPMCTRL_b.PTP_PLS_RSTn = 0;

    p_reg_ethss->PTPMCTRL_b.PTP_MODE = *p_time_num & (R_ETHSS_PTPMCTRL_PTP_MODE_Msk >> R_ETHSS_PTPMCTRL_PTP_MODE_Pos);

    p_reg_ethss->PTPMCTRL_b.PTP_PLS_RSTn = 1;

    r_ethsw_reg_protection_enable(p_instance_ctrl->p_reg_ethss);

    return err;
}

/********************************************************************************************************************//**
 * Sets pulse generator.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_PulseGeneratorSet (ethsw_ctrl_t * const p_ctrl, ethsw_ts_pulse_generator_t * p_pulse)
{
    ethsw_instance_ctrl_t     * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t                   err             = FSP_SUCCESS;
    R_ETHSW_PTP_Type volatile * p_reg_ethsw_ptp;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_pulse), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PULSE_GENERATOR_COUNT > p_pulse->pulse_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_ethsw_ptp = p_instance_ctrl->p_reg_ethsw_ptp;

    r_ethsw_reg_protection_disable(p_instance_ctrl->p_reg_ethss);

    /* PTP Timer Pulse Output Disable */
    p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].EN_b.OUTEN = 0;

    if (true == p_pulse->enable)       // enable
    {
        /* Set the Pulse Width of ETHSW_PTPOUTn in the cycle number */
        p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].WTH_b.WIDTH = p_pulse->wide;

        /* PTP Timer Pulse Period */
        p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].PSEC = p_pulse->period_sec;
        p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].PNS  = p_pulse->period_ns;

        /* PTP Timer Pulse Start */
        p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].STSEC = p_pulse->start_sec;
        p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].STNS  = p_pulse->start_ns;

        /* PTP Timer Pulse Output Enable */
        p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].EN_b.OUTEN = p_pulse->enable;
    }

    r_ethsw_reg_protection_enable(p_instance_ctrl->p_reg_ethss);

    return err;
}

/********************************************************************************************************************//**
 * Gets pulse generator.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_PulseGeneratorGet (ethsw_ctrl_t * const p_ctrl, ethsw_ts_pulse_generator_t * p_pulse)
{
    ethsw_instance_ctrl_t     * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t                   err             = FSP_SUCCESS;
    R_ETHSW_PTP_Type volatile * p_reg_ethsw_ptp;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_pulse), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PULSE_GENERATOR_COUNT > p_pulse->pulse_num, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_ethsw_ptp = p_instance_ctrl->p_reg_ethsw_ptp;

    /* Set the Pulse Width of ETHSW_PTPOUTn in the cycle number */
    p_pulse->wide = p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].WTH_b.WIDTH;

    /* PTP Timer Pulse Period */
    p_pulse->period_sec = p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].PSEC;
    p_pulse->period_ns  = p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].PNS;

    /* PTP Timer Pulse Start */
    p_pulse->start_sec = p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].STSEC;
    p_pulse->start_ns  = p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].STNS;

    /* PTP Timer Pulse Output Enable */
    p_pulse->enable = p_reg_ethsw_ptp->SWTM[p_pulse->pulse_num].EN_b.OUTEN;

    return err;
}

/********************************************************************************************************************//**
 * Sets enable cut-through for the specified port.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_CutThroughSet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_port_mask)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    uint32_t                port_mask; /* converted portmask */

    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_port_mask), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PORT_MASK & *p_port_mask, FSP_ERR_INVALID_ARGUMENT);
 #endif

    /* remove portmask bit and translate host port if set */
    port_mask = *p_port_mask & ETHSW_PORT_BITS;
    if (*p_port_mask & ETHSW_PORT_HOST)
    {
        port_mask |= (1U << ETHSW_HOST_PORT_NUMBER);
    }

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_reg_switch->MODE_CONFIG_b.CUT_THRU_EN = port_mask &
                                              (R_ETHSW_MODE_CONFIG_CUT_THRU_EN_Msk >>
                                               R_ETHSW_MODE_CONFIG_CUT_THRU_EN_Pos);

    return err;
}

/********************************************************************************************************************//**
 * Gets enable cut-through for the specified port.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_CutThroughGet (ethsw_ctrl_t * const p_ctrl, uint32_t * p_port_mask)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;

    R_ETHSW_Type volatile * p_reg_switch;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_port_mask), FSP_ERR_INVALID_POINTER);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    *p_port_mask = p_reg_switch->MODE_CONFIG_b.CUT_THRU_EN;

    /* if host port is set convert to host port */
    if (*p_port_mask & (1U << ETHSW_HOST_PORT_NUMBER))
    {
        *p_port_mask &= ~(1U << ETHSW_HOST_PORT_NUMBER);
        *p_port_mask |= ETHSW_PORT_HOST;
    }

    /* set port mask flag */
    *p_port_mask |= ETHSW_PORT_MASK;

    return err;
}

/********************************************************************************************************************//**
 * Sets the cut-through delay for the specified port,
 * delay Value in 400 ns / 40 ns / 8 ns increments (fequency of the MII PHY interface).
 * This is a programmable value in MII clock cycles that can delay the assertion of the cut-through flag
 * to the forwarding engine in order to hide the variable latency of the forwarding decision.
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_CtDelaySet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_ct_delay)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_ct_delay_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_ct_delay), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_ct_delay_reg = &p_reg_switch->CT_DELAY_P0;

    p_ct_delay_reg[port] = *p_ct_delay & R_ETHSW_CT_DELAY_P0_CT_DELAY_Msk;

    return err;
}

/********************************************************************************************************************//**
 * Gets the cut-through delay for the specified port,
 *
 * @retval  FSP_SUCCESS                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER     Pointer to arguments are NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 ***********************************************************************************************************************/
fsp_err_t R_ETHSW_CtDelayGet (ethsw_ctrl_t * const p_ctrl, uint32_t port, uint32_t * p_ct_delay)
{
    ethsw_instance_ctrl_t * p_instance_ctrl = (ethsw_instance_ctrl_t *) p_ctrl;
    fsp_err_t               err             = FSP_SUCCESS;
    R_ETHSW_Type volatile * p_reg_switch;
    uint32_t volatile     * p_ct_delay_reg;

 #if (ETHSW_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHSW_ERROR_RETURN(ETHSW_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    ETHSW_ERROR_RETURN((NULL != p_ct_delay), FSP_ERR_INVALID_POINTER);
    ETHSW_ERROR_RETURN(ETHSW_PORT_COUNT > port, FSP_ERR_INVALID_ARGUMENT);
 #endif

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    p_ct_delay_reg = &p_reg_switch->CT_DELAY_P0;

    *p_ct_delay = p_ct_delay_reg[port] & R_ETHSW_CT_DELAY_P0_CT_DELAY_Msk;

    return err;
}

#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_SWITCH)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Interrupt handler for Ethernet switch.
 * Supports the interrupts for DLR, PHY link change, TSM(Timestamping) and TDMA.
 *
 * @retval      none
 **********************************************************************************************************************/
void ethsw_isr_intr (void)
{
    ethsw_callback_args_t callback_arg;
    uint32_t              ulInt_Stat_Ack_Val;
    R_ETHSW_Type        * p_reg_switch;
    volatile uint32_t     dammy_read;

    IRQn_Type               irq             = R_FSP_CurrentIrqGet();
    ethsw_instance_ctrl_t * p_instance_ctrl =
        (ethsw_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    p_reg_switch = p_instance_ctrl->p_reg_switch;

    ulInt_Stat_Ack_Val = p_reg_switch->INT_STAT_ACK;

    /* Clear IRQ_LINK Interrupt */
    p_reg_switch->INT_STAT_ACK = ulInt_Stat_Ack_Val;

    /* Dammy read for Countermeasure when the CPU clock is 400 or 800MHz */
    dammy_read = p_reg_switch->INT_STAT_ACK;
    dammy_read = dammy_read;

    /* Callback : Interrupt handler */
    if ((0 != (ulInt_Stat_Ack_Val & ETHSW_INT_CONFIG_IRQ_LNK_MASK)) &&
        (ETHSW_PHYLINK_ENABLE == p_instance_ctrl->p_switch_cfg->phylink))
    {
        callback_arg.channel = p_instance_ctrl->p_switch_cfg->channel;

        callback_arg.status_link  = p_reg_switch->STATUS_P0_b.PHYLINK;
        callback_arg.status_link |= (uint32_t) (p_reg_switch->STATUS_P1_b.PHYLINK << 1);
        callback_arg.status_link |= (uint32_t) (p_reg_switch->STATUS_P2_b.PHYLINK << 2);

#if 0
        callback_arg.status_link |= p_reg_switch->STATUS_P1_b.PHYLINK << 1U;
        callback_arg.status_link |= p_reg_switch->STATUS_P2_b.PHYLINK << 2;
#endif

        callback_arg.event     = ETHSW_EVENT_LINK_CHG;
        callback_arg.p_context = p_instance_ctrl->p_switch_cfg->p_context;

        (*p_instance_ctrl->p_switch_cfg->p_callback)((void *) &callback_arg);
    }

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

    /* TSM Interrupt */
    if (ETHSW_INT_CONFIG_TSM_INT == (ulInt_Stat_Ack_Val & ETHSW_INT_CONFIG_TSM_INT))
    {
        ethsw_isr_tsm(p_instance_ctrl);
    }

    /* TDMA Interrupt */
    if (ETHSW_INT_CONFIG_TDMA_INT == (ulInt_Stat_Ack_Val & ETHSW_INT_CONFIG_TDMA_INT))
    {
        ethsw_isr_tdma(p_instance_ctrl);
    }

    /* DLR Interrupt */
    if (ETHSW_INT_CONFIG_DLR_INT == (ulInt_Stat_Ack_Val & ETHSW_INT_CONFIG_DLR_INT))
    {
        ethsw_isr_dlr(p_instance_ctrl);
    }

    /* RX Pattern Matcher Interrupt */
    if (ETHSW_INT_CONFIG_PATTERN_INT == (ulInt_Stat_Ack_Val & ETHSW_INT_CONFIG_PATTERN_INT))
    {
        ethsw_isr_rx_pattern_matcher(p_instance_ctrl);
    }
#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */
}

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

/*******************************************************************************************************************//**
 * Interrupt handler for TSM(Timestamping)
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_isr_tsm (ethsw_instance_ctrl_t * p_instance_ctrl)
{
    ethsw_timestamp_t timestamp;
    uint32_t          port;
    uint32_t          irq_tx_en;
    uint32_t          port_bit;
    uint32_t          irq_stat_ack;
    R_ETHSW_Type    * p_switch_reg;
    uint32_t          ts_fifo_read_ctrl;

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    irq_stat_ack = p_switch_reg->TSM_IRQ_STAT_ACK;
    p_switch_reg->TSM_IRQ_STAT_ACK = irq_stat_ack;

    irq_tx_en = (irq_stat_ack & ETHSW_CONFIG_IRQ_TX_EN_MASK) >> ETHSW_CONFIG_IRQ_TX_EN_SHIFT;
    for (port = 0; irq_tx_en; port++)
    {
        port_bit = 1U << port;
        if (irq_tx_en & port_bit)
        {
            irq_tx_en &= (~port_bit);

            while (1)
            {
                ts_fifo_read_ctrl = (port << ETHSW_TS_FIFO_READ_CTRL_PORT_NUM_SHIFT) &
                                    ETHSW_TS_FIFO_READ_CTRL_PORT_NUM_MASK;
                p_switch_reg->TS_FIFO_READ_CTRL = ts_fifo_read_ctrl;

                ts_fifo_read_ctrl = p_switch_reg->TS_FIFO_READ_CTRL;

                if (ETHSW_TS_FIFO_READ_CTRL_TS_VALID != (ts_fifo_read_ctrl & ETHSW_TS_FIFO_READ_CTRL_TS_VALID))
                {
                    break;
                }

                timestamp.tim_num = (uint8_t) ((ts_fifo_read_ctrl & ETHSW_TS_FIFO_READ_CTRL_TS_SEL_MASK) >>
                                               ETHSW_TS_FIFO_READ_CTRL_TS_SEL_SHIFT);

                timestamp.timestamp_id = ((ts_fifo_read_ctrl & ETHSW_TS_FIFO_READ_CTRL_TS_ID_MASK) >>
                                          ETHSW_TS_FIFO_READ_CTRL_TS_ID_SHIFT);

                timestamp.tim_nsec = p_switch_reg->TS_FIFO_READ_TIMESTAMP;

                if (gp_ethsw_ptp_callback)
                {
                    (*gp_ethsw_ptp_callback)(ETHSW_PTP_CALLBACK_TS_VALID, (uint8_t) port, &timestamp);
                }
            }
        }
    }
}                                      /* End of ethsw_isr_tsm() */

/*******************************************************************************************************************//**
 * Interrupt handler for TDMA.
 * This function is called by ethsw_isr_intr.
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
 #define ETHSW_DEBUG

 #ifdef ETHSW_DEBUG
uint32_t g_tdma_irq_stat_ack;
 #endif

static void ethsw_isr_tdma (ethsw_instance_ctrl_t * p_instance_ctrl)
{
    ethsw_tdma_callback_data_t callback_data;
    R_ETHSW_Type             * p_switch_reg;
    uint32_t tdma_irq_stat_ack;

    p_switch_reg                    = p_instance_ctrl->p_reg_switch;
    tdma_irq_stat_ack               = p_switch_reg->TDMA_IRQ_STAT_ACK;
    p_switch_reg->TDMA_IRQ_STAT_ACK = tdma_irq_stat_ack;

 #ifdef ETHSW_DEBUG
    g_tdma_irq_stat_ack = tdma_irq_stat_ack;
 #endif

    if (0 != gp_ethsw_tdma_callback)
    {
        if (ETHSW_TDMA_IRQ_CONTROL_TCV_INT_EN == (tdma_irq_stat_ack & ETHSW_TDMA_IRQ_CONTROL_TCV_INT_EN))
        {
            callback_data.tcv_s_idx = p_switch_reg->TCV_SEQ_LAST_b.ACTIVE;

            (*gp_ethsw_tdma_callback)(ETHSW_TDMA_CALLBACK_TCV_INT, &callback_data);
        }

        if (ETHSW_TDMA_IRQ_CONTROL_CTR1_INT_EN == (tdma_irq_stat_ack & ETHSW_TDMA_IRQ_CONTROL_CTR1_INT_EN))
        {
            callback_data.tcv_s_idx = 0;

            (*gp_ethsw_tdma_callback)(ETHSW_TDMA_CALLBACK_COUNTER1_INT, &callback_data);
        }
    }
}                                      /* End of ethsw_isr_tdma() */

/*******************************************************************************************************************//**
 * Call the callback function registered for the DLR.
 *
 * @param[in]   event           event ID
 * @param[in]   port            source port of DLR event
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_dlr_callback (ethsw_dlr_event_t event, uint32_t port)
{
    if (0 != gp_ethsw_dlr_callback)
    {
        (*gp_ethsw_dlr_callback)(event, port);
    }
}                                      /* End of ethsw_dlr_callback() */

/*******************************************************************************************************************//**
 * Interrupt handler for DLR
 * This function is called by ethsw_isr_intr
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_isr_dlr (ethsw_instance_ctrl_t * p_instance_ctrl)
{
    R_ETHSW_Type * p_switch_reg;
    uint32_t       link_state = 0;     /* link state */
    uint32_t       int_ID;             /* interrupt Flag */
    uint32_t       reg_val;            /* register content */
    int32_t        cnt;                /* loop counter */

    p_switch_reg = p_instance_ctrl->p_reg_switch;

    while (p_switch_reg->DLR_IRQ_STAT_ACK)
    {
        reg_val = p_switch_reg->DLR_IRQ_STAT_ACK;
        p_switch_reg->DLR_IRQ_STAT_ACK = reg_val;

        /* scan all status bits */
        for (cnt = 0; cnt < 16; cnt++)
        {
            int_ID = reg_val & (1U << cnt);

            if (int_ID == 0)
            {
                /* bit wasn't set, check next one */
                continue;
            }

            switch (int_ID)
            {
                case ETHSW_TGT_DLR_INT_NEWSTATUS:
                {
                    ethsw_dlr_callback(DLR_EVENT_NEWSTATE, ETHSW_PORT_HOST);
                    break;
                }

                case ETHSW_TGT_DLR_INT_BEACONTMO_0:
                {
                    ethsw_dlr_callback(DLR_EVENT_BEACONTIMEOUT, ETHSW_PORT(0));
                    break;
                }

                case ETHSW_TGT_DLR_INT_BEACONTMO_1:
                {
                    ethsw_dlr_callback(DLR_EVENT_BEACONTIMEOUT, ETHSW_PORT(1));
                    break;
                }

                case ETHSW_TGT_DLR_INT_SUVICHNG:
                {
                    ethsw_dlr_callback(DLR_EVENT_NEWSUPERVISOR, ETHSW_PORT(0));
                    break;
                }

                case ETHSW_TGT_DLR_INT_LNKCHNG_0:
                {
                    R_ETHSW_LinkStateGet((ethsw_ctrl_t *) p_instance_ctrl, ETHSW_PORT(0), &link_state);

                    if (ETHSW_STATE_UP == link_state)
                    {
                        ethsw_dlr_callback(DLR_EVENT_LINKRESTORED, ETHSW_PORT(0));
                    }
                    else
                    {
                        ethsw_dlr_callback(DLR_EVENT_LINKLOST, ETHSW_PORT(0));
                    }

                    break;
                }

                case ETHSW_TGT_DLR_INT_LNKCHNG_1:
                {
                    R_ETHSW_LinkStateGet((ethsw_ctrl_t *) p_instance_ctrl, ETHSW_PORT(1), &link_state);

                    if (ETHSW_STATE_UP == link_state)
                    {
                        ethsw_dlr_callback(DLR_EVENT_LINKRESTORED, ETHSW_PORT(1));
                    }
                    else
                    {
                        ethsw_dlr_callback(DLR_EVENT_LINKLOST, ETHSW_PORT(1));
                    }

                    break;
                }

                case ETHSW_TGT_DLR_INT_LOCALLOOP_0:
                {
                    ethsw_dlr_callback(DLR_EVENT_OWNFRAME, ETHSW_PORT(0));
                    break;
                }

                case ETHSW_TGT_DLR_INT_LOCALLOOP_1:
                {
                    ethsw_dlr_callback(DLR_EVENT_OWNFRAME, ETHSW_PORT(1));
                    break;
                }

                default:
                {
                    break;
                }
            }
        }
    }
}                                      /* End of ethsw_isr_dlr() */

/*******************************************************************************************************************//**
 * Interrupt handler for Rx pattern matcher
 * This function is called by ethsw_isr_intr
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_isr_rx_pattern_matcher (ethsw_instance_ctrl_t * p_instance_ctrl)
{
    R_ETHSW_Type                * p_switch_reg = p_instance_ctrl->p_reg_switch;
    ethsw_rx_pattern_event_data_t event_data;
    uint32_t pattern_irq_stat_ack;

    pattern_irq_stat_ack = p_switch_reg->PATTERN_IRQ_STAT_ACK;

    p_switch_reg->PATTERN_IRQ_STAT_ACK = pattern_irq_stat_ack;

    if (0 != gp_ethsw_rx_pattern_callback)
    {
        event_data.match_int = (pattern_irq_stat_ack & R_ETHSW_PATTERN_IRQ_STAT_ACK_MATCHINT_Msk) >>
                               R_ETHSW_PATTERN_IRQ_STAT_ACK_MATCHINT_Pos;
        event_data.error_int = (pattern_irq_stat_ack & R_ETHSW_PATTERN_IRQ_STAT_ACK_ERROR_INT_Msk) >>
                               R_ETHSW_PATTERN_IRQ_STAT_ACK_ERROR_INT_Pos;

        (*gp_ethsw_rx_pattern_callback)(ETHSW_RX_PATTERN_MATCHER, &event_data);
    }
}

#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */

/*******************************************************************************************************************//**
 * Enable ICU for Ether Switcht
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_enable_icu (ethsw_instance_ctrl_t * const p_instance_ctrl)
{
    /** Configure the Ethernet Switch interrupt. */
    R_BSP_IrqCfgEnable(p_instance_ctrl->p_switch_cfg->irq, p_instance_ctrl->p_switch_cfg->ipl, p_instance_ctrl);
}                                      /* End of function ethsw_enable_icu() */

/*******************************************************************************************************************//**
 * Disable ICU for Ether Switcht
 *
 * @param[in]   p_instance_ctrl     pointer to control structure
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_disable_icu (ethsw_instance_ctrl_t * const p_instance_ctrl)
{
    /* Get IRQ number for A5PSW_Int interrupt. */
    R_BSP_IrqDisable(p_instance_ctrl->p_switch_cfg->irq);
    R_FSP_IsrContextSet(p_instance_ctrl->p_switch_cfg->irq, NULL);
}                                      /* End of function ether_disable_icu() */

#ifdef ETHSW_EXTENDED_API_FOR_FUTURE

/*******************************************************************************************************************//**
 * Configures the action for matched frames.
 *
 * @param[in,out]   p_switch_reg    base address of ether switch register
 * @param[in]       mode1           Snoop mode for block 1
 * @param[in]       mode2           Snoop mode for block 2
 *
 * @retval      FSP_SUCCESS                 Command successfully.
 **********************************************************************************************************************/
static fsp_err_t ethsw_snoop_init (R_ETHSW_Type volatile * p_switch_reg,
                                   ethsw_snoop_action_t    mode1,
                                   ethsw_snoop_action_t    mode2)
{
    volatile uint32_t * p_reg;         /* arith reg */

    block1Mode = mode1;
    block2Mode = mode2;

    /* Set operation for all arith to OR */
    p_reg = &(p_switch_reg->GARITH0);
    while (p_reg <= &p_switch_reg->GARITH3)
    {
        *p_reg |= ETHSW_GPARSER_PARSER_OPER_OR;
        p_reg   = (volatile uint32_t *) ((uint8_t *) p_reg + ETHSW_PORT_OFFSET_4BYTES);
    }

    p_reg = &(p_switch_reg->GARITH4);
    while (p_reg <= &p_switch_reg->GARITH7)
    {
        *p_reg |= ETHSW_GPARSER_PARSER_OPER_OR;
        p_reg   = (volatile uint32_t *) ((uint8_t *) p_reg + ETHSW_PORT_OFFSET_4BYTES);
    }

    /* Take all results of the stage into account */
    p_switch_reg->GARITH3_b.SEL_MATCH = ETHSW_GPARSER_ARTIH_ALL;
    p_switch_reg->GARITH7_b.SEL_MATCH = ETHSW_GPARSER_ARTIH_ALL;

    /* Set snooping modes for both stages */
    p_switch_reg->GARITH3_b.SNP_MD = mode1;
    p_switch_reg->GARITH7_b.SNP_MD = mode2;

    return FSP_SUCCESS;
}                                      /* End of ethsw_snoop_init() */

/*******************************************************************************************************************//**
 * Configures the generic parser with the given ID.
 *
 * @param[in,out]   p_switch_reg    base address of ether switch register
 * @param[in]       parserid        the id of the parser to configure. Valid range is 0-7
 * @param[in]       enable          enable/disable the parser
 * @param[in]       mode            the mode of comparison
 * @param[in]       compval         the value to compare with
 * @param[in]       mask            the mask which used to AND with the compare value
 * @param[in]       offset          offset in bytes for the field to match
 *
 * @retval      FSP_SUCCESS                 Command successfully.
 * @retval      FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 **********************************************************************************************************************/
static fsp_err_t ethsw_gparser_set (R_ETHSW_Type volatile * p_switch_reg,
                                    uint8_t                 parserid,
                                    bool                    enable,
                                    uint32_t                mode,
                                    uint16_t                compval,
                                    uint8_t                 mask,
                                    uint16_t                offset)
{
    fsp_err_t           res = FSP_SUCCESS; /* Result */
    volatile uint32_t * p_reg;             /* Register */
    uint32_t            val = 0;           /* Parser config */

    /* Check parser id */
    if (ETHSW_GPARSER_PARSER_MAX < parserid)
    {
        res = FSP_ERR_INVALID_ARGUMENT;
    }

    if (FSP_SUCCESS == res)
    {
        /* Calc offset depending on block */
        if (parserid <= ETHSW_GPARSER_GPARSER_BLOCK1)
        {
            p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->GPARSER0 +
                                           (ETHSW_PORT_OFFSET_4BYTES * parserid));
        }
        else
        {
            p_reg = (volatile uint32_t *) ((uint8_t *) &p_switch_reg->GPARSER4 +
                                           (ETHSW_PORT_OFFSET_4BYTES * (parserid - 4)));
        }

        if (enable)
        {
            /* Check whether modes are ok */
            if ((mode & ETHSW_GPARSER_MODE_COMPARE16) && (mode & ETHSW_GPARSER_MODE_CMP_OR))
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else if ((mode & ETHSW_GPARSER_MODE_IPPROTO) && (mode & ETHSW_GPARSER_MODE_NONIPPROT))
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else if ((mode & ETHSW_GPARSER_MODE_IPDATA) && (mode & ETHSW_GPARSER_MODE_SKIPVLAN))
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else if (mode & ETHSW_GPARSER_MODE_INVALID)
            {
                res = FSP_ERR_INVALID_ARGUMENT;
            }
            else
            {
                ;
            }

            if (FSP_SUCCESS == res)
            {
                /* Set compare values */
                if ((mode & ETHSW_GPARSER_MODE_COMPARE16) || (mode & ETHSW_GPARSER_MODE_OFFSETPLUS2))
                {
                    /* In COMPARE16 or OFFSETPLUS2 mode, no mask is given */
                    val |= (compval & ETHSW_HEX_0000FFFF);
                }
                else
                {
                    val |= ((uint32_t) compval & ETHSW_HEX_000000FF) << ETHSW_GPARSER_CMP_OFFSET;
                    val |= mask;
                }

                /* Set offset and activate parser */
                val   |= ((uint32_t) offset & ETHSW_GPARSER_OFFSET_MASK) << ETHSW_GPARSER_OFFSET_OFFSET;
                val   |= ETHSW_GPARSER_PARSER_VALID;
                val   |= mode;
                *p_reg = val;
            }
        }
        else
        {
            *p_reg &= ~(uint32_t) ETHSW_GPARSER_PARSER_VALID;
        }
    }

    return res;
}                                                                             /* End of ethsw_gparser_set() */

/*******************************************************************************************************************//**
 * Search for free parser in given block
 *
 * @param[in,out]   p_switch_reg    base address of ether switch register
 * @param[in]       block           block number
 * @param[out]      p_id            ID pointer
 *
 * @retval      FSP_SUCCESS                 Command successfully.
 * @retval      FSP_ERR_INVALID_ARGUMENT    Invalid input parameter.
 * @retval      FSP_ERR_OUT_OF_MEMORY       Not found paeser
 **********************************************************************************************************************/
static fsp_err_t ethsw_snoop_free_gparser_get (R_ETHSW_Type volatile * p_switch_reg,
                                               uint32_t                block, /* block number */
                                               uint32_t              * p_id)  /* id */
{
    fsp_err_t res   = FSP_SUCCESS;                                            /* Result */
    bool      found = false;                                                  /* True if free parser was found */
    uint32_t  idx;                                                            /* Index */

    volatile uint32_t * p_reg;                                                /* Register */

    /* Check whether block id is valid */
    if ((ETHSW_GPARSER_BLOCK_CNT < block) || (0 == block))
    {
        res = FSP_ERR_INVALID_ARGUMENT;
    }

    if (FSP_SUCCESS == res)
    {
        /* Select start register depending on block */
        if (ETHSW_GPARSER_PARSER_BLOCK_ID1 == block)
        {
            p_reg = &(p_switch_reg->GPARSER0);
        }
        else
        {
            p_reg = &(p_switch_reg->GPARSER4);
        }

        /* Look for free parser */
        for (idx = 0; idx < ETHSW_GPARSER_PARSER_PER_BLOCK && !found; idx++)
        {
            if (!(*(p_reg) & ETHSW_GPARSER_PARSER_VALID))
            {
                found = true;
                *p_id = idx;
                if (ETHSW_GPARSER_PARSER_BLOCK_ID2 == block)
                {
                    *p_id += ETHSW_GPARSER_PARSER_PER_BLOCK;
                }
            }

            p_reg = (volatile uint32_t *) ((uint8_t *) p_reg + ETHSW_PORT_OFFSET_4BYTES);
        }
    }

    /* Set error if no free parser was found */
    if (false == found)
    {
        res = FSP_ERR_OUT_OF_MEMORY;
    }

    return res;
}                                      /* End of ethsw_snoop_free_gparser_get() */

/*******************************************************************************************************************//**
 * Memcpy with volatile support
 *
 * @param[out]  p_dst       dest address
 * @param[in]   p_src       source address
 * @param[in]   cnt         copy size
 *
 * @retval      none
 **********************************************************************************************************************/
static void ethsw_memcpy_volatile (volatile void * p_dst, volatile const void * p_src, uint32_t cnt)
{
    volatile uint8_t       * p_dst_mem = p_dst; ///< de-volatiled dst pointer
    volatile const uint8_t * p_src_mem = p_src; ///< de-volatiled src pointer

    uint32_t i;

    for (i = 0; i < cnt; i++)
    {
        *p_dst_mem++ = *p_src_mem++;
    }
}                                      /* End of function ethsw_memcpy_volatile() */

/*******************************************************************************************************************//**
 * Unlock write access protection for Ethernet subsystem registers
 *
 * @param[in]   p_reg_base      Base address of Ethernet subsystem registers
 *
 * @retval      none
 **********************************************************************************************************************/

/* Key code for PRCMD register */
 #define ETHSW_PRCMD_UNLOCK1    (0x000000A5U)
 #define ETHSW_PRCMD_UNLOCK2    (0x00000001U)
 #define ETHSW_PRCMD_UNLOCK3    (0x0000FFFEU)
 #define ETHSW_PRCMD_UNLOCK4    (0x00000001U)
 #define ETHSW_PRCMD_LOCK       (0x00000000U)

static void r_ethsw_reg_protection_disable (void * const p_reg_base)
{
    R_ETHSS_Type * p_reg_ethss = (R_ETHSS_Type *) p_reg_base;

    /* Unlock the protection of ethernet subsystem register */
    p_reg_ethss->PRCMD = ETHSW_PRCMD_UNLOCK1;
    p_reg_ethss->PRCMD = ETHSW_PRCMD_UNLOCK2;
    p_reg_ethss->PRCMD = ETHSW_PRCMD_UNLOCK3;
    p_reg_ethss->PRCMD = ETHSW_PRCMD_UNLOCK4;
}

/*******************************************************************************************************************//**
 * Lock write access protection for Ethernet subsystem registers
 *
 * @param[in]   p_reg_base      Base address of Ethernet subsystem registers
 *
 * @retval      none
 **********************************************************************************************************************/
static void r_ethsw_reg_protection_enable (void * const p_reg_base)
{
    R_ETHSS_Type * p_reg_ethss = (R_ETHSS_Type *) p_reg_base;

    /* Lock the protection of ethernet access register */
    p_reg_ethss->PRCMD = ETHSW_PRCMD_LOCK;
}

#endif                                 /* ETHSW_EXTENDED_API_FOR_FUTURE */
