//
//  main.c
//  x1info
//
//  Created by John Heaton on 2/11/14.
//
//

#include <CoreFoundation/CoreFoundation.h>
#include "USBDevice.h"
#include <iostream>

using std::cout; using std::endl;

// these are for the 360 controller i have. i'm using it for reference testing
#define AFTERGLOW_VENDOR    0x0e6f
#define AFTERGLOW_PRODUCT   0x0213

#define X1_VENDOR           0x045E  // MSFT
#define X1_PID              0x02D1

#define TESTING_X1          1       // easy testing for me

int main(int argc, const char * argv[]) {
    USBDevice d
#if TESTING_X1 == 0
    (AFTERGLOW_VENDOR, AFTERGLOW_PRODUCT);
#else
    (X1_VENDOR, X1_PID);
#endif
    cout <<
    "open: " << d.open() << endl <<
    "setConfiguration: " << d.setConfiguration() << endl <<
    "numInterfaces: " << (int)d.numInterfaces() << endl;
    
    for(int i=0;i<d.numInterfaces();++i) {
        d.openInterface(i);
        d.setAltInterface(i, 1);
        printf("Enumerating %d pipes\n", d.numEndpoints(i));
        
        UInt8 dir, type;
        UInt16 mpkt;
        UInt8 dummy, num;
        for(int pipe=0;pipe<d.numEndpoints(i);++pipe) {
            d.getPipeProperties(i, pipe, &dir, &num, &type, &mpkt, &dummy);
            printf("dir(interface %d, pipe %d): [dir=0x%x type=0x%x, num=0x%x]\n", i, pipe, dir, type, num);
        }
    }
    
    return 0;
}

