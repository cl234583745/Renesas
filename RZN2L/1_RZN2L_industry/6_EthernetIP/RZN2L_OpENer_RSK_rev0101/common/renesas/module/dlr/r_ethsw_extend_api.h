/***********************************************************************************************************************
 * Copyright [2020-2021] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_INTERFACES
 * @defgroup ETHSW_API Ethernet Switch Interface
 * @brief Interface for Ethernet Switch functions.
 *
 * @section ETHSW_API_Summary Summary
 * The ETHSW module (r_ethsw) provides an API for Ethernet Switch that use the ETHSW peripheral.
 *
 * The ETHSW interface supports the following features:
 * - Link change check support
 *
 * Implemented by:
 * - @ref ETHSW
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHSW_API_H
#define R_ETHSW_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHSW_API_VERSION_MAJOR    (1U)
#define ETHSW_API_VERSION_MINOR    (0U)

#define ETHSW_NANOSEC_MODULO       (1000 * 1000 * 1000)

#define ETHSW_MAC_ADDR_LEN         (6U)

#define ETHSW_STATE_DEFAULT        0   ///< default state
#define ETHSW_STATE_UNKNOWN        1   ///< unknown state
#define ETHSW_STATE_DOWN           2   ///< state down
#define ETHSW_STATE_UP             3   ///< state up

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef uint32_t ethsw_port_cast;
typedef uint32_t ethsw_port_mask_t;

/** Ethernet port macros */
#define ETHSW_PORT_HOST_CNT    1
#define ETHSW_PORT_HOST        ((ethsw_port_cast) (1 << 30))
#define ETHSW_PORT_MASK        ((ethsw_port_cast) ((uint64_t) 1 << 31))
#define ETHSW_PORT(x)        ((ethsw_port_cast) (x) & ~ETHSW_PORT_MASK)
#define ETHSW_PORT_BIT(x)    ((ethsw_port_cast) (1 << (x)) | ETHSW_PORT_MASK)
#define ETHSW_PORT_BITS        (~(ETHSW_PORT_HOST | ETHSW_PORT_MASK))

typedef uint8_t ethsw_mac_addr_t[ETHSW_MAC_ADDR_LEN];

/**  speed and duplex of the port */
typedef enum e_ethsw_link_speed
{
    ETHSW_LINK_SPEED_NO_LINK = 0,      ///< Link is not established
    ETHSW_LINK_SPEED_10H     = 1,      ///< Link status is 10Mbit/s and half duplex
    ETHSW_LINK_SPEED_10F     = 2,      ///< Link status is 10Mbit/s and full duplex
    ETHSW_LINK_SPEED_100H    = 3,      ///< Link status is 100Mbit/s and half duplex
    ETHSW_LINK_SPEED_100F    = 4,      ///< Link status is 100Mbit/s and full duplex
    ETHSW_LINK_SPEED_1000H   = 5,      ///< Link status is 1000Mbit/s and half duplex
    ETHSW_LINK_SPEED_1000F   = 6       ///< Link status is 1000Mbit/s and full duplex
} ethsw_link_speed_t;

/** Event code of callback function */
typedef enum e_ethsw_event
{
    ETHSW_EVENT_LINK_CHG               ///< Link status change detection event
} ethsw_event_t;

/** Enable/Disable Management Port Specific Frame Tagging */
typedef enum e_ethsw_specifig_tag
{
    ETHSW_SPECIFIC_TAG_DISABLE = 0,    ///< Disable
    ETHSW_SPECIFIC_TAG_ENABLE  = 1     ///< Enable
} ethsw_specifig_tag_t;

/** Enable/Disable PHYLINK Change Interrupt */
typedef enum e_ethsw_phylink
{
    ETHSW_PHYLINK_DISABLE = 0,         ///< Disable
    ETHSW_PHYLINK_ENABLE  = 1          ///< Enable
} ethsw_phylink_t;

/** Callback function parameter data */
typedef struct st_ethsw_callback_args
{
    uint32_t      channel;             ///< Device channel number
    ethsw_event_t event;               ///< Event code

    uint32_t status_link;              ///< Link status bit0:port0. bit1:port1. bit2:port2, bit3:port3

    void const * p_context;            ///< Placeholder for user data.  Set in @ref ethsw_api_t::open function in @ref ethsw_cfg_t.
} ethsw_callback_args_t;

/** Control block.  Allocate an instance specific control block to pass into the API calls.
 * @par Implemented as
 * - ethsw_instance_ctrl_t
 */
typedef void ethsw_ctrl_t;

/** Configuration parameters. */
typedef struct st_ethsw_cfg
{
    uint8_t channel;                                     ///< Channel

    ethsw_specifig_tag_t specific_tag;                   ///< Management port specific frmame tag
    uint32_t             specific_tag_id;                ///< Management port specific frmame tag ID

    ethsw_phylink_t phylink;                             ///< Enable or disable link status change by PHYLINK
    ///< Call callback function when enable

    IRQn_Type irq;                                       ///< MCU interrupt number
    uint8_t   ipl;                                       ///< MCU interrupt priority

    void (* p_callback)(ethsw_callback_args_t * p_args); ///< Callback provided when an ISR occurs.

    /** Placeholder for user data.  Passed to the user callback in ethsw_callback_args_t. */
    void const * p_context;
    void const * p_extend;                               ///< Placeholder for user extension.
} ethsw_cfg_t;

/** MAC table entry */
typedef struct st_ethsw_mactab_entry
{
    ethsw_mac_addr_t * addr;           ///< MAC address pointer
    uint32_t           portmask;       ///< port mask
    uint32_t           priority;       ///< switching priority
} ethsw_mactab_entry_t;

/** MAC table config */
typedef struct st_ethsw_mactab_conf
{
    bool learn;                        ///< Enable/disable MAC learning
    bool aging;                        ///< Enable/disable address aging
    bool migration;                    ///< Enable/disable the migration of devices from one port to another
    bool pervlan;                      ///< Enable/disable per VLAN MAC learning
    bool discunknown;                  ///< Enable/disable discarding of unknown destination frames
} ethsw_mactab_conf_t;

/** MAC table clear/flush modes */
typedef enum e_ethsw_mactab_clr_modes
{
    ETHSW_MACTAB_CLR_STATIC,           ///< Static MAC table entries
    ETHSW_MACTAB_CLR_DYNAMIC,          ///< Learned MAC table entries
    ETHSW_MACTAB_CLR_ALL               ///< Static and learn entries
} ethsw_mactab_clr_modes_t;

/** VLAN input processing modes */
typedef enum e_ethsw_vlan_in_mode
{
    ETHSW_VLANIN_PASSTHROUGH_OVERRIDE, ///< Single Tagging with Passthrough/VID Overwrite
    ETHSW_VLANIN_REPLACE,              ///< Single Tagging with Replace
    ETHSW_VLANIN_TAG_ALWAYS,           ///< Tag always
    ETHSW_VLANIN_DISABLE = 0xFF        ///< Disable VLAN
} ethsw_vlan_in_mode_t;

/** VLAN output processing mode */
typedef enum e_ethsw_vlan_out_mode
{
    ETHSW_VLANOUT_DISABLE,             ///< No manipulation
    ETHSW_VLANOUT_STRIP,               ///< Strip mode
    ETHSW_VLANOUT_TAGTHROUGH,          ///< Tag Thru mode
    ETHSW_VLANOUT_DOMAINTRANSP         ///< Domain / Transparent mode
} ethsw_vlan_out_mode_t;

/** VLAN table entry */
typedef struct st_ethsw_vlan_entry
{
    uint32_t index;                    ///< Index of entry
    bool     valid;                    ///< Entry valid
    uint32_t vlan;                     ///< VLAN ID
    uint32_t portmask;                 ///< Port mask
} ethsw_vlan_entry_t;

/** Timer enable/disable control */
typedef struct st_ethsw_time_enable
{
    bool    flag_enable;               ///< Timer enable flag
    uint8_t tim_num;                   ///< Timer number
    uint8_t clock_period;              ///< Clock period of the timestamping clock
} ethsw_time_enable_t;

/** Timer value */
typedef struct st_ethsw_timestamp
{
    uint8_t  tim_num;                  ///< Timer number
    uint32_t tim_sec;                  ///< Second
    uint32_t tim_nsec;                 ///< Nanosecond
    uint32_t timestamp_id;             ///< Timeatamp ID
} ethsw_timestamp_t;

/** Domain & Timer mapping info */
typedef struct st_ethsw_ptp_domain
{
    uint8_t tim_num;                   ///< Timer number
    uint8_t domain_num;                ///< Domain number
} ethsw_ptp_domain_t;

