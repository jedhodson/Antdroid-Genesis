/**
   Motion.h
   Functions that make the antdroid move
*/

#ifndef MOTION_H
#define MOTION_H

/** Last set position of all femures */
int allFemureLastPos = 0;

/**
 * Set all femure to specified position, relative to inital
 * 
 * @param startPos  Position to move servos from
 * @param targetPos New target position
 */
void setFemurs(int startPos, int targetPos)
{
  DEBUG_PRINT("setFemurs(" + (String)targetPos + ")");

  servoSetRelativeToInital(new int[6]{1, 4, 7, 10, 13, 16}, 6, startPos, targetPos);
  allFemureLastPos = targetPos;
}

/** 
 * Set all femures to specified position, relative to inital
 * Uses last set position of all femures as startPos
 * 
 * @param targetPos New target position
*/
void setFemurs(int targetPos)
{
  setFemurs(allFemureLastPos, targetPos);
}

/** Last set position of all tibias */
int allTibiaLastPos = 0;

/**
 * Set all tibias to specified position, relative to inital
 * 
 * @param startPos  Position to move servos from
 * @param targetPos New target position
 */
void setTibias(int startPos, int targetPos)
{
  DEBUG_PRINT("setTibias(" + (String)targetPos + ")");

  servoSetRelativeToInital(new int[6]{2, 5, 8, 11, 14, 17}, 6, startPos, targetPos);
  allTibiaLastPos = targetPos;
}

/** 
 * Set all tibias to specified position, relative to inital
 * Uses last set position of all tibias as startPos
 * 
 * @param targetPos New target position
*/
void setTibias(int targetPos)
{
  setTibias(allTibiaLastPos, targetPos);
}

#endif