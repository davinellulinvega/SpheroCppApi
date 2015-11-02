#ifndef BLUETOOTHCLIENT_H
#define BLUETOOTHCLIENT_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


class BluetoothClient
{
   public:
      /** Default constructor */
      BluetoothClient();
      /** Default destructor */
      virtual ~BluetoothClient();
      /**
       * @brief getAvailableDevices: Query the surrounding for available bluetooth devices.
       * @param int timeout: The function will be looking for devices for a duration of timeout * 1.28s
       * @return void
       */
      void getAvailableDevices(int timeout);

      /**
       * @brief chooseDevice();
       *
       */
      bdaddr_t chooseDevice();

      /**
       * @brief connect: Connect to a bluetooth device
       * @param btAddr: The bluetooth address of the device
       * @return bool
       */
      bool connect(std::string btAddr);

      /**
       * @brief disconnect: Close the connection with the bluetooth device.
       * @return void
       */
      void disconnect();


   protected:
      /**
       * @brief openSocket: Tries to open a socket on the first available adapter
       * @return bool
       */
      bool openSocket();
   private:
      inquiry_info *_devices;
      int _socket;
      int _maxRsp;
      int _numRsp;
      bdaddr_t _activeAddr;
};

#endif // BLUETOOTHCLIENT_H
