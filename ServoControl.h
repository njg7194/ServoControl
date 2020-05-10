#ifndef ServoControl_h
#define ServoControl_h

//#define ARDUINO 10000       //development on Visual code

#include "arduino.h"
//#include <ModbusRtu.h>
#include <inttypes.h>
#include "Queue.h"

typedef struct
{
    uint32_t PAaddress;
    uint32_t PAvalue;
    bool check;
}PA_;
/*
enum TYPE
{
    T_COM_CONTMODE
    T_SRV_CONTMODE
    T_DI
    T_DO
};*/

enum COM_CONTMODE
{
    WIRING_MODE                 =0,                 // control external wiring connection.
    COMMUNICATION_MODE          =1                  // control modbus connection.
};

enum SRV_CONTMODE
{
    POSITION                    = 0,                // 
    SPEED                       = 1,                // 
    TORQUE                      = 2,                // 
    POSnSPD                     = 3,                // 
    POSnTOQ                     = 4,                // 
    SPDnTOQ                     = 5                 // 
};

enum DI
{
    SRV_ON                      = 0,                 // P/S/T    : Servo enabling
    A_CLR                       = 1,                 // P/S/T    : Alarm release
    CWL                         = 2,                 // P/S/T    : Clockwise stroke limit
    CCWL                        = 3,                 // P/S/T    : Anticlockwise stroke limit
    C_MODE                      = 4,                 // P/S/T    : Control mode switching
    ZEROSPD                     = 5,                 // P/S/T    : Zero speed clamp
    DIV                         = 6,                 // P/S/T    : Command pulse frequency selection
    SPD_DIR                     = 7,                 // P/S/T    : Speed command direction
    INH                         = 8,                 // P/S/T    : Command pulse prohibition
    GAIN                        = 9,                 // P/S/T    : Gain switching
    CL                          = 10,                // P/S/T    : Clear the deviation counter to 0
    INTSPD1                     = 11,                // P/S/T    : Internal command selection 1
    INTSPD2                     = 12,                // P/S/T    : Internal command selection 2
    INTSPD3                     = 14,                // P/S/T    : Internal command selection 3
    INTSPD4                     = 13,                // P/S/T    : Internal command selection 4
    TL_SEL                      = 15,                // P/S/T    : Torque limit switchover
    HOMING                      = 16,                // P/S/T    : Start position of "back to zero"
    ORG_SW                      = 17,                // P/S/T    : Origin switch position
    POS_LOCK                    = 18,                // P/S/T    : Servo locking
    JOG_BIT                     = 19,                // P/S/T    : JOG starting position
    POS_LOAD                    = 20,                // P/S/T    : Position loading signal
    EMG                         = 21                 // P/S/T    : Emergency stion or external error input
};

enum DO
{
    S_RDY                       = 0,                 // : Servo ready
    ALM                         = 1,                 // : Servo aarm
    COIN                        = 2,                 // : Location arrival
    BRK_OFF                     = 3,                 // : Brake Release
    ZSP                         = 4,                 // : Zero speed detection
    TLC                         = 5,                 // : Torque limiting
    V_COIN                      = 6,                 // : Speed consitency
    AT_SPEED                    = 7,                 // : Speed arrival
    OVERLOAD_O                  = 9,                 // : OVERLOAD WARNING
    BRAKE_ON                    = 10,                // : Brake pipe conduction state
    ORG_FOUND                   = 11,                // : Origin has been found
    BRAKE_ON_ERR_O              = 14,                // : Brake error message
    EEPROM_STATE_O              = 15,                // : EEPROm completion status
    JOG_RUN                     = 16,                // : JOG running position
    HOMING_ATIVED               = 17                 // : Servo back to zero status
};

const uint8_t defaultDIinit[]   = {SRV_ON, A_CLR, CWL, CCWL, EMG, POS_LOAD, ORG_SW, HOMING};
const uint8_t* positionDIinit   = defaultDIinit;
const uint8_t speedDIinit[]     = {SRV_ON, A_CLR, CWL, CCWL, ZEROSPD, SPD_DIR, INTSPD1, INTSPD2};
const uint8_t torqueDIinit[]    = {SRV_ON, A_CLR, CWL, CCWL, ZEROSPD, SPD_DIR, TL_SEL, INTSPD1, INTSPD2};

const uint8_t defaultDOinit[]   = {S_RDY, ALM, COIN, BRK_OFF, ZSP, TLC};
const uint8_t* positionDOinit   = defaultDOinit;
const uint8_t speedDOinit[]     = {S_RDY, ALM, AT_SPEED, BRK_OFF, ZSP, TLC};
const uint8_t* torqueDOinit     = defaultDOinit;

class ServoControl
{
    private://////////////////////////////////////////////////////////////////
    Queue<PA_> PAqueue = Queue<PA_>();             //PA대기열 
    //communication value
    PA_ _slavNum;                                  // 고유번호
    PA_ _baudRte;                                  // 통신속도
    PA_ _comCont;                                  // 작동모드
    PA_ _comSwic;                                  // 제어선택

    //processing
    PA_ _comSiml;                                  // DI 모의제어
    PA_ _contMod;                                  // 모터 제어모드

    PA_ _DIconf[8];                                 // DI설정
    PA_ _DOconf[6];                                 // DO설정
    uint8_t _DOstatus;                              // DO상태

    void _addrInit();                               // 주소값 할당
    void _DIconfig();                               // DI초기화(기본값)
    void _DOconfig();                               // DO초기화(기본값)

    uint8_t nowDI(DI di);
    //position

    //speed
    PA_ _Acceleration;                              // 가속도
    PA_ _Deceleration;                              // 감속도

    //torque
    
    public://////////////////////////////////////////////////////////////////////
    ServoControl(uint8_t slavenum = 1, uint32_t baudrate = 19200);
    ~ServoControl();
    void start(SRV_CONTMODE cont = POSITION);

    void DIconfig(const uint8_t *confvalue);       // vDI초기화(유저세팅)
    void DOconfig(const uint8_t *confvalue);       // DO초기화(유저세팅)
    
    void setContmod(SRV_CONTMODE contmd);          // 모터 제어모드 세팅
    void DIcmdSend(DI di);                         // DI 모의제어 커멘드 전송
    PA_ queueExe(PA_* pa);

    void angle_P();                                 // 각도만큼 이동
    void speed_S();                                 // 속도로 이동

};


#define SLAVNUM 0x00                                // 고유번호 주소
#define BAUDRATE 0x00D                              // 통신속도 주소
#define COMCONT 0x090                               // 작동모드 주소
#define COMMOD_SW 0x01A0                            // 통신 제어 선택 주소
//#define COMMOD_MSK 0x1A5                          // 통신 제어 마스크 주소

#define COMSIMUL 0x01A4                             // 모의제어 주소
#define CONTMOD 0x002                               // 모터 제어모드 주소

#define DICONF1 0x080                               // DI0설정 주소
#define NUMofDI 8                                   // DI설정 갯수

#define DOCONF1 0x088                               // DO0설정 주소
#define NUMofDO 6                                   // DO설정 갯수
#define DOBUFFER                                    //

#define ACCLERATION 0x058                           // 가속도 주소
#define DECELERATION 0x059                          // 감속도 주소

#endif  //ServoControl_h