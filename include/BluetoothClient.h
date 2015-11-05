#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>

#include "Packet.h"
#include "SimpleResponse.h"

struct Command {
   uint8_t devId;
   uint8_t cmdId;
};


class BluetoothClient
{
   public:
      /** Default constructor */
      BluetoothClient();
      /** Default destructor */
      virtual ~BluetoothClient();

      /**
       * @brief connect: Connect to a bluetooth device
       * @param btAddr: The bluetooth address of the device
       * @return bool
       */
      bool connectToDevice(char* btAddr);

      /**
       * @brief disconnect: Close the connection with the bluetooth device.
       * @return void
       */
      void disconnect();

      /**
       * @brief send: Send a packet to the bluetooth device
       * @param packet: A pointer to the packet to send
       * @return size_t
       */
      size_t sendPacket(Packet *packet);


   protected:
      /**
       * @brief openSocket: Tries to open an HCI socket on the first available adapter.
       * @param devId: id of the bluetooth adapter to use.
       * @return bool
       */
      bool openHciSocket(int &devId);

      /**
       * @brief getAvailableDevices: Query the surrounding for available bluetooth devices.
       * @param devices: the list of available devices
       * @param numRsp: The number of available devices
       * @return void
       */
      void getAvailableDevices(inquiry_info* devices, int &numRsp);

      /**
       * @brief chooseDevice: Allow you to choose which bluetooth device you want to connect to.
       * @return bdaddr_t
       */
      bdaddr_t chooseDevice();


   private:
      int _socket;
      Command _lookup[256];

      const int MAX_RESPONSE = 255;
      const int DISCOVERY_TIMEOUT = 8;

};

#endif // BLUETOOTHCLIENT_H
