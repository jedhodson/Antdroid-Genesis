/**
 * Antdroid Genesis
 * First generation firmware for Kumonga
 *
 * Coxa   - Servo that connects leg to body 'the first or basal segment of the leg of an insect'
 * Femur  - Middle leg servo
 * Tibia  - Lower leg (the blue legs)
*/

#include "Debug.h"
#include "Configuration.h"
#include "Helpers.h"
#include "Servos.h"
#include "Motion.h"
#include "Motions.h"

typedef enum {
  RELATIVE_INITIAL = 0,
  RELATIVE_CURRENT = 1,
  ABSOLUTE = 2
} CONTROL_MODE;

CONTROL_MODE _mode = RELATIVE_INITIAL;

void setup()
{
  Serial.begin(115200);
  Serial.println("Antdroid starting...");

  initializeServos();

  /** DO STUFF */
  MotionPrepareForStand();
  delay(1000);

  MotionPushUpright();

  Serial.println("Done!");
}

void loop()
{
  // Wait for serial commands to execute. @TODO future version should be optimized so arduino isnt parsing a string
  while (Serial.available() > 0)
  {
    String str = Serial.readString();
    setCommand(str);
  }
}

void setCommand(String input)
{
  String _servo = getSplitString(input, ',', 0);
  String _pos = getSplitString(input, ',', 1);

  int pos = _pos.toInt();

  DEBUG_PRINT("SERVO[" + _servo + "], pos: " + (String)pos);

  char servoSwitch = _servo.charAt(0);
  switch (servoSwitch)
  {
  case 't': // Set all tibias to pos
    setTibias(pos);
    break;
  case 'f': // Set all femurs to pos
    setFemurs(pos);
    break;
  case 'r': // Get servo position (from memory)
  { 
    int servoPosition = SERVO_POSITION[pos];
    Serial.println("Position of servo " + (String)pos + " is " + (String)servoPosition);
    break;
  }
  case 'm': // Change control mode
  { 
    if(pos == 0) {
      int _current = (int)_mode;
      if(_current = 2) {
        _current = 0;
      }
      else {
        _current++;
      }
      _mode = (CONTROL_MODE)_current;
    } else {
      _mode = (CONTROL_MODE)(pos-1);
    }

    Serial.println("Changed control mode to " + getControlModeName());
    break;
  }
  case 's': // Adjust the speed
    DEBUG_PRINT("Changing wait time from " + (String)SERVO_WAIT_TIME + " to " + (String)pos);
    SERVO_WAIT_TIME = pos;
    break;    
  default: // Move a specific servo
    moveServoFromString(_servo, pos);
    break;
  }
}

String getControlModeName()
{
  switch(_mode) {
    case RELATIVE_CURRENT:
      return "RELATIVE_CURRENT";
      break;
    case RELATIVE_INITIAL:
      return "RELATIVE_INTITIAL";
      break;
    case ABSOLUTE:
      return "ABSOLUTE";
      break;
  }
}

void moveServoFromString(String _servo, int pos)
{
  int servo = _servo.toInt();

  if (servo >= 0 && servo < 18)
  {
    DEBUG_PRINT("Setting servo " + (String)servo + " to position " + (String)pos + ", mode: " + getControlModeName());

    switch(_mode) {
      case RELATIVE_CURRENT:
      setSingleServoRelativeToSelf(servo, pos, SERVO_WAIT_TIME);
        break;
      case RELATIVE_INITIAL:
        setSingleServoRelativeToInitial(servo, pos, SERVO_WAIT_TIME);
        break;
      case ABSOLUTE:
        servoSmoothSet(servo, pos, SERVO_WAIT_TIME);
        break;
    }
  }
  else
  {
    DEBUG_PRINT("Specificed servo is out of range: " + (String)servo);
  }
}