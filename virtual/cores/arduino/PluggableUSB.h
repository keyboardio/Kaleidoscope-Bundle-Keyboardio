/* -*- mode: c++ -*-
 * Kaleidoscope
 * Copyright (C) 2019 noseglasses (shinynoseglasses@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>

// This file contains only stub implementation.
// The content is in great parts copied from
//   arduino/avr/cores/arduino/PluggableUSB.*

struct InterfaceDescriptor {
  template<typename..._Dummy>
  InterfaceDescriptor(_Dummy...) {}
};
struct EndpointDescriptor {
  template<typename..._Dummy>
  EndpointDescriptor(_Dummy...) {}
};

struct USBSetup;

class PluggableUSBModule {
  public:
    PluggableUSBModule(uint8_t numEps, uint8_t numIfs, uint8_t *epType) :
      numEndpoints(numEps), numInterfaces(numIfs), endpointType(epType)
    { }

  protected:
    virtual bool setup(USBSetup &setup) = 0;
    virtual int getInterface(uint8_t *interfaceCount) = 0;
    virtual int getDescriptor(USBSetup &setup) = 0;
    virtual uint8_t getShortName(char *name) {
      name[0] = 'A'+pluggedInterface;
      return 1;
    }

    uint8_t pluggedInterface = 0;
    uint8_t pluggedEndpoint;

    const uint8_t numEndpoints;
    const uint8_t numInterfaces;
    const uint8_t *endpointType;

    PluggableUSBModule *next = nullptr;

    friend class PluggableUSB_;
};

class PluggableUSB_ {
  public:
    PluggableUSB_() {}
    bool plug(PluggableUSBModule *node) {
      return true;
    }
    int getInterface(uint8_t *interfaceCount) {
      return 1;
    }
    int getDescriptor(USBSetup &setup) {
      return 1;
    }
    bool setup(USBSetup &setup) {
      return true;
    }
    void getShortName(char *iSerialNum) {}
};

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
PluggableUSB_ &PluggableUSB();

#define EP_TYPE_INTERRUPT_IN 1
#define USB_DEVICE_CLASS_HUMAN_INTERFACE 1
#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
  { 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }
#define USB_ENDPOINT_IN(addr)                  (lowByte((addr) | 0x80))
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03
#define USB_EP_SIZE 1
#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
  { 7, 5, _addr,_attr,_packetSize, _interval }
#define REQUEST_DEVICETOHOST_STANDARD_INTERFACE 0
#define REQUEST_DEVICETOHOST_CLASS_INTERFACE 0
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE 0

#define TRANSFER_PGM 0

#define TRANSFER_RELEASE 0

inline
int USB_SendControl(uint8_t flags, const void *d, int len) {
  return 1;
}

inline
int USB_RecvControl(void *d, int len) {
  return 0;
}

inline
int USB_Send(uint8_t ep, const void *data, int len) {
  return 0;
}

struct USBSetup {
  uint8_t bmRequestType = REQUEST_DEVICETOHOST_CLASS_INTERFACE;
  uint8_t bRequest = 0x01; //HID_GET_REPORT;
  uint8_t wValueL;
  uint8_t wValueH = 0x21; //HID_REPORT_DESCRIPTOR_TYPE
  uint16_t wIndex = 0;
  uint16_t wLength;
};
