/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
* https://www.beckhoff.com/media/downloads/slave-stack-code/ethercat_ssc_license.pdf
*/

/**
\addtogroup EL9800_HW EL9800 Platform (Serial ESC Access)
@{
*/

/**
\file    el9800hw.c
\author EthercatSSC@beckhoff.com
\brief Implementation
Hardware access implementation for EL9800 onboard PIC18/PIC24 connected via SPI to ESC

\version 5.13

<br>Changes to version V5.12:<br>
V5.13 EL9800 1: update includes and configurations bits<br>
<br>Changes to version V5.11:<br>
V5.12 EL9800 1: improve the SPI access<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT10: change PROTO handling to prevent compiler errors<br>
V5.11 EL9800 2: change PDI access test to 32Bit ESC access and reset AL Event mask after test even if AL Event is not enabled<br>
<br>Changes to version V5.01:<br>
V5.10 ESC5: Add missing swapping<br>
V5.10 HW3: Sync1 Isr added<br>
V5.10 HW4: Add volatile directive for direct ESC DWORD/WORD/BYTE access<br>
           Add missing swapping in mcihw.c<br>
           Add "volatile" directive vor dummy variables in enable and disable SyncManger functions<br>
           Add missing swapping in EL9800hw files<br>
<br>Changes to version V5.0:<br>
V5.01 HW1: Invalid ESC access function was used<br>
<br>Changes to version V4.40:<br>
V5.0 ESC4: Save SM disable/Enable. Operation may be pending due to frame handling.<br>
<br>Changes to version V4.30:<br>
V4.40 : File renamed from spihw.c to el9800hw.c<br>
<br>Changes to version V4.20:<br>
V4.30 ESM: if mailbox Syncmanger is disabled and bMbxRunning is true the SyncManger settings need to be revalidate<br>
V4.30 EL9800: EL9800_x hardware initialization is moved to el9800.c<br>
V4.30 SYNC: change synchronisation control function. Add usage of 0x1C32:12 [SM missed counter].<br>
Calculate bus cycle time (0x1C32:02 ; 0x1C33:02) CalcSMCycleTime()<br>
V4.30 PDO: rename PDO specific functions (COE_xxMapping -> PDO_xxMapping and COE_Application -> ECAT_Application)<br>
V4.30 ESC: change requested address in GetInterruptRegister() to prevent acknowledge events.<br>
(e.g. reading an SM config register acknowledge SM change event)<br>
GENERIC: renamed several variables to identify used SPI if multiple interfaces are available<br>
V4.20 MBX 1: Add Mailbox queue support<br>
V4.20 SPI 1: include SPI RxBuffer dummy read<br>
V4.20 DC 1: Add Sync0 Handling<br>
V4.20 PIC24: Add EL9800_4 (PIC24) required source code<br>
V4.08 ECAT 3: The AlStatusCode is changed as parameter of the function AL_ControlInd<br>
<br>Changes to version V4.02:<br>
V4.03 SPI 1: In ISR_GetInterruptRegister the NOP-command should be used.<br>
<br>Changes to version V4.01:<br>
V4.02 SPI 1: In HW_OutputMapping the variable u16OldTimer shall not be set,<br>
             otherwise the watchdog might exceed too early.<br>
<br>Changes to version V4.00:<br>
V4.01 SPI 1: DI and DO were changed (DI is now an input for the uC, DO is now an output for the uC)<br>
V4.01 SPI 2: The SPI has to operate with Late-Sample = FALSE on the Eva-Board<br>
<br>Changes to version V3.20:<br>
V4.00 ECAT 1: The handling of the Sync Manager Parameter was included according to<br>
              the EtherCAT Guidelines and Protocol Enhancements Specification<br>
V4.00 APPL 1: The watchdog checking should be done by a microcontroller<br>
                 timer because the watchdog trigger of the ESC will be reset too<br>
                 if only a part of the sync manager data is written<br>
V4.00 APPL 4: The EEPROM access through the ESC is added

*/


/*--------------------------------------------------------------------------------------
------
------    Includes
------
--------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#include "ecatslv.h"


#define    _EL9800HW_ 1
#include "el9800hw.h"
#undef    _EL9800HW_
/*remove definition of _EL9800HW_ (#ifdef is used in el9800hw.h)*/

#include "ecatappl.h"

/*--------------------------------------------------------------------------------------
------
------    internal Types and Defines
------
--------------------------------------------------------------------------------------*/

