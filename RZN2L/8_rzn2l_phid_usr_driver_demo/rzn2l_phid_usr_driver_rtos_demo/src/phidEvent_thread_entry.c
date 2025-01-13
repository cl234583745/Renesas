/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "phidEvent_thread.h"

#include <stdio.h>

#include "usr_hal_data.h"
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_usb_phid_api.h"
#include"logger.h"
static enum LOG_LEVEL logger=LOG_LEVEL_ALL;
/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#define USB_RECEIVE_REPORT_DESCRIPTOR    (76U)              // report descriptor
#define USB_RECEIVE_HID_DESCRIPTOR       (9U)               // hid descriptor
#define DATA_LEN                         (8U)               // data length
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
#define RESET_VALUE             (0x00)
/*****************************************************************************
 * Typedef definitions
 ******************************************************************************/
/******************************************************************************
 * Exported global variables (to be accessed by other files)
 ******************************************************************************/
extern uint8_t g_apl_configuration[];
extern uint8_t g_apl_report[];

/******************************************************************************
 * Private global variables and functions
 ******************************************************************************/
void usb_phid_callback(usb_event_info_t * p_usb_event , usb_hdl_t task, usb_onoff_t state);

static void usb_enumeration(void);
static void usb_status_update(void);
static void usb_write_operation(void);
static void deinit_usb(void);


/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/* Error status flag */
static volatile bool g_err_flag = false;
static usb_event_info_t * p_usb_phid_event = NULL;
static uint16_t g_numlock = RESET_VALUE;
static uint8_t send_data[BUFF_SIZE] BSP_ALIGN_VARIABLE(ALIGN);
static uint8_t * p_idle_value = NULL;
static uint8_t g_idle = RESET_VALUE;
static uint8_t g_buf[DATA_LEN]  = {RESET_VALUE,}; /* HID NULL data */
static uint8_t g_data[DATA_LEN] = {RESET_VALUE,};

 uint8_t send_buf[64] =   {30,31,32,33,33,32,31};
/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/* New Thread entry function */
/* pvParameters contains TaskHandle_t */
void phidEvent_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    BaseType_t err_queue       = pdFALSE;

    __enable_irq();
    __ISB();

    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    printf("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);

    float PI = 3.1415926f;
    printf("PI=%f\n", PI);

    fsp_err_t err = FSP_SUCCESS;
    /* Open USB instance */
    err = R_USB_Open(&g_basic0_ctrl, &g_basic0_cfg);
    if (FSP_SUCCESS != err)
    {
        log_err(logger,"\r\nR_USB_Open failed\r\n");
        while(1);
    }

    while (true)
    {
        /* Check if USB event is received */
        err_queue = xQueueReceive(g_phidEvent_queue, &p_usb_phid_event, (portMAX_DELAY));
        if(pdTRUE != err_queue)
        {
            log_err(logger,"\r\nNo USB Event received. Please check USB connection \r\n");
        }

        log_info(logger,"xQueueReceive: event=%d\r\n", p_usb_phid_event->event);
        /* check for usb event */
        switch (p_usb_phid_event->event)
        {
            /* Configured event */
            case USB_STATUS_CONFIGURED:
            {
                log_info(logger,"USB STATUS :USB_STATUS_CONFIGURED:USB Status Configured Successful\r\n");
                break;
            }
            /* Receive Class Request */
            case USB_STATUS_REQUEST:
            {
                log_info(logger,"USB STATUS :USB_STATUS_REQUEST:\r\n");
                usb_enumeration();
                break;
            }
            /* Complete Class Request */
            case USB_STATUS_REQUEST_COMPLETE:
            {
                log_info(logger,"USB STATUS :USB_STATUS_REQUEST_COMPLETE:\r\n");
                usb_status_update();
                break;
            }
            case USB_STATUS_READ_COMPLETE:
            {
                R_USB_Read(&g_basic0_ctrl, (uint8_t *) &send_buf, 64, USB_CLASS_PHID);
                log_info(logger,"USB STATUS :USB_STATUS_READ_COMPLETE: USB_CLASS_PHID\r\n");
                break;
            }
            /* Write complete event */
            case USB_STATUS_WRITE_COMPLETE:
            {
                log_info(logger,"USB STATUS :USB_STATUS_WRITE_COMPLETE\r\n");
                vTaskDelay(1);

                //usb_write_operation();
                break;
            }
            /* Suspend event */
            case USB_STATUS_SUSPEND:
            {
                log_info(logger,"USB STATUS : USB_STATUS_SUSPEND\r\n");
                break;
            }
            /* detach event */
            case USB_STATUS_DETACH:
            {
                log_info(logger,"USB STATUS : USB_STATUS_SUSPEND\r\n");
                break;
            }
            default:
            {
                break;
            }
        }
        vTaskDelay (1);
    }

}

