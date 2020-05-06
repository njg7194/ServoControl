#include "ServoControl.h"


ServoControl::ServoControl(uint8_t slavenum = 1, uint32_t baudrate = 19200, uint8_t controlmode = POSITION)
{
    this->_slavNum.PAvalue=slavenum;
    this->_baudRte.PAvalue=baudrate;
    this->_contMod.PAvalue=controlmode;

    
}

ServoControl::~ServoControl()
{
}

void ServoControl::angle()
{
    
}