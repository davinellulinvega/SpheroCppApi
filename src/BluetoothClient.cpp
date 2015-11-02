#include "BluetoothClient.h"

BluetoothClient::BluetoothClient():_socket=0,_maxRsp=255
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
   char addr[19] = {0}; // Will temporarily store the address of device
   char name[248] = {0}; // Will temporarily store the name of a device

   // Open a socket on an available adapter
   devId = hci_get_route(NULL); // This way the first adapter will be chosen
   _socket = hci_open_dev(devId);
   if(devId < 0 || _socket < 0) {
      perror("Unable to open a socket on the bluetooth adapter.");
      exit(1);
   }

   // Look for devices in the surrounding
   numRsp = hci_inquiry(devId, timeout, maxRsp, NULL, &_devices, flags);
   if(numRsp < 0) {
      perror("An error happened while querying for bluetooth devices.");
      exit(2);
   }

   // Close the socket
   close(_socket);
}
