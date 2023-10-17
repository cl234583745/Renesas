/*******************************************************************************
 * Copyright (c) 2009, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

#ifndef GENERIC_NETWORKHANDLER_H_
#define GENERIC_NETWORKHANDLER_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Include OpENer porting header */
#include "socket_timer.h"              ///< For struct SocketTimer

/** Include OpENer libraries */
#include "typedefs.h"                  ///< for generic CIP types

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
/**
 * Nothing
 */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief Struct representing the current network status.
 */
typedef struct
{
    CipUdint ip_address;               /* IP being valid during NetworkHandlerInitialize() */
    CipUdint network_mask;             /* network mask being valid during NetworkHandlerInitialize() */
} NetworkStatus;

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
/** Export to cip/cipconnectionmanager.c */
extern NetworkStatus g_network_status; /* This global variable holding the current network status. */

/** Export to enet_encap/encap.c */
extern SocketTimer  g_timestamps[OPENER_NUMBER_OF_SUPPORTED_SESSIONS];
extern MilliSeconds g_actual_time;

/***********************************************************************************************************************
 * Exported global function (to be accessed by other files)
 ***********************************************************************************************************************/
/** Export to cip/cipconnectionmanager.c and cip/cipioconnection.c */
void CloseUdpSocket(int socket_handle);
void CloseTcpSocket(int socket_handle);

#endif ///< GENERIC_NETWORKHANDLER_H_
