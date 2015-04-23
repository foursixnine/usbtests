#include <iostream>
#include <limits>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <fstream>      // std::ofstream
#include "libusb-1.0/libusb.h"
#include <thread>
#include <chrono>

/** Need to look a little bit more closer to:
 *
 * https://github.com/clarkli86/hidapi/blob/master/libusb/hid.c
 *
 * */

#undef max
#define MAX_TRANSFERS MAX_URB
#define MAX_PACKETS_PER_TRANSFER 8

using namespace std;

union int_thing {
        unsigned char usb_data[4];
        int32_t number;
};

unsigned char *currentCommand;
int totalLength = 0;
const unsigned char commandList[] = {

    0x00,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0A,
    0x0B,
    0x0C,
    0x0D,
    0x0E,
    0x0F,
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1A,
    0x1B
};

        int dataBufferSize = 1024*1024*32;
        unsigned char *dataBuffer = (unsigned char *) malloc(dataBufferSize);
        int bufferSize = MAX_PACKETS_PER_TRANSFER * 512;

        std::ofstream ofs;
        int completed;
        //~ auto start_time = std::chrono::high_resolution_clock::now();
        //~ auto end_time = std::chrono::high_resolution_clock::now();


void error(string s, int err) {
        cout << s << " Error: " << libusb_error_name(err)  << endl << flush;
        //exit(1);
}


void callback(struct libusb_transfer *utp)
{
        int res;
        int *p;        p = (int *) utp->user_data;

        switch (utp->status)
        {
                case LIBUSB_TRANSFER_COMPLETED:
                        if (utp->actual_length == 0){

                        completed = 1;

                        return;

                        } else {
                            memcpy(dataBuffer+totalLength, utp->buffer, utp->actual_length);
                            totalLength +=utp->actual_length;
                        }
                        break;
                case LIBUSB_TRANSFER_TIMED_OUT:
                        if (utp->actual_length == 0){

                        completed = 1;
                        return;

                        } else {

                            memcpy(dataBuffer+totalLength, utp->buffer, utp->actual_length);
                            totalLength +=utp->actual_length;
                        }
                        break;
                case LIBUSB_TRANSFER_CANCELLED:  case LIBUSB_TRANSFER_NO_DEVICE: case LIBUSB_TRANSFER_ERROR:
                      cout << __LINE__ << endl << flush;
                      error("Problem submitting data", res);
                default:
                        cout << "Resubmitting urb" << endl << flush;
                        goto resend_urb;
        }

        //~ cout << "Completed callback" << endl << flush;

resend_urb:
res = libusb_submit_transfer(utp);
//~ cout << " Error: " << libusb_error_name(utp->status)  << endl << flush;
if(res != 0)
        error("Problem submitting data", res);

}



void waitForEvents( int command ){

         int err;
         double transferRate;

           completed=0;

           cerr << "Event handling started" << endl;
            auto start_time = std::chrono::high_resolution_clock::now();

           do {
                  err = libusb_handle_events(NULL);
                   if (err < 0){ // negative values are errors
                           cout << "Bye bye" << endl << flush;
                        break;
                        }
                } while(completed != 1 );

            auto end_time = std::chrono::high_resolution_clock::now();

            cerr << "Event Handling finished" << endl;
            completed=0;

            transferRate = totalLength/chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            cout << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << ";" ;

        }


