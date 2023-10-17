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

#ifndef UM_OPENER_PORT_INTERNAL_H
#define UM_OPENER_PORT_INTERNAL_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/
/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Functions Prototypes
 **********************************************************************************************************************/
/**
 * OpENer CIP object layer interface.
 */
usr_err_t um_opener_port_cip_open( opener_port_cip_ctrl_t * const p_ctrl, opener_port_cip_user_device_cfg_t const * const p_cfg );
usr_err_t um_opener_port_cip_close( opener_port_cip_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_cip_set_device_status_from_module_state( opener_port_cip_ctrl_t * const p_ctrl,
                                                                  opener_port_module_state_t current_state,
                                                                  opener_port_module_state_t new_state );
usr_err_t um_opener_port_cip_set_device_status_from_network_state( opener_port_cip_ctrl_t * const p_ctrl,  opener_port_network_state_t state );

/**
 * OpENer encapsulation layer interface.
 */
usr_err_t um_opener_port_encap_open( opener_port_encap_ctrl_t * const p_ctrl,
                                     opener_port_tcpip_netif_ctrl_t * const p_tcpip_netif_ctrl );
usr_err_t um_opener_port_encap_check_any_connections_exist( opener_port_encap_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_encap_find_socket_in_connection_list( opener_port_encap_ctrl_t * const p_ctrl, int32_t target_socket );
usr_err_t um_opener_port_encap_handle_explicit_udp_packet ( opener_port_encap_ctrl_t * p_ctrl,
                                                            int32_t explicit_udp_socket,
                                                            uint8_t * p_incoming_message_buffer,
                                                            int32_t recv_size,
                                                            struct sockaddr_in * p_src_address,
                                                            struct sockaddr_in * p_dst_address );
usr_err_t um_opener_port_encap_handle_implicit_udp_packet ( opener_port_encap_ctrl_t * p_ctrl,
                                                            uint8_t * p_incoming_message_buffer,
                                                            int32_t recv_size,
                                                            struct sockaddr_in * p_src_address );
usr_err_t um_opener_port_encap_handle_explicit_tcp_packet ( opener_port_encap_ctrl_t * p_ctrl,
                                                            int32_t explicit_tcp_socket,
                                                            uint8_t * p_incoming_message_buffer,
                                                            int32_t recv_size,
                                                            struct sockaddr_in * p_src_address );
usr_err_t um_opener_port_encap_check_socket_timer_timeout ( opener_port_encap_ctrl_t * p_ctrl, int32_t socket, uint32_t timeout_ms );
usr_err_t um_opener_port_encap_manage_io_connections( opener_port_encap_ctrl_t * const p_ctrl,  uint32_t elapsed_time );
usr_err_t um_opener_port_encap_remove_tcp_session ( opener_port_encap_ctrl_t * p_ctrl, int32_t explicit_tcp_socket );
usr_err_t um_opener_port_encap_set_actual_time ( opener_port_encap_ctrl_t * const p_ctrl, uint32_t * p_elapsed_time );
usr_err_t um_opener_port_encap_update_socket_timer ( opener_port_encap_ctrl_t * p_ctrl, int32_t socket );
usr_err_t um_opener_port_encap_clear_socket_timer ( opener_port_encap_ctrl_t * p_ctrl, int32_t socket );

/**
 * Ethernet netif module interface.
 */
usr_err_t um_opener_port_ether_netif_open( opener_port_ether_netif_ctrl_t * const p_ctrl,
                                           ether_netif_instance_t const * const p_ether_netif_instance );
usr_err_t um_opener_port_ether_netif_start( opener_port_ether_netif_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_ether_netif_stop( opener_port_ether_netif_ctrl_t * const p_ctrl );

/**
 * lwIP porting module interface.
 */
usr_err_t um_opener_port_lwip_port_open( opener_port_lwip_port_ctrl_t * const p_ctrl,
                                         lwip_port_instance_t const * const p_lwip_port_instance,
                                         opener_port_cip_user_device_cfg_t const * const p_cip_device_cfg,
                                         opener_port_cip_settable_nvdata_t const * const p_cip_nvdata_cfg);
usr_err_t um_opener_port_lwip_port_start( opener_port_lwip_port_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_lwip_port_stop( opener_port_lwip_port_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_lwip_port_wait_netif_up( opener_port_lwip_port_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_lwip_port_netif_is_up( opener_port_lwip_port_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_lwip_port_get_netif_config( opener_port_lwip_port_ctrl_t * const p_ctrl, lwip_port_netif_cfg_t * const p_netif_cfg );
usr_err_t um_opener_port_lwip_port_get_encap_inactivity_timeout( opener_port_lwip_port_ctrl_t * const p_ctrl,  uint32_t * p_timeout_sec );
usr_err_t um_opener_port_lwip_port_get_mcast_address_and_ttl_value( opener_port_lwip_port_ctrl_t * const p_ctrl,
                                                                    uint32_t * p_mcast_starting_address,
                                                                    uint8_t * p_mcast_ttl_value );
usr_err_t um_opener_port_lwip_port_get_cip_tcpip_nvdata( opener_port_lwip_port_ctrl_t * const p_ctrl,
                                                        opener_port_cip_tcpip_nvdata_t * const p_cip_tcpip_cfg );

/**
 * Indicator task interface.
 */
usr_err_t um_opener_port_task_indicator_open( opener_port_indicator_ctrl_t * const p_ctrl,
                                              opener_port_callback_ctrl_t * const p_callback_ctrl,
                                              opener_port_cip_ctrl_t * const p_cip_ctrl );
usr_err_t um_opener_port_task_indicator_start( opener_port_indicator_ctrl_t *	const p_ctrl );
usr_err_t um_opener_port_task_indicator_stop( opener_port_indicator_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_task_indicator_get_network_state( opener_port_indicator_ctrl_t * const p_ctrl,
                                                           opener_port_network_state_t * const p_network_state );
usr_err_t um_opener_port_task_indicator_set_network_state( opener_port_indicator_ctrl_t * const p_ctrl,
                                                           opener_port_network_state_t network_state);
usr_err_t um_opener_port_task_indicator_set_module_state( opener_port_indicator_ctrl_t * const p_ctrl,
                                                          opener_port_module_state_t module_state);

/**
 * OpENer port network interface task.
 */
usr_err_t um_opener_port_task_netif_open( opener_port_netif_ctrl_t * const p_ctrl,
                                          opener_port_lwip_port_ctrl_t * const p_lwip_port_ctrl,
                                          opener_port_tcpip_netif_ctrl_t * const p_tcpip_netif_ctrl,
                                          opener_port_encap_ctrl_t * const p_encap_ctrl,
                                          opener_port_indicator_ctrl_t * const p_indicator_ctrl );
usr_err_t um_opener_port_task_netif_close( opener_port_netif_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_task_netif_start( opener_port_netif_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_task_netif_stop( opener_port_netif_ctrl_t * const p_ctrl );

/**
 * lwIP socket API interface.
 */
usr_err_t um_opener_port_tcpip_netif_open( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                           opener_port_lwip_port_ctrl_t * const p_lwip_port_ctrl,
                                           opener_port_cip_settable_nvdata_t const * const p_cip_cfg );
usr_err_t um_opener_port_tcpip_netif_create_explicit_tcp_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t * const p_socket );
usr_err_t um_opener_port_tcpip_netif_create_explicit_udp_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t * const p_socket );
usr_err_t um_opener_port_tcpip_netif_create_implicit_udp_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                                 int32_t * const p_socket,
                                                                 uint8_t const qos_dscp,
                                                                 uint32_t multicast_address );
usr_err_t um_opener_port_tcpip_netif_close_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t socket );
usr_err_t um_opener_port_tcpip_netif_check_all_socket_events( opener_port_tcpip_netif_ctrl_t * const p_ctrl, uint32_t timeout_ms );
usr_err_t um_opener_port_tcpip_netif_check_socket_readable( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t socket );
usr_err_t um_opener_port_tcpip_netif_get_highest_socket( opener_port_tcpip_netif_ctrl_t * const p_ctrl, int32_t * p_highest_socket );
usr_err_t um_opener_port_tcpip_netif_accept_tcp_session ( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                          int32_t const listner_socket,
                                                          int32_t * const session_socket,
                                                          struct sockaddr * peer_address );
usr_err_t um_opener_port_tcpip_netif_send_tcp_packet ( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                       int32_t           socket,
                                                       const void      * p_buffer,
                                                       uint32_t          buffer_size,
                                                       int32_t         * p_sent_size);
usr_err_t um_opener_port_tcpip_netif_recv_tcp_packet ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                       int32_t socket,
                                                       void * p_buffer,
                                                       uint32_t buffer_size,
                                                       int32_t * p_recv_size);
usr_err_t um_opener_port_tcpip_netif_send_udp_packet ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                       int32_t           socket_handle,
                                                       const void      * buffer,
                                                       uint32_t          buffer_size,
                                                       struct sockaddr * address,
                                                       int32_t         * p_sent_size);
usr_err_t um_opener_port_tcpip_netif_recv_udp_packet ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                       int32_t socket,
                                                       void * p_buffer,
                                                       uint32_t buffer_size,
                                                       int32_t * p_recv_size,
                                                       struct sockaddr * p_src_addr,
                                                       struct sockaddr * p_dst_addr);
usr_err_t um_opener_port_tcpip_netif_get_remote_address ( opener_port_tcpip_netif_ctrl_t * p_ctrl,
                                                          int32_t socket_handle,
                                                          struct sockaddr * address );
usr_err_t um_opener_port_tcpip_netif_get_cip_qos_nvdata( opener_port_tcpip_netif_ctrl_t * const p_ctrl,
                                                          opener_port_cip_qos_nvdata_t * const p_cip_qos_cfg );

/**
 * User callback interface.
 */
usr_err_t um_opener_port_callback_open( opener_port_callback_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_callback_add( opener_port_callback_ctrl_t * const p_ctrl, opener_port_callback_link_node_t * p_new_nodes );
usr_err_t um_opener_port_callback_request( opener_port_callback_ctrl_t * const p_ctrl,
                                           opener_port_callback_event_t event,
                                           opener_port_indicator_status_t indicator_status);

/**
 * DLR module interface.
 */
#if defined(UM_OPENER_PORT_FEATURE_DLR) && UM_OPENER_PORT_FEATURE_DLR
usr_err_t um_opener_port_dlr_open( opener_port_dlr_ctrl_t * const p_ctrl,
                                   dlr_instance_t const * const p_dlr_instance );
usr_err_t um_opener_port_dlr_start( opener_port_dlr_ctrl_t * const p_ctrl );
usr_err_t um_opener_port_dlr_stop( opener_port_dlr_ctrl_t * const p_ctrl );
#endif

#endif /** UM_OPENER_PORT_INTERNAL_H */


