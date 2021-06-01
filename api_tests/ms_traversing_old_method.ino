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

void new_follow_segment(){
    m_forward();

    while (1)
    {
    compensate();

    if((read_detector_front() == 0) &&  (read_detector_left() == 0) && (read_detector_right() == 0)) 
    {
      delay(100);
      if( (read_lrange_binary_left() == 0) && (read_lrange_binary_right() == 0))
      {
        return;
      }
      
    }
    else if((read_lrange_binary_left() == 1) || (read_lrange_binary_right() == 1)) return;
  }
}




void setup()
{
  delay(3000);
  //Serial.begin(115200);

Speed = 40;

  Wire.begin();
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);


  //Initialisation of the long-range sensors
  pinMode(LO_RNG_B_LEFT, INPUT);
  pinMode(LO_RNG_B_RIGHT, INPUT);

  SetSpeeds(0, 0);

    pinMode(DETECTION_LEFT, INPUT);    
  pinMode(DETECTION_RIGHT, INPUT);   
  pinMode(DETECTION_FRONT, INPUT);    

  pinMode(COMPENSATION_LEFT, INPUT);
  pinMode(COMPENSATION_RIGHT, INPUT);

  //while (value != 0xEF) //wait button pressed
 //{
 // PCF8574Write(0x1F | PCF8574Read());
 // value = PCF8574Read() | 0xE0;
// }
  value = 0;
}

void loop()
{
  while (1)
  {
    new_follow_segment();  // exit if found an intersection
    m_stop();            // stop the robot

    // These variables record whether the robot has seen a line to the
    // left, straight ahead, and right, whil examining the current
    // intersection.
    found_left = 0;
    found_straight = 0;
    found_right = 0;

    if(read_detector_front() == 0) found_straight = 1;
    if(read_detector_left() == 0) found_left = 1;
    if(read_detector_right()== 0) found_right = 1;


    dir = m_select_turn(found_left, found_straight, found_right);

    // Make the m_turn indicated by the path.
    m_turn(dir);

    //Podjedz troche, żeby nie łapać poprzednich odczytów.
    SetSpeeds(50, 50);
    delay(700);
    


    // Store the intersection in the path variable.
    path[path_length] = dir;
    path_length++;

    // You should check to make sure that the path_length does not
    // exceed the bounds of the array.  We'll ignore that in this
    // example.

    // Simplify the learned path.
    simplify_path();
  }

  // Solved the maze!

  // Now enter an infinite loop - we can re-run the maze as many
  // times as we want to.
  while (1)
  {
    SetSpeeds(0, 0);

    //Serial.println("End !!!");

    delay(500);

    value = 0;
    while (value != 0xEF) //wait button pressed
    {
      PCF8574Write(0x1F | PCF8574Read());
      value = PCF8574Read() | 0xE0;
    }
    delay(1000);

    // Re-run the maze.  It's not necessary to identify the
    // intersections, so this loop is really simple.
    int i;
    for (i = 0; i < path_length; i++)
    {
      m_follow_segment();

      // Drive straight while slowing down, as before.
      SetSpeeds(30, 30);
      delay(40);
      SetSpeeds(30, 30);
      delay(150);

      // Make a m_turn according to the instruction stored in
      // path[i].
      m_turn(path[i]);
    }



    // Follow the last segment up to the finish.
    m_follow_segment();
    // Now we should be at the finish!  Restart the loop.
  }
}
