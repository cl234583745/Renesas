/***********************************************************************************************************************
 * File Name    : r_usb_phid_descriptor.c
 * Description  : Contains macros, data structure of usb phid
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include "r_usb_basic_api.h"
#include "r_usb_phid_api.h"
//#include "usb_phid_application.h"

/*******************************************************************************************************************//**
 * @addtogroup usb_phid_ep
 * @{
 **********************************************************************************************************************/
/******************************************************************************
 Macro definitions
 ******************************************************************************/

#define USB_BCDNUM               (0x0200U)                               /* bcdUSB */
#define USB_RELEASE              (0x0200U)                               /* Release Number */
#define USB_CONFIGNUM            (1U)                                    /* Configuration number */
#define USB_DCPMAXP              (64U)                                   /* DCP max packet size */

#define USB_IFPROTOCOL           (USB_IFPRO_KBD)                         /* Protocol code(None(0)/Keyboard(1)/Mouse(2)) */
#define MXPS                     (8)

#if INTERRUPT_OUT_ENDPOINT_ENABLE
#define NUM_EP                   (2)
#else
#define USB_IFPROTOCOL           (USB_IFPRO_KBD)
#define NUM_EP                   (1U)
#endif

/* [Vendor ID & Product ID setting] */
#define USB_VENDORID             (0x0000U)
#define USB_PRODUCTID            (0x0013U)

/* Descriptor length */
#define NUM_STRING_DESCRIPTOR    (7U)
#define HID_DESCRIPTOR_LENGTH    (9U)

/* Configuration Descriptor Length */
#if INTERRUPT_OUT_ENDPOINT_ENABLE
#define CD_LEN                   (USB_CD_BLENGTH + USB_ID_BLENGTH + HID_DESCRIPTOR_LENGTH + USB_ED_BLENGTH + USB_ED_BLENGTH)
#else
#define CD_LEN                   (USB_CD_BLENGTH + USB_ID_BLENGTH + HID_DESCRIPTOR_LENGTH + USB_ED_BLENGTH)
#endif

/* Descriptor data Mask */
#define USB_UCHAR_MAX            (0xffU)
#define USB_W_TOTAL_LENGTH_MASK  (256U)

#if 0
/************************************************************
 *  HID Report Descriptor for Echo mode                     *
 ************************************************************/
const uint8_t g_apl_report[] =
{
    0x05, 0x01, /* Usage Page (Generic Desktop)                    */
    0x09, 0x06, /* Usage (Keyboard)                                */
    0xA1, 0x01, /* Collection (Application)                        */

    0x05, 0x07, /* Usage Page (Key Codes)                          */
    0x19, 0xE0, /* Usage Minimum (Keyboard LeftControl)            */
    0x29, 0xE7, /* Usage Maximun (Keyboard Right GUI)              */
    0x15, 0x00, /* Logical Minimum (0)                             */
    0x25, 0x01, /* Logical Maximum (1)                             */

    0x75, 0x01, /* Report Size (1)                                 */
    0x95, 0x08, /* Report Count (8)                                */
    0x81, 0x02, /* Input (Data, Variable, Absolute); Modifier byte */
    0x95, 0x01, /* Report Count (1)                                */
    0x75, 0x08, /* Report Size (8)                                 */
    0x81, 0x03, /* Input (Constant); Reserved byte                 */
    0x95, 0x05, /* Report Count (5)                                */
    0x75, 0x01, /* Report Size (1)                                 */

    0x05, 0x08, /* Usage Page (Page# for LEDs)                     */
    0x19, 0x01, /* Usage Minimum (1)                               */
    0x29, 0x05, /* Usage Maximum (5)                               */
    0x91, 0x02, /* Output (Data, Variable, Absolute); LED report   */
    0x95, 0x01, /* Report Count (1)                                */
    0x75, 0x03, /* Report Size (3)                                 */
    0x91, 0x03, /* Output (Constant) ;LED report padding           */
    0x95, 0x06, /* Report Count (6)                                */
    0x75, 0x08, /* Report Size (8)                                 */
    0x15, 0x00, /* Logical Minimum (0)                             */
    0x25, 0x65, /* Logical Maximum(101)                            */

    0x05, 0x07, /* Usage Page (Key Codes)                          */
    0x19, 0x00, /* Usage Minimum (0)                               */
    0x29, 0x65, /* Usage Maximum (101)                             */
    0x81, 0x00, /* Input (Data, Array); Key arrays (6 bytes)       */
    0xc0        /* End Collection                                  */
};
#endif

