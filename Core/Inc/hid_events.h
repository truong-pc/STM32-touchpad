/**
  ******************************************************************************
  * @file    hid_events.h
  * @brief   Shared HID event definitions between TouchGFX (C++), the button
  *          task and the USB HID task. Every event source pushes one 4-byte
  *          HID_Event_t into hidQueue; only usbHidTask reads the queue and
  *          talks to USB.
  ******************************************************************************
  */

#ifndef HID_EVENTS_H
#define HID_EVENTS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Report IDs used in the composite HID report descriptor */
#define HID_REPORTID_MOUSE      1U
#define HID_REPORTID_KEYBOARD   2U
#define HID_REPORTID_CONSUMER   3U

/* Keyboard usages (HID Usage Page 0x07 - Keyboard/Keypad) */
#define HID_KEY_RIGHT_ARROW     0x4FU
#define HID_KEY_LEFT_ARROW      0x50U
#define HID_KEY_DOWN_ARROW      0x51U
#define HID_KEY_UP_ARROW        0x52U

/* Consumer usages (HID Usage Page 0x0C - Consumer) */
#define HID_CONSUMER_PLAY_PAUSE 0xCDU
#define HID_CONSUMER_VOL_UP     0xE9U
#define HID_CONSUMER_VOL_DOWN   0xEAU

typedef enum
{
    EV_MOUSE    = 0,
    EV_KEYBOARD = 1,
    EV_CONSUMER = 2
} HID_EvType_t;

typedef struct
{
    uint8_t type;   /* HID_EvType_t */
    uint8_t a;      /* MOUSE: buttons (bit0=left, bit1=right) | KEYBOARD/CONSUMER: usage code */
    int8_t  b;      /* MOUSE: deltaX | KEYBOARD/CONSUMER: pressed (1=press, 0=release) */
    int8_t  c;      /* MOUSE: deltaY | unused otherwise */
} HID_Event_t;

#ifdef __cplusplus
static_assert(sizeof(HID_Event_t) == 4, "HID_Event_t phai 4 byte");
#else
_Static_assert(sizeof(HID_Event_t) == 4, "HID_Event_t phai 4 byte");
#endif

/* Report senders, implemented in usbd_custom_hid_if.c.
   Only usbHidTask may call these. */
void SendMouse(uint8_t buttons, int8_t x, int8_t y);
void SendKey(uint8_t keycode, uint8_t pressed);
void SendConsumer(uint8_t usage, uint8_t pressed);

#ifdef __cplusplus
}
#endif

#endif /* HID_EVENTS_H */
