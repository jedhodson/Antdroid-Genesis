/**
 * Servos.h
 * Functions for driving and configuring servos depending on driver type
 * Supported Drivers:
 *  * Arduino Servo Library
 *  * TLC5940 16 Channel PWM Driver 
 * Set target driver in Configuration.h
 */

#ifndef SERVOS_H
#define SERVOS_H

#ifndef CONFIGURATION_H
#include "Configuration.h"
#endif

/** Store servo positions in memory */
int SERVO_POSITION[18];

/** Default wait time inbetween servo updates */
int SERVO_WAIT_TIME = SERVO_WAIT_TIME_DEFAULT;

/**********************************
 * Servo functions for onboard pwm drivers 
 **********************************/
#ifdef SERVO_DRIVER_ONBOARD
#include <Servo.h>

Servo SERVO[18];

/** Push positions to driver. Not used for SERVO_DRIVER_ONBOARD */
void servoUpdate() {}

/** 
 * Set servo to specified position 
 * Checks if servo is on SERVO_ENABLED list
 * 
 * @param servoId Index of servo in SERVO[]
 * @param pos     Absolute position to set servo
 * @param update  Push update driver. Not used in SERVO_DRIVER_ONBOARD
 */
void servoSet(int servoId, int pos, bool update)
{
    DEBUG_SERVO(servoId, pos);

    if (SERVO_ENABLED[servoId])
    {
        if (pos < 0)
            pos = 0;
        if (pos > 180)
            pos = 180;

        SERVO[servoId].write(pos);
        SERVO_POSITION[servoId] = pos;
    }
}

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

  SERVO[index].write(SERVO_INITPOS_OFFSET[index]);
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
#endif

/**********************************
 *  Servo functions for TLC5940   *
 *********************************/
#ifdef SERVO_DRIVER_TLC5940
#include "Tlc5940.h"
#include "tlc_servos.h"

/** Push positions to TLC5940 driver */
void servoUpdate()
{
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
        if (pos < 0)
            pos = 0;
        if (pos > 180)
            pos = 180;

        tlc_setServo(servoId, pos);
        SERVO_POSITION[servoId] = pos;
        if (update)
            servoUpdate();
    }
}

/** Build the servo array and initialize the servos */
void initializeServos()
{
  DEBUG_PRINT("initalizeServos() with driver TLC_5940");

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

#endif

/**********************************
 *    Common servo functions      *
 **********************************/

/**
 * Get absolute position of servo
 * 
 * @param servoId   Servo ID
 */
int getServoPositionAbsolute(int servoId)
{
    return SERVO_POSITION[servoId];
}

/**
 * Get the current servo position relative to initial
 * 
 * @param servoId   Servo ID
 */
int getServoPositionRelativeInitial(int servoId)
{
    int absolutePos = getServoPositionAbsolute(servoId);
    int newRelPos = (absolutePos - SERVO_INITPOS_OFFSET[servoId]) / SERVO_INVERTED_STATE[servoId]; // @TODO Check math on this line

    return newRelPos;
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
   Useful for bulk moving servos the same distance. 
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

    servoSetRelativeToInital(new int[1]{servoId}, 1, getServoPositionRelativeInitial(servoId), targetPos, servoWaitTime);
}

/**
 * Set a servo position relative to itself
 * 
 * @param servoId       Index of the servo
 * @param targetPos     Target position relative to self
 * @param servoWaitTime Delay between each position iteration
 */
void setSingleServoRelativeToSelf(int servoId, int targetPos, int servoWaitTime)
{
    int currentServoPos = getServoPositionRelativeInitial(servoId);
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

#endif