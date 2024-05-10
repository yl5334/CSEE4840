#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <libusb-1.0/libusb.h>


#define USB_HID_GAME_CONTROLLER_PROTOCOL 1


#define BUTTON_UP    (1 << 0)
#define BUTTON_DOWN  (1 << 1)
#define BUTTON_LEFT  (1 << 2)
#define BUTTON_RIGHT (1 << 3)
#define BUTTON_A     (1 << 4)
#define BUTTON_B     (1 << 5)
#define BUTTON_SELECT (1 << 6)
#define BUTTON_START (1 << 7)

struct controller_packet {
    uint8_t buttons;  // 
};

struct controllers {
    struct libusb_device_handle *controller1;
    struct libusb_device_handle *controller2;
    uint8_t endpoint1;
    uint8_t endpoint2;
};


extern struct controllers opencontrollers(void);

#endif

