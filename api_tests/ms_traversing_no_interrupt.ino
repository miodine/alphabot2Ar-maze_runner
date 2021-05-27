//CONFIG
#include "mobility.h"

#define COMPENSATION_LEFT 2      // sensor for path trajectory compensation - pin  
#define COMPENSATION_RIGHT 3     // sensor for path trajectory compensation - pin     

#define DETECTION_FRONT A5      //DEPRECATED - SHARP PIN
#define DETECTION_LEFT 11       //Long-range binary IR sensor input pin - left side
#define DETECTION_RIGHT 10      //Long-range binary IR sensor input pin - right side



unsigned char found_left; //DEPRECATED
unsigned char found_straight; //DEPRECATED
unsigned char found_right; //DEPRECATED

unsigned char dir; //DEPRECATED

int is_turning = 0;  //DEPRECATED
// 1 - lewo
// 2 - prawo 


int left_speed = 40; // BASE 'POWER' DELIVERED TO THE LEFT MOTOR -> FOR COMPENSATION
int right_speed = 40; // BASE  'POWER' DELIVERED TO THE RIGHT MOTOR -> FOR COMPENSATION

int readout_left = 0;
int readout_right = 0;

char readout_bottom_left = 0;
char readout_bottom_right = 0;

int delay_value = 1000;
const int delta_speed = 10; //MAX DIFFERENCE IN POWER ON THE WHEELS
const int delta_compensation = 5; //ADD/SUBTRACT VALUE IN COMPENSATION ( < delta_speed)

int IR1 = 0; // LEFT IR SENSOR
int IR2 = 0; // FRONT IR SENSOR(S)
int IR3 = 0; // RIGHT IR SENSOR

/*
* hold_at_turn assures that the robot will go straight after turning,
* so that it could safely enter the corridor that it has selected
* on intersection.
*
* The idea is - the corridor detector gives us HIGH state whenever an avalialbe 
* path is detected, so the hold_at_turn simply waits for the corridor detector
* (the binary IR sensors on the back of the platform) to give the LOW state again.
*
* @param direction - 1 (left) or 2 (right), depending on what type of turn has been performed
*/

void hold_at_turn(int direction){
  // the function is triggered AFTER the robot gets the m_forward() instruction
  // so at this time, it is driving straight
  switch(direction) 
  {
    case 1: //if the robot turned left
    {
      while(true) //enter infinite loop, block execution of anything else, and repeat the following...
      {
        compensate(); //compensate path trajectory -- NEEDS TO BE MODDED SO IT WOULD STICK TO THE RIGHT WALL!!!
        if(read_detector_left() == LOW) // if detected LOW state...
        {
          delay(200); // wait for a bit to make sure it wasn't noise
          if(read_detector_left() == LOW) break; // if not, then exit loop, which results in exiting from the function
        } 
      }
    }
    break;

    case 2: //if the robot turned right -- do the same thing, but waiting for readout from the other sensor xD
    {
      while(true) 
      {
        compensate();
        if(read_detector_right() == LOW) 
        {
          delay(200);
          if(read_detector_right() == LOW) break;
        }
      }
    }
    break;
  }


}

/*
* compensator = path trajectory compensation sensor (triggered LOW if detected)
*/
int read_compensator_left(){   
  return digitalRead(COMPENSATION_LEFT);
}


/*
* compensator = path trajectory compensation sensor (triggered LOW if detected; short range IR)
* -- right side
*/
int read_compensator_right(){
  return digitalRead(COMPENSATION_RIGHT);
}

/*
* compensator = path trajectory compensation sensor (triggered LOW if detected; short range IR)
* -- left side
*/
int read_detector_left(){
    return digitalRead(DETECTION_LEFT);
}


/*
* detector = corridor detection sensor (long range IR)
* --right side
*/
int read_detector_right(){
    return digitalRead(DETECTION_RIGHT);
}


/*
* compensator = path trajectory compensation sensor (triggered LOW if detected)
*/
int read_detector_front()
{
  if(read_infrared(false) == 'N') return 1; // if no sensor detects an obstacle, return 1
  else return 0;
}


/*
* function for path trajectory compensation, for driving straight -  
* it is based on short range IR sensors, and its operation
* can be equated to 'n-position relay' - depending on the delta_compensation
* and delta_speed parameters (eg if for delta_compensation = 5, and delta_speed = 15 -> the controller
* is 3 position relay).  
* TODO: improve, adjust, and utilise callback
*/

