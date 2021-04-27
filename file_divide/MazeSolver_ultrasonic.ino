//CONFIG

#include "mobility.h"

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println("TRSensor example");
  Wire.begin();
  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  SetSpeeds(0, 0);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the spxdd d lashscreen.
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 0);
  display.println("WaveShare");
  display.setCursor(10, 25);
  display.println("AlhpaBot2");
  display.setTextSize(1);
  display.setCursor(10, 55);
  display.println("Press to calibrate");
  display.display();

  while (value != 0xEF) //wait button pressed
  {
    PCF8574Write(0x1F | PCF8574Read());
    value = PCF8574Read() | 0xE0;
  }

  RGB.begin();
  RGB.setPixelColor(0, 0x00FF00);
  RGB.setPixelColor(1, 0x00FF00);
  RGB.setPixelColor(2, 0x00FF00);
  RGB.setPixelColor(3, 0x00FF00);
  RGB.show();
  delay(500);
  //  analogWrite(PWMA,60);
  //  analogWrite(PWMB,60);
  for (int i = 0; i < 100; i++) // make the calibration take about 10 seconds
  {
    if (i < 25 || i >= 75)
    {
      //      digitalWrite(AIN2,HIGH);
      //      digitalWrite(AIN1,LOW);
      //      digitalWrite(BIN1,LOW);
      //      digitalWrite(BIN2,HIGH);
      SetSpeeds(80, -80);
    }
    else
    {
      //      digitalWrite(AIN2,LOW);
      //      digitalWrite(AIN1,HIGH);
      //      digitalWrite(BIN1,HIGH);
      //      digitalWrite(BIN2,LOW);
      SetSpeeds(-80, 80);
    }
    trs.calibrate(); // reads all sensors 100 times
  }
  SetSpeeds(0, 0);
  RGB.setPixelColor(0, 0x0000FF);
  RGB.setPixelColor(1, 0x0000FF);
  RGB.setPixelColor(2, 0x0000FF);
  RGB.setPixelColor(3, 0x0000FF);
  RGB.show(); // Initialize all pixels to 'off'

  value = 0;
  while (value != 0xEF) //wait button pressed
  {
    PCF8574Write(0x1F | PCF8574Read());
    value = PCF8574Read() | 0xE0;
    position = trs.readLine(sensorValues) / 200;
    display.clearDisplay();
    display.setCursor(0, 25);
    display.println("Calibration Done !!!");
    display.setCursor(0, 55);
    for (int i = 0; i < 21; i++)
    {
      display.print('_');
    }
    display.setCursor(position * 6, 55);
    display.print("**");
    display.display();
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("AlhpaBot2");
  display.setTextSize(3);
  display.setCursor(40, 30);
  display.println("Go!");
  display.display();
  delay(500);
}


void loop()
{
  while (1)
  {
    m_follow_segment();

    // Drive straight a bit.  This helps us in case we entered the
    // intersection at an angle.
    // Note that we are slowing down - this prevents the robot
    // from tipping forward too much.
    SetSpeeds(30, 30);
    delay(40);

    // These variables record whether the robot has seen a line to the
    // left, straight ahead, and right, whil examining the current
    // intersection.
    unsigned char found_left = 0;
    unsigned char found_straight = 0;
    unsigned char found_right = 0;

    
    //CODE: TURN 360, SEE WHERE YOU CAN GO  
  

    
    unsigned char dir = m_select_turn(found_left, found_straight, found_right);

    // Make the m_turn indicated by the path.
    m_turn(dir);

    // Store the intersection in the path variable.
    path[path_length] = dir;
    path_length++;

    // You should check to make sure that the path_length does not
    // exceed the bounds of the array.  We'll ignore that in this
    // example.

    // Simplify the learned path.
    simplify_path();

    // Display the path on the LCD.
    // display_path();
  }

  // Solved the maze!

  // Now enter an infinite loop - we can re-run the maze as many
  // times as we want to.
  while (1)
  {
    // Beep to show that we solved the maze.
    SetSpeeds(0, 0);
    // OrangutanBuzzer::play(">>a32");
    Serial.println("End !!!");
    // Wait for the user to press a button, while displaying
    // the solution.
    //    while (!OrangutanPushbuttons::isPressed(BUTTON_B))
    //    {
    //      if (millis() % 2000 < 1000)
    //      {
    //        OrangutanLCD::clear();
    //        OrangutanLCD::print("Solved!");
    //        OrangutanLCD::gotoXY(0, 1);
    //        OrangutanLCD::print("Press B");
    //      }
    //      else
    //        display_path();
    //      delay(30);
    //    }
    //    while (OrangutanPushbuttons::isPressed(BUTTON_B));
    //    display.clearDisplay();
    //    display.setTextSize(2);
    //    display.setTextColor(WHITE);
    //    display.setCursor(20,0);
    //    display.println("AlhpaBot");
    //    display.setTextSize(3);
    //    display.setCursor(40,30);
    //    display.println("Go!");
    //    display.display();
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


