#include "mobility.h"

void SetSpeeds(int Aspeed, int Bspeed)
{
  if (Aspeed < 0)
  {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, -Aspeed);
  }
  else
  {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, Aspeed);
  }

  if (Bspeed < 0)
  {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, -Bspeed);
  }
  else
  {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, Bspeed);
  }
}

void m_forward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void m_backward()
{
  analogWrite(PWMA,Speed);
  analogWrite(PWMB,Speed);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void m_right()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void m_left()
{
  analogWrite(PWMA,50);
  analogWrite(PWMB,50);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void m_stop()
{
  analogWrite(PWMA,0);
  analogWrite(PWMB,0);
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,LOW);
}

void m_ninety_right()
{
  m_right();
  delay(290); //adjust
}

void m_ninety_left()
{
  m_right();
  delay(290); //adjust
}

void m_follow_segment()
{
  m_forward();


  while (1)
  {

    //CODE: compensation routines

    
    
    if(read_binary_front() == 1) return;
    if(read_lrange_binary_left() == 1) return;
    if(read_lrange_binary_right() == 1) return; 
  }
}

void m_turn(unsigned char dir)
{
  // if(millis() - lasttime >500)
  {
    switch (dir)
    {
    case 'L':
      // Turn left.
      //SetSpeeds(-100, 100);
      //delay(190);
      m_ninety_left();
      
      break;
    case 'R':
      // Turn right.
      //SetSpeeds(100, -100);
      //delay(190);
      m_ninety_right();

      break;
    case 'B':
      // Turn around.
      //SetSpeeds(100, -100);
      //delay(400);
      m_ninety_left();
      m_ninety_left();
      break;
    case 'S':
      // Don't do anything!
      break;
    }
  }
  SetSpeeds(0, 0);
  // value = 0;
  //  while(value != 0xEF)  //wait button pressed
  //  {
  //    PCF8574Write(0x1F | PCF8574Read());
  //    value = PCF8574Read() | 0xE0;
  //  }
  //Serial.write(dir);
  //Serial.println();
  //lasttime = millis();
}

unsigned char m_select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
  // Make a decision about how to m_turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // m_turn as far to the left as possible.
  if (found_left)
    return 'L';
  else if (found_straight)
    return 'S';
  else if (found_right)
    return 'R';
  else
    return 'B';
}
