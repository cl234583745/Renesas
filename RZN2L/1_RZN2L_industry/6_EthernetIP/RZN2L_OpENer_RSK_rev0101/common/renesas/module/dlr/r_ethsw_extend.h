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
 * @addtogroup ETHSW
 * @{
 **********************************************************************************************************************/

#ifndef R_ETHSW_H
#define R_ETHSW_H

#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ethsw_cfg.h"
#if 0
#include "r_ethsw_api.h"
#else
#include "r_ethsw_extend_api.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ETHSW_CODE_VERSION_MAJOR    (0U)
#define ETHSW_CODE_VERSION_MINOR    (8U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** ETHER SWITCH control block. DO NOT INITIALIZE.  Initialization occurs when @ref ethsw_api_t::open is called. */
typedef struct st_ethsw_instance_ctrl
{
    uint32_t open;                     ///< Used to determine if the channel is configured

    /* Configuration of Ethernet SWITCH-LSI module. */
    ethsw_cfg_t const * p_switch_cfg;  ///< Pointer to initial configurations.

    /* Interface for Ethernet Swith */
    R_ETHSW_Type *      p_reg_switch;       ///< Pointer to Ethernet Switch peripheral registers.
    R_ETHSS_Type *      p_reg_ethss;        ///< Pointer to Ethernet Subsystem peripheral registers.
    R_ETHSW_PTP_Type *  p_reg_ethsw_ptp;    ///< Pointer to PTP Timer Pulse Control Registers.
} ethsw_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ethsw_api_t g_ethsw_on_ethsw;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHSW_Open(ethsw_ctrl_t * const p_ctrl, ethsw_cfg_t const * const p_cfg);

fsp_err_t R_ETHSW_Close(ethsw_ctrl_t * const p_ctrl);

fsp_err_t R_ETHSW_SpeedCfg(ethsw_ctrl_t * const p_ctrl, uint8_t const port, ethsw_link_speed_t const speed);

fsp_err_t R_ETHSW_VersionGet(fsp_version_t * const p_version);

fsp_err_t R_ETHSW_PortCount(ethsw_ctrl_t * const p_ctrl,                       ///< Pointer to control structure.
                            uint32_t           * p_cnt_ports);                 ///< Pointer to count of port

fsp_err_t R_ETHSW_MacTableSet(ethsw_ctrl_t * const   p_ctrl,                   ///< Pointer to control structure.
                              ethsw_mactab_entry_t * p_mac_tab);               ///< MAC table entry ptr

fsp_err_t R_ETHSW_MacTableGet(ethsw_ctrl_t * const   p_ctrl,                   ///< Pointer to control structure.
                              ethsw_mactab_entry_t * p_mac_tab);               ///< MAC table entry ptr

fsp_err_t R_ETHSW_MacTableConfSet(ethsw_ctrl_t * const  p_ctrl,                ///< Pointer to control structure.
                                  ethsw_mactab_conf_t * p_mac_tab_cnf);        ///< MAC table config ptr

fsp_err_t R_ETHSW_MacTableConfGet(ethsw_ctrl_t * const  p_ctrl,                ///< Pointer to control structure.
                                  ethsw_mactab_conf_t * p_mac_tab_cnf);        ///< MAC table config ptr

fsp_err_t R_ETHSW_MacTableClear(ethsw_ctrl_t * const       p_ctrl,             ///< Pointer to control structure.
                                ethsw_mactab_clr_modes_t * p_mac_tab_clr);     ///< MAC table clear modes ptr

fsp_err_t R_ETHSW_LearningSet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                              uint32_t             port,                       ///< port or port mask
                              bool               * p_flag_dlr_learn);          ///< DLR learning flag

fsp_err_t R_ETHSW_LearningGet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                              uint32_t             port,                       ///< port or port mask
                              bool               * p_flag_dlr_learn);          ///< DLR learning flag

fsp_err_t R_ETHSW_PortForwardAdd(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t             port);                   ///< port or port mask

fsp_err_t R_ETHSW_PortForwardDel(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t             port);                   ///< port or port mask

fsp_err_t R_ETHSW_DiscardUnknownSet(ethsw_ctrl_t * const p_ctrl,               ///< Pointer to control structure.
                                    bool               * p_flag_enable);       ///< enable flag

fsp_err_t R_ETHSW_DiscardUnknownGet(ethsw_ctrl_t * const p_ctrl,               ///< Pointer to control structure.
                                    bool               * p_flag_enable);       ///< enable flag