uint8_t g_apl_report[] =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x00,                    // USAGE (Undefined)
    0xa1, 0x01,                    // COLLECTION (Application)

    //input
    0x09, 0x00,                    //   USAGE ID(Undefined)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)

    //output
    0x09, 0x00,                    //   USAGE (Undefined)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)

    0xc0                           // END_COLLECTION
};


#if 0
/************************************************************
 *  Device Descriptor                                       *
 ************************************************************/
uint8_t g_apl_device[] =
{
    USB_DD_BLENGTH,                                                      /*  0:bLength */
    USB_DT_DEVICE,                                                       /*  1:bDescriptorType */
    (uint8_t) ( USB_BCDNUM & (uint8_t) USB_UCHAR_MAX),                   /*  2:bcdUSB_lo */
    (uint8_t) ((uint8_t) (USB_BCDNUM >> 8) & (uint8_t) USB_UCHAR_MAX),   /*  3:bcdUSB_hi */
    0x03,                                                                /*  4:bDeviceClass */
    0x00,                                                                /*  5:bDeviceSubClass */
    0x00,                                                                /*  6:bDeviceProtocol */
    (uint8_t) USB_DCPMAXP,                                               /*  7:bMAXPacketSize(for DCP) */
    (uint8_t) (USB_VENDORID & (uint8_t) USB_UCHAR_MAX),                  /*  8:idVendor_lo */
    (uint8_t) ((uint8_t) (USB_VENDORID >> 8) & (uint8_t) USB_UCHAR_MAX), /*  9:idVendor_hi */
    (uint8_t) (USB_PRODUCTID & (uint8_t) USB_UCHAR_MAX),                 /* 10:idProduct_lo */
    (uint8_t) ((uint8_t) (USB_PRODUCTID >> 8) & (uint8_t) USB_UCHAR_MAX),/* 11:idProduct_hi */
    (uint8_t) (USB_RELEASE & (uint8_t) USB_UCHAR_MAX),                   /* 12:bcdDevice_lo */
    (uint8_t) ((uint8_t) (USB_RELEASE >> 8) & (uint8_t) USB_UCHAR_MAX),  /* 13:bcdDevice_hi */
    1,                                                                   /* 14:iManufacturer */
    2,                                                                   /* 15:iProduct */
    3,                                                                   /* 16:iSerialNumber */
    USB_CONFIGNUM,                                                       /* 17:bNumConfigurations */
};
#endif

uint8_t g_apl_device[] =
{
 0x12,                       /*bLength */
 USB_DT_DEVICE,       /*bDescriptorType*/
 0x00,                       /*bcdUSB */
 0x02,
 0x00,                       /*bDeviceClass*/
 0x00,                       /*bDeviceSubClass*/
 0x00,                       /*bDeviceProtocol*/
 0x40,           /*bMaxPacketSize*/
 0x24,           /*idVendor*/
 0x20,           /*idVendor*/
 0x17,        /*idProduct*/
 0x01,        /*idProduct*/
 0x00,                       /*bcdDevice rel. 2.00*/
 0x02,
 1,           /*Index of manufacturer  string*/
 2,       /*Index of product string*/
 3,        /*Index of serial number string*/
 USB_CONFIGNUM  /*bNumConfigurations*/
};

// #define USB_HID_DESCRIPTOR_LENGTH         (sizeof(g_apl_report))

