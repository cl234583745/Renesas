/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
 * \addtogroup EL9800_HW EL9800 Platform (Serial ESC Access)
 * @{
 */

/**
\file el9800hw.h
\author EthercatSSC@beckhoff.com
\brief Defines to access the EL9800 specific periphery and ESC (via SPI)

\version 5.13

<br>Changes to version V5.11:<br>
V5.13 EL9800 1: update includes and configurations bits<br>
<br>Changes to version V5.01:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
<br>Changes to version - :<br>
V5.01 : Start file change log
 */

#ifndef _EL9800HW_H_
#define _EL9800HW_H_


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include  "esc.h"


/*ECATCHANGE_START(V5.13) EL9800 1*/
#include <xc.h>
/*ECATCHANGE_END(V5.13) EL9800 1*/


/*-----------------------------------------------------------------------------------------
------
------    Defines and Types
------
-----------------------------------------------------------------------------------------*/
#define ESC_RD                    0x02 /**< \brief Indicates a read access to ESC or EEPROM*/
#define ESC_WR                    0x04 /**< \brief Indicates a write access to ESC or EEPROM.*/


/*---------------------------------------------
-    Microcontroller definitions
-----------------------------------------------*/
#if    defined(_18F242) || defined(_18F252) || defined(_18F442) || defined(_18F452)
#define  ADREG                ADCON1
#define  ADREG_ALL_DIG_IO     0x07
#elif    defined(_18F2420) || defined(_18F2520) || defined(_18F4420) || defined(_18F4520)
#define  ADREG                ADCON1
#define  ADREG_ALL_DIG_IO     0x0F
#elif    defined(_18F23K20) || defined(_18F24K20) || defined(_18F25K20) || defined(_18F43K20) || defined(_18F44K20) || defined(_18F45K20)
#define  ADREG                ANSEL
#define  ADREG_ALL_DIG_IO     0x00
#endif



#define PORT_CFG            {TRISD = 0xFFFF; TRISB = 0x0008; TRISF = 0xFFCC; TRISG = 0x210C; PORTB = 0x00F4; PORTF = 0x0030; PORTG = 0x1243;}
#define SWITCH_1            PORTDbits.RD7 /**< \brief Access to switch 1 input*/
#define SWITCH_2            PORTDbits.RD6 /**< \brief Access to switch 2 input*/
#define SWITCH_3            PORTDbits.RD5 /**< \brief Access to switch 3 input*/
#define SWITCH_4            PORTDbits.RD4 /**< \brief Access to switch 4 input*/
#define SWITCH_5            PORTDbits.RD3 /**< \brief Access to switch 5 input*/
#define SWITCH_6            PORTDbits.RD2 /**< \brief Access to switch 6 input*/
#define SWITCH_7            PORTDbits.RD1 /**< \brief Access to switch 7 input*/
#define SWITCH_8            PORTDbits.RD0 /**< \brief Access to switch 8 input*/

#define LED_1               LATBbits.LATB8 /**< \brief Access to led 1 output*/
#define LED_2               LATBbits.LATB9 /**< \brief Access to led 2 output*/
#define LED_3               LATBbits.LATB10 /**< \brief Access to led 3 output*/
#define LED_4               LATBbits.LATB11 /**< \brief Access to led 4 output*/
#define LED_5               LATBbits.LATB12 /**< \brief Access to led 5 output*/
#define LED_6               LATBbits.LATB13 /**< \brief Access to led 6 output*/
#define LED_7               LATBbits.LATB14 /**< \brief Access to led 7 output*/
#define LED_8               LATBbits.LATB15 /**< \brief Access to led 8 output*/




/*---------------------------------------------
-    hardware timer settings
-----------------------------------------------*/

#define ECAT_TIMER_INC_P_MS                0x271 /**< \brief 625 ticks per ms*/



/*---------------------------------------------
-    Interrupt and Timer defines
-----------------------------------------------*/

#ifndef DISABLE_ESC_INT
#define    DISABLE_ESC_INT()            SET_CPU_IPL(4) /**< \brief Disable external interrupts*/
#endif
#ifndef ENABLE_ESC_INT
#define    ENABLE_ESC_INT()            SET_CPU_IPL(0) /**< \brief Enable external interrupts*/
#endif

#ifndef HW_GetTimer
#define HW_GetTimer()        ((UINT16)(TMR7)) /**< \brief Access to the hardware timer*/
#endif

#ifndef HW_ClearTimer
#define HW_ClearTimer()        {(TMR7) = 0;} /**< \brief Clear the hardware timer*/
#endif





#define HW_EscReadWord(WordValue, Address) HW_EscRead(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2) /**< \brief 16Bit ESC read access*/
#define HW_EscReadDWord(DWordValue, Address) HW_EscRead(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4) /**< \brief 32Bit ESC read access*/
#define HW_EscReadMbxMem(pData,Address,Len) HW_EscRead(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len)) /**< \brief The mailbox data is stored in the local uC memory therefore the default read function is used.*/

#define HW_EscReadWordIsr(WordValue, Address) HW_EscReadIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2) /**< \brief Interrupt specific 16Bit ESC read access*/
#define HW_EscReadDWordIsr(DWordValue, Address) HW_EscReadIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4) /**< \brief Interrupt specific 32Bit ESC read access*/


#define HW_EscWriteWord(WordValue, Address) HW_EscWrite(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2) /**< \brief 16Bit ESC write access*/
#define HW_EscWriteDWord(DWordValue, Address) HW_EscWrite(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4) /**< \brief 32Bit ESC write access*/
#define HW_EscWriteMbxMem(pData,Address,Len) HW_EscWrite(((MEM_ADDR *)(pData)),((UINT16)(Address)),(Len)) /**< \brief The mailbox data is stored in the local uC memory therefore the default write function is used.*/

#define HW_EscWriteWordIsr(WordValue, Address) HW_EscWriteIsr(((MEM_ADDR *)&(WordValue)),((UINT16)(Address)),2) /**< \brief Interrupt specific 16Bit ESC write access*/
#define HW_EscWriteDWordIsr(DWordValue, Address) HW_EscWriteIsr(((MEM_ADDR *)&(DWordValue)),((UINT16)(Address)),4) /**< \brief Interrupt specific 32Bit ESC write access*/

#endif //_EL9800HW_H_

#if defined(_EL9800HW_) && (_EL9800HW_ == 1)
    #define PROTO
#else
    #define PROTO extern
#endif


/*-----------------------------------------------------------------------------------------
------
------    Global variables
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    Global functions
------
-----------------------------------------------------------------------------------------*/
PROTO UINT8 HW_Init(void);
PROTO void HW_Release(void);
PROTO UINT16 HW_GetALEventRegister(void);

PROTO UINT16 HW_GetALEventRegister_Isr(void);

PROTO void HW_SetLed(UINT8 RunLed,UINT8 ErrLed);

PROTO void HW_EscRead( MEM_ADDR * pData, UINT16 Address, UINT16 Len );
PROTO void HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len );

PROTO void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len );

PROTO void HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len );



#undef    PROTO
/** @}*/

