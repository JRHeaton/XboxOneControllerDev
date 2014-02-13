//
//  XboxOneController.cpp
//  XboxOneDev
//
//  Created by John Heaton on 2/12/14.
//
//

#include "XboxOneController.h"
#include "XboxOneDefinitions.h"

#define X1_INTERRUPT_INTERFACE 0
#define X1_INTERRUPT_OUT 0

void XboxOneController::init()
{
    this->openAllInterfaces();
    this->setAltInterface(0, 1);
    this->setAltInterface(1, 1);
    this->setAltInterface(2, 1);

    // I have no idea what I'm doing
    UInt8 ipl[2][2];
    ipl[0][0] = 0x25;
    ipl[0][1] = 0x39;
    ipl[1][0] = 0xc5;
    ipl[1][1] = 0xe0;

    this->write(0, 0, ipl[0], 2);
    this->write(0, 0, ipl[1], 2);
    this->write(0, 0, ipl[0], 2);
    this->write(0, 0, ipl[1], 2);
}

void XboxOneController::shutdown()
{
    UInt8 ipl[2][2];
    ipl[0][0] = 0xc5;
    ipl[0][1] = 0xe0;
    ipl[1][0] = 0x25;
    ipl[1][1] = 0x39;

    this->write(0, 0, ipl[0], 2);
    this->write(0, 0, ipl[1], 2);
    this->write(0, 0, ipl[0], 2);
    this->write(0, 0, ipl[1], 2);
}


bool XboxOneController::ledOn() {
    UInt8 cmd[2] = { 0xe4, 0x60 };
    return this->write(X1_INTERRUPT_INTERFACE, 0, cmd, 2);
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
    XboxOneSplitBuffer btnv = _sb(bbuf[4]);
    buttons bState = {
        .A = _cf(btnv.hb, XboxOneButtonA),
        .B = _cf(btnv.hb, XboxOneButtonB),
        .X = _cf(btnv.hb, XboxOneButtonX),
        .Y = _cf(btnv.hb, XboxOneButtonY)
    };
    printf("\t_BTN (%x) X=%d, Y=%d, B=%d, A=%d\n", btnv.hb, bState.X, bState.Y, bState.B, bState.A);

    menu mState = {
        .SELECT = _cf(btnv.lb, XboxOneMenuButtonSelect),
        .START  = _cf(btnv.lb, XboxOneMenuButtonStart),
        .SYNC   = _cf(btnv.lb, XboxOneMenuButtonSync)
    };
    printf("\t_MENU (%x) SELECT=%d, START=%d, SYNC=%d\n", btnv.lb, mState.SELECT, mState.START, mState.SYNC);

    XboxOneSplitBuffer ctlv = _sb(bbuf[5]);
    trigBtn tState = {
        .LB = _cf(ctlv.hb, XboxOneTriggerStickButtonLB),
        .RB = _cf(ctlv.hb, XboxOneTriggerStickButtonRB)
    };

    printf("\t_TRIG (%x) LB=%d, RB=%d\n", ctlv.hb, tState.LB, tState.RB);

    joyBtn jState = {
        .LS = _cf(ctlv.hb, XboxOneTriggerStickButtonLS),
        .RS = _cf(ctlv.hb, XboxOneTriggerStickButtonRS),
    };

    printf("\t_JOY (%x) LS=%d, RS=%d\n", ctlv.hb, jState.LS, jState.RS);

    dPad pState = {
        .UP = _cf(ctlv.lb, XboxOneDirectionalPadUp),
        .DOWN = _cf(ctlv.lb, XboxOneDirectionalPadDown),
        .LEFT = _cf(ctlv.lb, XboxOneDirectionalPadLeft),
        .RIGHT = _cf(ctlv.lb, XboxOneDirectionalPadRight)
    };

    printf("\t_DPAD (%x) UP=%d, DOWN=%d, LEFT=%d, RIGHT=%d\n", ctlv.lb, pState.UP, pState.DOWN, pState.LEFT, pState.RIGHT);

    trigger lt = {
        .value = bbuf[6] + 0xff * bbuf[7],
        .state = bbuf[7]
    };

    printf("\t_LT state=%d, raw=%d\n", lt.state, lt.value);

    trigger rt = {
        .value = bbuf[8] + 0xff * bbuf[9],
        .state = bbuf[9]
    };

    printf("\t_RT state=%d, raw=%d\n", rt.state, rt.value);

    stick ls = {
        .x = bbuf[10],
        .x_dev = bbuf[11],
        .y = bbuf[12],
        .y_dev = bbuf[13]
    };

    printf("\t_LS x=%d, y=%d, x_dev=%d, y_dev=%d\n", ls.x, ls.y, ls.x_dev, ls.y_dev);

    stick rs = {
        .x = bbuf[14],
        .x_dev = bbuf[15],
        .y = bbuf[16],
        .y_dev = bbuf[17]
    };

    printf("\t_RS x=%d, y=%d, x_dev=%d, y_dev=%d\n", rs.x, rs.y, rs.x_dev, rs.y_dev);
}

bool XboxOneController::vibrate(UInt intensity, XboxOneVibrationRotor rotor)
{
    UInt8 party = rotor;
    return this->write(X1_INTERRUPT_INTERFACE, 0, &party, sizeof(party));
}
