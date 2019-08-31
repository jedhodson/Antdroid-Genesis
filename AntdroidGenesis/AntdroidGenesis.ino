/**
   Antdroid Genesis
   A nicer interface for controlling the Antdroid

   Coxa   - Servo that connects leg to body 'the first or basal segment of the leg of an insect'
   Femur  - Middle leg servo
   Tibia  - Lower leg (the blue legs)
*/

#include <Servo.h>
Servo SERVO[18];

#include "Debug.h"
#include "Configuration.h"
#include "Helpers.h"
#include "Motion.h"
#include "Motions.h"

/**
    Initialize Servo
    Attach servo to pin if no already attached and set send to initial position

    @param  index   Index of the servo in the SERVO array
*/
void initializeServo(int index) {
  if (! SERVO[index].attached()) SERVO[index].attach(SERVO_PIN_MAP[index]);

  SERVO[index].write(SERVO_INITPOS_OFFSET[index]);
}

/** Build the servo array and initialize the servos */
void initializeServos() {
  DEBUG_PRINT("initalizeServos()");

  for (int i = 0; i < 18; i ++) {
    if (! isIntInArray(SERVO_SKIP_INITALIZATION, i) ) {
      DEBUG_PRINT("Configuring servo " + (String)i + " on pin " + (String)SERVO_PIN_MAP[i]);

      SERVO[i] = Servo();   // Add the servo
      initializeServo(i);    // Initialize the servo
    } else {
      DEBUG_PRINT("Skipping servo " + (String)i + " configuration for pin " + (String)SERVO_PIN_MAP[i]);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Antdroid starting...");

  initializeServos();

  /** DO STUFF */
  MotionPrepareForStand();
  delay(1000);

  MotionPushUpright();
}

void loop() {
  // Wait for serial commands to execute
  while(Serial.available() > 0 ) {
    String str = Serial.readString();
    setCommand(str);
  }
}

void setCommand(String input) {
  String _servo = getSplitString(input, ',', 0);
  String _pos   = getSplitString(input, ',', 1);

  int pos   = _pos.toInt();

  Serial.println("SERVO[" + _servo + "], pos: " + (String)pos);

  if(_servo == "t") {
    setTibias(0, pos);
  } 
  else if(_servo == "f") {
    setFemurs(0, pos);
  } 
  else {
    int servo = _servo.toInt();
    if(servo >= 0 && servo < 18) {
      DEBUG_PRINT("Setting servo " + (String)servo + " to position " + (String)pos);
      setServoRelativeToInitialBasic(servo, pos, SERVO_WAIT_TIME);
    }
  }
}

void setServoRelativeToInitialBasic(int servoId, int pos, int servoWaitTime) {
  DEBUG_PRINT("setServoRelativeToInitalBasic()");
  
  int currentPos  = SERVO[servoId].read();
  int posDiff     = pos - currentPos;

  if(posDiff > 0) {
    for(int newPos = currentPos; newPos <= pos; newPos++) {
      SERVO[servoId].write(SERVO_INITPOS_OFFSET[servoId] + (newPos * SERVO_INVERTED_STATE[servoId]));
      delay(servoWaitTime);
    }
  } else {
    for(int newPos = currentPos; newPos >= pos; newPos--) {
      SERVO[servoId].write(SERVO_INITPOS_OFFSET[servoId] + (newPos * SERVO_INVERTED_STATE[servoId]));
      delay(servoWaitTime);
    }
  }
}