fsp_err_t R_ETHSW_FloodUnknownSet(ethsw_ctrl_t * const     p_ctrl,             ///< Pointer to control structure.
                                  ethsw_flood_unk_conf_t * p_flood_unk_conf);  ///< configuration of flood domain for frames with unknown destination

fsp_err_t R_ETHSW_FloodUnknownGet(ethsw_ctrl_t * const     p_ctrl,             ///< Pointer to control structure.
                                  ethsw_flood_unk_conf_t * p_flood_unk_conf);  ///< configuration of flood domain for frames with unknown destination

fsp_err_t R_ETHSW_LinkStateGet(ethsw_ctrl_t * const p_ctrl,                    ///< Pointer to control structure.
                               uint32_t             port,                      ///< port or port mask
                               uint32_t           * p_state_link);             ///< link state (up/down)

fsp_err_t R_ETHSW_PortAdminStateSet(ethsw_ctrl_t * const p_ctrl,               ///< Pointer to control structure.
                                    uint32_t             port,                 ///< port or port mask
                                    bool               * p_state_port);        ///< port state (on/off)

fsp_err_t R_ETHSW_PortAdminStateGet(ethsw_ctrl_t * const p_ctrl,               ///< Pointer to control structure.
                                    uint32_t             port,                 ///< port or port mask
                                    bool               * p_state_port);        ///< port state (on/off)

fsp_err_t R_ETHSW_FrameSizeMaxSet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                  uint32_t             port,                     ///< port or port mask
                                  uint32_t           * p_frame_size_max);        ///< maximum frame size

fsp_err_t R_ETHSW_FrameSizeMaxGet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                  uint32_t             port,                     ///< port or port mask
                                  uint32_t           * p_frame_size_max);        ///< maximum frame size

fsp_err_t R_ETHSW_VlanDefaultSet(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t             port,                    ///< port or port mask
                                 uint16_t           * p_id_vlan);              ///< VLAN id

fsp_err_t R_ETHSW_VlanDefaultGet(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t             port,                    ///< port or port mask
                                 uint16_t           * p_id_vlan);              ///< VLAN id

fsp_err_t R_ETHSW_VlanPortAdd(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                              uint32_t             port,                       ///< port or port mask
                              uint16_t           * p_id_vlan);                 ///< VLAN id

fsp_err_t R_ETHSW_VlanPortRemove(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t             port,                    ///< port or port mask
                                 uint16_t           * p_id_vlan);              ///< VLAN id

fsp_err_t R_ETHSW_VlanInModeSet(ethsw_ctrl_t * const   p_ctrl,                 ///< Pointer to control structure.
                                uint32_t               port,                   ///< port or port mask
                                ethsw_vlan_in_mode_t * p_mode_vlan_in);        ///< VLAN in mode

fsp_err_t R_ETHSW_VlanInModeGet(ethsw_ctrl_t * const   p_ctrl,                 ///< Pointer to control structure.
                                uint32_t               port,                   ///< port or port mask
                                ethsw_vlan_in_mode_t * p_mode_vlan_in);        ///< VLAN in mode

fsp_err_t R_ETHSW_VlanOutModeSet(ethsw_ctrl_t * const    p_ctrl,               ///< Pointer to control structure.
                                 uint32_t                port,                 ///< port or port mask
                                 ethsw_vlan_out_mode_t * p_mode_vlan_out);     ///< VLAN out mode

fsp_err_t R_ETHSW_VlanOutModeGet(ethsw_ctrl_t * const    p_ctrl,               ///< Pointer to control structure.
                                 uint32_t                port,                 ///< port or port mask
                                 ethsw_vlan_out_mode_t * p_mode_vlan_out);     ///< VLAN out mode

fsp_err_t R_ETHSW_VlanTableCount(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t           * p_cnt_tab_entries);      ///< table entries

fsp_err_t R_ETHSW_VlanTableGet(ethsw_ctrl_t * const p_ctrl,                    ///< Pointer to control structure.
                               ethsw_vlan_entry_t * p_vlan);                   ///< VLAN entry

fsp_err_t R_ETHSW_VlanVerifySet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                uint32_t             port,                     ///< port or port mask
                                bool               * p_flag_enable);           ///< enable flag

fsp_err_t R_ETHSW_VlanVerifyGet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                uint32_t             port,                     ///< port or port mask
                                bool               * p_flag_enable);           ///< enable flag

