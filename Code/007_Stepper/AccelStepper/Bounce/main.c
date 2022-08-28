#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include <math.h>
#include "NuMicro.h"
#include "system_init.h"
//***************************************************************************//
uint32_t HCLK;
unsigned long now = 0;
unsigned long time;
//***************************************************************************//
typedef enum
{ FULL4WIRE = 4, ///< 4 wire full stepper, 4 motor pins required
  HALF4WIRE = 8  ///< 4 wire half stepper, 4 motor pins required
} MotorInterfaceType;

typedef enum
{
  DIRECTION_CCW = 0,  ///< Counter-Clockwise
  DIRECTION_CW  = 1   ///< Clockwise
} Direction;
//***************************************************************************//
u8 _pin[4] = {2, 3, 4, 5};
u8 _pinInverted[4] = {0};
//***************************************************************************//
bool _direction = DIRECTION_CCW;
float _maxSpeed = 1.0;
float _acceleration = 0.0;
float _cmin = 1.0;
long  _n = 0;
float _speed = 0.0;
unsigned long  _stepInterval = 0;
float _cn = 0.0;
float _c0 = 0.0;
long  _targetPos = 0;
long  _currentPos = 0;
unsigned long  _lastStepTime = 0;
u8 _interface;
u8 numpins;
//***************************************************************************//
void AccelStepper_stepper(u8 interface);
void setMaxSpeed(float speed);
void computeNewSpeed(void);
long distanceToGo(void);
void setSpeed(float speed);
bool runSpeed(void);
void step(long step);
void step4(long step);
void step8(long step);
void setAcceleration(float acceleration);
long currentPosition();
bool run();
void moveTo(long absolute);
void micros(void);
//***************************************************************************//
int main()
{
    SYS_Init();
    UART0_Init();
    HCLK = CLK_GetHCLKFreq();
    printf("HCLK %u Hz\n", HCLK);
    GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 | BIT3), GPIO_MODE_OUTPUT);
    AccelStepper_stepper(FULL4WIRE);
    setMaxSpeed(500000);
    setAcceleration(25000);
    moveTo(1024);
    while(1){
    if (distanceToGo() == 0)
    moveTo(-currentPosition());
    run();
    }
}
//***************************************************************************//
void AccelStepper_stepper(u8 interface) {
  _interface = interface;
}
//---------------------------------------------------------------------------//
void setMaxSpeed(float speed)
{
  if (speed < 0.0)
    speed = -speed;
  if (_maxSpeed != speed)
  {
    _maxSpeed = speed;
    _cmin = 1000000.0 / speed;
    if (_n > 0)
    {
      _n = (long)((_speed * _speed) / (2.0 * _acceleration));
      computeNewSpeed();
    }
  }
}
//---------------------------------------------------------------------------//
void computeNewSpeed(void) {
  long distanceTo = distanceToGo();
  long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration));
  if (distanceTo == 0 && stepsToStop <= 1)
  {
    _stepInterval = 0;
    _speed = 0.0;
    _n = 0;
    return;
  }
  if (distanceTo > 0)
  {
    if (_n > 0)
    {
      if ((stepsToStop >= distanceTo) || _direction == DIRECTION_CCW)
        _n = -stepsToStop; // Start deceleration
    }
    else if (_n < 0)
    {
      if ((stepsToStop < distanceTo) && _direction == DIRECTION_CW)
        _n = -_n; // Start accceleration
    }
  }
  else if (distanceTo < 0)
  {
    if (_n > 0)
    {
      if ((stepsToStop >= -distanceTo) || _direction == DIRECTION_CW)
        _n = -stepsToStop; // Start deceleration
    }
    else if (_n < 0)
    {
      if ((stepsToStop < -distanceTo) && _direction == DIRECTION_CCW)
        _n = -_n; // Start accceleration
    }
  }
  if (_n == 0)
  {
    _cn = _c0;
    _direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
  }
  else
  {
    _cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1)); // Equation 13
    _cn = max(_cn, _cmin);
  }
  _n++;
  _stepInterval = _cn;
  _speed = 1000000.0 / _cn;
  if (_direction == DIRECTION_CCW)
    _speed = -_speed;

#if 0

#endif
}
//---------------------------------------------------------------------------//
long distanceToGo(void)
{
  return _targetPos - _currentPos;
}
//---------------------------------------------------------------------------//
void setSpeed(float speed)
{
  if (speed == _speed)
    return;
  speed = constrain(speed, -_maxSpeed, _maxSpeed);
  if (speed == 0.0)
    _stepInterval = 0;
  else
  {
    _stepInterval = fabs(1000000.0 / speed);
    _direction = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
  }
  _speed = speed;
}
//---------------------------------------------------------------------------//
bool runSpeed(void)
{
  if (!_stepInterval)
    return false;
  micros();
  time = now;
  if (time - _lastStepTime >= _stepInterval)
  {
    if (_direction == DIRECTION_CW)
    {
      _currentPos += 1;
    }
    else
    {
      _currentPos -= 1;
    }
    step(_currentPos);

    _lastStepTime = time;

    return true;
  }
  else
  {
    return false;
  }
}
//---------------------------------------------------------------------------//
void step(long step)
{
  switch (_interface)
  {
    case FULL4WIRE:
      step4(step);
      break;

    case HALF4WIRE:
      step8(step);
      break;
  }
}
//---------------------------------------------------------------------------//
void step4(long step)
{
  switch (step & 0x3)
  {
    case 0:    // 1010
    PA0=1;PA1=0;PA2=0;PA3=0;
      break;

    case 1:    // 0110
    PA0=0;PA1=1;PA2=0;PA3=0;
      break;

    case 2:    //0101
   PA0=0;PA1=0;PA2=1;PA3=0;
      break;

    case 3:    //1001
    PA0=0;PA1=0;PA2=0;PA3=1;
      break;
  }
}
//---------------------------------------------------------------------------//
void step8(long step)
{
  //in the process...
}
//---------------------------------------------------------------------------//
void setAcceleration(float acceleration)
{
  if (acceleration == 0.0)
    return;
  if (acceleration < 0.0)
    acceleration = -acceleration;
  if (_acceleration != acceleration)
  {
    _n = _n * (_acceleration / acceleration);
    _c0 = 0.676 * sqrt(2.0 / acceleration) * 1000000.0;
    _acceleration = acceleration;
    computeNewSpeed();
  }
}
//---------------------------------------------------------------------------//
void moveTo(long absolute)
{
  if (_targetPos != absolute)
  {
    _targetPos = absolute;
    computeNewSpeed();
  }
}
//---------------------------------------------------------------------------//
long currentPosition()
{
  return _currentPos;
}
//---------------------------------------------------------------------------//
bool run()
{
  if (runSpeed())
    computeNewSpeed();
  return _speed != 0.0 || distanceToGo() != 0;
}
//---------------------------------------------------------------------------//
void micros(){
  TIMER_Delay(TIMER0, 1000);
  now ++;
}
//---------------------------------------------------------------------------//
