/** Debug stuff */
#define DEBUG
//#define DEBUG_SERVO_SIGNAL

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x) 
#endif

#ifdef DEBUG_SERVO_SIGNAL
#define DEBUG_SERVO(servo, position) Serial.println("servoSet(" + (String)servo + ", " + (String)position + ")")
#else
#define DEBUG_SERVO(servo, position) 
#endif