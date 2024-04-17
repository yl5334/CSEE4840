#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

struct controller_list {

        struct libusb_device_handle *device1;
        struct libusb_device_handle *device2;
        uint8_t device1_addr;
        uint8_t device2_addr;

};

struct controller_packet {

        uint8_t constant1;
        uint8_t constant2;
        uint8_t constant3;
        uint8_t x_dir;
        uint8_t y_dir;
        uint8_t prim;
        uint8_t sec;

};

typedef uint16_t controller_button;

#define dir_L          (((controller_button)1) << 0)
#define dir_R          (((controller_button)1) << 1)
#define dir_U          (((controller_button)1) << 2)
#define dir_D          (((controller_button)1) << 3)

#define button_A       (((controller_button)1) << 4)
#define button_B       (((controller_button)1) << 5)

#define button_SELECT  (((controller_button)1) << 6)
#define button_START   (((controller_button)1) << 7)

#define controller_default   ((controller_button)0)




void controller_init();
void controller_destroy()
void controller_set_listener(void(*listener)(controller_button));



#endif
