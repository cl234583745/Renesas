/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "hal_data.h"
#include "sio_char.h"
#include "r_uart_api.h"
#include "r_sci_uart.h"

extern volatile uint32_t g_transfer_complete;
extern volatile uint32_t g_receive_complete;

extern sci_uart_instance_ctrl_t g_uart0_ctrl;

/*******************************************************************************
Typedef definitions
*******************************************************************************/


/*******************************************************************************
Macro definitions
*******************************************************************************/


/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/


/*******************************************************************************
Private global variables and functions
*******************************************************************************/

/*******************************************************************************
* Function Name: IoGetchar
* Description  : One character is received from SCI, and it's data is returned.
*              : This function keeps waiting until it can obtain the receiving data.
* Arguments    : none
* Return Value : Character to receive (Byte).
*******************************************************************************/
int32_t IoGetchar (void)
{
    int32_t    data;
    uint8_t    data_char;

    if( R_SCI_UART_Read(&g_uart0_ctrl, &data_char, TRANSFER_LENGTH) != FSP_SUCCESS )
    {
        return -1;
    }
    while (!g_receive_complete)
    {
    }
    g_receive_complete = 0;

    data = (int32_t)(data_char &0xff);

    return data;
}

/*******************************************************************************
* Function Name: IoPutchar
* Description  : Character "buffer" is output to SCIF.
*              : This function keeps waiting until it becomes the transmission
*              : enabled state.
* Arguments    : int_t buffer : character to output
* Return Value : None
*******************************************************************************/
void IoPutchar (int32_t buffer)
{
      R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t*) &buffer, TRANSFER_LENGTH);
      while (!g_transfer_complete)
      {
      }
      g_transfer_complete = 0;
}


/* End of File */