fsp_err_t R_ETHSW_VlanDiscardUnknownSet(ethsw_ctrl_t * const p_ctrl,           ///< Pointer to control structure.
                                        uint32_t             port,             ///< port or port mask
                                        bool               * p_flag_enable);   ///< enable flag

fsp_err_t R_ETHSW_VlanDiscardUnknownGet(ethsw_ctrl_t * const p_ctrl,           ///< Pointer to control structure.
                                        uint32_t             port,             ///< port or port mask
                                        bool               * p_flag_enable);   ///< enable flag

fsp_err_t R_ETHSW_TimeStamp(ethsw_ctrl_t * const p_ctrl,                       ///< Pointer to control structure.
                            uint32_t             port,                         ///< port or port mask
                            bool               * p_flag_enable);               ///< enable flag

fsp_err_t R_ETHSW_TsLatencyEgressGet(ethsw_ctrl_t * const p_ctrl,              ///< Pointer to control structure.
                                     uint32_t             port,                ///< port or port mask
                                     uint32_t           * p_latency_egress);   ///< egress latency

fsp_err_t R_ETHSW_TsLatencyIngressGet(ethsw_ctrl_t * const p_ctrl,             ///< Pointer to control structure.
                                      uint32_t             port,               ///< port or port mask
                                      uint32_t           * p_latency_ingress); ///< ingress latency

fsp_err_t R_ETHSW_TsActive(ethsw_ctrl_t * const  p_ctrl,                       ///< Pointer to control structure.
                           ethsw_time_enable_t * p_time_enable);               ///< timer enable/disable control ptr

fsp_err_t R_ETHSW_TsSet(ethsw_ctrl_t * const p_ctrl,                           ///< Pointer to control structure.
                        ethsw_timestamp_t  * p_timestamp);                     ///< timer value ptr

fsp_err_t R_ETHSW_TsGet(ethsw_ctrl_t * const p_ctrl,                           ///< Pointer to control structure.
                        ethsw_timestamp_t  * p_timestamp);                     ///< timer value ptr

fsp_err_t R_ETHSW_TsDomainSet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                              ethsw_ptp_domain_t * p_domain);                  ///< domain & Timer mapping info ptr

fsp_err_t R_ETHSW_TsDomainGet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                              ethsw_ptp_domain_t * p_domain);                  ///< domain & Timer mapping info ptr

fsp_err_t R_ETHSW_TsPeerDelaySet(ethsw_ctrl_t * const    p_ctrl,               ///< Pointer to control structure.
                                 uint32_t                port,                 ///< port or port mask
                                 ethsw_ptp_peerdelay_t * p_peerdelay);         ///< peer delay info ptr

fsp_err_t R_ETHSW_TsPeerDelayGet(ethsw_ctrl_t * const    p_ctrl,               ///< Pointer to control structure.
                                 uint32_t                port,                 ///< port or port mask
                                 ethsw_ptp_peerdelay_t * p_peerdelay);         ///< peer delay info ptr

fsp_err_t R_ETHSW_TsCallbackSet(ethsw_ctrl_t * const    p_ctrl,                ///< Pointer to control structure.
                                void (                * p_ptp_callback)(       ///< Callback provided when an ISR occurs.
                                    ethsw_ptp_event_t   event,
                                    uint8_t             port,
                                    ethsw_timestamp_t * p_timestamp));

fsp_err_t R_ETHSW_TsOffsetCorrectionSet (
                                ethsw_ctrl_t * const           p_ctrl,         ///< Pointer to control structure.
                                ethsw_ts_offset_correction_t * p_offset);      ///< Offset correction parameter.

fsp_err_t R_ETHSW_TsRateCorrectionSet (ethsw_ctrl_t * const         p_ctrl,    ///< Pointer to control structure.
                                       ethsw_ts_rate_correction_t * p_rate);   ///< Rate correction parameter.

fsp_err_t R_ETHSW_TsRateCorrectionGet (ethsw_ctrl_t * const         p_ctrl,    ///< Pointer to control structure.
                                       ethsw_ts_rate_correction_t * p_rate);   ///< Rate correction parameter.

fsp_err_t R_ETHSW_TdmaEnableSet(ethsw_ctrl_t * const  p_ctrl,                    ///< Pointer to control structure.
                                ethsw_tdma_enable_t * p_tdma_enable);            ///< tdma enable info

