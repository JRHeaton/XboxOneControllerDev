//
//  main.c
//  x1info
//
//  Created by John Heaton on 2/11/14.
//
//

#include <CoreFoundation/CoreFoundation.h>
#include "USBDevice.h"
#include "XboxOneController.h"
#include <iostream>

using std::cout; using std::endl;

// these are for the 360 controller i have. i'm using it for reference testing
#define AFTERGLOW_VENDOR    0x0e6f
#define AFTERGLOW_PRODUCT   0x0213

#define TESTING_X1          1       // easy testing for me
#define RESET_AND_QUIT      0       // for ease during testing

int main(int argc, const char * argv[]) {
    XboxOneController d
#if TESTING_X1 == 0
    (AFTERGLOW_VENDOR, AFTERGLOW_PRODUCT);
#else
    (X1_VENDOR, X1_PID);
#endif
    
    if(RESET_AND_QUIT) {
        d.open();
        d.reset();
        return 0;
    }
    
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
        UInt8 inter, num;
        static const char *dir_names[4] = {
            "out",
            "in",
            "none",
            "any"
        },
        *type_names[5] = {
            "control",
            "isochronous",
            "bulk",
            "interrupt",
            "any"
        };
        for(int pipe=0;pipe<d.numEndpoints(i);++pipe) {
            d.getPipeProperties(i, pipe, &dir, &num, &type, &mpkt, &inter);
            printf("dir(interface %d, pipe %d): [dir=%s, type=%s, mpktsize=0x%x, poll=%ums]\n", i, pipe, dir_names[dir], type_names[type], mpkt, inter);
        }
    }
    
    printf("%d\n", d.ledOn());
    
    return 0;
}

