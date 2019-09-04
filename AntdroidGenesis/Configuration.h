/**
 * Configuration values for the Antdroid
 */

/** Default servo settings */
#define SERVO_WAIT_TIME 40    // Delay inbetween each updating the same servo
#define SERVO_SETUP_DELAY 200 // Delay between setting up each servo on inital startup

/** Servo pin map */
int SERVO_PIN_MAP[18] = {
    22, // Front  Left  Coxa
    23, // Front  Left  Femur
    24, // Front  Left  Tibia
    25, // Middle Left  Coxa
    26, // Middle Left  Femur
    27, // Middle Left  Tibia
    28, // Back   Left  Coxa
    29, // Back   Left  Femur
    30, // Back   Left  Tibia
    31, // Front  Right Coxa
    32, // Front  Right Femur
    33, // Front  Right Tibia
    34, // Middle Right Coxa
    35, // Middle Right Femur
    36, // Middle Right Tibia
    37, // Back   Right Coxa
    38, // Back   Right Femur
    39  // Back   Right Tibia
};

/** Servo inital position offsets @TODO Update initial position to be legs on ground */
int SERVO_INITPOS_OFFSET[18] = {
    90,
    48,
    77,
    127,
    52,
    115,
    130,
    10,
    100,
    90, // @TODO FIX THIS SERVO
    155,
    65,
    123,
    180,
    98,
    95,
    180,
    95
};

/** Skip initializing/writing to a servo it is listed as false */
bool SERVO_ENABLED[18] = {
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    false,  // Servo 9 always attempts to go to 0, regardless of sent position
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,
};

/** Return -1 if servo is inverted. Servos on the right are inverted */
int SERVO_INVERTED_STATE[18] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1,         // Left hand servos
    -1, -1, -1, -1, -1, -1, -1, -1, -1 // Right hand servos
};