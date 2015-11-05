#include "Packet.h"

// Initialize the sequence
uint8_t Packet::_seq = 0;

/** Default constructor */
Packet::Packet(uint8_t devId, uint8_t cmdId, uint8_t* data, uint8_t dataLen, bool isAsync):_sop1(0xFF), _devId(devId), _cmdId(cmdId), _data(data)
{
   // Make the packet asynchronous if required
   if(isAsync) {
      _sop2 = Packet::ASYNC_PACKET;

      // Sequence number has no meaning in async mode
      _seqNbr = 0;
   }
   else {
      _sop2 = Packet::SYNC_PACKET;

      // Define a sequence number for the packet
      _seqNbr = Packet::_seq++;
   }

   // Compute the length
   _len = dataLen + sizeof(_chkSum);

   // Compute the checksum for the packet
   _chkSum = ~((_devId + _cmdId + _seqNbr + _len + sizeof(_data)) % 256); // ~ is the bitwise NOT operator realizing the bit inverted part of the operation
}

/** Default destructor */
Packet::~Packet()
{
   // This is not the function you are looking for
}

/**
 * @brief getSize: Compute the total length of the packet
 * @return size_t
 */
size_t Packet::getSize() {
   // Simply returns the size of the packet which is 6bytes + the data payload and checksum already contained in _len
   return (size_t)(6 + _len);
}