uint8_t g_apl_configuration[41] =
{
    USB_CD_BLENGTH,                                             /*  0:bLength */
    USB_DT_CONFIGURATION,                                       /*  1:bDescriptorType */
    0x29,                                                         /*  2:wTotalLength(L) */
    0x00,                                                       /*  3:wTotalLength(H) */
    1,                                                          /*  4:bNumInterfaces */
    1,                                                          /*  5:bConfigurationValue */
    0x00,                                                       /*  6:iConfiguration */
    0xC0,                                                       /*  7:bmAttributes */
    0x32,                                                       /*  8:bMaxPower (2mA unit) */

    /* Interface Descriptor */
    0x09,                                             /*  0:bLength */
    0x04,                                                       /*  1:bDescriptor */
    0x00,                                                       /*  2:bInterfaceNumber */
    0x00,                                                       /*  3:bAlternateSetting */
    0x02,                                                       /*  4:bNumEndpoints */
    USB_IFCLS_HID,                                              /*  5:bInterfaceClass(HID) */
    USB_IFSUB_NOBOOT,                                           /*  6:bInterfaceSubClass(NonBOOT) */
    0x00,                                                       /*  7:bInterfaceProtocol */
    0,                                                          /*  8:iInterface */

    /* HID Descriptor */
    0x09,                                                          /*  0:bLength */
    USB_DT_TYPE_HIDDESCRIPTOR,                                  /*  1:bDescriptor */
    0x11,                                                       /*  2:HID Ver */
    0x01,                                                       /*  3:HID Ver */
    0x00,                                                       /*  4:bCountryCode */
    0x01,                                                       /*  5:bNumDescriptors */
    0x22,                                                       /*  6:bDescriptorType */
    0x21,                                                       /*  7:wItemLength(L) Total length of Report descriptor*/
    0x00,                                                       /*  8:wItemLength(H) */

    /* Endpoint Descriptor 0 */
    USB_ED_BLENGTH,                                             /*  0:bLength */
    USB_DT_ENDPOINT,                                            /*  1:bDescriptorType */
    (uint8_t) (USB_EP_IN | USB_EP6),                            /*  2:bEndpointAddress */
    USB_EP_INT,                                                 /*  3:bmAttribute */
    0x40,                                                       /*  4:wMaxPacketSize_lo */
    0,                                                          /*  5:wMaxPacketSize_hi */
    0x01,                                                       /*  6:bInterval */

    /* Endpoint Descriptor 1 */
     USB_ED_BLENGTH,                                             /*  0:bLength */
     USB_DT_ENDPOINT,                                            /*  1:bDescriptorType */
     (uint8_t) (USB_EP_OUT | USB_EP7),                            /*  2:bEndpointAddress */
     USB_EP_INT,                                                 /*  3:bmAttribute */
     0x40,                                                       /*  4:wMaxPacketSize_lo */
     0,                                                          /*  5:wMaxPacketSize_hi */
     0x01                                                       /*  6:bInterval */
};

#if 0
/************************************************************
 *  Configuration Descriptor                                *
 ************************************************************/
uint8_t g_apl_configuration[CD_LEN + (CD_LEN % 2)] =
{
    USB_CD_BLENGTH,                                             /*  0:bLength */
    USB_DT_CONFIGURATION,                                       /*  1:bDescriptorType */
    (uint8_t) (CD_LEN % USB_W_TOTAL_LENGTH_MASK),               /*  2:wTotalLength(L) */
    (uint8_t) (CD_LEN / USB_W_TOTAL_LENGTH_MASK),               /*  3:wTotalLength(H) */
    1,                                                          /*  4:bNumInterfaces */
    1,                                                          /*  5:bConfigurationValue */
    4,                                                          /*  6:iConfiguration */
    (uint8_t) (USB_CF_RESERVED | USB_CF_SELFP | USB_CF_RWUPON), /*  7:bmAttributes */
    (uint8_t) (100 / 2),                                        /*  8:bMaxPower (2mA unit) */

    /* Interface Descriptor */
    USB_ID_BLENGTH,                                             /*  0:bLength */
    USB_DT_INTERFACE,                                           /*  1:bDescriptor */
    0,                                                          /*  2:bInterfaceNumber */
    0,                                                          /*  3:bAlternateSetting */
    NUM_EP,                                                     /*  4:bNumEndpoints */
    USB_IFCLS_HID,                                              /*  5:bInterfaceClass(HID) */
    USB_IFSUB_NOBOOT,                                           /*  6:bInterfaceSubClass(NonBOOT) */
    USB_IFPROTOCOL,                                             /*  7:bInterfaceProtocol */
    0,                                                          /*  8:iInterface */

    /* HID Descriptor */
    9,                                                          /*  0:bLength */
    USB_DT_TYPE_HIDDESCRIPTOR,                                  /*  1:bDescriptor */
    0x10,                                                       /*  2:HID Ver */
    0x01,                                                       /*  3:HID Ver */
    0x21,                                                       /*  4:bCountryCode */
    NUM_EP,                                                     /*  5:bNumDescriptors */
    USB_DT_TYPE_RPTDESCRIPTOR,                                  /*  6:bDescriptorType */
    (uint8_t)(USB_HID_DESCRIPTOR_LENGTH),                       /*  7:wItemLength(L) */
    (uint8_t)(USB_HID_DESCRIPTOR_LENGTH >> 8),                  /*  8:wItemLength(H) */

    /* Endpoint descriptor (Interrupt-In) */
    USB_ED_BLENGTH,                                             /*  0:bLength */
    USB_DT_ENDPOINT,                                            /*  1:bDescriptorType */
    (uint8_t) (USB_EP_IN | USB_EP6),                            /*  2:bEndpointAddress */
    USB_EP_INT,                                                 /*  3:bmAttribute */
    (uint8_t)(0x8),                                             /* 4 wMaxPacketSize */
    (uint8_t)(0x8 >> 8),                                        /* 5 wMaxPacketSize */
    0x0A,                                                       /*  6:bInterval, Expressed in milliseconds. */

#if INTERRUPT_OUT_ENDPOINT_ENABLE
    /* Endpoint descriptor (Interrupt-Out) */
    USB_ED_BLENGTH,                                             /*  0:bLength */
    USB_DT_ENDPOINT,                                            /*  1:bDescriptorType */
    (uint8_t) (USB_EP_OUT | USB_EP7),                           /*  2:bEndpointAddress */
    USB_EP_INT,                                                 /*  3:bmAttribute */
    (uint8_t)(0x8),                                             /* 4 wMaxPacketSize */
    (uint8_t)(0x8 >> 8),                                        /* 5 wMaxPacketSize */
    0x8,                                                        /*  6:bInterval, Expressed in milliseconds. */
#endif
} ;
#endif


