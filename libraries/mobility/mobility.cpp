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
  m_stop();
}

void m_ninety_left()
{
  m_left();
  delay(290); //adjust
  m_stop();
}

void m_follow_segment()
{
  m_forward();

  int sp_left = Speed;
  int sp_right = Speed;
  int readout_left = 0;
  int readout_right = 0;

  while (1)
  {
    readout_left = read_srange_analog_left();
    readout_right = read_srange_analog_right();

    //CODE: compensation routines
  
    if(readout_left < 600)
    {
      if(sp_left <(Speed+10)) sp_left += 2;
      if(sp_right >(Speed -10)) sp_right -= 2;
    }
    else if(readout_right < 600)
    {
      if(sp_left >(Speed - 10)) sp_left -= 2;
      if(sp_right <(Speed +10)) sp_right += 2;
    }
    else 
    {
      sp_left = Speed;
      sp_right = Speed;
    }

    SetSpeeds(sp_left, sp_right);
    


    if((read_sharp() == 1) &&  (read_lrange_binary_left() == 0) && (read_lrange_binary_right() == 0)) 
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

void m_turn(unsigned char dir)
{
  {
    switch (dir)
    {
    case 'L':
      m_ninety_left();
      break;
    case 'R':
      m_ninety_right();
      break;
    case 'B': {
      m_ninety_left();
      m_ninety_left();
      break;
      }
    
    case 'S':
      m_forward();
      break;
    }
  }
  // value = 0;
  //  while(value != 0xEF)  //wait button pressed
  //  {
  //    PCF8574Write(0x1F | PCF8574Read());
  //    value = PCF8574Read() | 0xE0;
  //  }
  //Serial.write(dir);
  //Serial.println();
  
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