/**
 *
 * @param p_usb_event
 * @param task
 * @param state
 */
void usb_phid_callback(usb_event_info_t * p_usb_event , usb_hdl_t task, usb_onoff_t state)
{
    FSP_PARAMETER_NOT_USED (task);
    FSP_PARAMETER_NOT_USED (state);
    /* capture the usb event info */
    if (pdTRUE != (xQueueSendFromISR(g_phidEvent_queue, (const void *)&p_usb_event, NULL)))
    {
        g_err_flag = true;
    }
}
/*
 *
 */
static void usb_enumeration(void)
{
    fsp_err_t err = FSP_SUCCESS;
    //log_info(logger,"USB_SET_REPORT:p_usb_phid_event->setup.request_type=0x%x", p_usb_phid_event->setup.request_type);
    /* check for request type */
    if (USB_SET_REPORT == (p_usb_phid_event->setup.request_type & USB_BREQUEST))
    {
        /* Get the NumLock data */
        err = R_USB_Read(&g_basic0_ctrl, (uint8_t *) &g_numlock, SIZE_NUM, USB_CLASS_PHID);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"R_USB_Read failed err=%d", err);
            deinit_usb();
            return;
        }
        log_info(logger,"USB_SET_REPORT:p_usb_phid_event->setup.request_type=0x%x", p_usb_phid_event->setup.request_type);
    }
    else if (USB_GET_DESCRIPTOR == (p_usb_phid_event->setup.request_type & USB_BREQUEST))
    {
        /* check for request value */
        if (USB_GET_REPORT_DESCRIPTOR == p_usb_phid_event->setup.request_value)
        {
            err = R_USB_PeriControlDataSet (&g_basic0_ctrl,(uint8_t *)g_apl_report, USB_RECEIVE_REPORT_DESCRIPTOR);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"\r\nR_USB_PeriControlDataSet failed\r\n");
                deinit_usb();
                //APP_ERR_TRAP(err);
            }
            log_info(logger,"USB_GET_REPORT_DESCRIPTOR:p_usb_phid_event->setup.request_type=0x%x, p_usb_phid_event->setup.request_value=0x%x",p_usb_phid_event->setup.request_type, p_usb_phid_event->setup.request_value);

        }
        else if (USB_GET_HID_DESCRIPTOR == p_usb_phid_event->setup.request_value)
        {
            for (uint8_t hid_des = RESET_VALUE; hid_des < USB_RECEIVE_HID_DESCRIPTOR; hid_des++)
            {
                send_data[hid_des] = g_apl_configuration[CD_LENGTH + hid_des];
            }

            /* Configuration Descriptor address set. */
            err = R_USB_PeriControlDataSet(&g_basic0_ctrl, send_data, USB_RECEIVE_HID_DESCRIPTOR);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"\r\nR_USB_PeriControlDataSet failed\r\n");
                deinit_usb();
                //APP_ERR_TRAP(err);
            }
            log_info(logger,"USB_GET_HID_DESCRIPTOR:p_usb_phid_event->setup.request_type=0x%x, p_usb_phid_event->setup.request_value=0x%x",p_usb_phid_event->setup.request_type, p_usb_phid_event->setup.request_value);
        }
        else
        {
            /* None */
        }
    }
    else if (USB_SET_IDLE == (p_usb_phid_event->setup.request_type & USB_BREQUEST))
    {
        /* Get SetIdle value */
        p_idle_value = (uint8_t *) &p_usb_phid_event->setup.request_value;

        g_idle = p_idle_value[IDLE_VAL_INDEX];

        err = R_USB_PeriControlStatusSet(p_usb_phid_event, USB_SETUP_STATUS_ACK);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"\r\nR_USB_PeriControlStatusSet failed\r\n");
            deinit_usb();
            //APP_ERR_TRAP(err);
        }
        log_info(logger,"USB_SET_IDLE:p_usb_phid_event->setup.request_type=0x%x, g_idle=0x%x", p_usb_phid_event->setup.request_type, g_idle);

    }
    else
    {
        /* None */
    }
}
/*
 *
 */
