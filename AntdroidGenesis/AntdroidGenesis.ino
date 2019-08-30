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
  initializeServos();

  /** DO STUFF */
  MotionPrepareForStand();
  delay(1000);

  MotionPushUpright();
}

void loop() {

}
