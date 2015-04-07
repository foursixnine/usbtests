#include <iostream>
#include <limits>
#include <string>
#include <stdlib.h>
#include <fstream>      // std::ofstream
#include "libusb-1.0/libusb.h"
#include <mutex>

/** Need to look a little bit more closer to:
 *
 * https://github.com/clarkli86/hidapi/blob/master/libusb/hid.c
 *
 * */

#undef max

using namespace std;

union int_thing {
        unsigned char usb_data[4];
        int32_t number;
};



        std::ofstream ofs;
        int *completed;

std::mutex mtx;

void error(string s, int err) {
        cout << s << " Error: " << libusb_error_name(err)  << endl << flush;
        exit(1);
}

void callback(struct libusb_transfer *utp)
{
        int res;


        mtx.lock();
        cout << "Starting Callback" << endl << flush;
        switch (utp->status)
        {
                case LIBUSB_TRANSFER_COMPLETED: case LIBUSB_TRANSFER_TIMED_OUT:
                        *completed=1;
                        break;
                case LIBUSB_TRANSFER_CANCELLED:  case LIBUSB_TRANSFER_NO_DEVICE: case LIBUSB_TRANSFER_ERROR:
                     error("Problem submitting data", res);
                default:

                ofs << utp->buffer;

                res = libusb_submit_transfer(utp);
                if(res != 0)
                        error("Problem submitting data", res);

        }
        cout << "Completed callback" << endl << flush;
        mtx.unlock();


}

int main() {

        int transfer_size;
        int err;
        unsigned char *buffer;
        unsigned char command[1];
        // Device Handle
        libusb_device_handle*   dev;
        struct libusb_transfer  *utp;
        int completed;

        // Initialize libusb with default context

        libusb_init(NULL);

        // Open Device VID = 0x1111, PID = 0x1111 with the default libusb context
        dev = libusb_open_device_with_vid_pid( NULL, 0x28cd, 0x41c0 );

        // If device is null, we didn't find it.
        if (dev == NULL) {
                cout << "Device not found, exiting." << endl;
                return -1;
        }

        // Set configuration 1 on the device
        err = libusb_set_configuration(dev, 1);
        if( err )
                error("Cannot set configuration!", err);



        // Claim interface 0 on the device
        err = libusb_claim_interface(dev, 0);
        if( err )
                error("Cannot claim interface!", err);

        // Set alternate setting 0 on interface 0
        err = libusb_set_interface_alt_setting(dev, 0, 0);
        if( err )
                error("Cannot set alternate setting!", err);

        union int_thing number;


                /*

                libusb_bulk transfer is part of the synchronous API.
                That means it will wait until the transfer is complete to return.

                int libusb_bulk_transfer        (       struct libusb_device_handle *   dev_handle,
                                                                                unsigned char   endpoint,
                                                                                unsigned char *         data,
                                                                                int     length,
                                                                                int *   transferred,
                                                                                unsigned int    timeout
                                                                        )
                */

                command[0]='3';

                err = libusb_bulk_transfer(dev,  0x01, command, 1, &transfer_size, 5);

                if ( err )
                        error( "Bulk OUT Transfer Failed!", err);

                cout << "Data:" << command << endl;
                cout << "Data: Actual Length" << transfer_size << endl;

                  ofs.open ("test.txt", std::ofstream::out | std::ofstream::app);

                libusb_transfer * transfer[3];

                cout << "Gonna transmit" << endl;
                buffer = (unsigned char *) malloc(16777216);

                for (int i=0; i < 3; i++){

                        cout << "Transfer Submitted" << endl << flush;

                        transfer[i] = libusb_alloc_transfer(0);
                        libusb_fill_bulk_transfer(transfer[i], dev, 0x81, buffer, 512, callback, NULL, 5);
                        libusb_submit_transfer(transfer[i]);

                }

                   while(true){

                          cout << "." << flush;

                          err = libusb_handle_events_completed(NULL,&completed);
                           if (err < 0){ // negative values are errors
                                   cout << "Bye bye" << endl;
                                break;
                                }

                          cout << "*" << flush;
                        }

                cout << "Data: Completed" << endl << flush;
                ofs.close();
                libusb_close(dev);
                exit(0);

}
