#include "mobility.h"

#define COMPENSATION_LEFT A4 // sensor for path trajectory compensation analog value

#define DETECTION_LEFT 12 // binary IR sensor right
#define DETECTION_FRONT 3  // binary IR sensor front
#define COMPENSATION_CONDITIONAL_LEFT 4

#define WALL_TOO_FAR   600
#define WALL_TOO_CLOSE 400 //ADJUST!!

int left_speed = 30; // BASE 'POWER' DELIVERED TO THE LEFT MOTOR -> FOR COMPENSATION
int right_speed = 30; // BASE  'POWER' DELIVERED TO THE RIGHT MOTOR -> FOR COMPENSATION


int readout_left_compensate = 0;


bool iw_left = false;              // iw = is wall (present) -> readout memory placeholder
bool iw_front = false;             // same as above
bool just_turned_left = false;     // state marker - if present move was to turn left?

int delay_value = 1000;
const int delta_speed = 10;       //MAX DIFFERENCE IN POWER ON THE WHEELS
const int delta_compensation = 2; //ADD/SUBTRACT VALUE IN COMPENSATION ( < delta_speed)



bool is_wall_left(){
    if(digitalRead(DETECTION_LEFT)==0) return true;
    else return false;
}

bool wall_just_dissapeared()
{
  if(digitalRead(COMPENSATION_CONDITIONAL_LEFT)==0) return true;
    else return false; 
}

bool is_wall_front(){
    if(digitalRead(DETECTION_FRONT)==0) return true;
    else return false;
}


int read_compensator_left(){
  return analogRead(COMPENSATION_LEFT);
}


void compensate() {
    readout_left_compensate = read_compensator_left(); 
    
    if(wall_just_dissapeared())
    {
    
    if(readout_left_compensate > WALL_TOO_FAR) 
    {
      if(left_speed >(Speed - delta_speed)) left_speed -= delta_compensation;
      if(right_speed <(Speed +delta_speed)) right_speed += delta_compensation;

    }
    
    else if(readout_left_compensate < WALL_TOO_CLOSE) 
    {

      if(left_speed <(Speed+delta_speed)) left_speed += delta_compensation;
      if(right_speed >(Speed -delta_speed)) right_speed -= delta_compensation;
    }

    else 
    {
      left_speed = Speed;
      right_speed = Speed;
    }
    }
    
}

void setup()
{
  Speed = 30;

  delay(3000);
  Serial.begin(115200);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);


  pinMode(DETECTION_LEFT, INPUT);   
  pinMode(DETECTION_FRONT, INPUT);    
  pinMode(COMPENSATION_LEFT, INPUT);
  pinMode(COMPENSATION_CONDITIONAL_LEFT,INPUT);
  
  SetSpeeds(0, 0);
  value = 0;
}

void loop()
{
    compensate();
    SetSpeeds(left_speed, right_speed);

}