/** Peer delay info */
typedef struct st_ethsw_ptp_peerdelay
{
    uint8_t  tim_num;                  ///< Timer number
    uint32_t peerdelay;                ///< Peer delay value
} ethsw_ptp_peerdelay_t;

/** PTP callback event */
typedef enum e_ethsw_ptp_event
{
    ETHSW_PTP_CALLBACK_TS_VALID = 0x10,
    ETHSW_PTP_CALLBACK_TS_OVER,
} ethsw_ptp_event_t;

/** tdma callback event */
typedef enum e_ethsw_tdma_event
{
    ETHSW_TDMA_CALLBACK_TCV_INT      = 0x100, ///< active event of TCV sequence entry
    ETHSW_TDMA_CALLBACK_COUNTER1_INT = 0x101, ///< active event of TDMA count1
} ethsw_tdma_event_t;

/** tdma callback data */
typedef union
{
    uint16_t tcv_s_idx;                ///< index of TCV sequence entry (valu of TDMA_IRQ_STAT_ACK_b.TCV_IDX_ACK)
} ethsw_tdma_callback_data_t;

/** tdma enable info */
typedef struct st_ethsw_tdma_enable
{
    bool     flg_enable;
    uint8_t  tim_num;                  ///< timer number
    uint32_t portmask;                 ///< port mask
    uint32_t tdma_start;               ///< start time for the first cycle of TDMA
    uint32_t tdma_modulo;              ///< system timer modulo for 1s
    uint32_t tdma_cycle;               ///< periodic cycle time for TDMA scheduler
    void (* p_tdma_callback)(          ///< pointer of callback function
        ethsw_tdma_event_t           event,
        ethsw_tdma_callback_data_t * p_tdma_data);
} ethsw_tdma_enable_t;

/** tdma sequence entry info */
typedef struct st_ethsw_tdma_seq_entry
{
    uint32_t tcv_func;                 ///< TCV function
    uint16_t tcv_d_index;              ///< index in the TCV data table
    uint16_t tcv_s_addr;               ///< index in the TCV sequence entry
    uint8_t  tcv_gpio;                 ///< TDMA_GPIO
} ethsw_tdma_seq_entry_t;

/** define of tcv_func member in ethsw_tdma_seq_entry_t */
#define ETHSW_TDMA_TCV_NON      0x0    ///< non
#define ETHSW_TDMA_TCV_START    0x1    ///< TCV start vector
#define ETHSW_TDMA_TCV_INT      0x2    ///< TCV generate interrupt

/** tdma data entry info */
typedef struct st_ethsw_tdma_dat_entry
{
    uint32_t tcv_d_addr;               ///< index in the TCV data table
    uint32_t tcv_d_offset;             ///< time offset from the TDMA Cycle Start
    uint16_t tcv_d_qgate;              ///< gate state of each queue
    uint32_t tcv_d_pmask;              ///< tcv_d_qgate affected ports
    bool     hold_req;                 ///< Preemption hold request
    bool     gate_mode;                ///< Gate mode enable
    bool     trigger_mode;             ///< Trigger mode enable
    bool     in_ct_ena;                ///< Input Cut-Through Enable
    bool     out_ct_ena;               ///< Output Cut-Through Enable
    bool     red_period;               ///< Period Color Control (for Profinet IRT)
    bool     inc_crt1;                 ///< Increment Control for Counter 1
    bool     inc_crt0;                 ///< Increment Control for Counter 0
} ethsw_tdma_dat_entry_t;

/** range(start-last) of tdma sequence entry */
typedef struct st_ethsw_tdma_seq_range
{
    uint32_t tcv_start;                ///< first index in the TCV sequence entry
    uint32_t tcv_last;                 ///< last index in the TCV sequence entry
} ethsw_tdma_seq_range_t;

/** queue gate */
typedef struct st_ethsw_mmctl_qgate
{
    uint32_t portmask;                 ///< per-port bit mask
    uint16_t qgate;                    ///< action to be performed on each queue (2-bits per queue)
} ethsw_mmclt_qgate_t;

/** port mirroring modes */
typedef enum e_ethsw_mirr_mode
{
    ETHSW_MIRR_MODE_EGRESS_DA,         ///< Egress DA match
    ETHSW_MIRR_MODE_EGRESS_SA,         ///< Egress SA match
    ETHSW_MIRR_MODE_INGRESS_DA,        ///< Ingress DA match
    ETHSW_MIRR_MODE_INGRESS_SA,        ///< Ingress SA match
    ETHSW_MIRR_MODE_INGRESS_PORT,      ///< Ingress port match
    ETHSW_MIRR_MODE_EGRESS_PORT        ///< Egress port match
} ethsw_mirr_mode_t;

/** mirror port config */
typedef struct st_ethsw_mirror_conf
{
    uint32_t           port;           ///< The port id that will received all mirrored frames. A value of 0 disables the port mirroring.
    ethsw_mirr_mode_t  mirrortype;     ///< The type of mirroring to configure.
    uint8_t            portmap;        ///< The port map to which the mirror configuration will be applied to.
    ethsw_mac_addr_t * macaddr;        ///< MAC address pointer
} ethsw_mirror_conf_t;

/** QoS mode */
typedef struct st_ethsw_qos_mode
{
    uint16_t mode;                     ///< A bitfield describing which modes shall be used.
    uint8_t  defprio;                  ///< Default priority
} ethsw_qos_mode_t;

/** QoS IP priority */
typedef struct st_ethsw_qos_prio_ip
{
    uint8_t diffserv;                  ///< The DiffServ field of the IP packet
    uint8_t priority;                  ///< The priority to assign.
} ethsw_qos_prio_ip_t;

/** QoS Ethertype priority */
typedef struct st_ethsw_qos_prio_type
{
    uint16_t type1;                    ///< The first type to match against. A value of 0 disables that match.
    uint8_t  prio1;                    ///< The priority to be assigned for the first match.
    uint16_t type2;                    ///< The second type to match against. A value of 0 disables that match.
    uint8_t  prio2;                    ///< The priority to be assigned for the second match.
} ethsw_qos_prio_type_t;

/** flood domain configuration for unknown frames */
typedef struct st_ethsw_flood_unk_conf
{
    ethsw_port_mask_t port_mask_bcast; ///< flood domain port mask for broadcasts with unkown destination
    ethsw_port_mask_t port_mask_mcast; ///< flood domain port mask for multicasts with unkown destination
    ethsw_port_mask_t port_mask_ucast; ///< flood domain port mask for unicasts with unkown destination
} ethsw_flood_unk_conf_t;

/** EEE */
typedef struct st_ethsw_eee
{
    bool     state;                    ///< State of EEE auto mode (enabled/disabled)
    uint32_t idletime;                 ///< Idle time of EEE
} ethsw_eee_t;

/** snooping modes */
typedef enum e_ethsw_snoop_matchmode
{
    ETHSW_SNOOP_MODE_IPDATA,           ///< Match data within IP pkt
    ETHSW_SNOOP_MODE_IPPROT,           ///< Match IP protocol
    ETHSW_SNOOP_MODE_ETYPE,            ///< Match ether type
    ETHSW_SNOOP_MODE_DATA              ///< Match any data
} ethsw_snoop_matchmode_t;

/** snooping actions */
typedef enum e_ethsw_snoop_action
{
    ETHSW_SNOOP_ACTION_CAPTURE = 1,    ///< Frames are only forwarded to mgt port
    ETHSW_SNOOP_ACTION_CPY_FW,         ///< Frames are forwarded to mgt port and dest port
    ETHSW_SNOOP_ACTION_DISCARD         ///< Discard frames
} ethsw_snoop_action_t;

/** Snoop config */
typedef struct st_ethsw_snoop_config
{
    ethsw_snoop_matchmode_t mode;      ///< Snooping mode
    ethsw_snoop_action_t    action;    ///< How to handle matched frames
    uint16_t                match;     ///< Data to match
    uint8_t                 mask;      ///< Match mask
    uint32_t                flags;     ///< Flags
    uint32_t                offset;    ///< Data offset
    uint32_t                id;        ///< Id of snoop config
} ethsw_snoop_config_t;

/** Port authorization states */
#define ETHSW_AUTH_DEFAULT         0   ///< default state
#define ETHSW_AUTH_UNKNOWN         1   ///< unknown state
#define ETHSW_AUTH_UNAUTHORIZED    2   ///< port is unauthorized
#define ETHSW_AUTH_AUTHORIZED      3   ///< port is authorized

