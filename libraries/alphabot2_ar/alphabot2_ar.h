#ifndef ALPHABOT2_AR_H
#define ALPHABOT2_AR_H

#include <Adafruit_SSD1306.h>
#include <TRSensors.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define ECHO 2          // ultrasonic echo
#define TRIG 3          // ultrasonic trigger

#define SHARP 3

#define OLED_RESET 9    // OLED display reset pin
#define OLED_SA0 8      // OLED display set pin
#define PWMA 6          //Left Motor Speed pin (ENA)
#define AIN2 A0         //Motor-L forward (IN2).
#define AIN1 A1         //Motor-L backward (IN1)
#define PWMB 5          //Right Motor Speed pin (ENB)
#define BIN1 A2         //Motor-R forward (IN3)
#define BIN2 A3         //Motor-R backward (IN4)
#define PIN 7           //LED pin

#define NUM_SENSORS 5   //IR bottom sensors
#define OLED_RESET 9
#define OLED_SA0 8
#define Addr 0x20

#define SH_RNG_ADC_LEFT A5    //Short-range analogue IR sensor input pin - left side 
#define SH_RNG_ADC_RIGHT A4   //Short-range analogue IR sensor input pin - right side
#define LO_RNG_B_LEFT 11       //Long-range binary IR sensor input pin - left side
#define LO_RNG_B_RIGHT 10      //Long-range binary IR sensor input pin - right side



// compensation




extern Adafruit_NeoPixel RGB;
extern Adafruit_SSD1306 display;

extern TRSensors trs;               // declare line sensors as TRSensor type object
extern unsigned int sensorValues[NUM_SENSORS];      // declare global array for line sensor values

extern unsigned long lasttime;
extern unsigned int position;
extern uint16_t i, j;
extern byte value;




// The path variable will store the path that the robot has taken.  It
// is stored as an array of characters, each of which represents the
// m_turn that should be made at one intersection in the sequence:
//  'L' for left
//  'R' for right
//  'S' for straight (going straight through an intersection)
//  'B' for back (U-m_turn)
//
// Whenever the robot makes a U-m_turn, the path can be simplified by
// removing the dead end.  The follow_next_turn() function checks for
// this case every time it makes a m_turn, and it simplifies the path
// appropriately.
extern char path[100];
extern unsigned char path_length;
extern const int fs_turn_rate;
extern int Speed;

/*
 * Function for sorting arrays.
 *
 * @param Pointer to an array, length.
 */
void sort(int *arr, int n);

/*
 * Function for setting a color for RGB diodes.
 *
 * @param Four integer arrays containing three values for R, G and B.
 */
void lights(int led_1[3], int led_2[3], int led_3[3], int led_4[3]);


/*
 * Path simplification.
 * Strategy: a sequence xBx can be simplified by cutting out the dead end.
 * E.g.: LBL -> S, because a single S bypasses the dead end represented by LBL.
 */
void simplify_path();


#endif // ALPHABOT2_AR_H
