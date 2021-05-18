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

/*
* Read the infrared short range sensor value.
*
*/
int read_srange_analog_left();

/*
* Read the infrared short range sensor value.
*
*/
int read_srange_analog_right();

/*
* Read the infrared short range sensor value.
*
*/
int read_lrange_binary_left();

/*
* Read the infrared long range sensor value.
*
*/
int read_lrange_binary_right();

/*
* Detect obstacles using binary IR sensors
* 
* @return int - 1 if detected obstacle, 0 otherwise
*/

int read_sharp();

#endif // SENSORS_H