/** Port control directions */
#define ETHSW_CTRL_DIR_DEFAULT     0   ///< default EAPOL reception state
#define ETHSW_CTRL_DIR_UNKNOWN     1   ///< unknown EAPOL reception state
#define ETHSW_CTRL_DIR_INPUT       2   ///< EAPOL reception disabled
#define ETHSW_CTRL_DIR_BOTH        3   ///< EAPOL reception enabled

/** Port EAPOL reception states */
#define ETHSW_EAPOL_DEFAULT        0   ///< default EAPOL reception state
#define ETHSW_EAPOL_UNKNOWN        1   ///< unknown EAPOL reception state
#define ETHSW_EAPOL_DISABLE        2   ///< EAPOL reception disabled
#define ETHSW_EAPOL_ENABLE         3   ///< EAPOL reception enabled

/** BPDU management control states */
typedef enum e_ethsw_bpdu_ctrl
{
    ETHSW_BPDU_CTRL_UNKNOWN,           ///< unknown
    ETHSW_BPDU_CTRL_DEFAULT,           ///< default
    ETHSW_BPDU_CTRL_DISCARD,           ///< discarding
    ETHSW_BPDU_CTRL_MGMT_FWD,          ///< mgmt port forward only
} ethsw_bpdu_ctrl_t;

/** DLR events for the DLR State Machine */
typedef enum e_ethsw_dlr_event
{
    DLR_EVENT_LINKLOST,                ///< Link was lost
    DLR_EVENT_OWNFRAME,                ///< the device received its own frame
    DLR_EVENT_LINKRESTORED,            ///< Link is restored
    DLR_EVENT_BEACONFRAME,             ///< Beacon Frame received
    DLR_EVENT_BEACONTIMEOUT,           ///< Beacon Timer timed out
    DLR_EVENT_NEWSUPERVISOR,           ///< a new Ring Supervisor was detected
    DLR_EVENT_LOCATEFAULT,             ///< locate Fault frame received
    DLR_EVENT_NEIGHBORCHECKREQ,        ///< Neighbor_Check_Request received
    DLR_EVENT_NEIGHBORCHECKRES,        ///< Neighbor_Check_Response received
    DLR_EVENT_NEIGHBORTIMEOUT,         ///< NeighborCheck Timer expired
    DLR_EVENT_FLUSHTABLES,             ///< Flush_Tables Frame received
    DLR_EVENT_SIGNON,                  ///< Sign_On Frame received
    DLR_EVENT_NEWSTATE                 ///< Beacon Hardware detected new state
} ethsw_dlr_event_t;

/** DLR initilize */
typedef struct st_ethsw_dlr_init
{
    ethsw_mac_addr_t * p_host_addr;    ///< host MAC address pointer
    void (* p_dlr_callback)(           ///< callback function pointer
        ethsw_dlr_event_t event,
        uint32_t          port);
} ethsw_dlr_init_t;

/** Definition for GOAL interface compatibility */
typedef struct st_ethsw_portentry
{
    uint32_t latency_egress;           ///< Egress latency
    uint32_t latency_ingress;          ///< Ingress latency
} ethsw_portentry_t;

/** Operating mode of RX Pattern Matcher */
typedef enum e_ethsw_rx_pattern_mode
{
    ETHSW_RX_PATTER_MODE_FIXED = 0,         ///< Mode1: Fixed 12-byte match
    ETHSW_RX_PATTER_MODE_LOOKUP,            ///< Mode2: 2-byte lookup mode
    ETHSW_RX_PATTER_MODE_RANGE_MATCH,       ///< Mode3: 2-byte range match at set offset
    ETHSW_RX_PATTER_MODE_RANGE_NOT_MATCH,   ///< Mode4: 2-byte range not-match at set offset
} ethsw_rx_pattern_mode_t;

/** RX Pattern Matcher Function Control */
typedef struct st_ethsw_rx_pattern_ctrl
{
    bool     match_not;          ///< The functions of this control are executed if the pattern does not match.
    bool     mgmt_fwd;           ///< The frame is forwarded to the management port only
    bool     discard;            ///< The frame is discarded.
    bool     set_prio;           ///< Set frame priority, overriding normal classification.

    ethsw_rx_pattern_mode_t mode;   ///< Operating mode

    bool     timer_sel_ovr;      ///< Overrides the default timer to use by timestamp operations. 
    bool     force_forward;      ///< The frame is forwarded to the ports indicated in PORTMASK,
    bool     hub_trigger;        ///< The port defined in the PORTMASK setting is allowed for transmitting one frame. Usable for Hub mode only.
    bool     match_red;          ///< Enable the pattern matcher only when the TDMA indicates that this is the RED period.
    bool     match_not_red;      ///< Enable the pattern matcher only when the TDMA indicates that this is not the RED period.
    bool     vlan_skip;          ///< Skips VLAN tag (4 bytes)
    uint8_t  priority;           ///< Priority of the frame used when SET_PRIO is set.
    bool     learning_dis;       ///< The hardware learning function is not executed.
    uint32_t port_mask;          ///< A port mask used depending on the control bits (for example, HUBTRIGGER).
    bool     imc_trigger;        ///< The ports defined in the PORTMASK setting are allowed for transmitting one frame from the queues indicated by QUEUESEL. 
    bool     imc_trigger_dely;   ///< The ports defined in the PORTMASK setting are allowed for transmitting one frame from the queues indicated by QUEUESEL. Delayed by the value programmed in MMCTL_DLY_QTRIGGER_CTRL.
    bool     swap_bytes;         ///< The byte order is swapped from the order received by the frame. 
    bool     match_lt;           ///< The Length/Type field in the frame is compared against the value in length_type in the compare register.
    uint8_t  timer_sel;          ///< Override value to use when TIMER_SEL_OVR is set to 1 for selecting the timer for this frame.
    uint8_t  queue_sel;          ///< A queue selector for the HUBTRIGGER function. 
} ethsw_rx_pattern_ctrl_t;

/** RX Pattern Matcher Function Data */
typedef union st_ethsw_rx_pattern_data
{
    /* Mode1: Fixed 12-byte match */
    struct
    { 
        uint8_t     cmp[12];        ///< Compare value 0 .. 12
        uint8_t     msk[12];        ///< Masks bits 0 .. 12
    } fixed_mode;

    /* Mode2: 2-byte lookup mode */
    struct
    {
        uint16_t    cmp0;           ///< Compare value 0
        uint16_t    cmp1;           ///< Compare value 1
        uint16_t    cmp2;           ///< Compare value2
        uint16_t    cmp3;           ///< Compare value 3
        uint16_t    cmp4;           ///< Compare value 4
        uint16_t    cmp5;           ///< Compare value 5
        uint16_t    mask;           ///< Masks bits
        uint16_t    length_type;    ///< Value to compare against the frame�fs Length/Type when MATCH_LT is set in the config register. 
        uint8_t     offset;         ///< Byte offset to do the match after the MAC source address of the frame. 
        uint8_t	    reserve;
        uint16_t    and_mask;       ///< Indicates that this pattern matcher matches if its own matching conditions match AND the pattern matchers which have their bits set to 1 in and_mask also match. 
        uint16_t    cmp6;           ///< Compare value 6
        uint16_t    cmp7;           ///< Compare value 7
    } lookup_mode;

    /* Mode3: 2-byte range match at set offset */
    /* Mode4: 2-byte range not-match at set offset */
    struct
    {
        uint16_t    min;            ///< Minimum length to compare the 2 bytes at the offset.
        uint16_t    max;            ///< Maximum length to compare the 2 bytes at the offset.
        uint16_t    reserve_1;
        uint16_t    reserve_2;
        uint16_t    reserve_3;
        uint16_t    reserve_4;
        uint16_t    mask;           ///< Masks bits
        uint16_t    length_type;    ///< Value to compare against the frame�fs Length/Type when MATCH_LT is set in the config register. 
        uint8_t     offset;         ///< Byte offset to do the match after the MAC source address of the frame. 
        uint8_t     reserve_5;
        uint16_t    and_mask;       ///< Indicates that this pattern matcher matches if its own matching conditions match AND the pattern matchers which have their bits set to 1 in and_mask also match. 
        uint16_t    reserve_6;
        uint16_t    reserve_7;
    } range_mode;

    /* register */
    struct 
    {
        uint32_t    cmp_30;     ///< Value to set in PTRN_CMP_30
        uint32_t    cmp_74;     ///< Value to set in PTRN_CMP_74
        uint32_t    cmp_118;    ///< Value to set in PTRN_CMP_118
        uint32_t    msk_30;     ///< Value to set in PTRN_MSK_30
        uint32_t    msk_74;     ///< Value to set in PTRN_MSK_74
        uint32_t    msk_118;    ///< Value to set in PTRN_MSK_118
    } reg;
} ethsw_rx_pattern_data_t;

