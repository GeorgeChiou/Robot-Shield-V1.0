// 最後編輯 2018-2-05 by ShinWei Chiou

// CMUcam5 PixyCMUcam5 Pixy module.
// http://www.cmucam.org/attachments/download/1157/arduino_pixy-0.1.7.zip

// For Linkit-7697 + Robot Shield V1.2
// Left Motor : P10 P12，Right Motor : P13 P17
// HR-SC04P , Trig : P2 , Echo : P3
// Pixy CMUcam5 , SCL : P8 , SDA : P9
// Buzzer : P15


#include <Wire.h>
#include <PixyI2C.h>
#include <LRemote.h>


// Button
LRemoteLabel label;
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteButton buzzerbutton;
LRemoteSwitch ultrasonicswitchButton;
LRemoteSwitch pixyswitchButton;

// Ultrasonic SENSOR
const char TRIGGER = 2;
const char ECHO = 3;
int Ultrasonic_OnOff = 0;
int Ultrasonic_Distance = 0;

// Buzzer
const int BuzzerPin = 15;

// Pixy CMUcam5 object
PixyI2C pixy;
int Pixy_OnOff = 0;
int signature = 0;
int xblocks = 0;
int yblocks = 0;
unsigned int width = 0;
unsigned int height = 0;
unsigned int area = 0;
unsigned int newarea = 0;
int Xmin = 70;  //left
int Xmax = 200; //right
int maxArea = 0;
int minArea = 0;
static int isignature = 0;


/*------------------------------------------------------------*/
void Move_Forward()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
}

void Move_Backward()
{
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
}

void Move_TurnLeft()
{
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
}

void Move_TurnRight()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
}

void Motor_Break()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, LOW);
}

void Pixy_scan()
{
  uint16_t blocks;
  blocks = pixy.getBlocks();
  signature = pixy.blocks[isignature].signature;
  xblocks = pixy.blocks[isignature].x;
  width = pixy.blocks[isignature].width;
  height = pixy.blocks[isignature].height;
}

/*------------------------------------------------------------*/
void setup()
{
  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Motor PIN Set
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(17, OUTPUT);

  // Ultrasonic PIN Set
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  // Buzzer PIN Set
  pinMode(BuzzerPin, OUTPUT);

  // Initialize Pixy CMUcam5
  pixy.init();

  // Setup the Remote Control's UI canvas
  LRemote.setName("SMARS");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 6);

  // Add a Forward button
  forwardbutton.setText("Forward");
  forwardbutton.setPos(1, 0);
  forwardbutton.setSize(1, 1);
  forwardbutton.setColor(RC_BLUE);
  LRemote.addControl(forwardbutton);

  // Add a Backward button
  backwardbutton.setText("Backward");
  backwardbutton.setPos(1, 2);
  backwardbutton.setSize(1, 1);
  backwardbutton.setColor(RC_BLUE);
  LRemote.addControl(backwardbutton);

  // Add a TurnLeft button
  turnleftbutton.setText("TurnLeft");
  turnleftbutton.setPos(0, 1);
  turnleftbutton.setSize(1, 1);
  turnleftbutton.setColor(RC_BLUE);
  LRemote.addControl(turnleftbutton);

  // Add a TurnRight button
  turnrightbutton.setText("TurnRight");
  turnrightbutton.setPos(2, 1);
  turnrightbutton.setSize(1, 1);
  turnrightbutton.setColor(RC_BLUE);
  LRemote.addControl(turnrightbutton);

  // Add a Buzzer button
  buzzerbutton.setText("Buzzer");
  buzzerbutton.setPos(1, 1);
  buzzerbutton.setSize(1, 1);
  buzzerbutton.setColor(RC_YELLOW);
  LRemote.addControl(buzzerbutton);

  // Add an Ultrasonic on/off switch
  ultrasonicswitchButton.setText("Ultrasonic");
  ultrasonicswitchButton.setPos(0, 3);
  ultrasonicswitchButton.setSize(1, 1);
  ultrasonicswitchButton.setColor(RC_PINK);
  LRemote.addControl(ultrasonicswitchButton);

  // Add an Pixy on/off switch
  pixyswitchButton.setText("Pixy");
  pixyswitchButton.setPos(2, 3);
  pixyswitchButton.setSize(1, 1);
  pixyswitchButton.setColor(RC_PINK);
  LRemote.addControl(pixyswitchButton);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("LRemote begin ...");
}


/*------------------------------------------------------------*/
void loop()
{
  // BLE central device, e.g. an mobile app
  if (!LRemote.connected())
  {
    Serial.println("Waiting for connection ...");
    delay(500);
  }


  // Process the incoming BLE write request
  LRemote.process();


  // Motor
  if (forwardbutton.isValueChanged()) {
    if (forwardbutton.getValue() == 1) {
      Move_Forward();
    } else {
      Motor_Break();
    }
  }

  if (backwardbutton.isValueChanged()) {
    if (backwardbutton.getValue() == 1) {
      Move_Backward();
    } else {
      Motor_Break();
    }
  }

  if (turnleftbutton.isValueChanged()) {
    if (turnleftbutton.getValue() == 1) {
      Move_TurnLeft();
    } else {
      Motor_Break();
    }
  }

  if (turnrightbutton.isValueChanged()) {
    if (turnrightbutton.getValue() == 1) {
      Move_TurnRight();
    } else {
      Motor_Break();
    }
  }


  // Buzzer
  if (buzzerbutton.isValueChanged()) {
    if (buzzerbutton.getValue() == 1) {
      tone(BuzzerPin, 523, 500);
      delay(100);
    } else {
      noTone(BuzzerPin);
    }
  }


  // Ultrasonic ON/OFF
  if (ultrasonicswitchButton.isValueChanged())
  {
    Ultrasonic_OnOff = ultrasonicswitchButton.getValue();
    Motor_Break();
  }

  // Ultrasonic Auto Demo
  if (Ultrasonic_OnOff == 1)
  {
    //Ultrasonic Distance
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);
    Ultrasonic_Distance = pulseIn(ECHO, HIGH) / 58.0;
    Serial.print("Distance CM : ");
    Serial.println(Ultrasonic_Distance);

    if (Ultrasonic_Distance >= 10)
    {
      Move_Forward();
    }
    else {
      Move_TurnLeft();
      delay(1000);
    }
  }


  // Pixy ON/OFF
  if (pixyswitchButton.isValueChanged())
  {
    Pixy_OnOff = pixyswitchButton.getValue();
    Motor_Break();
  }

  while (millis() < 5000)
  {
    Pixy_scan();
    area = width * height;
    maxArea = area + 1000;
    minArea = area - 1000;
  }

  // Pixy Auto Demo
  if (Pixy_OnOff == 1)
  {
    Pixy_scan();

    if (signature == 1)
    {
      newarea = width * height;

      if (xblocks < Xmin)
      {
        Move_TurnLeft();
      }
      else if (xblocks > Xmax)
      {
        Move_TurnRight();
      }
      else if (newarea < minArea)
      {
        Move_Forward();
      }
      else if (newarea > maxArea)
      {
        Move_Backward();
      }

      else
      {
        Motor_Break();
      }
    }
    else
    {
      Motor_Break();
    }
  }


}


