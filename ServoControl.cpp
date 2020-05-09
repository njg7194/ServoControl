#include "ServoControl.h"

ServoControl::ServoControl(uint8_t slavenum = 1, uint32_t baudrate = 19200)
{
    this->_slavNum.PAvalue=slavenum;                    // 모터 고유번호 설정
    this->_baudRte.PAvalue=baudrate;                    // 통신속도 설정
    this->_comCont.PAvalue=(uint8_t)COMMUNICATION_MODE; // 통신 제어모드

    this->_comSwic.PAvalue=0;   

    _addrInit();
    _DIconfig();
}

ServoControl::~ServoControl()
{
}


void ServoControl::start(SRV_CONTMODE cont = POSITION)
{
    this->_contMod.PAvalue=(uint8_t)cont;     // 컨트롤 방식 설정

}

/*
* @brif 외부 DI설정 함수
*/
void ServoControl::DIconfig(uint8_t *confvalue)
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        this->_DIconf[i].PAvalue=confvalue[i];
    }
}

/*
* @brif 외부 DI설정 함수
*/
void ServoControl::_DIconfig()
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        this->_DIconf[i].PAvalue=defaultDIinit[i];
    } 
}

/*
* 모터 제어모드 세팅
*/
void ServoControl::setContmod(SRV_CONTMODE contmd)
{
    this->_contMod.PAvalue=(uint32_t)contmd;
    PA_ buffer=this->_contMod;
    if (this->_contMod.PAaddress==CONTMOD)
    {
        DIqueue.push(buffer);
        
    }
}

/*
* 서보 커멘드 전송
*/
void ServoControl::servoCmd(DI di)
{
    this->_contMod.PAvalue=(uint32_t)di;
    PA_ buffer=this->_contMod;
    if (this->_contMod.PAaddress==CONTMOD)
    {
        DIqueue.push(buffer);
        
    }
}

//대기열 큐에 DI가 있는지 확인
bool ServoControl::cmdCheck(DI di)
{
    if (DIqueue.find((uint8_t)di))
    return true;
    else
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* @brif 변수 주소값 초기화 함수
*/
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

/*
* @brif DI기본값 디폴트로 초기화
*/
void ServoControl::_DIconfig()
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        this->_DIconf[i].PAvalue=defaultDIinit[i];
    } 
}
/*
* @brif DO기본값 디폴트로 초기화
*/
void ServoControl::_DOconfig()
{
    for (uint8_t i = 0; i < NUMofDO; i++)
    {
        this->_DOconf[i].PAvalue=defaultDOinit[i];
    } 
}


void ServoControl::_setPA(PA_ para, uint32_t value)
{
    para.PAvalue=value;
}

/*
* @brif 해당하는 명령의 현제 세팅된 DI포트를 출력
*/
uint8_t ServoControl::nowDI(DI di)
{
    for (uint8_t i = 0 ; i < NUMofDI; i++)
    {
        if (this->_DIconf[i].PAvalue==di) return i;
        else return 0;
    }
}
