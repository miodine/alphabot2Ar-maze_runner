//CONFIG
#include "mobility.h"

#define COMPENSATION_LEFT 2      // sensor for path trajectory compensation - pin  
#define COMPENSATION_RIGHT 3     // sensor for path trajectory compensation - pin     

#define DETECTION_FRONT A5      //DEPRECATED - SHARP PIN
#define DETECTION_LEFT 11       //Long-range binary IR sensor input pin - left side
#define DETECTION_RIGHT 10      //Long-range binary IR sensor input pin - right side



unsigned char found_left;
unsigned char found_straight;
unsigned char found_right;

unsigned char dir;

bool is_turning = false;


volatile int left_speed = 40;
volatile int right_speed = 40;
volatile int readout_left = 0;
volatile int readout_right = 0;


char readout_bottom_left = 0;
char readout_bottom_right = 0;

int delay_value = 1000;


int IR1 = 0;
int IR2 = 0;
int IR3 = 0;




int read_compensator_left(){
  return digitalRead(COMPENSATION_LEFT);
}

int read_compensator_right(){
  return digitalRead(COMPENSATION_RIGHT);
}

int read_detector_left(){
    return digitalRead(DETECTION_LEFT);
}

int read_detector_right(){
    return digitalRead(DETECTION_RIGHT);
}

int read_detector_front()
{
  if( read_infrared(false) == 'N') return 1; // if no sensor detects an obstacle, return 1
  else return 0;
}

void compensate_left()
{
  if(!is_turning)
  {
    if(left_speed <(Speed+10)) left_speed += 5;
    if(right_speed >(Speed -10)) right_speed -= 5;
    SetSpeeds(left_speed, right_speed);
  }
}

void compensate_right(){
  if(!is_turning)
  {
    if(left_speed >(Speed - 10)) left_speed -= 5;
    if(right_speed <(Speed +10)) right_speed += 5;
    SetSpeeds(left_speed, right_speed);
  }

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


  attachInterrupt(digitalPinToInterrupt(COMPENSATION_LEFT), compensate_left, FALLING);
  attachInterrupt(digitalPinToInterrupt(COMPENSATION_RIGHT), compensate_right, FALLING);
}

void loop()
{
  IR1 = read_detector_left();
  IR2 = read_detector_front();
  IR3 = read_detector_right();

  if (IR1 == LOW && IR2 == HIGH && IR3 == LOW)//Straight path
    {
    m_forward();
    //serial_print(1);
    }

  if (IR1 == HIGH && IR2 == LOW && IR3 == LOW)//Left turn
    {
    //serial_print(2);
    is_turning = true;
    m_ninety_left();
    is_turning = false;
    m_forward();
    delay(delay_value);
    }

  if (IR1 == LOW && IR2 == LOW && IR3 == HIGH)//Right Turn
    {
      //serial_print(3);
      is_turning = true;
      m_ninety_right();
      is_turning = false;
      m_forward();
      delay(delay_value);
    }

  

  if (IR1 == HIGH && IR2 == LOW && IR3 == HIGH)//T Intersection
    {
      //serial_print(4);
      is_turning = true;
      m_ninety_left(); // As left is possible
      is_turning = false;
      m_forward();
      delay(delay_value);
    }

  

  if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW)//Left T Intersection
    {
      //serial_print(5);
     is_turning = true;
      m_ninety_left();// As Left is possible
      is_turning = false;
      m_forward();
      delay(delay_value);
    }

  

  if (IR1 == LOW && IR2 == HIGH && IR3 == HIGH)//Right T Tntersection
    {
    //serial_print(6);
     m_forward();//As Straight path is possible
     delay(delay_value);
    }

  

  if (IR1 == LOW && IR2 ==LOW && IR3 == LOW)//Dead End
    {
    //serial_print(7);
    is_turning = true;
     m_ninety_left(); //As no other direction is possible
     m_ninety_left();
     is_turning =false;
     m_forward();

    }


  if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
    {
     m_forward();
      
     delay((int)0.5*delay_value);
      
     m_stop();

     if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
        {
          m_stop();
        }
     else
        {
        is_turning = true;
         m_ninety_left();
         is_turning = false;
         m_forward();
         
         delay(delay_value);

        }
    }
}



