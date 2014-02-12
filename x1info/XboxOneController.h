//
//  XboxOneController.h
//  XboxOneDev
//
//  Created by John Heaton on 2/12/14.
//
//

#ifndef __XboxOneDev__XboxOneController__
#define __XboxOneDev__XboxOneController__

#include "USBDevice.h"

#define X1_VENDOR           0x045E  // MSFT
#define X1_PID              0x02D1

class XboxOneController : public USBDevice {
public:
    XboxOneController(UInt16 idVendor=X1_VENDOR, UInt16 idProduct=X1_PID) : USBDevice(idVendor, idProduct) {}
    
    bool ledOn();
};

#endif /* defined(__XboxOneDev__XboxOneController__) */
