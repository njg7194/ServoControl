#ifndef _ServoControl_
#define _ServoControl_

#include "arduino.h"
#include <inttypes.h>

typedef struct
{
    uint16_t PAaddress;
    uint8_t PAvalue;
}PA8_;

typedef struct
{
    uint16_t PAaddress;
    uint32_t PAvalue;
}PA32_;

enum COM_CONTMODE
{
    WIRING_MODE                 =0,                 // control external wiring connection.
    COMMUNICATION_MODE          =1                  // control modbus connection.
};

enum SRV_CONTMODE
{
    POSTION                     = 0,                // 
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
const uint8_t positionDIinit*   = defaultDIinit;
const uint8_t speedDIinit[]     = {SRV_ON, A_CLR, CWL, CCWL, ZEROSPD, SPD_DIR, INTSPD1, INTSPD2};
const uint8_t torqueDIinit[]    = {SRV_ON, A_CLR, CWL, CCWL, ZEROSPD, SPD_DIR, TL_SEL, INTSPD1, INTSPD2};

const uint8_t defaultDOinit[]   = {S_RDY, ALM, COIN, BRK_OFF, ZSP, TLC};
const uint8_t positionDOinit*   = defaultDOinit;
const uint8_t speedDOinit[]     = {S_RDY, ALM, AT_SPEED, BRK_OFF, ZSP, TLC};
const uint8_t torqueDOinit*     = defaultDOinit;


class ServoControl
{
    private:
    //communication value
    PA8_ _slavNum;              // 고유번호
    PA8_ _baudRte;              // 통신속도
    PA8_ _comCont;              // 제어모드
    PA8_ _comSwic;              // 제어선택

    //processing
    PA8_ _contMod;              // 모터 제어모드
    PA8_ _comSiml;              // DI 모의제어

    PA8_ _DIconf[8];            // DI설정
    bool _DIbuffer;             // DI버퍼
    PA8_ _DOconf[6];            // DO설정
    bool _DObuffer;             // DO버퍼

    //position

    //speed
    PA32_ _Acceleration;        // 가속도
    PA32_ _Deceleration;        // 감속도

    //torque
    
    public:
    ServoControl(/* args */);
    ~ServoControl();

    void angle();
    void speed();
};

ServoControl::ServoControl(/* args */)
{
}

ServoControl::~ServoControl()
{
}

#define COMMOD_SW 0x01A0        // 제어선택 주소
#define COMMOD_MSK 0x1A5        // 제어마스크 주소
#define COMSIMUL 0x01A4         // 모의제어 주소
#define DICONF1 0x080           // DI1설정