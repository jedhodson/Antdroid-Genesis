/**
 * Antdroid Genesis
 * First generation firmware for Kumonga
 *
 * Coxa   - Servo that connects leg to body 'the first or basal segment of the leg of an insect'
 * Femur  - Middle leg servo
 * Tibia  - Lower leg (the blue legs)
*/

#include <Servo.h>
Servo SERVO[18];

#include "Debug.h"
#include "Configuration.h"
#include "Helpers.h"
#include "Motion.h"
#include "Motions.h"

bool controlMode = false;

/**
 * Initialize Servo
 * Attach servo to pin if no already attached and set send to initial position
 *
 * @param  index   Index of the servo in the SERVO array
*/
void initializeServo(int index)
{
  if (!SERVO[index].attached())
    SERVO[index].attach(SERVO_PIN_MAP[index]);

  setServo(index, SERVO_INITPOS_OFFSET[index]);
}

/** Build the servo array and initialize the servos */
void initializeServos()
{
  DEBUG_PRINT("initalizeServos()");

  for (int i = 0; i < 18; i++)
  {
    if (SERVO_ENABLED[i])
    {
      DEBUG_PRINT("Configuring servo " + (String)i + " on pin " + (String)SERVO_PIN_MAP[i]);

      SERVO[i] = Servo(); // Add the servo
      initializeServo(i); // Initialize the servo
    }
    else
    {
      DEBUG_PRINT("Skipping servo " + (String)i + " configuration for pin " + (String)SERVO_PIN_MAP[i]);
    }
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
    int servoPosition = SERVO[pos].read();
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