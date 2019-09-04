/**
 * Antdroid Genesis
 * First generation firmware for Kumonga
 *
 * Coxa   - Servo that connects leg to body 'the first or basal segment of the leg of an insect'
 * Femur  - Middle leg servo
 * Tibia  - Lower leg (the blue legs)
*/
#include "Tlc5940.h"
#include "tlc_servos.h"

#include "Debug.h"
#include "Configuration.h"
#include "Helpers.h"
#include "Motion.h"
#include "Motions.h"

bool controlMode = false;

/** Build the servo array and initialize the servos */
void initializeServos()
{
  DEBUG_PRINT("initalizeServos()");

  tlc_initServos();

  for (int i = 0; i < 18; i++)
  {
    if (SERVO_ENABLED[i])
    {
      DEBUG_PRINT("Configuring servo " + (String)i + " on pin " + (String)SERVO_PIN_MAP[i]);
      
      servoSet(i, SERVO_INITPOS_OFFSET[i], true);
    }
    else
    {
      DEBUG_PRINT("Skipping servo " + (String)i + " configuration for pin " + (String)SERVO_PIN_MAP[i]);
    }

    delay(SERVO_SETUP_DELAY);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Antdroid starting...");

  initializeServos();

  /** DO STUFF */
  MotionPrepareForStand();
  delay(1000);

  MotionPushUpright();
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
    controlMode = !controlMode;
    Serial.println("Changed control mode to " + getControlModeName());
    break;
  }
  default: // Move a specific servo
    moveServoFromString(_servo, pos);
    break;
  }
}

String getControlModeName()
{
  return controlMode ? "Relative to initial" : "Relative to current";
}

void moveServoFromString(String _servo, int pos)
{
  int servo = _servo.toInt();

  if (servo >= 0 && servo < 18)
  {
    DEBUG_PRINT("Setting servo " + (String)servo + " to position " + (String)pos + " mode: " + getControlModeName());

    if (controlMode)
      setSingleServoRelativeToInitial(servo, pos, SERVO_WAIT_TIME);
    else
      setSingleServoRelativeToSelf(servo, pos, SERVO_WAIT_TIME);
  }
  else
  {
    DEBUG_PRINT("Specificed servo is out of range: " + (String)servo);
  }
}