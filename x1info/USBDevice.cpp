//
//  USBDevice.cpp
//  XboxOneDev
//
//  Created by John Heaton on 2/11/14.
//
//

#include "USBDevice.h"
#include <IOKit/IOCFPlugIn.h>

#define _err(args...) fprintf(stderr, args)
#define _ifnot(val, exp) if((exp) != (val))
// faster dereferencing for these crazy double pointers
#define _d(ptr) (*ptr)

USBDevice::USBDevice(UInt16 idVendor, UInt16 idProduct) {
    if(!idProduct || !idVendor) {
        _err("idProduct and idVendor must both be non-zero\n");
    }
    
    CFMutableDictionaryRef matching;
    CFNumberRef _idVendor, _idProduct;
    IOCFPlugInInterface **plugin;
    SInt32 score;
    
    // create a dictionary of properties to match in the registry
    matching = IOServiceMatching(kIOUSBDeviceClassName);
    // set idProduct and idVendor (see System Information under USB or look in IORegistryExplorer)
    _idProduct = CFNumberCreate(nullptr, kCFNumberSInt16Type, &idProduct);
    _idVendor = CFNumberCreate(nullptr, kCFNumberSInt16Type, &idVendor);
    CFDictionarySetValue(matching, CFSTR(kUSBVendorID), _idVendor);
    CFDictionarySetValue(matching, CFSTR(kUSBProductID), _idProduct);
    CFRelease(_idVendor);
    CFRelease(_idProduct);
    // !!!!: current limitation = this will only get the first matching device
    // but i'm assuming you only have on plugged in (for now, easy fix later)
    devService = IOServiceGetMatchingService(kIOMasterPortDefault, matching);
    
    // Open CFPlugIn interface to the usb device user client in the kernel
    _ifnot(KERN_SUCCESS, IOCreatePlugInInterfaceForService(devService, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugin, &score)) {
        _err("failed to open device plugin interface\n");
        IOObjectRelease(devService);
        return;
    }
    // get the device interface
    _ifnot(KERN_SUCCESS, _d(plugin)->QueryInterface(plugin, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID650), (LPVOID *)&devIntf)) {
        _err("failed to get device interface\n");
        IOObjectRelease(devService);
        _d(plugin)->Release(plugin);
        return;
    }
    _d(plugin)->Release(plugin);
}

USBDevice::~USBDevice() {
    IOObjectRelease(devService);
    closeAllInterfaces();
    close();
}

bool USBDevice::valid() {
    return devService && devIntf;
}

bool USBDevice::open() {
    return valid() && !_d(devIntf)->USBDeviceOpen(devIntf);
}

bool USBDevice::close() {
    return !_d(devIntf)->USBDeviceClose(devIntf);
}

bool USBDevice::reset() {
    return valid() && !_d(devIntf)->ResetDevice(devIntf);
}

bool USBDevice::enumerateInterfaces() {
    if(!valid()) return 0;
    
    IOCFPlugInInterface **plugin;
    IOUSBFindInterfaceRequest intfReq;
    io_iterator_t it;
    io_service_t intf_service;
    SInt32 score;
    
    // close any open interfaces
    closeAllInterfaces();
    // remove all old interfaces from array
    interfaces.clear();

    // interate interfaces and get references
    intfReq.bAlternateSetting
    = intfReq.bInterfaceClass
    = intfReq.bInterfaceProtocol
    = intfReq.bInterfaceSubClass
    = kIOUSBFindInterfaceDontCare;
    _ifnot(KERN_SUCCESS, _d(devIntf)->CreateInterfaceIterator(devIntf, &intfReq, &it)) {
        _err("failed to create enumerator for interfaces\n");
        return 0;
    }
    while((intf_service = IOIteratorNext(it))) {
        IOUSBInterfaceInterface650 **intf;
        _ifnot(KERN_SUCCESS, IOCreatePlugInInterfaceForService(intf_service, kIOUSBInterfaceUserClientTypeID, kIOCFPlugInInterfaceID, &plugin, &score)) {
            _err("failed to open interface plugin interface\n");
            IOObjectRelease(devService);
            IOObjectRelease(intf_service);
            IOObjectRelease(it);
            _d(devIntf)->Release(devIntf);
            return 0;
        }
        _ifnot(KERN_SUCCESS, _d(plugin)->QueryInterface(plugin, CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID650), (LPVOID *)&intf)) {
            _err("failed to get interface interface\n");
            IOObjectRelease(devService);
            IOObjectRelease(it);
            IOObjectRelease(intf_service);
            _d(devIntf)->Release(devIntf);
            return 0;
        }
        _d(plugin)->Release(plugin);
        interfaces.push_back(intf);
    }
    
    return 1;
}