typedef union
{
    unsigned short    Word;
    unsigned char    Byte[2];
} UBYTETOWORD;

typedef union 
{
    UINT8           Byte[2];
    UINT16          Word;
}
UALEVENT;

/*-----------------------------------------------------------------------------------------
------
------    SPI defines/macros
------
-----------------------------------------------------------------------------------------*/
#define SPI1_SEL                        _LATB2
#define SPI1_IF                            _SPI1IF
#define SPI1_BUF                        SPI1BUF
#define SPI1_CON1                        SPI1CON1
#define SPI1_STAT                        SPI1STAT
#define    WAIT_SPI_IF                        while( !SPI1_IF)
#define    SELECT_SPI                        {(SPI1_SEL) = (SPI_ACTIVE);}
#define    DESELECT_SPI                    {(SPI1_SEL) = (SPI_DEACTIVE);}
#define    INIT_SSPIF                        {(SPI1_IF)=0;}
#define SPI1_STAT_VALUE                    0x8000
#define SPI1_CON1_VALUE                    0x027E
#define SPI1_CON1_VALUE_16BIT            0x047E
#define SPI_DEACTIVE                    1
#define SPI_ACTIVE                        0


/*-----------------------------------------------------------------------------------------
------
------    Global Interrupt setting
------
-----------------------------------------------------------------------------------------*/

#define DISABLE_GLOBAL_INT            SET_CPU_IPL(4) //set CPU priority to level 4 (disable interrupt level 1 - 4)
#define ENABLE_GLOBAL_INT            SET_CPU_IPL(0)

#define    DISABLE_AL_EVENT_INT        DISABLE_GLOBAL_INT
#define    ENABLE_AL_EVENT_INT            ENABLE_GLOBAL_INT


/*-----------------------------------------------------------------------------------------
------
------    ESC Interrupt
------
-----------------------------------------------------------------------------------------*/


#define    INIT_ESC_INT               {(_INT1EP) = 1;(_INT1IP) = 1;} //_INT1EP = 1:  negative edge ; _INT1IP = 1; //highest priority
#define    ESC_INT_REQ                (_INT1IF) //ESC Interrupt (INT1) state
#define    INT_EL                    (_RD8) //ESC Interrupt input port
#define    EscIsr                    (_INT1Interrupt) // primary interrupt vector name
#define    ACK_ESC_INT                {(ESC_INT_REQ)=0;}



/*-----------------------------------------------------------------------------------------
------
------    SYNC0 Interrupt
------
-----------------------------------------------------------------------------------------*/

#define    INIT_SYNC0_INT                   {(_INT3EP) = 1;(_INT3IP) = 1;}//_INT3EP = 1:  negative edge ; _INT3IP = 1; //highest priority
#define    SYNC0_INT_REQ                    (_INT3IF) //Sync0 Interrupt (INT3) state
#define    INT_SYNC0                        (_RD10) //Sync1 Interrupt input port
#define    Sync0Isr                        (_INT3Interrupt) // primary interrupt vector name
#define    DISABLE_SYNC0_INT                {(_INT3IE)=0;}//disable interrupt source INT3
#define    ENABLE_SYNC0_INT                {(_INT3IE) = 1;} //enable interrupt source INT3
#define    ACK_SYNC0_INT                    {(SYNC0_INT_REQ) = 0;}
#define    SET_SYNC0_INT                    {(SYNC0_INT_REQ) = 1;}
#define    SYNC0_INT_PORT_IS_ACTIVE        {(INT_EL) == 0;}


#define    INIT_SYNC1_INT                   {(_INT4EP) = 1;(_INT4IP) = 1;}//_INT4EP = 1:  negative edge ; _INT4IP = 1; //highest priority
#define    SYNC1_INT_REQ                    (_INT4IF) //Sync1 Interrupt (INT4) state
#define    INT_SYNC1                        (_RD11) //Sync1 Interrupt input port
#define    Sync1Isr                        (_INT4Interrupt) // primary interrupt vector name
#define    DISABLE_SYNC1_INT                {(_INT4IE)=0;}//disable interrupt source INT4
#define    ENABLE_SYNC1_INT                {(_INT4IE) = 1;} //enable interrupt source INT4
#define    ACK_SYNC1_INT                    {(SYNC1_INT_REQ) = 0;}
#define    SET_SYNC1_INT                    {(SYNC1_INT_REQ) = 1;}
#define    SYNC1_INT_PORT_IS_ACTIVE        {(INT_EL) == 0;}


