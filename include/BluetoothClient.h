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


   protected:
      /**
       * @brief openSocket: Tries to open a socket on the first available adapter
       * @param devId: if of the bluetooth adapter to use
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
       * @brief chooseDevice();
       *
       */
      bdaddr_t chooseDevice();


   private:
      int _socket;

      const int MAX_RESPONSE = 255;
      const int DISCOVERY_TIMEOUT = 8;

};

#endif // BLUETOOTHCLIENT_H
