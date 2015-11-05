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

/**
 * @brief format: format the packet object into an array of byte ready to be sent
 * @param dataLen: Length of the data payload
 * @return uint8_t*
 */
uint8_t* Packet::format() {
   // Declare an array that will containt our formated packet
   uint8_t fmtPckt[getSize()] = {};

   // Fill in the 6 first members
   fmtPckt[0] = _sop1;
   fmtPckt[1] = _sop2;
   fmtPckt[2] = _devId;
   fmtPckt[3] = _cmdId;
   fmtPckt[4] = _seqNbr;
   fmtPckt[5] = _len;

   // Fill in the data payload
   for(int i = 0; i < (_len - 1); ++i) {
      fmtPckt[6 + i] = *(_data + i);
   }

   // Finally the checksum goes in the last cell
   fmtPckt[6 + _len] = _chkSum;

   // And return the array
   return fmtPckt;
}

/**
 * @brief getSeqNbr : Getter for the member _seqNbr
 * @return uint8_t
 */
uint8_t Packet::getSeqNbr() {
   return _seqNbr;
}

/**
 * @brief getDevId : Getter for the member _devId
 * @return uint8_t
 */
uint8_t Packet::getDevId() {
   return _devId;
}

/**
 * @brief getCmdId : Getter for the member _cmdId
 * @return uint8_t
 */
uint8_t Packet::getCmdId() {
   return _cmdId;
}
