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
#include "XboxOneDefinitions.h"

#define X1_VENDOR           0x045E  // MSFT
#define X1_PID              0x02D1

class XboxOneController : public USBDevice {
private:
    void handleInput(UInt8 *bbuf, UInt64 len);

public:
    XboxOneController(UInt16 idVendor=X1_VENDOR, UInt16 idProduct=X1_PID) : USBDevice(idVendor, idProduct) {}

    bool ledOn();

    void parseInputBuffer(UInt8 *bbuf, UInt64 len);

    bool vibrate(UInt intensity, XboxOneVibrationRotor rotor);

    void init();
};

#endif /* defined(__XboxOneDev__XboxOneController__) */
