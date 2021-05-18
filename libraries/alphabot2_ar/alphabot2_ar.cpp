#include "alphabot2_ar.h"

//Adafruit_NeoPixel RGB = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800); // setup RGB LEDs
//Adafruit_SSD1306 display(OLED_RESET, OLED_SA0);

//TRSensors trs = TRSensors();                 // declare line sensors as TRSensor type object
//unsigned int sensorValues[NUM_SENSORS];      // declare global array for line sensor values




unsigned long lasttime = 0;     // initial value for measuring passed time [ms]
unsigned int position;
uint16_t i, j;
byte value;

char path[100] = "";            // array for remembering the path
unsigned char path_length = 0;  // initial value for the length of the path

int Speed = 30;

void sort(int *arr, int n){     // function for sorting arrays
    int tmp;
    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            if (*(arr + j) < *(arr + i)){
                tmp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = tmp;
            }
            else{
                continue;
            }
        }
    }
}

void lights(int led_1[3], int led_2[3], int led_3[3], int led_4[3]){
  RGB.begin();

  RGB.setPixelColor(0, RGB.Color(led_1[0], led_1[1], led_1[2])); // set colour of first LED
  RGB.setPixelColor(1, RGB.Color(led_2[0], led_2[1], led_2[2])); // set colour of second LED
  RGB.setPixelColor(2, RGB.Color(led_3[0], led_3[1], led_3[2])); // set colour of third LED
  RGB.setPixelColor(3, RGB.Color(led_4[0], led_4[1], led_4[2])); // set colour of fourth LED

  RGB.show(); // m_turn on LEDs
}


void simplify_path(){


  // only simplify the path if the second-to-last m_turn was a 'B'
  if (path_length < 3 || path[path_length - 2] != 'B')
    return;

  int total_angle = 0;
  int i;
  for (i = 1; i <= 3; i++)
  {
    switch (path[path_length - i])
    {
    case 'R':
      total_angle += 90;
      break;
    case 'L':
      total_angle += 270;
      break;
    case 'B':
      total_angle += 180;
      break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  total_angle = total_angle % 360;

  // Replace all of those turns with a single one.
  switch (total_angle)
  {
  case 0:
    path[path_length - 3] = 'S';
    break;
  case 90:
    path[path_length - 3] = 'R';
    break;
  case 180:
    path[path_length - 3] = 'B';
    break;
  case 270:
    path[path_length - 3] = 'L';
    break;
  }

  // The path is now two steps shorter.
  path_length -= 2;
}