/*-----------------------------------------------------------------------------------------
------
------    Hardware timer
------
-----------------------------------------------------------------------------------------*/


#define INIT_ECAT_TIMER                {(PR7) = 2000;/*set period*/ \
    (T7CONbits.TCKPS) = 2;/*set prescaler to 1:8*/ \
    (TMR7) = 0;/*clear timer register*/}

#define STOP_ECAT_TIMER                {(T7CONbits.TON) = 0; /*disable timer*/}

#define START_ECAT_TIMER            {(T7CONbits.TON) = 1; /*enable timer*/}



/*-----------------------------------------------------------------------------------------
------
------    Configuration Bits
------
-----------------------------------------------------------------------------------------*/
/*ECATCHANGE_START(V5.13) EL9800 1*/
//General Segment Code Protection
#pragma config GSS = GCP_OFF //Alias for OFF 

//Oscillator Mode
#pragma config FNOSC = PRI //	Primary Oscillator (XT, HS, EC) 
//Two-speed Oscillator Start-Up Enable
#pragma config IESO = OFF //	Start up with user-selected oscillator 

//Clock Switching and Monitor
#pragma config FCKSM = CSECMD	//Clock switching is enabled, Fail-Safe Clock Monitor is disabled
//OSC2 Pin Function
#pragma config OSCIOFNC = ON	//OSC2 pin has digital I/O function 
//Primary Oscillator Source
#pragma config POSCMD = HS	//HS Oscillator Mode 

//Watchdog Timer Enable
#pragma config FWDTEN = OFF	//Watchdog timer enabled/disabled by user software

//POR Timer Value
#pragma config FPWRT = PWR1	//Disabled 


#if EXT_DEBUGER_INTERFACE
//Comm Channel Select
#pragma config ICS = PGD2	//Communicate on PGC2/EMUC2 and PGD2/EMUD2 
#else
//Comm Channel Select
#pragma config ICS = PGD3	//Communicate on PGC3/EMUC3 and PGD3/EMUD3 
#endif

//JTAG Port Enable
#pragma config JTAGEN = OFF	//JTAG is Disabled 


/*ECATCHANGE_END(V5.13) EL9800 1*/

/*-----------------------------------------------------------------------------------------
------
------    LED defines
------
-----------------------------------------------------------------------------------------*/
// EtherCAT Status LEDs -> StateMachine
#define LED_ECATGREEN                 LATFbits.LATF1
#define LED_ECATRED                    LATFbits.LATF0

/*--------------------------------------------------------------------------------------
------
------    internal Variables
------
--------------------------------------------------------------------------------------*/
UALEVENT         EscALEvent;            //contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc
/*--------------------------------------------------------------------------------------
------
------    internal functions
------
--------------------------------------------------------------------------------------*/

