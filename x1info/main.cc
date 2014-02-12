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

int main(int argc, const char * argv[]) {
    USBDevice d(AFTERGLOW_VENDOR, AFTERGLOW_PRODUCT);
    d.open();
    d.setConfiguration();
    
    return 0;
}

