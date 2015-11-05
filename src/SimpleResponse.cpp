#include "SimpleResponse.h"

/**
 * @brief Default constructor
 * @param *buf: the received buffer
 */
SimpleResponse::SimpleResponse(uint8_t *buf)
{
   // Hydrate the object with the buffer's content
   _spo1 = *(buf);
   _spo2 = *(buf+1);
   _mrsp = *(buf+2);
   _seqNbr = *(buf+3);
   _len = *(buf+4);
   _chkSum = *(buf+5);
}

/** Default destructor */
SimpleResponse::~SimpleResponse()
{
   // This is not the function you're looking for
}

/**
 * @brief getMrsp: Getter for the _mrsp member
 * @return uint8_t
 */
uint8_t SimpleResponse::getMrsp(){
   return _mrsp;
}
