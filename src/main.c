#include <ti/screen.h>
#include <ti/getcsc.h>
#include <stdlib.h>
#include <stdint.h>
#include <ti/error.h>
#include <sys/timers.h>
#include <string.h>
#include <stdio.h>
#include <usbdrvce.h>
#include <keypadc.h>

#include "exceptions.h"

int entrypoint(void);
void cleanup(void);
void setup(void);

int main(void) {
    try({
        return entrypoint();
    }, e, {
        cleanup();

        char excname[13] = {0};
        sprintf(excname, "ERR 0x%X", e);
        strncpy(os_AppErr1, excname, 13);
        os_ThrowError(OS_E_APPERR1);
    })
}

usb_error_t usb_eventhandler(usb_event_t event, void *event_data, usb_callback_data_t *callback_data) {
    printf("usb event: %u\n", event);
    if (event == USB_ROLE_CHANGED_EVENT) {
        printf("role changed event\n");
        usb_role_t *new_role = event_data;

        if (*new_role & USB_ROLE_HOST)
            printf("USB_ROLE_HOST\n");
        else if (*new_role & USB_ROLE_DEVICE)
            printf("USB_ROLE_DEVICE\n");
        if (*new_role & USB_ROLE_A)
            printf("USB_ROLE_A\n");
        if (*new_role & USB_ROLE_B)
            printf("USB_ROLE_B\n");
    }

    return USB_SUCCESS;
}

void setup(void) {
    usb_error_t result = usb_Init(*usb_eventhandler, NULL, NULL, USB_USE_OS_HEAP);
    
    switch (result) {
        case USB_SUCCESS:
            printf("usb initialized successfully\n");
            break;
        
        default:
            printf("usb failed: %u\n", result);
            while (!os_GetCSC());
            throw(EXCEPTION_USB_GENERIC);

            break;
    }

    os_ClrHome();
}

void cleanup(void) {
    usb_Cleanup();
}

/* Main function, called first */
int entrypoint(void)
{
    setup();

    uint24_t time = usb_GetCounter();

    kb_Scan();
    while (!(kb_Data[6] & kb_Clear)) {
        kb_Scan();
        uint24_t new_time = usb_GetCounter();
        //printf("%u\n", new_time);
        if (new_time < (time + 500000) && new_time >= time) {
            // printf("continuing\n");
            continue;
        }

        printf("we've made it\n");

        time = new_time;

        usb_HandleEvents();
        
        if (kb_Data[4] & kb_5) {
            
            printf("Looping through connected devices.\n");
            
            int counter = 0;
            usb_device_t device = NULL;
            while ((device = usb_FindDevice(NULL, device, USB_SKIP_HUBS))) {
                printf("Device %i\n", counter);
                
                usb_device_descriptor_t descriptor = {0};
                size_t transfered;
                usb_error_t error = usb_GetDeviceDescriptor(device, &descriptor, sizeof(descriptor), &transfered);

                printf("device: %p\n", device);
                printf("getdec: %u\n", error);
                while (!os_GetCSC());
                printf("tranf: %u\n", transfered);
                while (!os_GetCSC());
                printf("bLength: %hhu\n", descriptor.bLength);
                while (!os_GetCSC());
                printf("bDescriptorType: %hhu\n", descriptor.bDescriptorType);
                while (!os_GetCSC());
                printf("bDeviceClass: %hhu\n", descriptor.bDeviceClass);
                while (!os_GetCSC());
                printf("bDeviceSubClass: %hhu\n", descriptor.bDeviceSubClass);
                while (!os_GetCSC());
                printf("bDeviceProtocol: %hhu\n", descriptor.bDeviceProtocol);
                while (!os_GetCSC());
                printf("bMaxPacketSize: %hhu\n", descriptor.bMaxPacketSize0);
                while (!os_GetCSC());
                printf("iManufacturer: %hhu\n", descriptor.iManufacturer);
                while (!os_GetCSC());
                printf("iProduct: %hhu\n", descriptor.iProduct);
                while (!os_GetCSC());
                printf("iSerialNumber: %hhu\n", descriptor.iSerialNumber);
                while (!os_GetCSC());
                printf("bNumConfigurations: %hhu\n", descriptor.bNumConfigurations);
                while (!os_GetCSC());

                //usb_GetConfigurationDescriptor(device, 0, usb_GetConfigurationDescriptorTotalLength(device, 0));

                //usb_HandleEvents();
                counter++;
            }
        }
        
    }

    /* Return 0 for success */
    cleanup();
    return 0;
}


// computer: 28, 29, 15, 13
// nothing: 11, 14, 13
// flash drive: 29, 15, 13, 17??
    // with the second bit: 29, 15, 13 | 29, 15, 13, 2 (device connected!!), 17
