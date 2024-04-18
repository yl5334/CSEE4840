#include "controller.h"
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>


/*controller information*/
#define Controller_ID_VENDOR 0x79
#define Controller_ID_PRODUCT 0x11


struct libusb_device_handle *controllers_open(unit8_t *endpoint_address)

/*Controller Device Handler*/
struct libusb_device_handle *controllers_open(unit8_t *endpoint_address) {

        printf("Searching for USB connections...\n");

        struct controller_list devices;
        libusb_device **devs;
        struct libusb_device_descriptor desc;
        struct libusb_device_handle *controller = NULL;
        ssize_t num_devs;
        uint8_t i, k;


        // Boot libusb library
        if (libusb_init(NULL) != 0) {
                printf("\nERROR: libusb failed to boot");
                exit(1);
        }

        if  ((num_devs = libusb_get_device_list(NULL, &devs)) < 0) {
                printf("\nERROR: no controllers found");
                exit(1);
        }

        //printf("Detected %d devices...\n", num_devs);
        int connection_count = 0;
        for (int i = 0; i < num_devs; i++) {

                libusb_device *dev = devs[i];

                if (libusb_get_device_descriptor(dev, &desc) < 0) {
                        printf("\nERROR: create device descriptor fail.");
                        exit(1);
                }

                // Our controllers have idProduct of 17
                if (desc.idVendor == Controller_ID_VENDOR && desc.idProduct == Controller_ID_PRODUCT) {

                        //printf("FOUND: idProduct-%d ", desc.idProduct);
                        struct libusb_config_descriptor *config;
                        if ((libusb_get_config_descriptor(dev, 0, &config)) < 0) {
                                printf("\nERROR: create device descriptor fail.");
                                exit(1);
                        }
                        //printf("interfaces-%d\n", config->bNumInterfaces);

                        // Our controllers only have a single interface, no need for looping
                        // This interface also only has one .num_altsetting, no need for looping

                        int r;
                        const struct libusb_interface_descriptor *inter = config->interface[0].altsetting;
                        if ((r = libusb_open(dev, &controller)) != 0) {
                                printf("\nERROR: couldn't find controller");
                                exit(1);
                        }
                        if (libusb_kernel_driver_active(controller, 0)) {
                                libusb_detach_kernel_driver(controller, 0);
                        }
                        libusb_set_auto_detach_kernel_driver(controller, 0);
                        if ((r = libusb_claim_interface(controller, 0)) != 0) {
                                printf("\nERROR: couldn't claim controller.");
                                exit(1);
                        }

                        endpoint_address = inter->endpoint[0].bEndpointAddress;
                        connection_count++;

                        if (connection_count == 1) {
                                devices.device1 = controller;
                                devices.device1_addr = endpoint_address;
                        } else {
                                devices.device2 = controller;
                                devices.device2_addr = endpoint_address;
                                //printf("%d:%d,%d:%d\n",devices.device1,devices.device1_addr,devices.device2,devices.device2_addr)                                goto found;
                        }
                }
        }

        if (connection_count < 2) {
                printf("ERROR: couldn't find 2 controllers.");
                exit(1);
        }

        found:
                printf("Connected %d controllers!\n", connection_count);
                libusb_free_device_list(devs, 1);

        return devices;
}


void *controller_worker(void *arg);
void controller_events(controller_button next);
void controller_set_buttons(controller_butto buttons);

struct libusb_device_handle *controller_open(uint8_t *endpoint_address);

controller_button controller_decode_packet(struct controller_packet packet);


typedef struct{
    pthread_mutex_t mu;
    pthread_t tid;
    bool dead;
    //current button state
    controller_button buttons;
    // usb
    uint8_t endpoint;
    struct libusb_device_handle *controller;
    // called by joystick worker()
    void(*listener)(controller_button bs)
    } controller_state;

    static controller_state js;

    void controller_init()
    {
        int error;
        
        pthread_mutex_init(&js.mu, NULL);
        pthread_mutex_lock(&js.mu);
        
        js.dead = false;//is.listener =NULL:
        js.buttons = controller_default;
        if((js.controller = joystick_open(&js.endpoint))== NULL)
        {
            fprintf( stderr,"Did not find a controller!\n");
            exit(1);
        }
            
        if((error = pthread_create(&js.tid, NuLL, &controller_worker, NULL)) != 0)
        {
            fprintf(stderr, "create controller_worker failed: $s\n", strerror(error));
            exit(1);
        }

        pthread_mutex_unlock(&js.mu);
        printf("Controller initialized \n");
    }

    void controller_destroy()
    {
        pthread_mutex_lock(&js.mu);
        js.dead = true;
        pthread_mutex_unlock(&js.mu);
        pthread_join(js.tid, NULL);
        pthread_mutex_destroy(&js.mu);
        
        printf("Controller destroyed\n");
    }
    
    void controller_set_listener(void(*listener)(controller_button))
    {
        pthread_mutex_lock(&js.mu);
        js.listener = listener;
        pthread_mutex_unlock(&js.mu);

        printf("Set controller listener\n");
    }

    void *controller_worker(void *arg)
    {
        struct controller_packet packet;
        controller_button buttons;
        int transferred;
        
        while (true){
        pthread_mutex_lock(&js.mu);

        if(js.dead)
        {
            pthread_mutex_unlock(&js.mu);
            break;
        }
        
        libusb_interrupt_transfer(js.controller, js.endpoint, (unsigned char *)&packet,
                                sizeof(packet), &transferred, 0):
        buttons = controller_decode_packet(packet);

        controller_events(buttons);
        pthread_mutex_unlock(&js.mu);

        }
        printf("Create Controller Worker Successfully\n");
        return NULL;

    }

    void controller_events(controller_button next){
        if(js.listener == NULL)
            return;
        if(next == controller_default)
            js.listener(J0YSTICK DEFAULT);
        if(next == dir_L)
            js.listener(dir_L)
        if(next == dir_R)
            js.listener(dir_R);
        if(next == dir_U)
            js.listener(dir_U)
        if(next == dir_D)
            js.listener(dir_D);
        if(next == button_A)
            js.listener(button_A);
        if(next == button_B)
            js.listener(button_B);
        if(next == button_SELECT)
            js.listener(button_SELECT);
        if(next == button_START)
            js.listener(button_START);
    }

controller_button controller_decode_packet(struct controller_packet packet){
    controller_button buttons = 0;
    if(packet.x_dir== 0x11)
        buttons = controller_default;
    if(packet.x_dir == 0x00)
        buttons = dir_L;
    if(packet.x_dir== 0xff)
        buttons = dir_R;
    if(packet.y_dir == 0x00)
        buttons = dir_U;
    if(packet.y_dir == 0xff)
        buttons = dir_D;
    if(packet.prim & ( 1<< 6))
        buttons = button_B;
    if(packet.prim & ( 1 << 5))
        buttons = button_A;
    if(packet.sec &(1 << 5))
        buttons = button_START;
    if(packet.sec &(1<< 4))
    buttons = button_SELECT;
    return buttons;

}
