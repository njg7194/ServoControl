#include "ServoControl.h"

ServoControl::ServoControl(uint8_t slavenum = 1, uint32_t baudrate = 19200)
{
    this->_slavNum.PAvalue=slavenum;        // 모터 고유번호 설정
    this->_baudRte.PAvalue=baudrate;        // 통신속도 설정


    this->_comSwic.PAvalue=0;   

    _addrInit();
    _DIconfig();
}

ServoControl::~ServoControl()
{
}

void ServoControl::_DIconfig(uint8_t *confvalue)
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        this->_DIconf[i].PAvalue=confvalue[i];
    }
}

void ServoControl::start(uint8_t controlmode = POSITION)
{
    this->_contMod.PAvalue=controlmode;
    this->_contMod.PAvalue=controlmode;     // 컨트롤 방식 설정
    _verify();
}

uint8_t ServoControl::setContmod()
{
    return this->_contMod.PAvalue;
}

void ServoControl::gohome_P()
{
    if (bitRead(this->_comSiml.PAvalue, nowDI(HOMING)) != true)
    {
        bitWrite(this->_comSiml.PAvalue, nowDI(HOMING), true);
    }
    _verify(_comSiml.);
}

void ServoControl::angle_P()
{
    
}

void ServoControl::speed_S()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

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
    this->_DIbuffer.PAaddress=COMMOD_SW;

    for (uint8_t i = DOCONF1; i < NUMofDO ; i++)
    {
        uint8_t iaddr = 0;
        this->_DIconf[iaddr].PAaddress=i;
        iaddr++;
    }

    this->_Acceleration.PAaddress=ACCLERATION;
    this->_Deceleration.PAaddress=DECELERATION;
}

void ServoControl::_DIconfig()
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        this->_DIconf[i].PAvalue=defaultDIinit[i];
    }
    
}

void ServoControl::_setPA(PA8_ para, uint8_t value)
{
    para.PAvalue=value;
}

void ServoControl::_setPA(PA32_ para, uint32_t value)
{
    para.PAvalue=value;
}

void ServoControl::_verify()
{
    
}

uint8_t ServoControl::nowDI(DI di)
{
    for (uint8_t i = 0 ; i < NUMofDI; i++)
    {
        if (this->_DIconf[i].PAvalue==di) return i;
        else return -1;
    }
}
