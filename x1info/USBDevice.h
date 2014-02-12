//
//  USBDevice.h
//  XboxOneDev
//
//  Created by John Heaton on 2/11/14.
//
//

#ifndef __XboxOneDev__USBDevice__
#define __XboxOneDev__USBDevice__

#include <iostream>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/usb/IOUSBLib.h>
#include <vector>

// General base class for handling IOKit
// USB user client stuff without having to
// rewrite it 20x

/*
 Natural order of things should be:
 
 USBDevice dev(myVID, myPID);
 if(dev.valid()) {
    dev.open();
    dev.setConfiguration(); // optionally provide a diff bConfigurationValue (for xbox controllers, not)
    dev.openInterface(interfaceIndex); // preparing to do stuff
 }
 */

class USBDevice {
public:
    // init for a specific device + manufacturer
    USBDevice(UInt16 idVendor, UInt16 idProduct);
    virtual ~USBDevice(); // if subclassed pls call this
    
    bool valid();   // device handle non-null
    bool open();    // opens device for readwrite
    bool close();   // release access lock on dev
    bool reset();   // resets device (WARNING: you will probably need to reenumerate stuff)
    
    UInt8 numConfigurations();
    IOUSBConfigurationDescriptor *getConfigurationDesc(UInt8 index);
    bool setConfiguration(UInt8 bConfigurationValue=1); // this value is in IOUSBConfigurationDescriptor
    
    bool enumerateInterfaces();         // this is called automatically on setConfiguration()
    UInt8 numInterfaces();              // interfaces in the current configuration
    bool openInterface(UInt8 index);    // open an interface for readwrite
    bool setAltInterface(UInt8 index,
                         UInt8 setting);// sets alternate setting for an interface
    bool closeInterface(UInt8 index);   // release readwrite lock on interface
    UInt8 openAllInterfaces();          // returns how many were opened (0=err)
    UInt8 closeAllInterfaces();         // same as openAllInterfaces
    
    UInt8 numEndpoints(UInt8 intIndex); // get endpoints in an interface
    
    IOUSBInterfaceDescriptor *
    getInterfaceDescriptor(UInt8 index);// gets the descriptor for the given index
    
    IOUSBEndpointDescriptor *
    getEndpointDescriptor(UInt8 interface,
                          UInt8 index);
    
    // Get info about a pipe
    bool getPipeProperties(UInt8 interface,
                           UInt8 endpoint,
                           UInt8 *direction,
                           UInt8 *number,
                           UInt8 *transferType,
                           UInt16 *maxPacketSize,
                           UInt8 *interval);
    
    IOUSBDeviceInterface650 **getDevInterface() { return devIntf; }
    
private:
    io_service_t devService;
    IOUSBDeviceInterface650 **devIntf;
    std::vector<IOUSBInterfaceInterface650 **> interfaces;
};

#endif /* defined(__XboxOneDev__USBDevice__) */