UInt8 USBDevice::numConfigurations() {
    UInt8 ret=0;
    if(!valid()) return 0;
    _d(devIntf)->GetNumberOfConfigurations(devIntf, &ret);
    return ret;
}

bool USBDevice::setConfiguration(UInt8 val) {
    return valid()
    && !_d(devIntf)->SetConfiguration(devIntf, val)
    && enumerateInterfaces();
}

UInt8 USBDevice::numInterfaces() {
    return interfaces.size();
}

bool USBDevice::openInterface(UInt8 index) {
    return valid()
    && index < interfaces.size()
    && !_d(interfaces[index])->USBInterfaceOpen(interfaces[index]);
}

bool USBDevice::closeInterface(UInt8 index) {
    return valid()
    && index < interfaces.size()
    && !_d(interfaces[index])->USBInterfaceClose(interfaces[index]);
}

UInt8 USBDevice::openAllInterfaces() {
    UInt8 ret=0;
    for(UInt8 i=0;i<numInterfaces();++i) {
        ret += (UInt8)openInterface(i);
    }
    return ret;
}

UInt8 USBDevice::closeAllInterfaces() {
    UInt8 ret=0;
    for(UInt8 i=0;i<numInterfaces();++i) {
        ret += (UInt8)closeInterface(i);
    }
    return ret;
}

bool USBDevice::setAltInterface(UInt8 index, UInt8 setting) {    
    return valid() &&
    index < interfaces.size() &&
    !_d(interfaces[index])->SetAlternateInterface(interfaces[index], setting);
}

IOUSBConfigurationDescriptor *USBDevice::getConfigurationDesc(UInt8 index) {
    if(!valid()) return nullptr;
    IOUSBConfigurationDescriptor *desc = new IOUSBConfigurationDescriptor;
    UInt8 ret = _d(devIntf)->GetConfigurationDescriptorPtr(devIntf, index, &desc);
    _ifnot(KERN_SUCCESS, ret) {
        delete desc;
    }
    return desc;
}

IOUSBInterfaceDescriptor *USBDevice::getInterfaceDescriptor(UInt8 index) {
    if(!valid() || index >= interfaces.size()) return nullptr;
    IOUSBInterfaceDescriptor *desc;
    desc = (typeof desc)_d(interfaces[index])->FindNextAssociatedDescriptor(interfaces[index], nullptr, kUSBInterfaceDesc);
    return desc;
}

bool USBDevice::write(UInt8 intfIndex, UInt8 pipeIndex, UInt8 *data, size_t len) {
    return valid()
    && intfIndex < interfaces.size()
    && !_d(interfaces[intfIndex])->WritePipe(interfaces[intfIndex], pipeIndex+1, data, (UInt32)len);
}

UInt8 USBDevice::numEndpoints(UInt8 intIndex) {
    if(!valid() || intIndex >= interfaces.size()) return 0;
    UInt8 ret = 0;
    _ifnot(KERN_SUCCESS, _d(interfaces[intIndex])->GetNumEndpoints(interfaces[intIndex], &ret)) {
        _err("failed to get endpoints\n");
    }
    return ret;
}

bool USBDevice::getPipeProperties(UInt8 interface,
                                  UInt8 endpoint,
                                  UInt8 *direction,
                                  UInt8 *number,
                                  UInt8 *transferType,
                                  UInt16 *maxPacketSize,
                                  UInt8 *interval) {
    return valid()
    && interface <= interfaces.size()
    && !_d(interfaces[interface])->GetPipeProperties(interfaces[interface],
                                                     endpoint+1, // exclude control pipe
                                                     direction,
                                                     number,
                                                     transferType,
                                                     maxPacketSize,
                                                     interval);
}