static void usb_status_update(void)
{
    fsp_err_t err = FSP_SUCCESS;
    /* check for request type */
    if (USB_SET_IDLE == (p_usb_phid_event->setup.request_type & USB_BREQUEST))
    {
        p_idle_value = (uint8_t *) &p_usb_phid_event->setup.request_value;

        g_idle = p_idle_value[IDLE_VAL_INDEX];
        log_info(logger,"USB_SET_IDLE:p_idle_value = 0x%x\r\n", p_idle_value);
    }
    else if (USB_SET_PROTOCOL == (p_usb_phid_event->setup.request_type & USB_BREQUEST))
    {
        /* None */
    }
    else if (USB_SET_REPORT == (p_usb_phid_event->setup.request_type & USB_BREQUEST))
    {
        /* Print status of g_numlock */
        log_info(logger,"USB_SET_REPORT\r\n");
    }
    else
    {
        /* Sending the zero data (8 bytes) */
        err = R_USB_Write(&g_basic0_ctrl, g_buf, 64, USB_CLASS_PHID);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"\r\nR_USB_Write failed\r\n");
            deinit_usb();
            //APP_ERR_TRAP(err);
        }
        log_info(logger,"usb_status_update:Sending the zero data (8 bytes)\r\n");
    }
}

/*******************************************************************************************************************//**
 * @brief       usb write operation
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void usb_write_operation(void)
{
    fsp_err_t err = FSP_SUCCESS;
    /* check flag from user input */
    if(1)// (true == g_write_flag)
    {
        /* Usage ID of A */
        static uint8_t data = USAGE_ID_A;

        if (USAGE_ID_0 >= data)
        {
            /* copy data to 3rd Index of buffer*/
            g_data[INDEX_THIRD] = data;
            /* increment the data */
            data++;
            /* Sending the 8 bytes data */
            err = R_USB_Write(&g_basic0_ctrl, (uint8_t *) g_data, DATA_LEN, USB_CLASS_PHID);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"\r\nR_USB_Write failed\r\n");
                deinit_usb();
                //APP_ERR_TRAP(err);
            }
        }
        else
        {
            data = USAGE_ID_A;

            /* Sending the zero data (8 bytes) */
            err = R_USB_Write(&g_basic0_ctrl, (uint8_t *) g_buf, DATA_LEN, USB_CLASS_PHID);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"\r\nR_USB_Write failed\r\n");
                deinit_usb();
                //APP_ERR_TRAP(err);
            }
        }
    }
    else
    {
        /* Sending the zero data (8 bytes) */
        err = R_USB_Write(&g_basic0_ctrl, (uint8_t *) g_buf, DATA_LEN, USB_CLASS_PHID);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"\r\nR_USB_Write failed\r\n");
            deinit_usb();
            //APP_ERR_TRAP(err);
        }
    }
}

/*******************************************************************************************************************//**
 * @brief       de-initialize USB
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void deinit_usb(void)
{
    fsp_err_t err = FSP_SUCCESS;
    /* close usb instance */
    err = R_USB_Close(&g_basic0_ctrl);
    if (FSP_SUCCESS != err)
    {
        log_err(logger,"\r\nR_USB_Close\r\n ");
    }
}
