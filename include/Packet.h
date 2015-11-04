#ifndef PACKET_H
#define PACKET_H


class Packet
{
   public:
      /** Default constructor */
      Packet(uint8_t devId, uint8_t cmdId, uint8_t* data, uint8_t dataLen, bool isAsync);
      /** Default destructor */
      virtual ~Packet();
   protected:
   private:
      uint8_t _sop1; // Start of packet 1, always 0xFF
      uint8_t _sop2; // Start of packet 2
      uint8_t _devId; // Device Id: 0x00 -> Core, 0x02 -> Sphero
      uint8_t _cmdId; // See sphero documentation
      uint8_t _seqNbr; // A user defined sequence number
      uint8_t _len; // Length of data + checksum
      uint8_t* _data; // Optional data to send with the command
      uint8_t _chkSum; // Sum of all bytes from _devId through to the end of the _data payload, modulo 256 and bit inverted

      const uint8_t ASYNC_PACKET = 0xfe;
      const uint8_t SYNC_PACKET = 0xff;

      static uint8_t _seq = 0;
};

#endif // PACKET_H
