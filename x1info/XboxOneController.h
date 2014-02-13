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

typedef enum XboxOneMessageType {
    XBMsgSystem     = 6,
    XBMsgInput      = 18,
    XBMsgUnknown    = 8
} XboxOneMessageType;

typedef struct {
    bool A;
    bool B;
    bool X;
    bool Y;
} buttons;

typedef struct {
    bool SELECT;
    bool START;
    bool SYNC;
} menu;

typedef struct {
    bool RB;
    bool LB;
} trigBtn;

typedef struct {
    bool RS;
    bool LS;
} joyBtn;

typedef struct {
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
} dPad;

class XboxOneController : public USBDevice {
public:
    XboxOneController(UInt16 idVendor=X1_VENDOR, UInt16 idProduct=X1_PID) : USBDevice(idVendor, idProduct) {}

    bool ledOn();

    void parseInputBuffer(UInt8 *bbuf, UInt64 len);

    void handleInput(UInt8 *bbuf, UInt64 len);
};

#endif /* defined(__XboxOneDev__XboxOneController__) */
