/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
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

#include "main_thread.h"
#include "FreeRTOS_IP.h"
#include "modbusTypedef.h"

#if( ipconfigUSE_DHCP != 0 )
   /* DHCP populates these IP address, Sub net mask and Gateway Address. So start with this is zeroed out values
    * The MAC address is Test MAC address.
    */
    static  uint8_t ucMACAddress[ 6 ]       = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    static  uint8_t ucIPAddress[ 4 ]        = {0x00};
    static  uint8_t ucNetMask[ 4 ]          = {0x00};
    static  uint8_t ucGatewayAddress[ 4 ]   = {0x00};
    static  uint8_t ucDNSServerAddress[ 4 ] = {0x00};
#else
/* Static IP configuration, when DHCP mode is not used for the Example Project.
 * This needs to be populated by the user according to the Network Settings of your LAN.
 * This sample address taken from the LAN where it is tested. This is different for different LAN.
 * get the Address using the PC IPconfig details.
 */
static uint8_t ucMACAddress[6] =
{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
static uint8_t ucIPAddress[4] =
{ 192, 168, 1, 100 };
static uint8_t ucNetMask[4] =
{ 255, 255, 255, 0 };
static uint8_t ucGatewayAddress[4] =
{ 192, 168, 1, 3 };
static uint8_t ucDNSServerAddress[4] =
{ 10, 60, 1, 2 };
#endif


/*******************************************************************************************************************//**
 * @brief      This is the User Thread for the EP.
 * @param[in]  Thread specific parameters
 * @retval     None
 **********************************************************************************************************************/
void main_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    /* FreeRTOS IP Initialization: This init initializes the IP stack  */
    FreeRTOS_IPInit (ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);

    /* Modbus TCP Initialization: This init initializes the Modbus protocol stack  */
    extern uint32_t modbus_init(void);
    modbus_init ();

    while (true)
    {
        vTaskDelay (100);
    }
}