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





void setup()
{
  delay(3000);
  Serial.begin(115200);
  Wire.begin();


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
  IR1 = read_detector_left();
  IR2 = read_detector_front();
  IR3 = read_detector_right();

  if (IR1 == LOW && IR2 == HIGH && IR3 == LOW)//Straight path
    {
     m_forward();
     delay(300);
    }

  if (IR1 == HIGH && IR2 == LOW && IR3 == LOW)//Left turn
    {
     m_ninety_left();
     delay(300);
    }

  if (IR1 == LOW && IR2 == LOW && IR3 == HIGH)//Right Turn
    {
      m_ninety_right();
      delay(300);
    }

  if (IR1 == HIGH && IR2 == LOW && IR3 == HIGH)//T Intersection
    {
      m_ninety_left(); // As left is possible
      delay(300);
    }

  if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW)//Left T Intersection
    {
      m_ninety_left();// As Left is possible
      delay(300);
    }

  if (IR1 == LOW && IR2 == HIGH && IR3 == HIGH)//Right T Tntersection
    {
     m_forward();//As Straight path is possible
     delay(300);
    }

  if (IR1 == LOW && IR2 ==LOW && IR3 == LOW)//Dead End
    {
     m_ninety_left(); //As no other direction is possible
     m_ninety_right();
    }

  if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
    {
     m_forward();
     delay(100);
     m_stop();

     if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
        {
          m_stop();
        }
     else
        {
         m_left();
        }
    }
}