int main() {

        int transfer_size;
        int err;
        unsigned char *buffer;
        unsigned char command[1];
        union int_thing number;
        std::thread eventHandler;
        // Device Handle
        libusb_device_handle*   dev;
        struct libusb_transfer  *utp;


            std::chrono::milliseconds timespan(1000); // or whatever
            cout << "command;Total Transfers;transaction time;event handling time;full time;data size" << endl << flush;
            for (int cmdIdx = 0; cmdIdx < sizeof(commandList); cmdIdx++)
            {

        auto timeBeforeExecution = std::chrono::high_resolution_clock::now();

        // Initialize libusb with default context

        libusb_init(NULL);

        // Open Device VID = 0x1111, PID = 0x1111 with the default libusb context
        dev = libusb_open_device_with_vid_pid( NULL, 0x28cd, 0x41c2 );

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



            command[0]=0x65;


//~
                //~ std::this_thread::sleep_for(timespan);
//~
                //~ currentCommand= (unsigned char *) 0x65;
                //~ command[0]=0x65;
                //~ err = libusb_bulk_transfer(dev,  0x01, command, 1, &transfer_size, 100);
//~
                //~ if ( err )
                        //~ error( "Fallo 0x65", err);
//~
//~
                //~ currentCommand= (unsigned char *) 0x63;
                //~ command[0]=0x63;
                //~ err = libusb_bulk_transfer(dev,  0x01, command, 1, &transfer_size, 100);
//~
                //~ if ( err )
                        //~ error( "Fallo 0x63!", err);
//~
                //~ std::this_thread::sleep_for(timespan);

                cerr << "Starting the true execution" << endl << flush;;

                auto timeBeforeBuffer = std::chrono::high_resolution_clock::now();


                dataBuffer = (unsigned char *) malloc(dataBufferSize);

                auto timeAfterBuffer = std::chrono::high_resolution_clock::now();

                cerr << "Buffer Àllocation:" << chrono::duration_cast<chrono::milliseconds>(timeAfterBuffer - timeBeforeBuffer).count() << endl << flush;

                completed = 0;
                totalLength = 0;


                currentCommand = (unsigned char *) &commandList[cmdIdx];
                cout << (int) commandList[cmdIdx] << ";";
                cout << MAX_TRANSFERS << ";";
                auto timeBeforeTransaction = std::chrono::high_resolution_clock::now();

                err = libusb_bulk_transfer(dev,  0x01, currentCommand, 1, &transfer_size, 100);

                if ( err )
                        error( "Fallo este comando", err);

                //~ cerr << "Command is: " << showbase << oct << currentCommand  << flush;;
                //~ cerr << noshowbase << "Data: Write size" << transfer_size << "." << endl << flush;;

                libusb_transfer *transfer[MAX_TRANSFERS];

                for (int i=0; i < MAX_TRANSFERS; i++){
                        transfer[i] = libusb_alloc_transfer(0);
                        libusb_fill_bulk_transfer(transfer[i], dev, 0x81, (unsigned char *) malloc(bufferSize), bufferSize, callback, &completed, 50);
                        libusb_submit_transfer(transfer[i]);
                }

                eventHandler = thread(waitForEvents, commandList[cmdIdx]);
                eventHandler.join();

                auto timeAfterTransaction = std::chrono::high_resolution_clock::now();

                cerr << "Finished execution sleeping" << endl << flush;
//~
                //~ std::this_thread::sleep_for(timespan);
                //~ currentCommand= (unsigned char *) 0x65;
                //~ command[0]=0x65;
                //~ err = libusb_bulk_transfer(dev,  0x01, command, 1, &transfer_size, 100);
//~
                //~ if ( err )
                        //~ error( "Fallo 0x65", err);
//~
//~
                //~ currentCommand= (unsigned char *) 0x63;
                //~ command[0]=0x63;
                //~ err = libusb_bulk_transfer(dev,  0x01, command, 1, &transfer_size, 100);
//~
                //~ if ( err )
                        //~ error( "Fallo 0x63!", err);

                cerr << "Woke up" << endl ;

                libusb_close(dev);

                auto timeAfterExecution = std::chrono::high_resolution_clock::now();
                     cout
                     << chrono::duration_cast<chrono::milliseconds>(timeAfterExecution - timeBeforeExecution).count() << ";"
                     << chrono::duration_cast<chrono::milliseconds>(timeAfterTransaction - timeBeforeTransaction).count() << ";"
                     << totalLength << ";"  << endl << flush;
                }

                free(dataBuffer);
                exit(0);

}

