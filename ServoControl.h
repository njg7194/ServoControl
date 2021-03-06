#ifndef ServoControl_h
#define ServoControl_h

//#define ARDUINO 10000       //development on Visual code

//#include "arduino.h"
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

static uint8_t defaultDIinit[]   = {SRV_ON, A_CLR, CWL, CCWL, EMG, POS_LOAD, ORG_SW, HOMING};
static uint8_t* positionDIinit   = defaultDIinit;
static uint8_t speedDIinit[]     = {SRV_ON, A_CLR, CWL, CCWL, ZEROSPD, SPD_DIR, INTSPD1, INTSPD2};
static uint8_t torqueDIinit[]    = {SRV_ON, A_CLR, CWL, CCWL, ZEROSPD, SPD_DIR, TL_SEL, INTSPD1, INTSPD2};

static uint8_t defaultDOinit[]   = {S_RDY, ALM, COIN, BRK_OFF, ZSP, TLC};
static uint8_t* positionDOinit   = defaultDOinit;
static uint8_t speedDOinit[]     = {S_RDY, ALM, AT_SPEED, BRK_OFF, ZSP, TLC};
static uint8_t* torqueDOinit     = defaultDOinit;

class ServoControl
{
    private://////////////////////////////////////////////////////////////////
    Queue<PA_> PAqueue = Queue<PA_>();             //
    //communication value
    PA_ _slavNum;                                  //
    PA_ _baudRte;                                  //
    PA_ _comCont;                                  //
    PA_ _comSwic;                                  //

    //processing
    PA_ _comSiml;                                  //
    PA_ _contMod;                                  //

    PA_ _DIconf[8];                                 // 
    PA_ _DOconf[6];                                 // 
    uint8_t _DOstatus;                              // 

    void _addrInit();                               // 
    void _DIconfig();                               // 
    void _DOconfig();                               // 

    uint8_t nowDI(uint8_t di);
    //position

    //speed
    PA_ _Acceleration;                              // 
    PA_ _Deceleration;                              // 

    //torque
    
    public://////////////////////////////////////////////////////////////////////
    ServoControl(uint8_t slavenum = 1, uint32_t baudrate = 19300);
    ~ServoControl();
    void start(uint8_t cont = POSITION);

    void DIconfig(const uint8_t *confvalue);       // 
    void DOconfig(const uint8_t *confvalue);       // 
    
    void setContmod(uint8_t contmd);          // 
    void DIcmdSend(uint8_t di);                         // 
    void queueExe(PA_& pa);

    void angle_P();                                 // 
    void speed_S();                                 // 

};


#define SLAVNUM 0x00                                // 
#define BAUDRATE 0x00D                              // 
#define COMCONT 0x090                               // 
#define COMMOD_SW 0x01A0                            // 
//#define COMMOD_MSK 0x1A5                          // 

#define COMSIMUL 0x01A4                             // 
#define CONTMOD 0x002                               // 

#define DICONF1 0x080                               // 
#define NUMofDI 8                                   // 

#define DOCONF1 0x088                               // 
#define NUMofDO 6                                   // 
#define DOBUFFER                                    //

#define ACCLERATION 0x058                           // 
#define DECELERATION 0x059                          // 

#endif  //ServoControl_h