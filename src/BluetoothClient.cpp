#include "BluetoothClient.h"

BluetoothClient::BluetoothClient():_socket=0,_maxRsp=255,_numRsp=0,_activeAddr=NULL
{
   // Reserve some space for the list of available devices
   _devices = (inquiry_info*)malloc(_maxRsp * sizeof(inquiry_info));
}

BluetoothClient::~BluetoothClient() {
   // Free the memory allocated for the devices
   free(_devices);
}


BluetoothClient::getAvailableDevices(int timeout) {
   // Define the temporary parameters
   int i, numRsp, devId, flags = IREQ_CACHE_FLUSH; // IREQ_CACHE_FLUSH, flushes the previous list of discovered devices before finding any new one

   // Open a socket on an available adapter
   if(BluetoothClient::openSocket()) {
      // Look for devices in the surrounding
      _numRsp = hci_inquiry(devId, timeout, maxRsp, NULL, &_devices, flags);
      if(_numRsp < 0) {
         perror("An error happened while querying for bluetooth devices.");
      }

      // Close the socket
      close(_socket);
   }
}

BluetoothClient::chooseDevice(){
   // Variables
   int i;
   int choice = -1;
   char addr[19] = {0}; // Will temporarily store the address of device
   char name[248] = {0}; // Will temporarily store the name of a device

   // Open a socket
   if(BluetoothClient::openSocket()) {
      for(i=0; i < _numRsp; i++) {
         // Ask the device for its friendly name
         if(hci_read_remote_name(_socket, &(_devices+i)->bdaddr, sizeof(name), name, 0) < 0) {
            // If no name is returned
            strcpy(name, "Unknown");
         }
         // Translate the address from bdaddr format to string
         ba2str(&(_devices+i)->bdaddr, addr);

         // Display the address followed by the name as a menu item
         printf("%d > %s %s\n", i, addr, name);
      }

      // Close the socket
      close(_socket);

      // Ask the user to choose a device
      std::cout << "\nChoose the device you wish to connect to: \n" << std::endl;
      std::cin >> choice;
   }

   // If the user was able to perform a choice
   if(choice != -1 && choice < _numRsp) {
      _activeAddr = &(_devices+choice);
   }

   // Return the address of the choosen device
   return _activeAddr;
}


BluetoothClient::openSocket() {
   // Define variables
   int devId;
   bool status = true;

   // Try to open the socket
   devId = hci_get_route(NULL); // This way the first adapter will be chosen
   _socket = hci_open_dev(devId);
   if(devId < 0 || _socket < 0) {
      // On error display a message
      perror("Unable to open a socket on the bluetooth adapter.");
      status = false;
   }

   // Return true on opened socket, false otherwise
   return status;
}
