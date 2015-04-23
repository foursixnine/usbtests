#include <iostream>
#include <stdlib.h>
#include <string>
#include <limits>
#include <fstream>
#include "libusb-1.0/libusb.h"
#include <thread>
#include <chrono>


#undef max
#define MAX_TRANSFERS 256
#define MAX_PACKETS_PER_TRANSFER 8

using namespace std;



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
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    };

int main(){

    cout << MAX_TRANSFERS << endl << flush;
    return 1;

}
