//
//  XboxOneController.cpp
//  XboxOneDev
//
//  Created by John Heaton on 2/12/14.
//
//

#include "XboxOneController.h"

#define X1_INTERRUPT_INTERFACE 0
#define X1_INTERRUPT_OUT 0

bool XboxOneController::ledOn() {
    UInt8 cmd[2] = { 0xe4, 0x60 };
    return write(X1_INTERRUPT_INTERFACE, 0, cmd, 2);
}