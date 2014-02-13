XboxOneDev
==========
Collection of tests, ideas, and whatever code we want relating to Xbox One peripherals.


## USB Information
The Xbox One controller has a single USB configuration (1), and two altinterface settings for each of the three (3) interfaces that that configuration supplies.

idVendor(MSFT): ```0x045E```
idProduct: ```0x02D1```

#### Device Descriptor
```
    Descriptor Version Number:   0x0200
    Device Class:   255   (Vendor-specific)
    Device Subclass:   71   (Vendor-specific)
    Device Protocol:   208
    Device MaxPacketSize:   64
    Device VendorID/ProductID:   0x045E/0x02D1   (Microsoft Corporation)
    Device Version Number:   0x0101
    Number of Configurations:   1
    Manufacturer String:   1 "Microsoft"
    Product String:   2 "Controller"
    Serial Number String:   3 "7EED8304C69E"
```

#### Interface Descriptors
```
Interface #1 - Vendor-specific (#1)
    Alternate Setting   1
    Number of Endpoints   2
    Interface Class:   255   (Vendor-specific)
    Interface Subclass;   71   (Vendor-specific)
    Interface Protocol:   208
    Endpoint 0x02 - Isochronous Output
        Address:   0x02  (OUT)
        Attributes:   0x01  (Isochronous no synchronization data endpoint)
        Max Packet Size:   228
        Polling Interval:   1 ms
    Endpoint 0x82 - Isochronous Input
        Address:   0x82  (IN)
        Attributes:   0x01  (Isochronous no synchronization data endpoint)
        Max Packet Size:   228
        Polling Interval:   1 ms



Interface #1 - Vendor-specific (#1)
    Alternate Setting   1
    Number of Endpoints   2
    Interface Class:   255   (Vendor-specific)
    Interface Subclass;   71   (Vendor-specific)
    Interface Protocol:   208
    Endpoint 0x02 - Isochronous Output
        Address:   0x02  (OUT)
        Attributes:   0x01  (Isochronous no synchronization data endpoint)
        Max Packet Size:   228
        Polling Interval:   1 ms
    Endpoint 0x82 - Isochronous Input
        Address:   0x82  (IN)
        Attributes:   0x01  (Isochronous no synchronization data endpoint)
        Max Packet Size:   228
        Polling Interval:   1 ms


Interface #2 - Vendor-specific (#1)
    Alternate Setting   1
    Number of Endpoints   2
    Interface Class:   255   (Vendor-specific)
    Interface Subclass;   71   (Vendor-specific)
    Interface Protocol:   208
    Endpoint 0x03 - Bulk Output
        Address:   0x03  (OUT)
        Attributes:   0x02  (Bulk no synchronization data endpoint)
        Max Packet Size:   64
        Polling Interval:   0 ms
    Endpoint 0x83 - Bulk Input
        Address:   0x83  (IN)
        Attributes:   0x02  (Bulk no synchronization data endpoint)
        Max Packet Size:   64
        Polling Interval:   0 ms
```

As you can see, I've only listed descriptors from altinterface 1, because in altinterface 0, the first interface is identical, and the rest have 0 endpoints (minimalistic config maybe?)
