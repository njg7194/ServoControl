#include "ServoControl.h"


ServoControl::ServoControl(uint8_t slavenum = 1, uint32_t baudrate = 19200)
{
    this->_slavNum.PAvalue=slavenum;        // 모터 고유번호 설정
    this->_baudRte.PAvalue=baudrate;        // 통신속도 설정
    this->_contMod.PAvalue=controlmode;     // 컨트롤 방식 설정

    this->_comSwic.PAvalue=0;   

    _addrInit();
    
}

ServoControl::~ServoControl()
{
}

void ServoControl::start(uint8_t controlmode = POSITION)
{
    this->_contMod.PAvalue=controlmode;

    _verify();
}

uint8_t ServoControl::setContmod()
{
    return this->_contMod.PAvalue;
}

void ServoControl::angle()
{
    
}

void ServoControl::speed()
{

}


///////////////////////////////////////////////////////////////////////////////


void ServoControl::_addrInit()
{

    this->_slavNum.PAaddress=SLAVNUM;
    this->_baudRte.PAaddress=BAUDRATE;
    this->_comCont.PAaddress=COMCONT;
    this->_comSwic.PAaddress=COMMOD_SW;

    this->_contMod.PAaddress=CONTMOD;
    this->_comSiml.PAaddress=COMSIMUL;

    for (uint8_t i = DICONF1; i < NUMofDI ; i++)
    {
        uint8_t iaddr = 0;
        this->_DIconf[iaddr].PAaddress=i;
        iaddr++;
    }

    for (uint8_t i = DOCONF1; i < NUMofDO ; i++)
    {
        uint8_t iaddr = 0;
        this->_DIconf[iaddr].PAaddress=i;
        iaddr++;
    }

    this->_Acceleration.PAaddress=ACCLERATION;
    this->_Deceleration.PAaddress=DECELERATION;
    

}

void ServoControl::_setPA(PA8_ para, uint8_t value)
{
    para.PAvalue=value;
}

void ServoControl::_setPA(PA32_ para, uint32_t value)
{
    para.PAvalue=value;
}

bool ServoControl::_verify()
{
    
}