fsp_err_t R_ETHSW_TdmaEnableGet(ethsw_ctrl_t * const  p_ctrl,                    ///< Pointer to control structure.
                                ethsw_tdma_enable_t * p_tdma_enable);            ///< tdma enable info

fsp_err_t R_ETHSW_TdmaTcvSeqEntrySet(ethsw_ctrl_t * const     p_ctrl,            ///< Pointer to control structure.
                                     ethsw_tdma_seq_entry_t * p_tdma_seq_entry); ///< tdma sequence entry info

fsp_err_t R_ETHSW_TdmaTcvSeqEntryGet(ethsw_ctrl_t * const     p_ctrl,            ///< Pointer to control structure.
                                     ethsw_tdma_seq_entry_t * p_tdma_seq_entry); ///< tdma sequence entry info

fsp_err_t R_ETHSW_TdmaTcvDatEntrySet(ethsw_ctrl_t * const     p_ctrl,            ///< Pointer to control structure.
                                     ethsw_tdma_dat_entry_t * p_tdma_dat_entry); ///< tdma data entry info

fsp_err_t R_ETHSW_TdmaTcvDatEntryGet(ethsw_ctrl_t * const     p_ctrl,            ///< Pointer to control structure.
                                     ethsw_tdma_dat_entry_t * p_tdma_dat_entry); ///< tdma data entry info

fsp_err_t R_ETHSW_TdmaTcvSeqRangeSet(ethsw_ctrl_t * const     p_ctrl,            ///< Pointer to control structure.
                                     ethsw_tdma_seq_range_t * p_tdma_seq_range); ///< range(start-last) of tdma sequence entry

fsp_err_t R_ETHSW_TdmaTcvSeqRangeGet(ethsw_ctrl_t * const     p_ctrl,            ///< Pointer to control structure.
                                     ethsw_tdma_seq_range_t * p_tdma_seq_range); ///< range(start-last) of tdma sequence entry

fsp_err_t R_ETHSW_TdmaGpioModeSet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                  uint16_t           * p_tdma_gpio_mode);        ///< TDMA_GPIO mode

fsp_err_t R_ETHSW_TdmaGpioModeGet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                  uint16_t           * p_tdma_gpio_mode);        ///< TDMA_GPIO mode

fsp_err_t R_ETHSW_TdmaCounter0Set (ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                   uint32_t *           p_tdma_counter0);        ///< Value of TDMA Counter 0

fsp_err_t R_ETHSW_TdmaCounter0Get (ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                   uint32_t *           p_tdma_counter0);        ///<  Value of TDMA Counter 0

fsp_err_t R_ETHSW_TdmaCounter1Set (ethsw_ctrl_t * const     p_ctrl,              ///< Pointer to control structure.
                                   ethsw_tdma_counter1_t *  p_tdma_counter1);    ///< Parameter of TDMA Counter 1

fsp_err_t R_ETHSW_TdmaCounter1Get (ethsw_ctrl_t * const     p_ctrl,              ///< Pointer to control structure.
                                   ethsw_tdma_counter1_t *  p_tdma_counter1);    ///< Parameter of TDMA Counter 1

fsp_err_t R_ETHSW_MmctlQgateSet(ethsw_ctrl_t * const  p_ctrl,                    ///< Pointer to control structure.
                                ethsw_mmclt_qgate_t * p_mmctl_qgate);            ///< queue gate.

fsp_err_t R_ETHSW_MmctlPoolSizeSet (ethsw_ctrl_t * const      p_ctrl,            ///< Pointer to control structure.
                                    ethsw_mmctl_pool_size_t * p_pool_size);      ///< Parameter of pool size.

fsp_err_t R_ETHSW_MmctlPoolSizeGet (ethsw_ctrl_t * const      p_ctrl,            ///< Pointer to control structure.
                                    ethsw_mmctl_pool_size_t * p_pool_size);      ///< Parameter of pool size.

fsp_err_t R_ETHSW_MmctlQueueAssignSet (
                            ethsw_ctrl_t * const         p_ctrl,                 ///< Pointer to control structure.
                            ethsw_mmctl_queue_assign_t * p_queue_assign);        ///< Parameter of queue assign.

fsp_err_t R_ETHSW_MmctlQueueAssignGet (
                            ethsw_ctrl_t * const         p_ctrl,                 ///< Pointer to control structure.
                            ethsw_mmctl_queue_assign_t * p_queue_assign);        ///< Parameter of queue assign.

