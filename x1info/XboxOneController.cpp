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

void XboxOneController::parseInputBuffer(UInt8 *bbuf, UInt64 len)
{
    XboxOneMessageType type = (XboxOneMessageType) len;
    switch (type)
    {
        case XBMsgInput:
            printf("INPUT ");
            break;
        case XBMsgSystem:
            printf("SYSTEM ");
            break;
        case XBMsgUnknown:
        default:
            printf("UNKNOWN ");
            break;
    }

    for (UInt64 i = 0; i < len; ++i)
    {
        printf("%02x ", bbuf[i]);
    }

    if (type == XBMsgInput)
    {
        printf("\n");
        this->handleInput(bbuf, len);
    }

    printf("\n");
}

void XboxOneController::handleInput(UInt8 *bbuf, UInt64 len)
{
    // Ignore 3 first bytes, they seem useless
    // Or they identify the gamepad, unsure about that, need a second pad.
    int buttonVal = bbuf[4];
    int lbV = buttonVal / 0x10;
    int rbV = buttonVal % 0x10;
    buttons bState = {
        .A = (lbV & 0x1) == 0x1,
        .B = (lbV & 0x2) == 0x2,
        .X = (lbV & 0x4) == 0x4,
        .Y = (lbV & 0x8) == 0x8
    };
    printf("\t_BTN (%x) X=%d, Y=%d, B=%d, A=%d\n", lbV, bState.X, bState.Y, bState.B, bState.A);

    menu mState = {
        .SELECT = (rbV & 0x8) == 0x8,
        .START = (rbV & 0x4) == 0x4,
        .SYNC = (rbV & 0x1) == 0x1
    };
    printf("\t_MENU (%x) SELECT=%d, START=%d, SYNC=%d\n", rbV, mState.SELECT, mState.START, mState.SYNC);

    int controlVal = bbuf[5];

    int lctlV = controlVal / 0x10;
    int rctlV = controlVal % 0x10;
    trigBtn tState = {
        .LB = (lctlV & 0x1) == 0x1,
        .RB = (lctlV & 0x2) == 0x2
    };

    printf("\t_TRIG (%x) LB=%d, RB=%d\n", lctlV, tState.LB, tState.RB);

    joyBtn jState = {
        .LS = (lctlV & 0x4) == 0x4,
        .RS = (lctlV & 0x8) == 0x8,
    };

    printf("\t_JOY (%x) LS=%d, RS=%d\n", lctlV, jState.LS, jState.RS);

    dPad pState = {
        .UP = (rctlV & 0x1) == 0x1,
        .DOWN = (rctlV & 0x2) == 0x2,
        .LEFT = (rctlV & 0x4) == 0x4,
        .RIGHT = (rctlV & 0x8) == 0x8
    };

    printf("\t_DPAD (%x) UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d\n", rctlV, pState.UP, pState.DOWN, pState.LEFT, pState.RIGHT);
}
