/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */
#include "hid_events.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb custom HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_HS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 1 */

  /* ============ Report ID 1: Mouse (52 bytes) ============
     Report layout (4 bytes): [ID=1][buttons: bit0=left, bit1=right][dX int8][dY int8] */
  0x05, 0x01,        /* Usage Page (Generic Desktop)        */
  0x09, 0x02,        /* Usage (Mouse)                       */
  0xA1, 0x01,        /* Collection (Application)            */
  0x85, 0x01,        /*   Report ID (1)                     */
  0x09, 0x01,        /*   Usage (Pointer)                   */
  0xA1, 0x00,        /*   Collection (Physical)             */
  0x05, 0x09,        /*     Usage Page (Button)             */
  0x19, 0x01,        /*     Usage Minimum (Button 1)        */
  0x29, 0x02,        /*     Usage Maximum (Button 2)        */
  0x15, 0x00,        /*     Logical Minimum (0)             */
  0x25, 0x01,        /*     Logical Maximum (1)             */
  0x95, 0x02,        /*     Report Count (2)                */
  0x75, 0x01,        /*     Report Size (1)                 */
  0x81, 0x02,        /*     Input (Data,Var,Abs) - buttons  */
  0x95, 0x01,        /*     Report Count (1)                */
  0x75, 0x06,        /*     Report Size (6)                 */
  0x81, 0x03,        /*     Input (Const) - 6 bit padding   */
  0x05, 0x01,        /*     Usage Page (Generic Desktop)    */
  0x09, 0x30,        /*     Usage (X)                       */
  0x09, 0x31,        /*     Usage (Y)                       */
  0x15, 0x81,        /*     Logical Minimum (-127)          */
  0x25, 0x7F,        /*     Logical Maximum (127)           */
  0x75, 0x08,        /*     Report Size (8)                 */
  0x95, 0x02,        /*     Report Count (2)                */
  0x81, 0x06,        /*     Input (Data,Var,Rel) - X,Y      */
  0xC0,              /*   End Collection (Physical)         */
  0xC0,              /* End Collection (Application)        */

  /* ============ Report ID 2: Keyboard (47 bytes) ============
     Report layout (9 bytes): [ID=2][modifiers][reserved][key1..key6] */
  0x05, 0x01,        /* Usage Page (Generic Desktop)        */
  0x09, 0x06,        /* Usage (Keyboard)                    */
  0xA1, 0x01,        /* Collection (Application)            */
  0x85, 0x02,        /*   Report ID (2)                     */
  0x05, 0x07,        /*   Usage Page (Keyboard/Keypad)      */
  0x19, 0xE0,        /*   Usage Minimum (Left Control)      */
  0x29, 0xE7,        /*   Usage Maximum (Right GUI)         */
  0x15, 0x00,        /*   Logical Minimum (0)               */
  0x25, 0x01,        /*   Logical Maximum (1)               */
  0x75, 0x01,        /*   Report Size (1)                   */
  0x95, 0x08,        /*   Report Count (8)                  */
  0x81, 0x02,        /*   Input (Data,Var,Abs) - modifiers  */
  0x95, 0x01,        /*   Report Count (1)                  */
  0x75, 0x08,        /*   Report Size (8)                   */
  0x81, 0x03,        /*   Input (Const) - reserved byte     */
  0x95, 0x06,        /*   Report Count (6)                  */
  0x75, 0x08,        /*   Report Size (8)                   */
  0x15, 0x00,        /*   Logical Minimum (0)               */
  0x25, 0x65,        /*   Logical Maximum (101)             */
  0x05, 0x07,        /*   Usage Page (Keyboard/Keypad)      */
  0x19, 0x00,        /*   Usage Minimum (0)                 */
  0x29, 0x65,        /*   Usage Maximum (101)               */
  0x81, 0x00,        /*   Input (Data,Array) - 6 keycodes   */
  0xC0,              /* End Collection                      */

  /* ============ Report ID 3: Consumer Control (29 bytes) ============
     Report layout (2 bytes): [ID=3][bitmap: bit0=Vol+, bit1=Vol-, bit2=Play/Pause]
     NOTE: the final 0xC0 (End Collection) is the generated byte just after
     this USER CODE block - do not add it here. Total descriptor = 128 bytes. */
  0x05, 0x0C,        /* Usage Page (Consumer)               */
  0x09, 0x01,        /* Usage (Consumer Control)            */
  0xA1, 0x01,        /* Collection (Application)            */
  0x85, 0x03,        /*   Report ID (3)                     */
  0x15, 0x00,        /*   Logical Minimum (0)               */
  0x25, 0x01,        /*   Logical Maximum (1)               */
  0x75, 0x01,        /*   Report Size (1)                   */
  0x95, 0x03,        /*   Report Count (3)                  */
  0x09, 0xE9,        /*   Usage (Volume Increment)  bit0    */
  0x09, 0xEA,        /*   Usage (Volume Decrement)  bit1    */
  0x09, 0xCD,        /*   Usage (Play/Pause)        bit2    */
  0x81, 0x02,        /*   Input (Data,Var,Abs)              */
  0x95, 0x05,        /*   Report Count (5)                  */
  0x81, 0x03,        /*   Input (Const) - 5 bit padding     */
  /* USER CODE END 1 */
   0xC0    /*     END_COLLECTION             */
};
/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_HS(void);
static int8_t CUSTOM_HID_DeInit_HS(void);
static int8_t CUSTOM_HID_OutEvent_HS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_HS =
{
  CUSTOM_HID_ReportDesc_HS,
  CUSTOM_HID_Init_HS,
  CUSTOM_HID_DeInit_HS,
  CUSTOM_HID_OutEvent_HS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_HS(void)
{
  /* USER CODE BEGIN 8 */
  return (USBD_OK);
  /* USER CODE END 8 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_HS(void)
{
  /* USER CODE BEGIN 9 */
  return (USBD_OK);
  /* USER CODE END 9 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_HS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 10 */
  UNUSED(event_idx);
  UNUSED(state);

    /* Start next USB packet transfer once data processing is completed */
  if (USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceHS) != (uint8_t)USBD_OK)
  {
    return -1;
  }

  return (USBD_OK);
  /* USER CODE END 10 */
}