fsp_err_t R_ETHSW_MmctlYellowLengthSet (
                            ethsw_ctrl_t * const      p_ctrl,                    ///< Pointer to control structure.
                            uint32_t                  port,                      ///< Port number (0..2)
                            ethsw_yellow_length_t *   p_yellow_length);          ///< Parameter of Byte length for yellow period.

fsp_err_t R_ETHSW_MmctlYellowLengthGet (
                            ethsw_ctrl_t * const      p_ctrl,                    ///< Pointer to control structure.
                            uint32_t                  port,                      ///< Port number (0..2)
                            ethsw_yellow_length_t *   p_yellow_length);          ///< Parameter of Byte length for yellow period.

fsp_err_t R_ETHSW_QueueFlushEventSet (
                            ethsw_ctrl_t * const          p_ctrl,                ///< Pointer to control structure.
                            ethsw_queue_flush_event_t *   p_queue_flush_event);  ///< Parameter of queue_flush_event.

fsp_err_t R_ETHSW_MirrorSet(ethsw_ctrl_t * const  p_ctrl,                        ///< Pointer to control structure.
                            ethsw_mirror_conf_t * p_cnf_mirror);                 ///< mirror port configuration

fsp_err_t R_ETHSW_MirrorGet(ethsw_ctrl_t * const  p_ctrl,                        ///< Pointer to control structure.
                            ethsw_mirror_conf_t * p_cnf_mirror);                 ///< mirror port configuration

fsp_err_t R_ETHSW_QosModeSet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                             uint32_t             port,                          ///< port or port mask
                             ethsw_qos_mode_t   * p_mode_qos);                   ///< QoS mode

fsp_err_t R_ETHSW_QosModeGet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                             uint32_t             port,                          ///< port or port mask
                             ethsw_qos_mode_t   * p_mode_qos);                   ///< QoS mode

fsp_err_t R_ETHSW_QosPrioValnSet(ethsw_ctrl_t * const p_ctrl,                    ///< Pointer to control structure.
                                 uint32_t             port,                      ///< port or port mask
                                 uint32_t           * p_bm_prio_vlan);           ///< VLAN priority bitmap

fsp_err_t R_ETHSW_QosPrioValnGet(ethsw_ctrl_t * const p_ctrl,                    ///< Pointer to control structure.
                                 uint32_t             port,                      ///< port or port mask
                                 uint32_t           * p_bm_prio_vlan);           ///< VLAN priority bitmap

fsp_err_t R_ETHSW_QosPrioIpSet(ethsw_ctrl_t * const  p_ctrl,                     ///< Pointer to control structure.
                               uint32_t              port,                       ///< port or port mask
                               ethsw_qos_prio_ip_t * p_prio_qos_ip);             ///< QoS IP priority

fsp_err_t R_ETHSW_QosPrioIpGet(ethsw_ctrl_t * const  p_ctrl,                     ///< Pointer to control structure.
                               uint32_t              port,                       ///< port or port mask
                               ethsw_qos_prio_ip_t * p_prio_qos_ip);             ///< QoS IP priority

fsp_err_t R_ETHSW_QosPrioTypeSet(ethsw_ctrl_t * const    p_ctrl,                 ///< Pointer to control structure.
                                 ethsw_qos_prio_type_t * p_prio_qos_ethtype);    ///< QoS Ethertype priority

fsp_err_t R_ETHSW_QosPrioTypeGet(ethsw_ctrl_t * const    p_ctrl,                 ///< Pointer to control structure.
                                 ethsw_qos_prio_type_t * p_prio_qos_ethtype);    ///< QoS Ethertype priority

fsp_err_t R_ETHSW_QueuesPerPortGet(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                   uint32_t           * p_out_queues);           ///< Number of output queues

fsp_err_t R_ETHSW_StormTimeSet(ethsw_ctrl_t * const p_ctrl,                      ///< Pointer to control structure.
                               uint16_t           * p_time_storm);               ///< timeframe for storm protection

fsp_err_t R_ETHSW_StormTimeGet(ethsw_ctrl_t * const p_ctrl,                      ///< Pointer to control structure.
                               uint16_t           * p_time_storm);               ///< timeframe for storm protection

fsp_err_t R_ETHSW_BcastLimitSet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                                uint16_t           * p_num_storm_frames);        ///< number of frames for storm protection

