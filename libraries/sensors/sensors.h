#ifndef SENSORS_H
#define SENSORS_H

#include "alphabot2_ar.h"
/*
 * Write to the PCF8574 I/O expansion module.
 *
 */
void PCF8574Write(byte data);

/*
 * Read from the PCF8574 I/O expansion module.
 *
 */
byte PCF8574Read();

/*
 * Read the front-facing infrared sensors for obstacle detection.
 *
 * @param boolean value (true/false).
 * @return char value represnts which sensor/s is/are active
 *      'L' - left, 'R' - right, 'B' - both, 'N' - none.
 */
char read_infrared(bool verbose=false);

/*
 * Read the ultrasonic sensor value.
 *
 * @param boolean value (true/false).
 * @return approximate distance to obtacle [cm]
 */
int read_ultrasonic(bool verbose = false);

#endif // SENSORS_H
