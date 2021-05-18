//CONFIG

#include "mobility.h"


unsigned char found_left;
unsigned char found_straight;
unsigned char found_right;

unsigned char dir;


void setup()
{
  delay(3000);
  //Serial.begin(115200);

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
    m_follow_segment();  // exit if found an intersection
    m_stop();            // stop the robot

    // These variables record whether the robot has seen a line to the
    // left, straight ahead, and right, whil examining the current
    // intersection.
    found_left = 0;
    found_straight = 0;
    found_right = 0;

    if(read_binary_front() == 0) found_straight = 1;
    if(read_lrange_binary_left() == HIGH) found_left = 1;
    if(read_lrange_binary_right() == HIGH) found_right = 1;


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
