/**
 * Motions.h
 * Motion functions that can be called from event or in loop
 */

#ifndef MOTIONS_H
#define MOTIONS_H

/** Move outer legs out in preparation for a stand */
void MotionPrepareForStand()
{
  DEBUG_PRINT("MotionPrepareForStand()");

  servoSmoothSet(0, 150);
  servoSmoothSet(6, 100);
  servoSmoothSet(9, 0); // BAD SERVO, SHOULD BE IN SKIP LIST
  servoSmoothSet(15, 130);
}

/** All legs touch the ground. Not suitable for standing */
void MotionTouchGround()
{
  DEBUG_PRINT("MotionTouchGround()");

  setTibias(15);
  setFemurs(20);
}

/** All legs touch the ground. Suitable for standing */
void MotionUpTouchGround()
{
  DEBUG_PRINT("MotionUpTouchGround()");

  setTibias(40);
}

/** Touch the ground, then move Femurs to push off the ground. 
      Needs coxa positions to be set prior to calling 
*/
void MotionPushUpright()
{
  DEBUG_PRINT("MotionPushUpright()");

  MotionUpTouchGround();
  setFemurs(25);
}

#endif