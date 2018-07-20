// 最後編輯 2018-7-20 by ShinWei Chiou

// For Linkit-7697 + Robot Shield V1.2
// Turn Motor : P10 P12
// Leg Motor : P13 P17
// https://docs.labs.mediatek.com/resource/linkit7697-arduino/en/kits-and-shields/robot-shield

// Pixy2 module.
// SCL : P8
// SDA : P9
// https://github.com/charmedlabs/pixy2/raw/master/releases/arduino/arduino_pixy2-1.0.2.zip

// Pixy module.
// http://www.cmucam.org/attachments/download/1157/arduino_pixy-0.1.7.zip


#include <Wire.h>
#include <LRemote.h>

#include <PixyI2C.h>

#define LegMotor_A    12
#define LegMotor_B    10
#define TurnMotor_A   17
#define TurnMotor_B   13

// Button
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteSwitch pixyswitchButton;

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
  digitalWrite(LegMotor_A, LOW);
  digitalWrite(LegMotor_B, HIGH);
  //digitalWrite(TurnMotor_A, HIGH);
  //digitalWrite(TurnMotor_B, LOW);
}

void Move_Backward()
{
  digitalWrite(LegMotor_A, HIGH);
  digitalWrite(LegMotor_B, LOW);
  //digitalWrite(TurnMotor_A, HIGH);
  //digitalWrite(TurnMotor_B, HIGH);
}

void Move_TurnRight()
{
  //digitalWrite(LegMotor_A, HIGH);
  //digitalWrite(LegMotor_B, HIGH);
  digitalWrite(TurnMotor_A, LOW);
  digitalWrite(TurnMotor_B, HIGH);
}

void Move_TurnLeft()
{
  //digitalWrite(LegMotor_A, HIGH);
  //digitalWrite(LegMotor_B, HIGH);
  digitalWrite(TurnMotor_A, HIGH);
  digitalWrite(TurnMotor_B, LOW);
}

void Motor_Break()
{
  digitalWrite(LegMotor_A, HIGH);
  digitalWrite(LegMotor_B, HIGH);
  digitalWrite(TurnMotor_A, HIGH);
  digitalWrite(TurnMotor_B, HIGH);
  delay(100);
  digitalWrite(LegMotor_A, LOW);
  digitalWrite(LegMotor_B, LOW);
  digitalWrite(TurnMotor_A, LOW);
  digitalWrite(TurnMotor_B, LOW);
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
void setup() {

  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Motor PIN Set
  pinMode(LegMotor_A, OUTPUT);
  pinMode(LegMotor_B, OUTPUT);
  pinMode(TurnMotor_A, OUTPUT);
  pinMode(TurnMotor_B, OUTPUT);

  // Initialize Pixy2
  pixy.init();

  // Initialize BLE subsystem & get BLE address
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }

  Serial.print("Device Address = [");
  LBLEAddress ble_address;
  String tank_address;

  ble_address = LBLE.getDeviceAddress();
  tank_address = ble_address.toString();
  Serial.print(tank_address);
  Serial.println("]");

  String tank_name;
  tank_name = "AI" + tank_address.substring(0, 2) + tank_address.substring(3, 5);

  // Setup the Remote Control's Name
  LRemote.setName(tank_name);

  // Setup the Remote Control's UI canvas
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

  // Add an Pixy on/off switch
  pixyswitchButton.setText("Pixy");
  pixyswitchButton.setPos(1, 1);
  pixyswitchButton.setSize(1, 1);
  pixyswitchButton.setColor(RC_PINK);
  LRemote.addControl(pixyswitchButton);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("LRemote begin ...");
}


/*------------------------------------------------------------*/
void loop() {

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
