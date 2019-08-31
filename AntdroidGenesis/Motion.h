/**
   Motion.h
   Functions that make the antdroid move
*/

/** 
 * Set servo to specified position 
 * Checks if servo is on SERVO_SKIP_INITIALIZATION list
 * 
 * @param servoId Index of servo in SERVO[]
 * @param pos     Absolute position to set servo
 */
void servoSet(int servoId, int pos) {
  if (! isIntInArray(SERVO_SKIP_INITALIZATION, servoId) ) {
    SERVO[servoId].write(pos);
  }
}

/**
   Set a series of servos to the same position relative to their initial positions
   Useful for bulk moving servos the same distance

   @param _servos[]     Array of servos to set
   @param startingPos   Position to move from
   @param targetPos     The target servo position
   @param servoWaitTime Delay between each position iteration
*/
void servoSetRelativeToInital(int _servos[], int servoCount, int startingPos, int targetPos, int servoWaitTime) {
  DEBUG_PRINT("servoSetRelativeToInital()");
  
  for (int pos = startingPos; pos < targetPos; pos++) {
    for (int i = 0; i < servoCount; i++) {
      int servoId = _servos[i];
      servoSet(servoId, SERVO_INITPOS_OFFSET[servoId] + (pos * SERVO_INVERTED_STATE[servoId]));
    }

    delay(servoWaitTime);
  }
}

/**
   Overload for bulkSetRelativeToInital with SERVO_WAIT_TIME for
    servoWaitTime param
*/
void servoSetRelativeToInital(int _servos[], int servoCount, int startingPos, int targetPos) {
  servoSetRelativeToInital(_servos, servoCount, startingPos, targetPos, SERVO_WAIT_TIME);
}

/** Set all tibias to specified position */
void setFemurs(int startPos, int targetPos) {
  DEBUG_PRINT("setFemurs(" + (String)targetPos + ")");

  servoSetRelativeToInital(new int[6]{1, 4, 7, 10, 13, 16}, 6, startPos, targetPos);
}

/** Set all tibias to specified position */
void setTibias(int startPos, int targetPos) {
  DEBUG_PRINT("setTibias(" + (String)targetPos + ")");

  servoSetRelativeToInital(new int[6]{2, 5, 8, 11, 14, 17}, 6, startPos, targetPos);
}

/**
 *  Set a servo to specified position (absolute) with smoothing
 *  servoWaitTime param sets speed (lower is faster).
 *  Checks if servo is on SERVO_SKIP_INITIALIZATION list
 *  
 *  @param servoId        Index of servo in SERVO[]
 *  @param pos            Position to set
 *  @param servoWaitTime  
 */
void servoSmoothSet(int servoId, int pos, int servoWaitTime) {
  if (! isIntInArray(SERVO_SKIP_INITALIZATION, servoId) ) {
    Servo _servo = SERVO[servoId];
    
    int current = _servo.read();
    int diff = pos - current;

    if(diff > 0) {
      for(int i = current; i <= pos; i++) {
        _servo.write(i);
        delay(servoWaitTime);
      }
    } else {
      for(int i = current; i >= pos; i--) {
        _servo.write(i);
        delay(servoWaitTime);
      }
    }
  }
}

/** Overload for servoSmoothSet with SERVO_WAIT_TIME set for
 *    servoWaitTime param
 */
void servoSmoothSet(int servoId, int pos) {
  servoSmoothSet(servoId, pos, SERVO_WAIT_TIME);
}
