/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/**
\addtogroup SampleAppl Sample Application
@{
*/

/**
\file Sampleappl.c
\author EthercatSSC@beckhoff.com
\brief Implementation

\version 5.12

<br>Changes to version V5.11:<br>
V5.12 ECAT1: update SM Parameter measurement (based on the system time), enhancement for input only devices and no mailbox support, use only 16Bit pointer in process data length caluclation<br>
V5.12 ECAT2: big endian changes<br>
V5.12 EOE1: move icmp sample to the sampleappl,add EoE application interface functions<br>
V5.12 EOE2: prevent static ethernet buffer to be freed<br>
V5.12 EOE3: fix memory leaks in sample ICMP application<br>
V5.12 FOE1: update new interface,move the FoE sample to sampleappl,add FoE application callback functions<br>
<br>Changes to version V5.10:<br>
V5.11 ECAT11: create application interface function pointer, add eeprom emulation interface functions<br>
V5.11 ECAT4: enhance SM/Sync monitoring for input/output only slaves<br>
<br>Changes to version V5.01:<br>
V5.10 ECAT10: Add missing include 'objdef.h'<br>
              Add process data size calculation to sampleappl<br>
V5.10 ECAT6: Add "USE_DEFAULT_MAIN" to enable or disable the main function<br>
V5.10 FC1100: Stop stack if hardware init failed<br>
<br>Changes to version V5.0:<br>
V5.01 APPL2: Update Sample Application Output mapping<br>
V5.0: file created
*/


/*-----------------------------------------------------------------------------------------
------
------    Includes
------
-----------------------------------------------------------------------------------------*/
#include "ecat_def.h"

#include "applInterface.h"


#define _SAMPLE_APPLICATION_
#include "sampleappl.h"
#undef _SAMPLE_APPLICATION_

#include "bsp_api.h"
#include "board_cfg.h"

#if defined(BOARD_RZT2M_RSK) | defined(BOARD_RZN2L_RSK)
extern bsp_leds_t g_bsp_leds;
UINT16 APPL_GetDipSw(void);
void APPL_SetLed(UINT16 value);
#endif

/*--------------------------------------------------------------------------------------
------
------    local types and defines
------
--------------------------------------------------------------------------------------*/
#if defined(BOARD_RZT2M_RSK) | defined(BOARD_RZN2L_RSK)
/* Information on how many SWs and what pins they are on. */
typedef struct st_bsp_dip_sws
{
    UINT16         sw_count;        ///< The number of DIP SWs on this board
    UINT16 const * p_sws;           ///< Pointer to an array of IOPORT pins for controlling DIP SWs
} bsp_dip_sws_t;

/* Available user-controllable DIP SWs on this board. 
 * These enums can be can be used to index into the array of DIP SW pins
 * found in the appl_dip_sws_t structure. */
typedef enum e_bsp_dipsw
{
    BSP_DIPSW_0 = 0,
    BSP_DIPSW_1 = 1,
    BSP_DIPSW_2 = 2,
    BSP_DIPSW_3 = 3,
    BSP_DIPSW_4 = 4,
    BSP_DIPSW_5 = 5,
    BSP_DIPSW_6 = 6,
    BSP_DIPSW_7 = 7,
} bsp_dipsw_t;
#endif
/*-----------------------------------------------------------------------------------------
------
------    local variables and constants
------
-----------------------------------------------------------------------------------------*/
#if defined(BOARD_RZT2M_RSK)
/** Array of DIP SW IOPORT pins. */
static const UINT16 g_bsp_prv_dip_sws[] =
{
	(UINT16) BSP_IO_PORT_11_PIN_0, // SW3-1
	(UINT16) BSP_IO_PORT_11_PIN_3, // SW3-2
	(UINT16) BSP_IO_PORT_11_PIN_4, // SW3-3
	(UINT16) BSP_IO_PORT_11_PIN_6, // SW3-4
	(UINT16) BSP_IO_PORT_10_PIN_6, // SW3-5
	(UINT16) BSP_IO_PORT_13_PIN_2, // SW3-6
	(UINT16) BSP_IO_PORT_13_PIN_7, // SW3-7
	(UINT16) BSP_IO_PORT_14_PIN_1, // SW3-8
};
#endif

#if defined(BOARD_RZN2L_RSK)
/** Array of DIP SW IOPORT pins. */
static const UINT16 g_bsp_prv_dip_sws[] =
{
	(UINT16) BSP_IO_PORT_13_PIN_6, // SW3-1
	(UINT16) BSP_IO_PORT_13_PIN_5, // SW3-2
	(UINT16) BSP_IO_PORT_14_PIN_0, // SW3-3
	(UINT16) BSP_IO_PORT_14_PIN_1, // SW3-4
};
#endif

