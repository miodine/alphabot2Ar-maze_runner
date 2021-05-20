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


int left_speed = 40;
int right_speed = 40;
int readout_left = 0;
int readout_right = 0;


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

void compensate() {
    readout_left = read_compensator_left();
    readout_right = read_compensator_right();

    if((readout_left == LOW) && (readout_right == LOW))
    {
      SetSpeeds(Speed, Speed);
      return;
    }


    if(readout_left == LOW)
    {
      if(left_speed <(Speed+10)) left_speed += 5;
      if(right_speed >(Speed -10)) right_speed -= 5;
    }
    
    else if(readout_right == LOW)
    {
      if(left_speed >(Speed - 10)) left_speed -= 5;
      if(right_speed <(Speed +10)) right_speed += 5;
    }

    else 
    {
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
  IR1 = read_detector_left();
  IR2 = read_detector_front();
  IR3 = read_detector_right();


  
  if (IR1 == LOW && IR2 == HIGH && IR3 == LOW)//Straight path
    {
    compensate();
    m_forward();
    compensate();
    //serial_print(1);
    }

  compensate();

  if (IR1 == HIGH && IR2 == LOW && IR3 == LOW)//Left turn
    {
    //serial_print(2);
    
    m_ninety_left();
    compensate();
    m_forward();
    compensate();

    delay(250);
    compensate();
    delay(250);
    compensate();
    delay(250);
    compensate();
    delay(250);
    compensate();
    }

  compensate();

  if (IR1 == LOW && IR2 == LOW && IR3 == HIGH)//Right Turn
    {
      //serial_print(3);
      compensate();
      m_ninety_right();
      compensate();
      m_forward();
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
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
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
    }

  compensate();

  if (IR1 == HIGH && IR2 == HIGH && IR3 == LOW)//Left T Intersection
    {
      m_ninety_left();// As Left is possible
      compensate();
      m_forward();
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
    }

  compensate();

  if (IR1 == LOW && IR2 == HIGH && IR3 == HIGH)//Right T Tntersection
    {
     compensate();
     m_forward();//As Straight path is possible
    compensate();
     delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
      delay(250);
      compensate();
    }

  compensate();

  if (IR1 == LOW && IR2 ==LOW && IR3 == LOW)//Dead End
    {
       compensate();
     m_ninety_left(); //As no other direction is possible
      compensate();
     m_ninety_left();
      compensate();
     m_forward();
    }

  compensate();

  if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
    {
     m_forward();
      compensate();
     delay(250);
      compensate();
      delay(250);
      compensate();
     m_stop();

     if (IR1 == HIGH && IR2 == HIGH && IR3 == HIGH)
        {
          m_stop();
        }
     else
        {
          compensate();
         m_left();
         compensate();
        }
    }
}



