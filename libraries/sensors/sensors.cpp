#include "sensors.h"

void PCF8574Write(byte data)
{
  Wire.beginTransmission(Addr);
  Wire.write(data);
  Wire.endTransmission();
}

byte PCF8574Read()
{
  int data = -1;
  Wire.requestFrom(Addr, 1);
  if (Wire.available())
  {
    data = Wire.read();
  }
  return data;
}

char read_infrared(bool verbose){
    byte value;                           // declare sensor value as byte type variable
    PCF8574Write(0xC0 | PCF8574Read());   // write sensor output to PCF8574 I/O expansion module
    value = PCF8574Read() | 0x3F;         // read PCF module (sensor output)

    if(verbose){    // print result
        if(value == 0x7F)
            Serial.println("Left sensor has detected an obstacle.");

        else if(value == 0xBF)
            Serial.println("Right sensor has detected an obstacle.");

        else if(value == 0x3F)
            Serial.println("Both sensors have detected an obstacle.");

        else if(value == 0xFF)
            Serial.println("No obstacle has been detected.");
    }

    if(value == 0x7F)       // check if left sensor is active
        return 'L';

    else if(value == 0xBF)  // check if right sensor is active
        return 'R';

    else if(value == 0x3F)  // check if both sensors are active
        return 'B';

    else if(value == 0xFF)  // check if no sensors are active
        return 'N';

    else
        Serial.println("ERROR!");
}

int read_ultrasonic(bool verbose)
{
  int length = 15; // number of measurements
  int measurements[length];
  float f_dist;

  for (int i = 0; i < length; i++)
  {                          // measure 'length' times for better accuracy
    digitalWrite(TRIG, LOW); // set trig pin to low for 2μs (reset trigger)
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH); // set trig pin to high for 10μs (generate ultrasonic sound wave)
    delayMicroseconds(10);

    digitalWrite(TRIG, LOW); // set trig pin to low (stop generating ultrasonic sound wave)

    f_dist = pulseIn(ECHO, HIGH); // read echo pin (time for wave to bounce back)
    f_dist = f_dist / 58;         // Y m=（X s*344）/2; X s=（ 2*Y m）/344 ==》X s=0.0058*Y m ==》cm = us /58

    *(measurements + i) = (int)f_dist;
  }

  sort(measurements, length); // sort measurements array

  int distance = 0; // set initial measurement to 0

  if (*measurements < 6) // if minimum measurement is < 6cm, then get the minimum (for avoiding errors)
    distance = *measurements;
  else
    distance = *(measurements + (int)length / 2); // if the measurement is > 6cm, get the middle measurement

  if (verbose)
  { // print result

    if ((distance < 3) || (distance > 400))
    { // ultrasonic range ranging 3cm to 400cm

      Serial.println("ERROR! OUT OF RANGE! ==》Ultrasonic range: 3cm - 400cm");
    }
    else
    {
      Serial.println("Distance = ");
      Serial.print(distance);
      Serial.println("cm");
    }
  }

  return distance;
}
