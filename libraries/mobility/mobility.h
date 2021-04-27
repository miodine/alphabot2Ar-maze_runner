#ifndef MOBILITY_H
#define MOBILITY_H

#include "sensors.h"
/*
 * Function for setting speed.
 *
 * @param Aspeed for Left Motor, Bspeed for Right Motor.
 */
void SetSpeeds(int Aspeed, int Bspeed);

void m_forward();   // move forward
void m_backward();  // move backward
void m_right();     // move left
void m_left();      // move right
void m_stop();      // stop movement
void m_ninety_right();  // turn 90 degrees right
void m_ninety_left();   // turn 90 degrees left

/*
 * Function for causing the alphabot to follow a segment of the maze until
 * it detects an intersection, a dead end, or the finish.
 */
void m_follow_segment();

/*
 * Function for performing various types of turns according to the parameter dir.
 * The delays here had to be calibrated for the alphabot's motors.
 *
 * @param 'L' (left), 'R' (right), 'S' (straight), or 'B' (back).
 */
void m_turn(unsigned char dir);

/*
 * Function for deciding which way to m_turn during the learning phase of maze solving.
 *
 * @param variables found_left, found_straight, and found_right,
 *      which indicate whether there is an exit in each of the three directions,
 *      applying the "left hand on the wall" strategy.
 * @return 'L' (left), 'R' (right), 'S' (straight), or 'B' (back).
 */
unsigned char m_select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right);

#endif // MOBILITY_H