static UINT8 RxTxSpiData(UINT8 MosiByte)
{
    VARVOLATILE UINT8 MisoByte = 0;
    
    if((SPI1_STAT & 0x1) != 0)
    {
        /*read buffer to prevent buffer overrun error*/
        MisoByte = SPI1_BUF;
    }
    
    
    SPI1_IF = 0;

    SPI1_BUF = MosiByte;
    
    /* wait until the transmission of the byte is finished */
    WAIT_SPI_IF;

    MisoByte = SPI1_BUF;

    /* reset transmission flag */
    SPI1_IF = 0;
    
    return MisoByte;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Command    ESC_WR performs a write access; ESC_RD performs a read access.

 \brief The function addresses the EtherCAT ASIC via SPI for a following SPI access.
*////////////////////////////////////////////////////////////////////////////////////////
static void AddressingEsc( UINT16 Address, UINT8 Command )
{
    VARVOLATILE UBYTETOWORD tmp;
    tmp.Word = ( Address << 3 ) | Command;



    /* select the SPI */
    SELECT_SPI;

    /* send the first address/command byte to the ESC 
       receive the first AL Event Byte*/
    EscALEvent.Byte[0] = RxTxSpiData(tmp.Byte[1]);

    EscALEvent.Byte[1] = RxTxSpiData(tmp.Byte[0]);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
static void GetInterruptRegister(void)
{
    VARVOLATILE UINT8            dummy;
    HW_EscRead((MEM_ADDR *)&dummy, 0, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief  The function operates a SPI access without addressing.
        Shall be implemented if interrupts are supported else this function is equal to "GetInterruptRegsiter()"

        The first two bytes of an access to the EtherCAT ASIC always deliver the AL_Event register (0x220).
        It will be saved in the global "EscALEvent"
*////////////////////////////////////////////////////////////////////////////////////////
static void ISR_GetInterruptRegister(void)
{
    VARVOLATILE UINT8 dummy;
    HW_EscReadIsr((MEM_ADDR *)&dummy, 0, 1);
}

/*--------------------------------------------------------------------------------------
------
------    exported hardware access functions
------
--------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0 if initialization was successful

 \brief    This function intialize the Process Data Interface (PDI) and the host controller.
*////////////////////////////////////////////////////////////////////////////////////////
UINT8 HW_Init(void)
{
    UINT32 intMask;

    PORT_CFG;

    //Setup analog input
    AD1PCFGLbits.PCFG3 = 0x0;    //set RB3 to analog input
    AD1CON1 = 0x0404; // ASAM bit = 1 implies sampling ..
    AD1CHS0= 0x0003; // Connect RB3/AN3 as CH0 input ..
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // Sample time manual, Tad = internal 2 Tcy
    AD1CON2 = 0;
    AD1CON1bits.ADON = 1; // turn ADC ON

    //configure clock
    // Im Referenzdesign lauft das ganze mit 10Mhz extere Oszillatorclock.
    // Fcy = (1/2) * ((10000000*32)/(2*2)) = 40 MIPS
    // extern 10Mhz / 2 (PLLPRE) = 5Mhz
    // 5 Mhz * 32 (PLLFBD) = 160Mhz (VCO)
    // 160Mhz / 2 (PLLPOST) = 80Mhz (FCY) -> entspricht 40 MIPS
    PLLFBD = 0x1E; // 30, da aber 0 == 2 entspricht dieser Wert 32
    CLKDIVbits.PLLPOST = 0; // 0 entspricht 2, Fcy == 80 MHz The resultant device operating speed is 80/2 = 40 MIPS
    CLKDIVbits.PLLPRE  = 0; // CLKDIVbits.PRESCLR = 0; 0 entspricht 2


    //unlock OSCCON-Register High
    //set Primary Oscillator with PLL
    __builtin_write_OSCCONH(0x03);

    //unlock OSCCON-Register Low
    //start clockswitching
    __builtin_write_OSCCONL(0x01);

    //wait for clockswitch and ready PLL
    while(OSCCONbits.COSC != 3);
    while(OSCCONbits.LOCK != 1);


      /* initialize the SPI registers for the ESC SPI */
    SPI1_CON1 = SPI1_CON1_VALUE;
    SPI1_STAT = SPI1_STAT_VALUE;


    do
    {
        intMask = 0x93;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while (intMask != 0x93);

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    INIT_ESC_INT
    (_INT1IE)=1;

    INIT_SYNC0_INT
    INIT_SYNC1_INT

    ENABLE_SYNC0_INT;
    ENABLE_SYNC1_INT;

    INIT_ECAT_TIMER;
    START_ECAT_TIMER;

    /* enable all interrupts */
    ENABLE_GLOBAL_INT;

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    This function shall be implemented if hardware resources need to be release
        when the sample application stops
*////////////////////////////////////////////////////////////////////////////////////////
void HW_Release(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  This function gets the current content of ALEvent register
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister(void)
{
    GetInterruptRegister();
    return EscALEvent.Word;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    first two Bytes of ALEvent register (0x220)

 \brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_GetALEventRegister()"
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 HW_GetALEventRegister_Isr(void)
{
     ISR_GetInterruptRegister();
    return EscALEvent.Word;
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param RunLed            desired EtherCAT Run led state
 \param ErrLed            desired EtherCAT Error led state

  \brief    This function updates the EtherCAT run and error led
*////////////////////////////////////////////////////////////////////////////////////////
void HW_SetLed(UINT8 RunLed,UINT8 ErrLed)
{
      LED_ECATGREEN = RunLed;
      LED_ECATRED   = ErrLed;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  This function operates the SPI read access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscRead( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    /* HBu 24.01.06: if the SPI will be read by an interrupt routine too the
                     mailbox reading may be interrupted but an interrupted
                     reading will remain in a SPI transmission fault that will
                     reset the internal Sync Manager status. Therefore the reading
                     will be divided in 1-byte reads with disabled interrupt */
    UINT16 i = Len;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, in that case the address has to be reinitialized,
           in that case the status flag will indicate an error because
           the reading operation was interrupted without setting the last
           sent byte to 0xFF */
        DISABLE_GLOBAL_INT;

         AddressingEsc( Address, ESC_RD );

         /*Each Byte will be read with a new addressing phase so the out data is 0xFF*/
         *pTmpData = RxTxSpiData(0xFF);
         
         pTmpData++;

         /*Wait for 250ns because the each byte is transmitted separate*/
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();


        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */
        DESELECT_SPI

        ENABLE_GLOBAL_INT;

        /* next address */
        Address++;

    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves read data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

\brief  The SPI PDI requires an extra ESC read access functions from interrupts service routines.
        The behaviour is equal to "HW_EscRead()"
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscReadIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    UINT8 data = 0;

   UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     AddressingEsc( Address, ESC_RD );

    /* loop for all bytes to be read */
    while ( i-- > 0 )
    {
        if ( i == 0 )
        {
            /* when reading the last byte the DI pin shall be 1 */
            data = 0xFF;
        }

        *pTmpData = RxTxSpiData(data);

        pTmpData++;
    }

     Nop();

     //only a single byte was transmitted => wait for the 250ns
     if(Len == 1)
     {
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
    }
    
    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

  \brief  This function operates the SPI write access to the EtherCAT ASIC.
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscWrite( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT8 dummy;

    UINT8 *pTmpData = (UINT8 *)pData;

    /* loop for all bytes to be written */
    while ( i-- > 0 )
    {
        /* the reading of data from the ESC can be interrupted by the
           AL Event ISR, so every byte will be written separate */
        DISABLE_GLOBAL_INT;

        /* HBu 24.01.06: wrong parameter ESC_RD */
         AddressingEsc( Address, ESC_WR );

         dummy = RxTxSpiData(*pTmpData);
         pTmpData++;

         /*Wait for 250ns because the each byte is transmitted separate*/
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();

        /* there has to be at least 15 ns + CLK/2 after the transmission is finished
           before the SPI1_SEL signal shall be 1 */

        DESELECT_SPI


        ENABLE_GLOBAL_INT;

        /* next address */
        Address++;


    }
}


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param pData        Pointer to a byte array which holds data to write or saves write data.
 \param Address     EtherCAT ASIC address ( upper limit is 0x1FFF )    for access.
 \param Len            Access size in Bytes.

 \brief  The SPI PDI requires an extra ESC write access functions from interrupts service routines.
        The behaviour is equal to "HW_EscWrite()"
*////////////////////////////////////////////////////////////////////////////////////////
void HW_EscWriteIsr( MEM_ADDR *pData, UINT16 Address, UINT16 Len )
{
    UINT16 i = Len;
    VARVOLATILE UINT16 dummy;
    UINT8 *pTmpData = (UINT8 *)pData;

    /* send the address and command to the ESC */
     AddressingEsc( Address, ESC_WR );
     /* loop for all bytes to be written */
    while ( i-- > 0 )
    {

         dummy = RxTxSpiData(*pTmpData);
         pTmpData++;
    }

     Nop();

     //only a single byte was transmitted => wait for the 250ns
     if(Len == 1)
     {
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
         Nop();
    }

    /* there has to be at least 15 ns + CLK/2 after the transmission is finished
       before the SPI1_SEL signal shall be 1 */
    DESELECT_SPI
}




/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the PDI interrupt from the EtherCAT Slave Controller
*////////////////////////////////////////////////////////////////////////////////////////

void __attribute__ ((__interrupt__, no_auto_psv)) EscIsr(void)
{
     PDI_Isr();

    /* reset the interrupt flag */
    ACK_ESC_INT;

}



/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the interrupts from SYNC0
*////////////////////////////////////////////////////////////////////////////////////////
void __attribute__((__interrupt__, no_auto_psv)) Sync0Isr(void)
{
    Sync0_Isr();

    /* reset the interrupt flag */
    ACK_SYNC0_INT;
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    Interrupt service routine for the interrupts from SYNC1
*////////////////////////////////////////////////////////////////////////////////////////
void __attribute__((__interrupt__, no_auto_psv)) Sync1Isr(void)
{
    Sync1_Isr();

    /* reset the interrupt flag */
    ACK_SYNC1_INT;
}


/** @} */
