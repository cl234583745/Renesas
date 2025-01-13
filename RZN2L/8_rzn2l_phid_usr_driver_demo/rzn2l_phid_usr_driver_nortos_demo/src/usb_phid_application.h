/*
 * usb_phid_application.h
 *
 *  Created on: Jul 6, 2021
 *      Author: a5123412
 */

#ifndef USB_PHID_APPLICATION_H_
#define USB_PHID_APPLICATION_H_

#define EP_INFO "\r\nThis project demonstrates the basic functionalities of USB PHID driver on \r\n"\
                "Renesas RA MCUs based on Renesas FSP. The project configures RA board as a mouse.\r\n"\
                "On pressing supported key, which includes \"a - z\" & numbers \"0 - 9\", from RTTViewer,\r\n"\
                "MCU sends the corresponding host PC character.\r\n"

#define INTERRUPT_OUT_ENDPOINT_ENABLE    (1)




/*****************************************************************************
 * Macro definitions
 ******************************************************************************/
#if 1
#define USB_RECEIVE_REPORT_DESCRIPTOR    (76U)              // report descriptor
#define USB_RECEIVE_HID_DESCRIPTOR       (9U)               // hid descriptor
#define DATA_LEN                         (64U)               // data length
#define SIZE_NUM                         (2U)               // numlock data size
#define USAGE_ID_A                       (0x04)             // keyboard usage id of A
#define USAGE_ID_0                       (0x27)             // keyboard usage id of 0
#define DELAY                            (100U)             // delay for write
#define INDEX_THIRD                      (2U)               // third index for usb buffer
#define TASK_DELAY                       (1U)               // delay for task
#define CD_LENGTH                        (18U)              // config descriptor length
#define IDLE_VAL_INDEX                   (1U)               // index value
#define BUFF_SIZE                        (16U)              // size of send data buffer
#define ALIGN                            (4U)               // alignment
#define DELAY_3_SEC                      (3000U)            // 3 seconds delay
#endif

/*****************************************************************************
 * Typedef definitions
 ******************************************************************************/
typedef volatile struct usb_event_setup
{
    uint8_t noneFlg;
    uint8_t configedFlg;
    uint8_t requestFlg;
    uint8_t requestCompleteFlg;
    uint8_t writeCompleteFlg;
    uint8_t readCompleteFlg;
    uint8_t suspendFlg;
    uint8_t resumeFlg;
    uint8_t detachFlg;

    uint8_t getDescriptorFlg;
    uint8_t readyToWorkFlg;
    uint8_t readyToReadFlg;

} usb_event_setup_t;

/******************************************************************************
Functions Prototypes
******************************************************************************/
void usb_phid_main (void);




#endif /* USB_PHID_APPLICATION_H_ */