/************************************************************
 *  String Descriptor 0                                     *
 ************************************************************/
/* UNICODE 0x0409 English (United States) */
uint8_t g_apl_string0[] =
{
    4,                                                          /*  0:bLength */
    USB_DT_STRING,                                              /*  1:bDescriptorType */
    0x09, 0x04                                                  /*  2:wLANGID[0] */
};

/************************************************************
 *  String Descriptor 1                                     *
 ************************************************************/
/* 14:iManufacturer */
uint8_t g_apl_string1[] =
{
    16,                                                         /*  0:bLength */
    USB_DT_STRING,                                              /*  1:bDescriptorType */
    'R', 0x00,
    'e', 0x00,
    'n', 0x00,
    'e', 0x00,
    's', 0x00,
    'a', 0x00,
    's', 0x00,
};

/************************************************************
 *  String Descriptor 2                                     *
 ************************************************************/
/* 15:iProduct */

uint8_t g_apl_string2[] =
{
    32,                 /*  0:bLength */
    USB_DT_STRING,      /*  1:bDescriptorType */
    'U', 0x00,
    'S', 0x00,
    'B', 0x00,
    ' ', 0x00,
    'P', 0x00,
    'e', 0x00,
    'r', 0x00,
    'i', 0x00,
    ' ', 0x00,
    'H', 0x00,
    'I', 0x00,
    'D', 0x00,
    ' ', 0x00,
    'F', 0x00,
    'W', 0x00,
};

/************************************************************
 *  String Descriptor 3                                     *
 ************************************************************/
/* 16:iSerialNumber */
uint8_t g_apl_string3[] =
{
    10,                 /*  0:bLength */
    USB_DT_STRING,      /*  1:bDescriptorType */
    'P', 0x00,
    '0', 0x00,
    '0', 0x00,
    '1', 0x00,
};

/************************************************************
 *  String Descriptor 4(iConfiguration)                     *
 ************************************************************/
uint8_t g_apl_string4[] =
{
    24,                 /*  0:bLength */
    USB_DT_STRING,      /*  1:bDescriptorType */

     /*  2:bString */
    'F', 0x00,
    'S', 0x00,
    ' ', 0x00,
    'K', 0x00,
    'e', 0x00,
    'y', 0x00,
    'b', 0x00,
    'o', 0x00,
    'a', 0x00,
    'r', 0x00,
    'd', 0x00
};

/************************************************************
 *  String Descriptor Table address                         *
 ************************************************************/
uint8_t * g_apl_string_table[] =
{
    g_apl_string0,                                  /* UNICODE 0x0409 English (United States) */
    g_apl_string1,                                  /* iManufacturer */
    g_apl_string2,                                  /* iProduct */
    g_apl_string3,                                  /* iSerialNumber */
    g_apl_string4,                                  /* iConfiguration */
};

const usb_descriptor_t g_usb_descriptor =
{
    g_apl_device,                   /* Pointer to the device descriptor */
    g_apl_configuration,            /* Pointer to the configuration descriptor for Full-speed */
    NULL,                           /* Pointer to the configuration descriptor for Hi-speed */
    NULL,                           /* Pointer to the qualifier descriptor */
    g_apl_string_table,             /* Pointer to the string descriptor table */
    NUM_STRING_DESCRIPTOR
};

/*******************************************************************************************************************//**
 * @} (end addtogroup usb_phid_ep)
 **********************************************************************************************************************/
/******************************************************************************
 End  Of File
 ******************************************************************************/
