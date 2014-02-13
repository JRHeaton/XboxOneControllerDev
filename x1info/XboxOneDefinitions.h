//
// Created by Mathieu Amiot on 13/02/2014.
//


#include <IOKit/IOTypes.h>

#ifndef __XboxOneDefinitions_H_
#define __XboxOneDefinitions_H_

// Macro to check flag
#define _cf(val, flag) (val & flag) == flag

typedef union XboxOneSplitBuffer {
    unsigned int whole;
    unsigned char hb;
    unsigned char lb;
} XboxOneSplitBuffer;

// Macro to split buffer
#define _sb(buf) { .whole = buf, .hb = (unsigned char)(buf / 0x10), .lb = (unsigned char)(buf % 0x10) }

typedef enum XboxOneMessageType {
    XBMsgSystem     = 6,
    XBMsgInput      = 18,
    XBMsgUnknown    = 8
} XboxOneMessageType;

// offsets to find
typedef enum XboxOneVibrationRotor {
    XboxOneVibrationRotorLeft,
    XboxOneVibrationRotorRight,
    XboxOneVibrationRotorLeftTrigger,
    XboxOneVibrationRotorRightTrigger
} XboxOneVibrationRotor;

typedef enum XboxOneButton {
    XboxOneButtonA = 0x1,
    XboxOneButtonB = 0x2,
    XboxOneButtonX = 0x4,
    XboxOneButtonY = 0x8
} XboxOneButton;

typedef struct buttons {
    bool A;
    bool B;
    bool X;
    bool Y;
} buttons;

typedef enum XboxOneMenuButton {
    XboxOneMenuButtonSync = 0x1,
    XboxOneMenuButtonStart = 0x4,
    XboxOneMenuButtonSelect = 0x8
} XboxOneMenuButton;

typedef struct menu {
    bool SELECT;
    bool START;
    bool SYNC;
} menu;

typedef enum XboxOneTriggerStickButton {
    XboxOneTriggerStickButtonLB = 0x1,
    XboxOneTriggerStickButtonRB = 0x2,
    XboxOneTriggerStickButtonLS = 0x4,
    XboxOneTriggerStickButtonRS = 0x8
} XboxOneTriggerStickButton;

typedef struct trigBtn {
    bool RB;
    bool LB;
} trigBtn;

typedef struct joyBtn {
    bool RS;
    bool LS;
} joyBtn;

typedef enum XboxOneDirectionalPad {
    XboxOneDirectionalPadUp = 0x1,
    XboxOneDirectionalPadDown = 0x2,
    XboxOneDirectionalPadLeft = 0x4,
    XboxOneDirectionalPadRight = 0x8
} XboxOneDirectionalPad;

typedef struct dPad {
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
} dPad;

typedef struct trigger {
    short unsigned int state;
    int value;
} trigger;

typedef struct stick {
    UInt x;
    UInt y;
    UInt x_dev;
    UInt y_dev;
} stick;



#endif //__XboxOneDefinitions_H_