#if defined(BOARD_RZT2M_RSK) | defined(BOARD_RZN2L_RSK)
/** Structure with DIP SW information for this board. */
const bsp_dip_sws_t g_bsp_dip_sws =
{
    .sw_count = (UINT16) ((sizeof(g_bsp_prv_dip_sws) / sizeof(g_bsp_prv_dip_sws[0]))),
    .p_sws    = &g_bsp_prv_dip_sws[0]
};
#endif

/*-----------------------------------------------------------------------------------------
------
------    application specific functions
------
-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
------
------    generic functions
------
-----------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////
/**
 \brief    The function is called when an error state was acknowledged by the master

*////////////////////////////////////////////////////////////////////////////////////////

void    APPL_AckErrorInd(UINT16 stateTrans)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from INIT to PREOP when
             all general settings were checked to start the mailbox handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
            The return code NOERROR_INWORK can be used, if the application cannot confirm
            the state transition immediately, in that case this function will be called cyclically
            until a value unequal NOERROR_INWORK is returned

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from PREEOP to INIT
             to stop the mailbox handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopMailboxHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    pIntMask    pointer to the AL Event Mask which will be written to the AL event Mask
                        register (0x204) when this function is succeeded. The event mask can be adapted
                        in this function
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from PREOP to SAFEOP when
           all general settings were checked to start the input handler. This function
           informs the application about the state transition, the application can refuse
           the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartInputHandler(UINT16 *pIntMask)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from SAFEOP to PREEOP
             to stop the input handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopInputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    AL Status Code (see ecatslv.h ALSTATUSCODE_....)

 \brief    The function is called in the state transition from SAFEOP to OP when
             all general settings were checked to start the output handler. This function
             informs the application about the state transition, the application can refuse
             the state transition when returning an AL Status error code.
           The return code NOERROR_INWORK can be used, if the application cannot confirm
           the state transition immediately, in that case the application need to be complete 
           the transition by calling ECAT_StateChange.
*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StartOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return     0, NOERROR_INWORK

 \brief    The function is called in the state transition from OP to SAFEOP
             to stop the output handler. This functions informs the application
             about the state transition, the application cannot refuse
             the state transition.

*////////////////////////////////////////////////////////////////////////////////////////