/** The parameter for set/get the Rx pattern matcher function */
typedef struct st_ethsw_rx_pattern_matcher
{
    uint16_t        pattern_sel;        ///< Pattern matcher number�i0�`11�j
    uint32_t        pattern_port_mask;  ///< Port mask
    bool            pattern_int;        ///< When true, called back from ISR

    ethsw_rx_pattern_ctrl_t * p_pattern_ctrl; ///< RX Pattern Matcher Function Control
    ethsw_rx_pattern_data_t * p_pattern_data; ///< RX Pattern Matcher Function Data
} ethsw_rx_pattern_matcher_t;

/** Callback event of the Rx pattern matcher */
typedef enum e_ethsw_rx_pattern_event
{
    ETHSW_RX_PATTERN_MATCHER,   ///< Rx pattern matcher event
} ethsw_rx_pattern_event_t; 


/** Callback parameter of the Rx pattern matcher */
typedef struct st_ethsw_rx_pattern_event_data
{
    uint16_t    match_int;      ///< Pattern mask to indicate pattern match
    uint16_t    error_int;      ///< Error for each port
} ethsw_rx_pattern_event_data_t; 

/** The parameter for set/get TDMA counter 1 function */
typedef struct st_ethsw_tdma_counter1
{
    bool        write_value;    ///< When true, set value to counter 1
    uint8_t     value;          ///< Setted counter 1 value
    uint8_t     max_value;      ///< Maximum value of counter 1
    bool        int_enable;     ///< When true, enable counter 1 interrupt
    uint8_t     int_value;      ///< Counter 1 value when the interrupt occurred
} ethsw_tdma_counter1_t;

#if 0
/** The parameter for set/get MMCTL allocation function */
typedef struct st_ethsw_mmctl_info
{
    uint16_t     pool0_cells;    ///< Number of cells to allocate to pool 0
    uint16_t     pool1_cells;    ///< Number of cells to allocate to pool 1
    uint16_t     global_cells;   ///< Number of cells to allocate to global pool

    bool        queue0_enable;  ///< When true, queue0_map enabled.  When false, use global pool.
    uint8_t     queue0_map;     ///< Select memory pool for queue 0 (pool 0 or pool 1)
    bool        queue1_enable;  ///< When true, queue1_map enabled.  When false, use global pool.
    uint8_t     queue1_map;     ///< Select memory pool for queue 1 (pool 0 or pool 1)
    bool        queue2_enable;  ///< When true, queue2_map enabled.  When false, use global pool.
    uint8_t     queue2_map;     ///< Select memory pool for queue 2 (pool 0 or pool 1)
    bool        queue3_enable;  ///< When true, queue3_map enabled.  When false, use global pool.
    uint8_t     queue3_map;     ///< Select memory pool for queue 3 (pool 0 or pool 1)
    bool        queue4_enable;  ///< When true, queue4_map enabled.  When false, use global pool.
    uint8_t     queue4_map;     ///< Select memory pool for queue 4 (pool 0 or pool 1)
    bool        queue5_enable;  ///< When true, queue5_map enabled.  When false, use global pool.
    uint8_t     queue5_map;     ///< Select memory pool for queue 5 (pool 0 or pool 1)
    bool        queue6_enable;  ///< When true, queue6_map enabled.  When false, use global pool.
    uint8_t     queue6_map;     ///< Select memory pool for queue 6 (pool 0 or pool 1)
    bool        queue7_enable;  ///< When true, queue7_map enabled.  When false, use global pool.
    uint8_t     queue7_map;     ///< Select memory pool for queue 7 (pool 0 or pool 1)
} ethsw_mmctl_info_t;
#else
/** MMCTL pool ID. */
typedef enum e_ethsw_mmctl_pool_id
{
    ETHSW_MMCTL_POOL_ID_0      = 0,     ///< Pool 0
    ETHSW_MMCTL_POOL_ID_1      = 1,     ///< Pool 1
    ETHSW_MMCTL_POOL_ID_GLOBAL = 8,     ///< Global Pool
} ethsw_mmctl_pool_id_t;

/** The parameter for set/get memory pool size function  */
typedef struct st_ethsw_mmctl_pool_size
{
    ethsw_mmctl_pool_id_t   pool_id;    ///< MMCTL pool ID.
    uint16_t                cells;      ///< Memory pool size, in sells.
} ethsw_mmctl_pool_size_t;

/** The parameter for function that assins a memory pool for a queue. */
typedef struct st_ethsw_mmctl_queue_assign
{
    uint8_t                 queue_num;  ///< Queue bumber
    ethsw_mmctl_pool_id_t   pool_id;    ///< MMCTL pool ID.
} ethsw_mmctl_queue_assign_t;
#endif

/** The parameter for set / get length in bytes of the YELLOW period. */
typedef struct st_ethsw_yellow_length
{
    bool        enable;         ///< When true, length is valid.
    uint16_t    length;         ///< Specify the byte length of the YELLOW period in units of 4 bytes.
} ethsw_yellow_length_t;

/** The action of queue flush event. */
typedef enum e_ethsw_queu_flush_action
{
    ETHSW_QUEUE_FLUSH_DISABLE = 0,           ///< Disable flush
    ETHSW_QUEUE_FLUSH_WHEN_QUEUE_IS_CLOSING, ///< Flush when queue is closed, discarding any new frames
    ETHSW_QUEUE_FLUSH_WHEN_QUEUE_CLOSE,      ///< Flush on queue close until empty, then stops flushing
    ETHSW_QUEUE_FLUSH_TRIGGER,               ///< TRIGGER flush until empty, then return to current flush mode
} ethsw_queu_flush_action_t;

/** The parameter for set / get queue flush event. */
typedef struct st_ethsw_queue_flush_event
{
    uint32_t                    port_mask;  ///< Per-port bit mask
    uint32_t                    queue_mask; ///< 1 bit per queue indicating for which queues of the ports indicated by port_mask the flush state is changed as indicated in action.
    ethsw_queu_flush_action_t   action;     ///< Selects the flush state for the queues indicated by queue_mask in the ports indicated by port_mask.
} ethsw_queue_flush_event_t;

/** The parameter for offset correction. */
typedef struct st_ethsw_ts_offset_correction
{
    uint8_t     time_num;       ///< Timer number (0 or 1)
    uint8_t     offs_inc;       ///< Offset Correction Increment Value�i0�`127�j
    uint32_t    offs_corr;      ///< Offset Correction Counter
    uint32_t    offset;         ///< Timer Offset Register
} ethsw_ts_offset_correction_t;

/** The parameter for clock rate correction. */
typedef struct st_ethsw_ts_rate_correction
{
    uint8_t     time_num;       ///< Timer number (0 or 1)
    uint8_t     corr_inc;       ///< Rate Correction Increment Value�i0�`127�j
    uint32_t    corr;           ///< Correction Period
} ethsw_ts_rate_correction_t;

/** The parameter for set/get pulse generator. */
typedef struct 
{
    uint8_t     pulse_num;      ///< Pulse generator number�i0�`3�j
    bool        enable;         ///< When true, enable
    uint32_t    start_sec;      ///< Pulse start second
    uint32_t    start_ns;       ///< Pulse start nanosecond
    uint16_t    wide;           ///< Pulse width
    uint32_t    period_sec;     ///< Pulse period second
    uint32_t    period_ns;      ///< Pulse period anosecond
} ethsw_ts_pulse_generator_t;

