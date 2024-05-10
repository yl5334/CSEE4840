#include "controller.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Attempts to open two USB game controllers and returns their handles.
 * Exits the application if unable to find two controllers.
 */
struct controllers opencontrollers(void) {
    libusb_device **devs;
    struct libusb_device_handle *controller = NULL;
    struct controllers ctrls = {NULL, NULL, 0, 0};
    struct libusb_device_descriptor desc;
    ssize_t num_devs;
    int found = 0;

    if (libusb_init(NULL) < 0) {
        fprintf(stderr, "Error initializing libusb.\n");
        exit(EXIT_FAILURE);
    }

    if ((num_devs = libusb_get_device_list(NULL, &devs)) < 0) {
        fprintf(stderr, "Error enumerating USB devices.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_devs; i++) {
        libusb_device *dev = devs[i];
        if (libusb_get_device_descriptor(dev, &desc) < 0) {
            continue;
        }

        if (desc.bDeviceClass == LIBUSB_CLASS_PER_INTERFACE) {
            struct libusb_config_descriptor *config;
            libusb_get_config_descriptor(dev, 0, &config);

            for (int j = 0; j < config->bNumInterfaces; j++) {
                for (int k = 0; k < config->interface[j].num_altsetting; k++) {
                    const struct libusb_interface_descriptor *inter = &config->interface[j].altsetting[k];
                    
                    if (inter->bInterfaceClass == LIBUSB_CLASS_HID && inter->bInterfaceProtocol == USB_HID_GAME_CONTROLLER_PROTOCOL) {
                        if (libusb_open(dev, &controller) != 0 || libusb_claim_interface(controller, j) != 0) {
                            if (controller) {
                                libusb_close(controller);
                            }
                            controller = NULL;
                            continue;
                        }

                        if (found == 0) {
                            ctrls.controller1 = controller;
                            ctrls.endpoint1 = inter->endpoint[0].bEndpointAddress;
                            found++;
                        } else if (found == 1) {
                            ctrls.controller2 = controller;
                            ctrls.endpoint2 = inter->endpoint[0].bEndpointAddress;
                            found++;
                            goto found_two;
                        }
                    }
                }
            }
        }
    }

found_two:
    libusb_free_device_list(devs, 1);

    if (found != 2) {
        fprintf(stderr, "Failed to find two controllers.\n");
        exit(EXIT_FAILURE);
    }

    return ctrls;
}