fsp_err_t R_ETHSW_BcastLimitGet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                                uint16_t           * p_num_storm_frames);        ///< number of frames for storm protection

fsp_err_t R_ETHSW_McastLimitSet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                                uint16_t           * p_num_storm_frames);        ///< number of frames for storm protection

fsp_err_t R_ETHSW_McastLimitGet(ethsw_ctrl_t * const p_ctrl,                     ///< Pointer to control structure.
                                uint16_t           * p_num_storm_frames);        ///< number of frames for storm protection

fsp_err_t R_ETHSW_TxRateSet(ethsw_ctrl_t * const p_ctrl,                         ///< Pointer to control structure.
                            uint32_t             port,                           ///< port or port mask
                            float              * p_rate);                        ///< rate

fsp_err_t R_ETHSW_TxRateGet(ethsw_ctrl_t * const p_ctrl,                         ///< Pointer to control structure.
                            uint32_t             port,                           ///< port or port mask
                            float              * p_rate);                        ///< rate

fsp_err_t R_ETHSW_EeeSet(ethsw_ctrl_t * const p_ctrl,                            ///< Pointer to control structure.
                         uint32_t             port,                              ///< port or port mask
                         ethsw_eee_t        * p_cnf_eee);                        ///< EEE configuration

fsp_err_t R_ETHSW_EeeGet(ethsw_ctrl_t * const p_ctrl,                            ///< Pointer to control structure.
                         uint32_t             port,                              ///< port or port mask
                         ethsw_eee_t        * p_cnf_eee);                        ///< EEE configuration

fsp_err_t R_ETHSW_SnoopAdd(ethsw_ctrl_t * const   p_ctrl,                        ///< Pointer to control structure.
                           ethsw_snoop_config_t * p_cnf_snoop);                  ///< IGMP snooping configuration

fsp_err_t R_ETHSW_SnoopRemove(ethsw_ctrl_t * const p_ctrl,                       ///< Pointer to control structure.
                              uint32_t           * p_id_cnf_snoop);              ///< IGMP snooping configuration id

fsp_err_t R_ETHSW_PortAuhSet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                             uint32_t             port,                          ///< port or port mask
                             uint32_t           * p_state_auth);                 ///< port authorization state

fsp_err_t R_ETHSW_PortAuhGet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                             uint32_t             port,                          ///< port or port mask
                             uint32_t           * p_state_auth);                 ///< port authorization state

fsp_err_t R_ETHSW_PortCtrlDirSet(ethsw_ctrl_t * const p_ctrl,                    ///< Pointer to control structure.
                                 uint32_t             port,                      ///< port or port mask
                                 uint32_t           * p_state_dir_port_ctrl);    ///< port control direction state

fsp_err_t R_ETHSW_PortCtrlDirGet(ethsw_ctrl_t * const p_ctrl,                    ///< Pointer to control structure.
                                 uint32_t             port,                      ///< port or port mask
                                 uint32_t           * p_state_dir_port_ctrl);    ///< port control direction state

fsp_err_t R_ETHSW_PortEapolSet(ethsw_ctrl_t * const p_ctrl,                      ///< Pointer to control structure.
                               uint32_t             port,                        ///< port or port mask
                               uint32_t           * p_mode_eapol_recv);          ///< port EAPOL receive mode

fsp_err_t R_ETHSW_PortEapolGet(ethsw_ctrl_t * const p_ctrl,                      ///< Pointer to control structure.
                               uint32_t             port,                        ///< port or port mask
                               uint32_t           * p_mode_eapol_recv);          ///< port EAPOL receive mode

fsp_err_t R_ETHSW_BpduSet(ethsw_ctrl_t * const p_ctrl,                           ///< Pointer to control structure.
                          ethsw_bpdu_ctrl_t  * p_state_bpdu_ctrl);               ///< BPDU management control state

fsp_err_t R_ETHSW_BpduGet(ethsw_ctrl_t * const p_ctrl,                           ///< Pointer to control structure.
                          ethsw_bpdu_ctrl_t  * p_state_bpdu_ctrl);               ///< BPDU management control state

fsp_err_t R_ETHSW_DlrInitSet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                          ethsw_dlr_init_t   * p_dlr_init);                      ///< DLR initilize

fsp_err_t R_ETHSW_DlrEnableSet(ethsw_ctrl_t * const p_ctrl);                     ///< Pointer to control structure.

