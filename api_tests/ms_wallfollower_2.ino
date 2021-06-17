#include "mobility.h"

#define COMPENSATION_LEFT A4 // sensor for path trajectory compensation analog value

#define DETECTION_LEFT 12 // binary IR sensor right
#define DETECTION_FRONT 3  // binary IR sensor front
#define COMPENSATION_CONDITIONAL_LEFT 4

#define WALL_TOO_FAR   600
#define WALL_TOO_CLOSE 400 //ADJUST!!

int left_speed = 20; // BASE 'POWER' DELIVERED TO THE LEFT MOTOR -> FOR COMPENSATION
int right_speed = 20; // BASE  'POWER' DELIVERED TO THE RIGHT MOTOR -> FOR COMPENSATION


int readout_left_compensate = 0;


bool iw_left = false;              // iw = is wall (present) -> readout memory placeholder
bool iw_front = false;             // same as above

int delay_value = 80;
const int delta_speed = 10;       //MAX DIFFERENCE IN POWER ON THE WHEELS
const int delta_compensation = 1; //ADD/SUBTRACT VALUE IN COMPENSATION ( < delta_speed)




unsigned long current_time = 0;
unsigned long prev_time = 0;
unsigned long stuck_interval = 1000;

char stuck_marker = 0;
bool stuck_flag = false;


bool is_wall_left_back(){
    if(digitalRead(DETECTION_LEFT)==0) return true;
    else return false;
}

bool is_wall_left_front()
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
    
    if(is_wall_left_front())
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
  Speed = 40; //PE: possible lag - change if necessary

  delay(3000);
  Serial.begin(115200);
  Wire.begin();

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

  iw_left = is_wall_left_back();              
  iw_front = is_wall_front();

  if(!iw_left)
  {
  delay(delay_value); //keep driving forward for a while
  m_stop(); 
  m_ninety_left();    // turn left

  SetSpeeds(Speed,Speed);
  while(!is_wall_left_back())
  {
   if(is_wall_left_front())
   {
     compensate();
     SetSpeeds(left_speed,right_speed);
   }
  }
}

if(iw_left)
{
  if(iw_front)
  {
    m_stop();
    m_ninety_right();
    m_stop();
  }
  else
  {
    compensate();
    SetSpeeds(left_speed,right_speed);
    

    // read state of the front bottom sensors
    stuck_marker = read_infrared(false);
    
    // if while going straight - either left or right
    // detects a signal 
    if((stuck_marker == 'L') || (stuck_marker == 'R')){
        
        // mark current time 
        if(!stuck_flag) prev_time = millis();
        
        // mark that the robot is stuck
        stuck_flag = true;

        //get current time
        current_time = millis();


        // if the difference between time when it became stuck
        // to the current time is greater than the predefined interval
        if(current_time - prev_time >= stuck_interval)
        {
            // it means that it is acutally stuck and needs to drive back xd
            m_backward();
            delay(500);
            stuck_flag = false;
        }
    }

    // if during that waiting process, the signal disappears
    // set the stuck flag to false, because it means that it is not stuck
    if(stuck_marker == 'N') stuck_flag = false;

  }
}

}

