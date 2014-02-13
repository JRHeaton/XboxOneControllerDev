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

UInt8 *bbuf;

int readsize;
void inputcb(void *refcon, IOReturn result, void *len) {
    printf("INPUT ");
    for(int i=0;i<((UInt64)len);++i) {
        printf("%02x ", bbuf[i]);
    }
    puts("");
    
    XboxOneController *d = (XboxOneController *)refcon;
    (* d->getInterface(0))->ReadPipeAsync(d->getInterface(0), 2, bbuf, readsize, inputcb, d);
}

mach_port_t inport;

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
    
    bbuf = (UInt8 *)malloc(64);
    
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
        
        (* d.getInterface(0))->CreateInterfaceAsyncPort(d.getInterface(0), &inport);
        CFRunLoopSourceRef src;
        (* d.getInterface(0))->CreateInterfaceAsyncEventSource(d.getInterface(0), &src);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), src, kCFRunLoopCommonModes);
        
        if(!i) {

            readsize = 64;
            (* d.getInterface(0))->ReadPipeAsync(d.getInterface(0), 2, bbuf, readsize, inputcb, &d);
        }
        
        for(int pipe=0;pipe<d.numEndpoints(i);++pipe) {
            d.getPipeProperties(i, pipe, &dir, &num, &type, &mpkt, &inter);
            printf("dir(interface %d, pipe %d): [dir=%s, type=%s, mpktsize=0x%x, poll=%ums]\n", i, pipe, dir_names[dir], type_names[type], mpkt, inter);
            
            if(dir & kUSBIn && i == 0) {
                
                // ------------------------------
                // This code enables the controller to report back info over the interrupt pipe
                // I have no fucking clue why
                // - you must send these messages twice ?
                // - they seem to sometimes turn on the LED also...
                // - but then what does the message in ledOn() (XboxOneController) do???
                // - I'm thinking these are binary proprietary messages, used to turn on or off
                // features of the controller, possibly to save bus bandwidth or power?
                // we really need a good idea and better way to investigate what means what
                
                // good start tho
                UInt8 c[2];
                c[0] = 0x25;
                c[1] = 0x39;
                
                d.write(0, 0, c, 2);
                
                c[0] = 0xc5;
                c[1] = 0xe0;
                d.write(0, 0, c, 2);
                
                c[0] = 0x25;
                c[1] = 0x39;
                
                d.write(0, 0, c, 2);
                
                c[0] = 0xc5;
                c[1] = 0xe0;
                d.write(0, 0, c, 2);
                // =======================================
            }
        }
    }
    
//    printf("%d\n", d.ledOn());

    (* d.getInterface(0))->ReadPipeAsync(d.getInterface(0), 2, bbuf, readsize, inputcb, &d);
    CFRunLoopRun();
    
    return 0;
}