UINT16 APPL_StopOutputHandler(void)
{
    return ALSTATUSCODE_NOERROR;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\return     0(ALSTATUSCODE_NOERROR), NOERROR_INWORK
\param      pInputSize  pointer to save the input process data length
\param      pOutputSize  pointer to save the output process data length

\brief    This function calculates the process data sizes from the actual SM-PDO-Assign
            and PDO mapping
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GenerateMapping(UINT16 *pInputSize,UINT16 *pOutputSize)
{
    UINT16 result = ALSTATUSCODE_NOERROR;
    UINT16 PDOAssignEntryCnt = 0;
    OBJCONST TOBJECT OBJMEM * pPDO = NULL;
    UINT16 PDOSubindex0 = 0;
    UINT32 *pPDOEntry = NULL;
    UINT16 PDOEntryCnt = 0;
    UINT16 InputSize = 0;
    UINT16 OutputSize = 0;

    /*Scan object 0x1C12 RXPDO assign*/
    for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sRxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
    {
        pPDO = OBJ_GetObjectHandle(sRxPDOassign.aEntries[PDOAssignEntryCnt]);
        if(pPDO != NULL)
        {
            PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
            for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
            {
/*ECATCHANGE_START(V5.12) ECAT1*/
                pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
/*ECATCHANGE_END(V5.12) ECAT1*/
                // we increment the expected output size depending on the mapped Entry
                OutputSize += (UINT16) ((*pPDOEntry) & 0xFF);
            }
        }
        else
        {
            /*assigned PDO was not found in object dictionary. return invalid mapping*/
            OutputSize = 0;
            result = ALSTATUSCODE_INVALIDOUTPUTMAPPING;
            break;
        }
    }

    OutputSize = (OutputSize + 7) >> 3;

    if(result == 0)
    {
        /*Scan Object 0x1C13 TXPDO assign*/
        for(PDOAssignEntryCnt = 0; PDOAssignEntryCnt < sTxPDOassign.u16SubIndex0; PDOAssignEntryCnt++)
        {
            pPDO = OBJ_GetObjectHandle(sTxPDOassign.aEntries[PDOAssignEntryCnt]);
            if(pPDO != NULL)
            {
                PDOSubindex0 = *((UINT16 *)pPDO->pVarPtr);
                for(PDOEntryCnt = 0; PDOEntryCnt < PDOSubindex0; PDOEntryCnt++)
                {
/*ECATCHANGE_START(V5.12) ECAT1*/
                    pPDOEntry = (UINT32 *)((UINT16 *)pPDO->pVarPtr + (OBJ_GetEntryOffset((PDOEntryCnt+1),pPDO)>>3)/2);    //goto PDO entry
/*ECATCHANGE_END(V5.12) ECAT1*/
                    // we increment the expected output size depending on the mapped Entry
                    InputSize += (UINT16) ((*pPDOEntry) & 0xFF);
                }
            }
            else
            {
                /*assigned PDO was not found in object dictionary. return invalid mapping*/
                InputSize = 0;
                result = ALSTATUSCODE_INVALIDINPUTMAPPING;
                break;
            }
        }
    }
    InputSize = (InputSize + 7) >> 3;

    *pInputSize = InputSize;
    *pOutputSize = OutputSize;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to input process data

\brief      This function will copies the inputs from the local memory to the ESC memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_InputMapping(UINT16* pData)
{
    MEMCPY(pData,&InputCounter,SIZEOF(InputCounter));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\param      pData  pointer to output process data

\brief    This function will copies the outputs from the ESC memory to the local memory
            to the hardware
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_OutputMapping(UINT16* pData)
{
    MEMCPY(&OutputCounter,pData,SIZEOF(OutputCounter));
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
\brief    This function will called from the synchronisation ISR 
            or from the mainloop if no synchronisation is supported
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_Application(void)
{
#if defined(BOARD_RZT2M_RSK) | defined(BOARD_RZN2L_RSK)
    /*Hardware independent sample application*/
	InputCounter = APPL_GetDipSw() & 0x000f;		// SW3.4-1
	APPL_SetLed((UINT16)OutputCounter);
#else
    if(OutputCounter > 0)
    {
        InputCounter = OutputCounter+1;
    }
    else
    {
        InputCounter++;
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
/**
 \return    The Explicit Device ID of the EtherCAT slave

 \brief     Calculate the Explicit Device ID
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GetDeviceID()
{
#if defined(BOARD_RZT2M_RSK)
	return ((APPL_GetDipSw() >> 4) & 0x000f);			// SW3.8-5
#elif defined(BOARD_RZN2L_RSK)
	return (APPL_GetDipSw());							// SW3.4-1
#else
    return 0x5;
#endif
}

#if defined(BOARD_RZT2M_RSK) | defined(BOARD_RZN2L_RSK)
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \param    UINT16 LED output value. The value one means ON.

 \brief    SET LED
*////////////////////////////////////////////////////////////////////////////////////////
void APPL_SetLed(UINT16 value)
{
	/* LED type structure */
	bsp_leds_t leds = g_bsp_leds;

    /* Holds level to set for pins */
    bsp_io_level_t pin_level[4];

	/* This code uses BSP IO functions to show how it is used.*/
	R_BSP_PinAccessEnable();

	pin_level[BSP_LED_RLED0] = ((value & 1) ?  BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
	pin_level[BSP_LED_RLED1] = ((value & 2) ?  BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
	pin_level[BSP_LED_RLED2] = ((value & 4) ?  BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
	pin_level[BSP_LED_RLED3] = ((value & 8) ?  BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);

	R_BSP_PinWrite((bsp_io_port_pin_t)leds.p_leds[BSP_LED_RLED0], pin_level[BSP_LED_RLED0]);
	R_BSP_PinWrite((bsp_io_port_pin_t)leds.p_leds[BSP_LED_RLED1], pin_level[BSP_LED_RLED1]);
	R_BSP_PinWrite((bsp_io_port_pin_t)leds.p_leds[BSP_LED_RLED2], pin_level[BSP_LED_RLED2]);
	R_BSP_PinWrite((bsp_io_port_pin_t)leds.p_leds[BSP_LED_RLED3], pin_level[BSP_LED_RLED3]);

	/* Protect PFS registers */
	R_BSP_PinAccessDisable();
}
/////////////////////////////////////////////////////////////////////////////////////////
/**
 \retuen   UINT16 DIP SW value. Low input level means ON.

 \brief    Get DIP SW
*////////////////////////////////////////////////////////////////////////////////////////
UINT16 APPL_GetDipSw(void)
{
	UINT16 u16DipSw;

	u16DipSw = 0;

	/* DIP SW type structure */
	bsp_dip_sws_t dipsws = g_bsp_dip_sws;

	/* This code uses BSP IO functions to show how it is used.*/
	R_BSP_PinAccessEnable();

	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_0]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x01;
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_1]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x02;
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_2]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x04;
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_3]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x08;

#if defined(BOARD_RZT2M_RSK)
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_4]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x10;
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_5]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x20;
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_6]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x40;
	if (R_BSP_PinRead((bsp_io_port_pin_t)dipsws.p_sws[BSP_DIPSW_7]) == BSP_IO_LEVEL_LOW) u16DipSw |= 0x80;
#endif

	/* Protect PFS registers */
	R_BSP_PinAccessDisable();

	return u16DipSw;
}
#endif
/** @} */

