#include "ServoControl.h"

ServoControl::ServoControl(uint8_t slavenum, uint32_t baudrate)
{
    _slavNum.PAvalue=slavenum;                    // 모터 고유번호 설정
    _baudRte.PAvalue=baudrate;                    // 통신속도 설정
    _comCont.PAvalue=(uint8_t)COMMUNICATION_MODE; // 작동모드modbus로
    _comSwic.PAvalue=0b11111111;                  // 모든DI modbus작동

    _addrInit();
    _DIconfig();
    _DOconfig();

    PAqueue.push(_slavNum);
    PAqueue.push(_baudRte);
    PAqueue.push(_comCont);
    PAqueue.push(_comSwic);

    for (uint8_t i = 0; i < NUMofDI; i++)
    PAqueue.push(_DIconf[i]);

    for (uint8_t i = 0; i < NUMofDO; i++)
    PAqueue.push(_DOconf[i]);
    
}

ServoControl::~ServoControl()
{
}


void ServoControl::start(uint8_t cont)
{
    _contMod.PAvalue=(uint32_t)cont;     // 컨트롤 방식 설정
    PAqueue.push(_contMod);
}

/*
* @brif 유저 DI설정 함수
*/
void ServoControl::DIconfig(const uint8_t* confvalue)
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        _DIconf[i].PAvalue=confvalue[i];
    }
}

/*
* @brif 유저 DO설정 함수
*/
void ServoControl::DOconfig(const uint8_t* confvalue)
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        _DIconf[i].PAvalue=confvalue[i];
    } 
}

/*
* 모터 제어모드 세팅
*/
void ServoControl::setContmod(uint8_t contmd)
{
    _contMod.PAvalue=(uint32_t)contmd;
    PAqueue.push(_contMod);
}

/*
* DI 모의제어 커멘드 전송
*/
void ServoControl::DIcmdSend(uint8_t di)
{
    _comSiml.PAvalue=(uint32_t)di;
    PAqueue.push(_comSiml);
}

/*
* @brif 현제 큐 반환
*/
void ServoControl::queueExe(PA_& pa)
{
    //PA_ buff = PAqueue.peek().check;
    pa = PAqueue.pop();
    pa.check = false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* @brif 변수 주소값 초기화 함수
*/
void ServoControl::_addrInit()
{
    _slavNum.PAaddress=SLAVNUM;
    _baudRte.PAaddress=BAUDRATE;
    _comCont.PAaddress=COMCONT;
    _comSwic.PAaddress=COMMOD_SW;

    _contMod.PAaddress=CONTMOD;
    _comSiml.PAaddress=COMSIMUL;

    for (uint8_t i = 0; i < NUMofDI ; i++)
    {
        uint8_t iaddr = DICONF1;
        _DIconf[i].PAaddress=iaddr;
        iaddr++;
    }

    for (uint8_t i = 0; i < NUMofDO ; i++)
    {
        uint8_t iaddr = DOCONF1;
        _DOconf[i].PAaddress=iaddr;
        iaddr++;
    }

    _Acceleration.PAaddress=ACCLERATION;
    _Deceleration.PAaddress=DECELERATION;
}


/*
* @brif DI기본값 디폴트로 초기화
*/
void ServoControl::_DIconfig()
{
    for (uint8_t i = 0; i < NUMofDI; i++)
    {
        _DIconf[i].PAvalue=defaultDIinit[i];
    } 
}
/*
* @brif DO기본값 디폴트로 초기화
*/
void ServoControl::_DOconfig()
{
    for (uint8_t i = 0; i < NUMofDO; i++)
    {
        _DOconf[i].PAvalue=defaultDOinit[i];
    } 
}

/*
* @brif 해당하는 명령의 현제 세팅된 DI포트를 출력
*/
uint8_t ServoControl::nowDI(uint8_t di)
{
    for (uint8_t i = 0 ; i < NUMofDI; i++)
    {
        if (_DIconf[i].PAvalue==di) return i;
        else return 0;
    }
}
