#include "Packet.h"

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
      _seqNbr = _seq++;
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
