/**
   Motion.h
   Functions that make the antdroid move
*/

/** Storage of servo position */
int SERVO_POSITION[18];

/**
 * Push update to the TLC5940
 */
void servoUpdate() {
  #ifdef DEBUG_SERVO_SIGNAL
  DEBUG_PRINT("servoUpdate()");
  #endif
  Tlc.update();
}

/** 
 * Set servo to specified position 
 * Checks if servo is on SERVO_ENABLED list
 * 
 * @param servoId Index of servo in SERVO[]
 * @param pos     Absolute position to set servo
 * @param update  Push update to TLC. If not called, it will need to be manually called
 */
void servoSet(int servoId, int pos, bool update)
{
  DEBUG_SERVO(servoId, pos);

  if (SERVO_ENABLED[servoId])
  {
    if(pos < 0) pos = 0;
    if(pos > 180) pos = 180;

    tlc_setServo(servoId, pos);
    SERVO_POSITION[servoId] = pos;
    if(update) servoUpdate();
  }
}

/**
   Set a series of servos to the same position relative to their initial positions
   Useful for bulk moving servos the same distance

   @param _servos[]     Array of servos to set
   @param startingPos   Position to move from
   @param targetPos     The target servo position
   @param servoWaitTime Delay between each position iteration
   @param servoInvertedState  Array of servo inverted states
*/
void servoSetRelativeToInital(int _servos[], int servoCount, int startingPos, int targetPos, int servoWaitTime, int servoInvertedState[])
{
  DEBUG_PRINT("servoSetRelativeToInital()");

  int posDiff = targetPos - startingPos;

  if (posDiff > 0)
  {
    for (int pos = startingPos; pos <= targetPos; pos++)
    {
      for (int i = 0; i < servoCount; i++)
      {
        int servoId = _servos[i];
        servoSet(servoId, SERVO_INITPOS_OFFSET[servoId] + (pos * servoInvertedState[servoId]), false);
      }
      servoUpdate();

      delay(servoWaitTime);
    }
  }
  else
  {
    for (int pos = startingPos; pos >= targetPos; pos--)
    {
      for (int i = 0; i < servoCount; i++)
      {
        int servoId = _servos[i];
        servoSet(servoId, SERVO_INITPOS_OFFSET[servoId] + (pos * servoInvertedState[servoId]), false);
      }
      servoUpdate();

      delay(servoWaitTime);
    }
  }
}

/**
   Set a series of servos to the same position relative to their initial positions
   Useful for bulk moving servos the same distance. Defaults with SERVO_INVERTED_STATE
   for param servoInvertedState[]

   @param _servos[]     Array of servos to set
   @param startingPos   Position to move from
   @param targetPos     The target servo position
   @param servoWaitTime Delay between each position iteration
*/
void servoSetRelativeToInital(int _servos[], int servoCount, int startingPos, int targetPos, int servoWaitTime)
{
  servoSetRelativeToInital(_servos, servoCount, startingPos, targetPos, servoWaitTime, SERVO_INVERTED_STATE);
}

/**
   Overload for bulkSetRelativeToInital with SERVO_WAIT_TIME for
    servoWaitTime param
*/
void servoSetRelativeToInital(int _servos[], int servoCount, int startingPos, int targetPos)
{
  servoSetRelativeToInital(_servos, servoCount, startingPos, targetPos, SERVO_WAIT_TIME);
}

/**
 * Set a servo position relative to inital
 * 
 * @param servoId       Index of the servo
 * @param targetPos     Target position
 * @param servoWaitTime Delay between each position iteration
*/
void setSingleServoRelativeToInitial(int servoId, int targetPos, int servoWaitTime)
{
  DEBUG_PRINT("setSingleServoRelativeToInitial(" + (String)servoId + ", " + (String)targetPos + ", " + (String)servoWaitTime);

  int currentServoPos = SERVO_POSITION[servoId];   /** @TODO Servo resets to 0 before moving to target position */

  servoSetRelativeToInital(new int[1]{servoId}, 1, currentServoPos, targetPos, servoWaitTime);
}

/**
 * Set a servo position relative to itself
 * 
 * @param servoId       Index of the servo
 * @param targetPos     Target position relative to self
 * @param servoWaitTime Delay between each position iteration
 */
void setSingleServoRelativeToSelf(int servoId, int targetPos, int servoWaitTime) {
  int currentServoPos = SERVO_POSITION[servoId];
  targetPos += currentServoPos;

  servoSetRelativeToInital(new int[1]{servoId}, 1, currentServoPos, targetPos, servoWaitTime);
}

/**
 * Overload for setSingleServoRelativeToInitial
 * Uses SERVO_WAIT_TIME as servoWaitTime param
 * 
 * @param servoId       Index of the servo
 * @param targetPos     Target position
 */
void setSingleServoRelativeToInitial(int servoId, int targetPos)
{
  setSingleServoRelativeToInitial(servoId, targetPos, SERVO_WAIT_TIME);
}

/** Set all femures to specified position */
int allFemureLastPos = 0;

void setFemurs(int startPos, int targetPos)
{
  DEBUG_PRINT("setFemurs(" + (String)targetPos + ")");

  servoSetRelativeToInital(new int[6]{1, 4, 7, 10, 13, 16}, 6, startPos, targetPos);
  allFemureLastPos = targetPos;
}

void setFemurs(int targetPos)
{
  setFemurs(allFemureLastPos, targetPos);
}

/** Set all tibias to specified position */
int allTibiaLastPos = 0;

void setTibias(int startPos, int targetPos)
{
  DEBUG_PRINT("setTibias(" + (String)targetPos + ")");

  servoSetRelativeToInital(new int[6]{2, 5, 8, 11, 14, 17}, 6, startPos, targetPos);
  allTibiaLastPos = targetPos;
}

void setTibias(int targetPos)
{
  setTibias(allTibiaLastPos, targetPos);
}

/**
 *  Set a servo to specified position (absolute) with smoothing
 *  servoWaitTime param sets speed (lower is faster).
 *  Checks if servo is on SERVO_ENABLED list
 *  
 *  @param servoId        Index of servo in SERVO[]
 *  @param pos            Position to set
 *  @param servoWaitTime  
 */
void servoSmoothSet(int servoId, int pos, int servoWaitTime)
{
  if (SERVO_ENABLED[servoId])
  {
    int current = SERVO_POSITION[servoId];
    int diff = pos - current;

    if (diff > 0)
    {
      for (int i = current; i <= pos; i++)
      {
        servoSet(servoId, i, true);
        delay(servoWaitTime);
      }
    }
    else
    {
      for (int i = current; i >= pos; i--)
      {
        servoSet(servoId, i, true);
        delay(servoWaitTime);
      }
    }
  }
}

/** Overload for servoSmoothSet with SERVO_WAIT_TIME set for
 *    servoWaitTime param
 */
void servoSmoothSet(int servoId, int pos)
{
  servoSmoothSet(servoId, pos, SERVO_WAIT_TIME);
}
