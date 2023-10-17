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

#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

/**
 * Include lwipopts settings for FreeRTOS.
 */
#include "FreeRTOS.h"
#include "lwipopts_freertos.h"

/**
 * Overwrite the lwipopts for freertos.h
 */
#undef  MEM_SIZE
#define MEM_SIZE    ( 40 * 1024 )

#undef  TCPIP_THREAD_PRIO
#define TCPIP_THREAD_PRIO   (6)

/**
 * The "struct timeval" is used in "lwip/sockets.h", and the "struct timeval" is defined in <sys/time.h> usually.
 * If LWIP_TIMEVAL_PRIVATE is enabled, "lwip/sockets.h" defines the "struct timeval" privately without <sys/time.h>.
 */
#if ( !defined( __CC_ARM ) ) && ( !defined( __ICCARM__ ) ) && ( !defined( __ARMCC_VERSION ) )
/**
 * The <sys/time.h> is included in "arch/cc.h" when a compiler is neither IAR ICC nor ARM' Compiler.
 * When the same condition, the LWIP_TIMEVAL_PRIVATE should be disabled for avoiding the redefinition compile error.
 */
#define LWIP_TIMEVAL_PRIVATE (0)
#else
#define LWIP_TIMEVAL_PRIVATE (1)
#endif

/**
 * The followings overwrite opt.h configurations.
 */
#define LWIP_NETIF_HOSTNAME (1)

/**
 * Use netif API
 */
#define LWIP_NETIF_API      (1)

/**
 * Enable socket API compatibilities
 */
#define LWIP_COMPAT_SOCKETS (1)

/**
 * Enable IGMP
 */
#define LWIP_IGMP            (1)

#endif /* LWIP_LWIPOPTS_H */