/** Functions implemented at the HAL layer will follow this API. */
typedef struct st_ethsw_api
{
    /** Open driver.
     * @par Implemented as
     * - @ref R_ETHSW_Open()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_cfg            Pointer to pin configuration structure.
     */
    fsp_err_t (* open)(ethsw_ctrl_t * const p_api_ctrl, ethsw_cfg_t const * const p_cfg);

    /** Close driver.
     * @par Implemented as
     * - @ref R_ETHSW_Close()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     */
    fsp_err_t (* close)(ethsw_ctrl_t * const p_api_ctrl);

    /** Configs speed and duplex of the port.
     * @par Implemented as
     * - @ref R_ETHSW_SpeedCfg()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  spee             Speed and Duplex,
     */
    fsp_err_t (* speedCfg)(ethsw_ctrl_t * const p_api_ctrl, uint8_t const port, ethsw_link_speed_t const speed);

    /** Return the version of the driver.
     * @par Implemented as
     * - @ref R_ETHSW_VersionGet()
     *
     * @param[out] p_data           Memory address to return version information to.
     */
    fsp_err_t (* versionGet)(fsp_version_t * const p_data);

    /** Gets the number of available external Ethernet ports.
     * @par Implemented as
     * - @ref R_ETHSW_PortCount()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[out] p_cnt_ports      Pointer to count of port.
     */
    fsp_err_t (* portCount)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_cnt_ports);

    /** Sets the static MAC address entry for the given MAC address.
     * @par Implemented as
     * - @ref R_ETHSW_MacTableSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_mac_tab        Pointer to MAC table entry.
     */
    fsp_err_t (* macTableSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mactab_entry_t * p_mac_tab);

    /** Retrieves the port mask for the given MAC address from the static MAC table.
     * @par Implemented as
     * - @ref R_ETHSW_MacTableGet()
     *
     * @param[in]       p_api_ctrl       Pointer to control structure.
     * @param[in,out]   p_mac_tab        Pointer to MAC table entry.
     */
    fsp_err_t (* macTableGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mactab_entry_t * p_mac_tab);

    /** Configures the MAC learning and aging parameters of MAC table.
     * @par Implemented as
     * - @ref R_ETHSW_MacTableConfSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_mac_tab_cnf    Pointer to MAC table config.
     */
    fsp_err_t (* macTableConfSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mactab_conf_t * p_mac_tab_cnf);

    /** Retrieves the MAC learning and aging parameters of MAC table.
     * @par Implemented as
     * - @ref R_ETHSW_MacTableConfGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[out] p_mac_tab_cnf    Pointer to MAC table config.
     */
    fsp_err_t (* macTableConfGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mactab_conf_t * p_mac_tab_cnf);

    /** Clears specific types of entries from the MAC table or clears the whole table.
     * @par Implemented as
     * - @ref R_ETHSW_MacTableClear()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_mac_tab_clr    Pointer to MAC table clear mode.
     */
    fsp_err_t (* macTableClear)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mactab_clr_modes_t * p_mac_tab_clr);

    /** Sets learning state for the given port.
     * @par Implemented as
     * - @ref R_ETHSW_LearningSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_flag_learn     Pointer to learning flag.
     */
    fsp_err_t (* learningSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_dlr_learn);

    /** Gets learning state for the given port.
     * @par Implemented as
     * - @ref R_ETHSW_LearningGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_flag_learn     Pointer to learning flag.
     */
    fsp_err_t (* learningGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_dlr_learn);

    /** Enables the forwarding of incoming frames on a port for RSTP.
     * @par Implemented as
     * - @ref R_ETHSW_PortForwardAdd()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     */
    fsp_err_t (* portForwardAdd)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port);

    /** Disables the forwarding of incoming frames on a port for RSTP.
     * @par Implemented as
     * - @ref R_ETHSW_PortForwardDel()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     */
    fsp_err_t (* portForwardDel)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port);

    /** Sets the Discard Unknown Destination Bit in Control register.
     * @par Implemented as
     * - @ref R_ETHSW_DiscardUnknownSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* discardUnknownSet)(ethsw_ctrl_t * const p_api_ctrl, bool * p_flag_enable);

    /** Gets the Discard Unknown Destination Bit in Control register.
     * @par Implemented as
     * - @ref R_ETHSW_DiscardUnknownGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[out] p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* discardUnknownGet)(ethsw_ctrl_t * const p_api_ctrl, bool * p_flag_enable);

    /** Sets the flood domain port masks for frames with unknown destinations.
     * @par Implemented as
     * - @ref  R_ETHSW_FloodUnknownSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  p_flood_unk_conf Pointer to configuration of flood domain for frames with unknown destination.
     */
    fsp_err_t (* floodUnknownSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_flood_unk_conf_t * p_flood_unk_conf);

    /** Gets the flood domain port masks for frames with unknown destinations.
     * @par Implemented as
     * - @ref  R_ETHSW_FloodUnknownGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[out] p_flood_unk_conf Pointer to configuration of flood domain for frames with unknown destination.
     */
    fsp_err_t (* floodUnknownGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_flood_unk_conf_t * p_flood_unk_conf);

    /** Gets link state for given port.
     * @par Implemented as
     * - @ref  R_ETHSW_LinkStateGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_state_link     Pointer to Link state (up/down).
     */
    fsp_err_t (* linkStateGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_state_link);

    /** Sets admin state for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_PortAdminStateSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_state_port     Pointer to Port state (enable/disable).
     */
    fsp_err_t (* portAdminStateSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_state_port);

    /** Gets admin state for the given port
     * @par Implemented as
     * - @ref  R_ETHSW_PortAdminStateGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_state_port     Pointer to Port state (enable/disable).
     */
    fsp_err_t (* portAdminStateGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_state_port);

    /** Sets maximum frame size of given port.
     * @par Implemented as
     * - @ref  R_ETHSW_FrameSizeMaxSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[in]       p_frame_size_max        Pointer to maximum frame size.
     */
    fsp_err_t (* frameSizeMaxSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_frame_size_max);

    /** Gets maximum frame size of given port.
     * @par Implemented as
     * - @ref  R_ETHSW_FrameSizeMaxGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[out]      p_frame_size_max        Pointer to maximum frame size.
     */
    fsp_err_t (* frameSizeMaxGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_frame_size_max);

    /** Sets the default VLAN for input/output processing for the specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanDefaultSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_id_vlan        Pointer to VLAN ID.
     */
    fsp_err_t (* vlanDefaultSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint16_t * p_id_vlan);

    /** Gets the default VLAN for input/output processing for the specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanDefaultGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_id_vlan        Pointer to VLAN ID.
     */
    fsp_err_t (* vlanDefaultGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint16_t * p_id_vlan);

    /** Adds a VLAN for a given port to the VLAN domain table.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanPortAdd()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_id_vlan        Pointer to VLAN ID.
     */
    fsp_err_t (* vlanPortAdd)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint16_t * p_id_vlan);

    /** Removes a VLAN for a given port to the VLAN domain table.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanPortRemove()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_id_vlan        Pointer to VLAN ID.
     */
    fsp_err_t (* vlanPortRemove)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint16_t * p_id_vlan);

    /** Sets the mode of VLAN input operations for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanInModeSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_mode_vlan_in   Pointer to VLAN in mode.
     */
    fsp_err_t (* vlanInModeSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_vlan_in_mode_t * p_mode_vlan_in);

    /** Gets the mode of VLAN input operations for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanInModeGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_mode_vlan_in   Pointer to VLAN in mode.
     */
    fsp_err_t (* vlanInModeGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_vlan_in_mode_t * p_mode_vlan_in);

    /** Sets the mode of VLAN output operations for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanOutModeSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_mode_vlan_out  Pointer to VLAN out mode.
     */
    fsp_err_t (* vlanOutModeSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port,
                                 ethsw_vlan_out_mode_t * p_mode_vlan_out);

    /** Gets the mode of VLAN output operations for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanOutModeGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_mode_vlan_out  Pointer to VLAN out mode.
     */
    fsp_err_t (* vlanOutModeGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port,
                                 ethsw_vlan_out_mode_t * p_mode_vlan_out);

    /** Gets count of VLAN table entries.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanTableCount()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[out] p_cnt_tab_entries    Pointer to count of VLAN table entries
     */
    fsp_err_t (* vlanTableCount)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_cnt_tab_entries);

    /** Gets information of VLAN table entry.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanTableGet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[out] p_cnt_tab_entries    Pointer to information of VLAN table entry.
     */
    fsp_err_t (* vlanTableGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_vlan_entry_t * p_vlan);

    /** Sets VLAN verification for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanVerifySet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* vlanVerifySet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_enable);

    /** Gets VLAN verification for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanVerifyGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* vlanVerifyGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_enable);

    /** Sets VLAN discard of unknown frames for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanDiscardUnknownSet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* vlanDiscardUnknownSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_enable);

    /** Gets VLAN discard of unknown frames for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_VlanDiscardUnknownGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[out] p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* vlanDiscardUnknownGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_enable);

    /** Enable Tx timestamping for an external port.
     * @par Implemented as
     * - @ref  R_ETHSW_TimeStamp()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_flag_enable    Pointer to enable flag.
     */
    fsp_err_t (* timeStamp)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, bool * p_flag_enable);

    /** Gets the latancy between leaving PHY and timestamping of outgoing frames.
     * @par Implemented as
     * - @ref  R_ETHSW_TsLatencyEgressGet()
     *
     * @param[in]  p_api_ctrl       Pointer to control structure.
     * @param[in]  port             Port number.
     * @param[in]  p_latency_egress Pointer to egress latency.
     */
    fsp_err_t (* tsLatencyEgressGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_latency_egress);

    /** Gets the latency between receiving in PHY and timestamping of incoming frames
     * @par Implemented as
     * - @ref  R_ETHSW_TsLatencyIngressGet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  port                 Port number.
     * @param[in]  p_latency_ingress    Pointer to ingress latency.
     */
    fsp_err_t (* tsLatencyIngressGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_latency_ingress);

    /** Enables / disables the timer with the specified timer number.
     * @par Implemented as
     * - @ref  R_ETHSW_TsActive()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  port                 Port number.
     * @param[in]  p_time_enable        Pointer to timer enable/disable control.
     */
    fsp_err_t (* tsActive)(ethsw_ctrl_t * const p_api_ctrl, ethsw_time_enable_t * p_time_enable);

    /** Sets the current timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsSet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_timestamp          Pointer to timer value.
     */
    fsp_err_t (* tsSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_timestamp_t * p_timestamp);

    /** Gets the current timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsGet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[out] p_timestamp          Pointer to timer value.
     */
    fsp_err_t (* tsGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_timestamp_t * p_timestamp);

    /** Sets the domain number in the specified timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsDomainSet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_domain             Pointer to domain and timer mapping info.
     */
    fsp_err_t (* tsDomainSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ptp_domain_t * p_domain);

    /** Gets the domain number in the specified timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsDomainGet()
     *
     * @param[in]       p_api_ctrl      Pointer to control structure.
     * @param[in,out]   p_domain        Pointer to domain and timer mapping info.
     */
    fsp_err_t (* tsDomainGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ptp_domain_t * p_domain);

    /** Sets the peer delay value for the specified timer number of the specified port number.
     * @par Implemented as
     * - @ref  R_ETHSW_TsPeerDelaySet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  port                 Port number.
     * @param[in]  p_peerdelay          Pointer to peer delay info.
     */
    fsp_err_t (* tsPeerDelaySet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_ptp_peerdelay_t * p_peerdelay);

    /** Gets the peer delay value for the specified timer number of the specified port number.
     * @par Implemented as
     * - @ref  R_ETHSW_TsPeerDelayGet()
     *
     * @param[in]       p_api_ctrl      Pointer to control structure.
     * @param[in]       port            Port number.
     * @param[in,out]   p_peerdelay     Pointer to peer delay info.
     */
    fsp_err_t (* tsPeerDelayGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_ptp_peerdelay_t * p_peerdelay);

    /** Register a callback function that receives a send timestamp event.
     * @par Implemented as
     * - @ref  R_ETHSW_TsCallbackSet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_ptp_callback       Pointer to callback function called when an ISR occurs.
     */
    fsp_err_t (* tsCallbackSet)(ethsw_ctrl_t * const p_api_ctrl,
                                void (* p_ptp_callback)(ethsw_ptp_event_t event, uint8_t port,
                                                        ethsw_timestamp_t * p_timestamp));

    /** Sets offset corection for the specified timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsOffsetCorrectionSet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_offset             Pointer to offset correction parameter.
     */
    fsp_err_t (* tsOffsetCorrectionSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ts_offset_correction_t * p_offset);

    /** Sets clock rate corection for the specified timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsRateCorrectionSet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_rate               Pointer to rate correction parameter.
     */
    fsp_err_t (* tsRateCorrectionSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ts_rate_correction_t * p_rate);

    /** Gets clock rate corection for the specified timer.
     * @par Implemented as
     * - @ref  R_ETHSW_TsRateCorrectionGet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_rate               Pointer to rate correction parameter.
     */
    fsp_err_t (* tsRateCorrectionGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ts_rate_correction_t * p_rate);

    /** Sets enable/disable status of the TDMA scheduler.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaEnableSet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_tdma_enable        Pointer to TDMA enable info.
     */
    fsp_err_t (* tdmaEnableSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_enable_t * p_tdma_enable);

    /** Gets enable/disable status of the TDMA scheduler.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaEnableGet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[out] p_tdma_enable        Pointer to TDMA enable info.
     */
    fsp_err_t (* tdmaEnableGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_enable_t * p_tdma_enable);

    /** Sets TCV sequence entry.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaTcvSeqEntrySet()
     *
     * @param[in]  p_api_ctrl           Pointer to control structure.
     * @param[in]  p_tdma_seq_entry     Pointer to TDMA sequence entry info.
     */
    fsp_err_t (* tdmaTcvSeqEntrySet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_seq_entry_t * p_tdma_seq_entry);

    /** Gets TCV sequence entry.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaTcvSeqEntryGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in,out]   p_tdma_seq_entry    Pointer to TDMA sequence entry info.
     */
    fsp_err_t (* tdmaTcvSeqEntryGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_seq_entry_t * p_tdma_seq_entry);

    /** Sets TCV data entry.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaTcvDatEntrySet()
     *
     * @param[in]   p_api_ctrl          Pointer to control structure.
     * @param[in]   p_tdma_dat_entry    Pointer to TDMA data entry info.
     */
    fsp_err_t (* tdmaTcvDatEntrySet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_dat_entry_t * p_tdma_dat_entry);

    /** Gets TCV data entry.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaTcvDatEntryGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in,out]   p_tdma_dat_entry    Pointer to TDMA data entry info.
     */
    fsp_err_t (* tdmaTcvDatEntryGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_dat_entry_t * p_tdma_dat_entry);

    /** Sets start and last of TCV sequence entry.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaTcvSeqRangeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_tdma_dat_entry    Pointer to range info(start-last) of tdma sequence entry.
     */
    fsp_err_t (* tdmaTcvSeqRangeSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_seq_range_t * p_tdma_seq_range);

    /** Gets start and last of TCV sequence entry.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaTcvSeqRangeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_tdma_dat_entry    Pointer to range info(start-last) of tdma sequence entry.
     */
    fsp_err_t (* tdmaTcvSeqRangeGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_seq_range_t * p_tdma_seq_range);

    /** Sets TDMA GPIO mode.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaGpioModeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_tdma_gpio_mode    Pointer to TDMA_GPIO mode.
     */
    fsp_err_t (* tdmaGpioModeSet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_tdma_gpio_mode);

    /** Gets TDMA GPIO mode.
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaGpioModeGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_tdma_gpio_mode    Pointer to TDMA_GPIO mode.
     */
    fsp_err_t (* tdmaGpioModeGet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_tdma_gpio_mode);


    /**  Sets TDMA counter 0
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaCounter0Set()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_tdma_counter0      Pointer to TDMA Counter 0
     */
    fsp_err_t (* tdmaCounter0Set)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_tdma_counter0);

    /**  Gets TDMA counter 0
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaCounter0Get()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_tdma_counter0     Pointer to TDMA Counter 0
     */
    fsp_err_t (* tdmaCounter0Get)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_tdma_counter0);

    /**  Sets TDMA counter 1
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaCounter1Set()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_tdma_counter1     Pointer to parameter of TDMA Counter 1
     */
    fsp_err_t (* tdmaCounter1Set)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_counter1_t * p_tdma_counter1);

    /**  Gets TDMA counter 1
     * @par Implemented as
     * - @ref  R_ETHSW_TdmaCounter1Get()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_tdma_counter1     Pointer to parameter of TDMA Counter 1
     */
    fsp_err_t (* tdmaCounter1Get)(ethsw_ctrl_t * const p_api_ctrl, ethsw_tdma_counter1_t * p_tdma_counter1);

    /** Sets queue gate.
     * @par Implemented as
     * - @ref  R_ETHSW_MmctlQgateSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_mmctl_qgate       Pointer to queue gate info.
     */
    fsp_err_t (* mmctlQgateSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mmclt_qgate_t * p_mmctl_qgate);

    /** Sets memory pool size, in sells
     * @par Implemented as
     * - @ref   R_ETHSW_MmctlPoolSizeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_pool_size         Pointer to parameter of pool size.
     */
    fsp_err_t (* mmctlPoolSizeSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mmctl_pool_size_t * p_pool_size);

    /** Gets memory pool size, in sells
     * @par Implemented as
     * - @ref  R_ETHSW_MmctlPoolSizeGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_pool_size         Pointer to parameter of pool size.
     */
    fsp_err_t (* mmctlPoolSizeGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mmctl_pool_size_t * p_pool_size);

    /** Assigns a pool to the specified queue.
     * @par Implemented as
     * - @ref  R_ETHSW_MmctlQueueAssignSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_queue_assign      Pointer to parameter of queue assign.
     */
    fsp_err_t (* mmctlQueueAssignSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mmctl_queue_assign_t * p_queue_assign);

    /** Gets the pool assigned to the specified queue
     * @par Implemented as
     * - @ref  R_ETHSW_MmctlQueueAssignGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_queue_assign      Pointer to parameter of queue assign.
     */
    fsp_err_t (* mmctlQueueAssignGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mmctl_queue_assign_t * p_queue_assign);

    /** Sets length in bytes of the YELLOW period for specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_MmctYelowLengthSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number
     * @param[in]       p_yellow_length     Pointer to parameter of Byte length for yellow period.
     */
    fsp_err_t (* mmctlYellowLengthSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_yellow_length_t * p_yellow_length);

    /** Gets length in bytes of the YELLOW period for specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_MmctlQgateSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number
     * @param[in]       p_yellow_length     Pointer to parameter of Byte length for yellow period.
     */
    fsp_err_t (* mmctlYellowLengthGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_yellow_length_t * p_yellow_length);

    /** Sets Queue Flush Event.
     * @par Implemented as
     * - @ref  R_ETHSW_QueueFlushEventSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number
     * @param[in]       p_yellow_length     Pointer to parameter of Byte length for yellow period.
     */
    fsp_err_t (* queueFlushEventSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_queue_flush_event_t * p_queue_flush_event);

    /** Sets the dedicated port that will received all frames that are mirrored as well as the type of mirrored frames.
     * @par Implemented as
     * - @ref  R_ETHSW_MirrorSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_cnf_mirror        Pointer to mirror port configuration.
     */
    fsp_err_t (* mirrorSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mirror_conf_t * p_cnf_mirror);

    /** Gets the current port mirroring configuration.
     * @par Implemented as
     * - @ref  R_ETHSW_MirrorGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_cnf_mirror        Pointer to mirror port configuration.
     */
    fsp_err_t (* mirrorGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mirror_conf_t * p_cnf_mirror);

    /** Sets the priority resolution mode to use for a specific port.
     * It also defines the default priority if no priority resolution produced a result.
     * @par Implemented as
     * - @ref  R_ETHSW_QosModeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_mode_qos          Pointer to QoS mode.
     */
    fsp_err_t (* qosModeSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_qos_mode_t * p_mode_qos);

    /** Gets the priority resolution mode to use for a specific port.
     * @par Implemented as
     * - @ref  R_ETHSW_QosModeGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[out]      p_mode_qos          Pointer to QoS mode.
     */
    fsp_err_t (* qosModeGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_qos_mode_t * p_mode_qos);

    /** Sets the priority mapping between VLAN tag priorities and the switch core priorities.
     * @par Implemented as
     * - @ref  R_ETHSW_QosPrioValnSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_bm_prio_vlan      Pointer to VLAN priority bitmap.
     */
    fsp_err_t (* qosPrioValnSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_bm_prio_vlan);

    /** Gets the priority mapping between VLAN tag priorities and the switch core priorities.
     * @par Implemented as
     * - @ref  R_ETHSW_QosPrioValnGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in,out]   p_bm_prio_vlan      Pointer to VLAN priority bitmap.
     */
    fsp_err_t (* qosPrioValnGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_bm_prio_vlan);

    /** Sets the priority for the given DiffServ field of an IP packet for the given port.
     * Up to 64 different DiffServ entries may be defined per port.
     * @par Implemented as
     * - @ref  R_ETHSW_QosPrioIpSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_prio_qos_ip       Pointer to QoS IP priority.
     */
    fsp_err_t (* qosPrioIpSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_qos_prio_ip_t * p_prio_qos_ip);

    /** Gets the priority for the given DiffServ field of an IP packet for the given port.
     * @par Implemented as
     * - @ref  R_ETHSW_QosPrioIpGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in,out]   p_prio_qos_ip       Pointer to QoS IP priority.
     */
    fsp_err_t (* qosPrioIpGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_qos_prio_ip_t * p_prio_qos_ip);

    /** Sets the priority for the given Ethertypes to the given values.
     * @par Implemented as
     * - @ref  R_ETHSW_QosPrioTypeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_prio_qos_ethtype  Pointer to QoS Ethertype priority.
     */
    fsp_err_t (* qosPrioTypeSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_qos_prio_type_t * p_prio_qos_ethtype);

    /** Gets the priority for the given Ethertypes to the given values.
     * @par Implemented as
     * - @ref  R_ETHSW_QosPrioTypeGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[out]      p_prio_qos_ethtype  Pointer to QoS Ethertype priority.
     */
    fsp_err_t (* qosPrioTypeGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_qos_prio_type_t * p_prio_qos_ethtype);

    /** Gets the number of output queues per port.
     * @par Implemented as
     * - @ref  R_ETHSW_QueuesPerPortGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_out_queues        Pointer to number of output queues.
     */
    fsp_err_t (* queuesPerPortGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_out_queues);

    /** Sets the timeframe for the broadcast/multicast rate limiting in ms.
     * @par Implemented as
     * - @ref  R_ETHSW_StormTimeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_time_storm        Pointer to timeframe for storm protection.
     */
    fsp_err_t (* stormTimeSet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_time_storm);

    /** Gets the timeframe for the broadcast/multicast rate limiting in ms.
     * @par Implemented as
     * - @ref  R_ETHSW_StormTimeGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_time_storm        Pointer to timeframe for storm protection.
     */
    fsp_err_t (* stormTimeGet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_time_storm);

    /** Sets the max. allowed broadcast frames that can be received in a given time.
     * @par Implemented as
     * - @ref  R_ETHSW_BcastLimitSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_num_storm_frames  Pointer to number of frames for storm protection.
     */
    fsp_err_t (* bcastLimitSet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_num_storm_frames);

    /** Gets the max. allowed broadcast frames that can be received in a given time.
     * @par Implemented as
     * - @ref  R_ETHSW_BcastLimitGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_num_storm_frames  Pointer to number of frames for storm protection.
     */
    fsp_err_t (* bcastLimitGet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_num_storm_frames);

    /** Sets the max. allowed multicast frames that can be received in a given time.
     * @par Implemented as
     * - @ref  R_ETHSW_McastLimitSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_num_storm_frames  Pointer to number of frames for storm protection.
     */
    fsp_err_t (* mcastLimitSet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_num_storm_frames);

    /** Gets the max. allowed multicast frames that can be received in a given time.
     * @par Implemented as
     * - @ref  R_ETHSW_McastLimitGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[out]      p_num_storm_frames  Pointer to number of frames for storm protection.
     */
    fsp_err_t (* mcastLimitGet)(ethsw_ctrl_t * const p_api_ctrl, uint16_t * p_num_storm_frames);

    /** Sets the max. allowed bandwidth for the given port in percent.
     * @par Implemented as
     * - @ref  R_ETHSW_TxRateSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_rate              Pointer to rate (0-80%).
     */
    fsp_err_t (* txRateSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, float * p_rate);

    /** Gets the max. allowed bandwidth for the given port in percent.
     * @par Implemented as
     * - @ref  R_ETHSW_TxRateGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[out]      p_rate              Pointer to rate (0-80%).
     */
    fsp_err_t (* txRateGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, float * p_rate);

    /** Enables/disables and configure the Energy Efficient Ethernet Mode of the switch.
     * @par Implemented as
     * - @ref  R_ETHSW_EeeSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_cnf_eee           Pointer to EEE configuration.
     */
    fsp_err_t (* eeeSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_eee_t * p_cnf_eee);

    /** Gets the Energy Efficient Ethernet configuration of the switch.
     * @par Implemented as
     * - @ref  R_ETHSW_EeeGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[out]      p_cnf_eee           Pointer to EEE configuration.
     */
    fsp_err_t (* eeeGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, ethsw_eee_t * p_cnf_eee);

    /** Adds a snooping configuration.
     * @par Implemented as
     * - @ref  R_ETHSW_SnoopAdd()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_cnf_snoop         Pointer to snooping configuration.
     */
    fsp_err_t (* snoopAdd)(ethsw_ctrl_t * const p_api_ctrl, ethsw_snoop_config_t * p_cnf_snoop);

    /** Removes a snooping configuration.
     * @par Implemented as
     * - @ref  R_ETHSW_SnoopRemove()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       p_id_cnf_snoop      Pointer to snooping configuration ID.
     */
    fsp_err_t (* snoopRemove)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_id_cnf_snoop);

    /** Sets authorization state for given port.
     * @par Implemented as
     * - @ref  R_ETHSW_PortAuhSet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[in]       p_state_auth        Pointer to port authorization state.
     */
    fsp_err_t (* portAuhSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_state_auth);

    /** Gets authorization state for given port.
     * @par Implemented as
     * - @ref  R_ETHSW_PortAuhGet()
     *
     * @param[in]       p_api_ctrl          Pointer to control structure.
     * @param[in]       port                Port number.
     * @param[out]      p_state_auth        Pointer to port authorization state.
     */
    fsp_err_t (* portAuhGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_state_auth);

    /** Sets controlled direction for given port.
     * @par Implemented as
     * - @ref  R_ETHSW_PortCtrlDirSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[in]       p_state_dir_port_ctrl   Pointer to port control direction state.
     */
    fsp_err_t (* portCtrlDirSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_state_dir_port_ctrl);

    /** Gets controlled direction for given port.
     * @par Implemented as
     * - @ref  R_ETHSW_PortCtrlDirGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[out]      p_state_dir_port_ctrl   Pointer to port control direction state.
     */
    fsp_err_t (* portCtrlDirGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_state_dir_port_ctrl);

    /** Sets EAPOL reception mode for given port.
     * @par Implemented as
     * - @ref  R_ETHSW_PortEapolSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[in]       p_mode_eapol_recv       Pointer to EAPOL receive mode (enable/disable).
     */
    fsp_err_t (* portEapolSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_mode_eapol_recv);

    /** Gets EAPOL reception mode for given port
     * @par Implemented as
     * - @ref  R_ETHSW_PortEapolGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[out]      p_mode_eapol_recv       Pointer to EAPOL receive mode (enable/disable).
     */
    fsp_err_t (* portEapolGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_mode_eapol_recv);

    /** Sets Bridge Protocol Frame (BPDU) forwarding mode.
     * @par Implemented as
     * - @ref  R_ETHSW_BpduSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       p_state_bpdu_ctrl       Pointer to BPDU management control state.
     */
    fsp_err_t (* bpduSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_bpdu_ctrl_t * p_state_bpdu_ctrl);

    /** Gets Bridge Protocol Frame (BPDU) forwarding mode.
     * @par Implemented as
     * - @ref  R_ETHSW_BpduGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      p_state_bpdu_ctrl       Pointer to BPDU management control state.
     */
    fsp_err_t (* bpduGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_bpdu_ctrl_t * p_state_bpdu_ctrl);

    /** Initialize DLR module.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrInitSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       p_dlr_init              Pointer to DLR initilize info.
     */
    fsp_err_t (* dlrInitSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_dlr_init_t * p_dlr_init);

    /** Enable DLR module.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrEnableSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     */
    fsp_err_t (* dlrEnableSet)(ethsw_ctrl_t * const p_api_ctrl);

    /** Shutdown DLR module.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrShutdownSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     */
    fsp_err_t (* dlrShutdownSet)(ethsw_ctrl_t * const p_api_ctrl);

    /** Gets DLR last beacon status.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrLastBeaconGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       port                    Port number.
     * @param[out]      p_state_dlr             Pointer to DLR last beacon status.
     */
    fsp_err_t (* dlrLastBeaconGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_state_dlr);

    /** Gets DLR node status.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrNodeStateGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      p_state_dlr             Pointer to DLR node status.
     */
    fsp_err_t (* dlrNodeStateGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_state_dlr);

    /** Gets DLR ring status.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrRingStateGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      p_state_dlr             Pointer to DLR ring status.
     */
    fsp_err_t (* dlrRingStateGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_state_dlr);

    /** Gets IP address of DLR supervisor.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrSvIpGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      p_state_dlr             Pointer to IP address of DLR supervisor.
     */
    fsp_err_t (* dlrSvIpGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_state_dlr);

    /** Gets preference of DLR supervisor.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrSvPrecGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      p_state_dlr             Pointer to preference of DLR supervisor.
     */
    fsp_err_t (* dlrSvPrecGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_state_dlr);

    /** Gets VLAN ID of DLR beacon frame.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrVlanGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      p_state_dlr             Pointer to VLAN ID of DLR beacon frame.
     */
    fsp_err_t (* dlrVlanGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_state_dlr);

    /** Gets MAC address of DLR beacon frame.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrSvMacGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[out]      pp_addr_mac             Pointer to pointer to MAC address of DLR beacon frame.
     */
    fsp_err_t (* dlrSvMacGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_mac_addr_t * pp_addr_mac);

    /** Delete the static MAC address entry used by DLR.
     * @par Implemented as
     * - @ref  R_ETHSW_DlrMacEntryDel()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     */
    fsp_err_t (* dlrMacEntryDel)(ethsw_ctrl_t * const p_api_ctrl);

    /** Sets Rx pattern matcher.
     * @par Implemented as
     * - @ref  R_ETHSW_RxPatternMatcherSet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in]       p_rx_pattern_matcher    Pointer to parameter of Rx Pattern Matcher
     */
    fsp_err_t (* rxPatternMatcherSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_rx_pattern_matcher_t * p_rx_pattern_matcher);

    /** Gets Rx pattern matcher.
     * @par Implemented as
     * - @ref  R_ETHSW_RxPatternMatcherGet()
     *
     * @param[in]       p_api_ctrl              Pointer to control structure.
     * @param[in,out]   p_rx_pattern_matcher    Pointer to parameter of Rx Pattern Matcher
     */
    fsp_err_t (* rxPatternMatcherGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_rx_pattern_matcher_t * p_rx_pattern_matcher);

    /** Register the callback function for getting events from Rx pattern matcher.
     * @par Implemented as
     * - @ref  R_ETHSW_RxPatternMatcherCallback()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       p_rx_pattern_callback_func  Pointer to callback function called when an ISR occurs.
     */
    fsp_err_t (* rxPatternMatcherCallback)(ethsw_ctrl_t * const p_api_ctrl, 
                                           void (* p_rx_pattern_callback_func)(ethsw_rx_pattern_event_t        event,
                                                                               ethsw_rx_pattern_event_data_t * p_data));

    /** Initilize pulse generator.
     * @par Implemented as
     * - @ref  R_ETHSW_TsPulseGeneratorInit()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       p_time_num                  Pointer to timer number.
     */
    fsp_err_t (* pulseGeneratorInit)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_time_num);

    /** Sets pulse generator.
     * @par Implemented as
     * - @ref  R_ETHSW_TsPulseGeneratorSet()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       p_pulse                     Pointer to parameter of Pulse Generator
     */
    fsp_err_t (* pulseGeneratorSet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ts_pulse_generator_t * p_pulse);

    /** Gets pulse generator.
     * @par Implemented as
     * - @ref  R_ETHSW_TsPulseGeneratorGet()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       p_pulse                     Pointer to parameter of Pulse Generator
     */
    fsp_err_t (* pulseGeneratorGet)(ethsw_ctrl_t * const p_api_ctrl, ethsw_ts_pulse_generator_t * p_pulse);

    /** Sets enable cut-through for the specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_CutThroughSet()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       p_port_mask                 Pointer to port mask
     */
    fsp_err_t (* cutThroughSet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_port_mask);

    /** Gets enable cut-through for the specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_CutThroughGet()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       p_port_mask                 Pointer to port mask
     */
    fsp_err_t (* cutThroughGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t * p_port_mask);

    /** Sets the cut-through delay for the specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_CtDelaySet()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       port                        Port Number
     * @param[in]       p_ct_delay                  Pointer to Cout-Through delay
     */
    fsp_err_t (* ctDelaySet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_ct_delay);

    /** Gets the cut-through delay for the specified port.
     * @par Implemented as
     * - @ref  R_ETHSW_CtDelayGet()
     *
     * @param[in]       p_api_ctrl                  Pointer to control structure.
     * @param[in]       port                        Port Number
     * @param[in]       p_ct_delay                  Pointer to Cout-Through delay
     */
    fsp_err_t (* ctDelayGet)(ethsw_ctrl_t * const p_api_ctrl, uint32_t port, uint32_t * p_ct_delay);

} ethsw_api_t;


/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ethsw_instance
{
    ethsw_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    ethsw_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    ethsw_api_t const * p_api;         ///< Pointer to the API structure for this instance
} ethsw_instance_t;


/*******************************************************************************************************************//**
 * @} (end addtogroup ETHSW_API)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ETHSW_API_H */
