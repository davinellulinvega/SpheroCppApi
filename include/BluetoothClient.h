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
       * @return void
       */
      void getAvailableDevices();

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
   private:
      inquiry_info *_devices;
      int _socket;
};

#endif // BLUETOOTHCLIENT_H
