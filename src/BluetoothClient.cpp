#include "BluetoothClient.h"

BluetoothClient::BluetoothClient():_socket(0),_numRsp(0)
{
   // Reserve some space for the list of available devices
   _devices = (inquiry_info*)malloc(_maxRsp * sizeof(inquiry_info));
}

BluetoothClient::~BluetoothClient() {
   // Free the memory allocated for the devices
   free(_devices);
}


void BluetoothClient::getAvailableDevices() {
   // Define the temporary parameters
   int flags = IREQ_CACHE_FLUSH; // IREQ_CACHE_FLUSH, flushes the previous list of discovered devices before finding any new one

   // Open a socket on an available adapter
   if(BluetoothClient::openHciSocket()) {
      // Look for devices in the surrounding
      _numRsp = hci_inquiry(_devId, BluetoothClient::DISCOVERY_TIMEOUT, BluetoothClient::MAX_RESPONSE, NULL, &_devices, flags);
      if(_numRsp < 0) {
         perror("An error happened while querying for bluetooth devices.");
      }

      // Close the socket
      close(_socket);
   }
}

bdaddr_t BluetoothClient::chooseDevice(){
   // Variables
   int i;
   int choice = -1;
   char addr[19] = {0}; // Will temporarily store the address of device
   char name[248] = {0}; // Will temporarily store the name of a device
   bdaddr_t chosenAddr = NULL;

   // Get the list of available devices
   BluetoothClient::getAvailableDevices();

   // Open a socket
   if(BluetoothClient::openHciSocket()) {
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
      if(_numRsp > 0) {
         // Ask the user to choose a device
         std::cout << "\nChoose the device you wish to connect to:" << std::endl;
         std::cin >> choice;
      }
      else {
         exit(1);
      }
   }

   // If the user was able to perform a choice
   if(choice != -1 && choice < _numRsp) {
      chosenAddr = (_devices+choice)->bdaddr;
   }

   // Return the address of the choosen device
   return chosenAddr;
}


bool BluetoothClient::openHciSocket() {
   // Define variables
   bool status = true;

   // Try to open the socket
   _devId = hci_get_route(NULL); // This way the first adapter will be chosen
   _socket = hci_open_dev(_devId);
   if(_devId < 0 || _socket < 0) {
      // On error display a message
      perror("Unable to open a socket on the bluetooth adapter.");
      status = false;
   }

   // Return true on opened socket, false otherwise
   return status;
}

bool BluetoothClient::connectToDevice(char* btAddr) {
   // Define variables
   bool connected = true;
   int status = 0;
   sockaddr_rc addr = { 0 };


   // Test the value of the provided address
   if(btAddr == NULL) {
      // Let you choose the device you want to connect to
      addr.rc_bdaddr = BluetoothClient::chooseDevice();
   }
   else {
      // Simply translate the provided address
      str2ba(btAddr, &addr.rc_bdaddr);
   }

   // Set the remaining parameters for the connection
   addr.rc_family = AF_BLUETOOTH;
   addr.rc_channel = (uint8_t)1; // Under linux this means that it will bind to the first available channel

   std::cout << "Trying to open a connection ..." << std::endl;

   // Allocate a socket
   if((_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0) { // We use RFCOMM in this case a we will be streaming data from sensors
      // Display an error message
      perror(">>> An error happened while creating the socket");
      connected = false;
      close(_socket);
   }
   else {
      // Try to connect
      if((status = connect(_socket, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
         // Display an error message to the user
         perror(">>> Connection has failed");
         connected = false;
         // Close the allocated socket
         close(_socket);
      }
      else {
         std::cout << ">>> Connection established" << std::endl;
      }
   }

   // Return the status of the connection
   return connected;
}

void BluetoothClient::disconnect() {
   std::cout << "Closing the socket ..." << std::endl;
   // Unceremoniously close the socket
   close(_socket);
   std::cout << ">>> Disconnected from the device" << std::endl;
}
