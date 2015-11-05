#ifndef SIMPLERESPONSE_H
#define SIMPLERESPONSE_H

#include <stdint.h>

class SimpleResponse
{
   public:
      /**
       * @brief Default constructor
       * @param *buf: the received buffer
       */
      SimpleResponse(uint8_t *buf);

      /** Default destructor */
      virtual ~SimpleResponse();

      /**
       * @brief getMrsp: Getter for the _mrsp member
       * @return uint8_t
       */
      uint8_t getMrsp();

      static const int PACKET_SIZE = 6;

   protected:
   private:
      uint8_t _spo1;
      uint8_t _spo2;
      uint8_t _mrsp;
      uint8_t _seqNbr;
      uint8_t _len;
      uint8_t _chkSum;
};

#endif // SIMPLERESPONSE_H