void compensate() {
    //get readouts from the sensors
    readout_left = read_compensator_left(); 
    readout_right = read_compensator_right(); 
    readout_bottom_left = read_infrared(false);


    // do nothing if both sensors detect wall - for now xd
    if((readout_left == LOW) && (readout_right == LOW))
    {
      SetSpeeds(Speed, Speed);
      return;
    }

    // if detected wall from the left:
    if(readout_left == LOW)
    {
      // if the maximal allowed values do not exceed the constrains,
      // increase the power delivered to the left wheel, 
      // while decreasing the power delivered to the right wheel.

      if(left_speed <(Speed+delta_speed)) left_speed += delta_compensation;
      if(right_speed >(Speed -delta_speed)) right_speed -= delta_compensation;
    }
    
    else if(readout_right == LOW) // if detected wall from the right:
    {
      // same thing, but increase right, decrease left
      if(left_speed >(Speed - delta_speed)) left_speed -= delta_compensation;
      if(right_speed <(Speed +delta_speed)) right_speed += delta_compensation;
    }

    else // if none of the sensors detect wall:
    {
      //set default speed
      left_speed = Speed;
      right_speed = Speed;
    }

    SetSpeeds(left_speed, right_speed);


}

void serial_print(int value){
	switch(value)
	{
	  case 1:
        Serial.println("forward");break; 
      case 2:
        Serial.println("left"); break;
      case 3:
        Serial.println("rigth");break; 
      case 4:
        Serial.println("T intersection");break;
      case 5:
        Serial.println("left T intersection");break;
	  case 6:
        Serial.println("right T intersection");break;
	  case 7:
        Serial.println("dead end");break;
	  case 8:
        Serial.println("maze end");break;
	  case 9:
        Serial.println("fake dead end");break;	
      default :
        Serial.println("unknow\n");
	}
}


void setup()
{

  //init
  delay(3000);
  Serial.begin(115200);
  Wire.begin();


  Speed = 40;
  
  Wire.begin();
  delay(1000);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  //Initialisation of the long-range sensors
  pinMode(DETECTION_LEFT, INPUT);    
  pinMode(DETECTION_RIGHT, INPUT);   
  pinMode(DETECTION_FRONT, INPUT);    

  pinMode(COMPENSATION_LEFT, INPUT);
  pinMode(COMPENSATION_RIGHT, INPUT);

  SetSpeeds(0, 0);
  value = 0;
}

void loop()
{

  //read values of th sensors 
  IR1 = read_detector_left();
  IR2 = read_detector_front();
  IR3 = read_detector_right();


  //decide what to do:
  
  if (IR1 == LOW && IR2 == HIGH && IR3 == LOW)//Straight path
    {
    compensate();
    m_forward();
    
    //serial_print(1);
    }

  compensate();

  if (IR1 == HIGH && IR2 == LOW && IR3 == LOW)//Left turn
    {
    //serial_print(2);
    
    m_ninety_left();
    m_forward();
    hold_at_turn(1);

    }

  compensate();

  if (IR1 == LOW && IR2 == LOW && IR3 == HIGH)//Right Turn
    {
      //serial_print(3);
      compensate();
      m_ninety_right();
      m_forward();
      compensate();
      hold_at_turn(2);
      m_forward();
      compensate();
    }

  compensate();

  if (IR1 == HIGH && IR2 == LOW && IR3 == HIGH)//T Intersection
    {
      //serial_print(4);
      m_ninety_left(); // As left is possible
      compensate();
      m_forward();
      compensate();
      hold_at_turn(1);
      m_forward();
    }

  compensate();

  if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW)//Left T Intersection
    {
      m_ninety_left();// As Left is possible
      compensate();
      m_forward();
      compensate();
      hold_at_turn(1);
      m_forward();
    }

  compensate();

  if (IR1 == LOW && IR2 == HIGH && IR3 == HIGH)//Right T Tntersection
    {
     m_forward();//As Straight path is possible
    compensate();
     hold_at_turn(2);
    }

  compensate();

  if (IR1 == LOW && IR2 ==LOW && IR3 == LOW)//Dead End
    {
     m_ninety_left(); //As no other direction is possible
     m_ninety_left();
     
     compensate();
     m_forward();
     compensate();
      
    }

  compensate();

  if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
    {
     m_forward();
      compensate();
     delay((int)0.5*delay_value);
      compensate();
     m_stop();

     if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
        {
          m_stop();
        }
     else
        {
         m_ninety_left();
         m_forward();
         compensate();
         hold_at_turn(1);
        m_forward();
        }
    }
}



