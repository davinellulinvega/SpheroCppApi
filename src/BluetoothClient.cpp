#include "BluetoothClient.h"

/** Default constructor */
BluetoothClient::BluetoothClient():_socket(0)
{
}

/** Default destructor */
BluetoothClient::~BluetoothClient() {
}

/**
 * @brief getAvailableDevices: Query the surrounding for available bluetooth devices.
 * @param devices: the list of available devices
 * @param numRsp: The number of available devices
 * @return void
 */
void BluetoothClient::getAvailableDevices(inquiry_info* devices, int &numRsp) {
   // Define the temporary parameters
   int flags = IREQ_CACHE_FLUSH; // IREQ_CACHE_FLUSH, flushes the previous list of discovered devices before finding any new one
   int devId = -1;

   // Open a socket on an available adapter
   if(BluetoothClient::openHciSocket(devId)) {
      // Look for devices in the surrounding
      numRsp = hci_inquiry(devId, BluetoothClient::DISCOVERY_TIMEOUT, BluetoothClient::MAX_RESPONSE, NULL, &devices, flags);
      if(numRsp < 0) {
         perror("An error happened while querying for bluetooth devices.");
      }

      // Close the socket
      close(_socket);
   }
}

/**
 * @brief chooseDevice: Allow you to choose which bluetooth device you want to connect to.
 * @return bdaddr_t
 */
bdaddr_t BluetoothClient::chooseDevice(){
   // Variables
   int devId = -1;
   int i;
   int choice = -1;
   int numRsp = 0;
   char addr[19] = {0}; // Will temporarily store the address of device
   char name[248] = {0}; // Will temporarily store the name of a device
   bdaddr_t chosenAddr = { 0 };
   inquiry_info* devices = (inquiry_info*)malloc(BluetoothClient::MAX_RESPONSE * sizeof(inquiry_info));

   // Get the list of available devices
   BluetoothClient::getAvailableDevices(devices, numRsp);

   // Open a socket
   if(BluetoothClient::openHciSocket(devId)) {
      for(i=0; i < numRsp; i++) {
         // Ask the device for its friendly name
         if(hci_read_remote_name(_socket, &(devices+i)->bdaddr, sizeof(name), name, 0) < 0) {
            // If no name is returned
            strcpy(name, "Unknown");
         }
         // Translate the address from bdaddr format to string
         ba2str(&(devices+i)->bdaddr, addr);

         // Display the address followed by the name as a menu item
         printf("%d > %s %s\n", i, addr, name);
      }

      // Close the socket
      close(_socket);

      if(numRsp > 0) {
         // Ask the user to choose a device
         std::cout << "\nChoose the device you wish to connect to:" << std::endl;
         std::cin >> choice;
      }
      else {
         exit(1);
      }
   }

   // If the user was able to perform a choice
   if(choice != -1 && choice < numRsp) {
      chosenAddr = (devices+choice)->bdaddr;
   }

   // Destroy the list of bluetooth devices
   free(devices);

   // Return the address of the choosen device
   return chosenAddr;
}

/**
 * @brief openSocket: Tries to open an HCI socket on the first available adapter.
 * @param devId: id of the bluetooth adapter to use.
 * @return bool
 */
bool BluetoothClient::openHciSocket(int &devId) {
   // Define variables
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

/**
 * @brief connect: Connect to a bluetooth device
 * @param btAddr: The bluetooth address of the device
 * @return bool
 */
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

/**
 * @brief disconnect: Close the connection with the bluetooth device.
 * @return void
 */
void BluetoothClient::disconnect() {
   std::cout << "Closing the socket ..." << std::endl;
   // Unceremoniously close the socket
   close(_socket);
   std::cout << ">>> Disconnected from the device" << std::endl;
}

/**
 * @brief send: Send a packet to the bluetooth device
 * @param packet: The packet to send
 * @return size_t
 */
size_t BluetoothClient::sendPacket(Packet *packet) {
   // Variables
   uint8_t* fmtPckt;
   Command cmdStruct;
   size_t status = 0;

   // Format the packet
   fmtPckt = packet->format();

   // Fill in the look up table
   cmdStruct.devId = packet->getDevId();
   cmdStruct.cmdId = packet->getCmdId();
   _lookup[packet->getSeqNbr()] = cmdStruct;

   // Send the package
   status = send(_socket, fmtPckt, packet->getSize(), NULL);

   // Return the status
   return status;
}