/* USER CODE BEGIN 11 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_HS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, report, len);
}
*/
/* USER CODE END 11 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/* Report senders (prototypes in hid_events.h). Only usbHidTask may call
   these. Buffers are static: the OTG core copies data to the TxFIFO from
   the interrupt after USBD_CUSTOM_HID_SendReport() has already returned,
   so a stack buffer would be invalid by then. */

/**
  * @brief  Send mouse report ID 1: [1][buttons][dX][dY]
  * @param  buttons: bit0 = left, bit1 = right
  * @param  x, y: relative movement, -127..127
  */
void SendMouse(uint8_t buttons, int8_t x, int8_t y)
{
  static uint8_t report[4];

  report[0] = HID_REPORTID_MOUSE;
  report[1] = buttons & 0x03U;
  report[2] = (uint8_t)x;
  report[3] = (uint8_t)y;

  (void)USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, report, sizeof(report));
}

/**
  * @brief  Send keyboard report ID 2: [2][modifiers][reserved][key1..key6]
  * @param  keycode: usage from keyboard page 0x07 (placed in key1)
  * @param  pressed: 1 = press keycode, 0 = release all keys
  */
void SendKey(uint8_t keycode, uint8_t pressed)
{
  static uint8_t report[9];

  memset(report, 0, sizeof(report));
  report[0] = HID_REPORTID_KEYBOARD;
  if (pressed != 0U)
  {
    report[3] = keycode; /* key1 slot */
  }

  (void)USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, report, sizeof(report));
}

/**
  * @brief  Send consumer report ID 3: [3][bitmap]
  * @param  usage: 0xE9 = Vol+ (bit0), 0xEA = Vol- (bit1), 0xCD = Play/Pause (bit2)
  * @param  pressed: 1 = press usage, 0 = release all
  */
void SendConsumer(uint8_t usage, uint8_t pressed)
{
  static uint8_t report[2];
  uint8_t bits = 0U;

  if (pressed != 0U)
  {
    switch (usage)
    {
    case HID_CONSUMER_VOL_UP:
      bits = 0x01U;
      break;
    case HID_CONSUMER_VOL_DOWN:
      bits = 0x02U;
      break;
    case HID_CONSUMER_PLAY_PAUSE:
      bits = 0x04U;
      break;
    default:
      break;
    }
  }

  report[0] = HID_REPORTID_CONSUMER;
  report[1] = bits;

  (void)USBD_CUSTOM_HID_SendReport(&hUsbDeviceHS, report, sizeof(report));
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