fsp_err_t R_ETHSW_DlrShutdownSet(ethsw_ctrl_t * const p_ctrl);                   ///< Pointer to control structure.

fsp_err_t R_ETHSW_DlrLastBeaconGet(ethsw_ctrl_t * const p_ctrl,                  ///< Pointer to control structure.
                                uint32_t             port,                       ///< port or port mask
                                uint32_t           * p_state_dlr);               ///< DLR status

fsp_err_t R_ETHSW_DlrNodeStateGet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                               uint32_t           * p_state_dlr);                ///< DLR status

fsp_err_t R_ETHSW_DlrRingStateGet(ethsw_ctrl_t * const p_ctrl,                   ///< Pointer to control structure.
                                  uint32_t           * p_state_dlr);             ///< DLR status

fsp_err_t R_ETHSW_DlrSvIpGet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                             uint32_t           * p_state_dlr);                  ///< DLR status

fsp_err_t R_ETHSW_DlrSvPrecGet(ethsw_ctrl_t * const p_ctrl,                      ///< Pointer to control structure.
                               uint32_t           * p_state_dlr);                ///< DLR status

fsp_err_t R_ETHSW_DlrVlanGet(ethsw_ctrl_t * const p_ctrl,                        ///< Pointer to control structure.
                             uint32_t           * p_state_dlr);                  ///< DLR status

fsp_err_t R_ETHSW_DlrSvMacGet(ethsw_ctrl_t * const p_ctrl,                       ///< Pointer to control structure.
                              ethsw_mac_addr_t   * pp_addr_mac);                 ///< MAC address ptr ref

fsp_err_t R_ETHSW_DlrMacEntryDel(ethsw_ctrl_t * const p_ctrl);                   ///< Pointer to control structure.

fsp_err_t R_ETHSW_RxPatternMatcherSet (
                            ethsw_ctrl_t * const         p_ctrl,                 ///< Pointer to control structure.
                            ethsw_rx_pattern_matcher_t * p_rx_pattern_matcher);  ///< Parameter of Rx Pattern Matcher

fsp_err_t R_ETHSW_RxPatternMatcherGet (
                            ethsw_ctrl_t * const         p_ctrl,                 ///< Pointer to control structure.
                            ethsw_rx_pattern_matcher_t * p_rx_pattern_matcher);  ///< Parameter of Rx Pattern Matcher

fsp_err_t R_ETHSW_RxPatternMatcherCallback (
                            ethsw_ctrl_t * const         p_ctrl,                 ///< Pointer to control structure.
                            void (* p_rx_pattern_callback_func)(                 ///< Callback provided when an ISR occurs.
                                    ethsw_rx_pattern_event_t        event,
                                    ethsw_rx_pattern_event_data_t * p_data));

fsp_err_t R_ETHSW_PulseGeneratorInit (ethsw_ctrl_t * const p_ctrl,               ///< Pointer to control structure.
                                      uint32_t *           p_time_num);          ///< Timer number

fsp_err_t R_ETHSW_PulseGeneratorSet (ethsw_ctrl_t * const         p_ctrl,        ///< Pointer to control structure.
                                     ethsw_ts_pulse_generator_t * p_pulse);      ///< Parameter of Pulse Generator

fsp_err_t R_ETHSW_PulseGeneratorGet (ethsw_ctrl_t * const         p_ctrl,        ///< Pointer to control structure.
                                     ethsw_ts_pulse_generator_t * p_pulse);      ///< Parameter of Pulse Generator

fsp_err_t R_ETHSW_CutThroughSet (ethsw_ctrl_t * const   p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t *             p_port_mask);            ///< Port mask

fsp_err_t R_ETHSW_CutThroughGet (ethsw_ctrl_t * const   p_ctrl,                  ///< Pointer to control structure.
                                 uint32_t *             p_port_mask);            ///< Port mask

fsp_err_t R_ETHSW_CtDelaySet (ethsw_ctrl_t * const  p_ctrl,                      ///< Pointer to control structure.
                              uint32_t              port,                        ///< Port Number
                              uint32_t *            p_ct_delay);                 ///< Cout-Through delay

fsp_err_t R_ETHSW_CtDelayGet (ethsw_ctrl_t * const  p_ctrl,                      ///< Pointer to control structure.
                              uint32_t              port,                        ///< Port Number
                              uint32_t *            p_ct_delay);                 ///< Cout-Through delay

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHSW)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_ETHSW_H
