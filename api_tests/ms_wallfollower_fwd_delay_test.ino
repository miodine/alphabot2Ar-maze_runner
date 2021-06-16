#include "mobility.h"

#define COMPENSATION_LEFT A4 // sensor for path trajectory compensation analog value

#define DETECTION_LEFT 11 // binary IR sensor right
#define DETECTION_FRONT 2  // binary IR sensor front


#define WALL_TOO_FAR   500
#define WALL_TOO_CLOSE 700 //ADJUST!!

int left_speed = 40; // BASE 'POWER' DELIVERED TO THE LEFT MOTOR -> FOR COMPENSATION
int right_speed = 40; // BASE  'POWER' DELIVERED TO THE RIGHT MOTOR -> FOR COMPENSATION


int readout_left_compensate = 0;


bool iw_left = false;              // iw = is wall (present) -> readout memory placeholder
bool iw_front = false;             // same as above
bool just_turned_left = false;     // state marker - if present move was to turn left?

int delay_value = 1000;
const int delta_speed = 10;       //MAX DIFFERENCE IN POWER ON THE WHEELS
const int delta_compensation = 1; //ADD/SUBTRACT VALUE IN COMPENSATION ( < delta_speed)



bool is_wall_left(){
    if(digitalRead(DETECTION_LEFT)==0) return true;
    else return false;
}

bool is_wall_front(){
    if(digitalRead(DETECTION_FRONT)==0) return true;
    else return false;
}


int read_compensator_left(){
  return analogRead(COMPENSATION_LEFT);
}


void setup()
{
  Speed = 40;

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



  SetSpeeds(0, 0);
  value = 0;
}


void loop()
{

iw_left = read_detector_left();              
iw_front = read_detector_front();

if(!iw_left) // if no wall on the left
{
  delay(300); //keep driving forward for a couple of minutes
  m_stop(); 
  m_ninety_left(); // turn left

  m_forward();
  while(!read_detector_front() && !read_detector_left()){}

}
else
{
    m_forward();
}