//CONFIG
#include "mobility.h"


unsigned char found_left;
unsigned char found_straight;
unsigned char found_right;

unsigned char dir;


void setup()
{
  delay(3000);
  Serial.begin(115200);
  Speed = 35;
  
  Wire.begin();
  delay(1000);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  //Initialisation of the long-range sensors
  pinMode(LO_RNG_B_LEFT, INPUT);
  pinMode(LO_RNG_B_RIGHT, INPUT);
  pinMode(SHARP, INPUT);

  SetSpeeds(0, 0);
  value = 0;
}

void loop()
{

    m_follow_segment();  // exit if found an intersection
    m_stop();            // stop the robot

    found_left = 0;
    found_straight = 0;
    found_right = 0;

    if(read_sharp() == 0) found_straight = 1;
    if(read_lrange_binary_left() == HIGH) found_left = 1;
    if(read_lrange_binary_right() == HIGH) found_right = 1;
    

    dir = m_select_turn(found_left, found_straight, found_right);

    if(found_right) dir = 'R';

    if(!found_left && !found_right && !found_straight)
    {
      dir = 'B';
    }


    // Make the m_turn indicated by the path.
    m_turn(dir);

    //Podjedz troche, żeby nie łapać poprzednich odczytów.
    SetSpeeds(45, 45);
    delay(700);
    


    // Store the intersection in the path variable.
    //path[path_length] = dir;
    //path_length++;

    //simplify_path();

}
