#include "ServoControl.h"
#include <ModbusRtu.h>

ServoControl servo(1,19200);

void setup() 
{

}

void loop() 
{
  PA_ buff = servo.queueExe();
}
