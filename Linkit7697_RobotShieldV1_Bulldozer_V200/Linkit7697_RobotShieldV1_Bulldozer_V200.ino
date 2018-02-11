// 最後編輯 2017-10-23 by ShinWei Chiou
// P2 : Left 左輪，P3 : Right 右輪，P4 : Bucket 鏟斗

// 最後編輯 2017-11-03 by ShinWei Chiou
// 增加 EEPROM 紀錄校正參數


#include <Servo.h>
#include <EEPROM.h>
#include <LRemote.h>

LRemoteLabel label;
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteSlider bucketslider;
LRemoteSlider leftslider;
LRemoteSlider rightslider;
LRemoteSwitch turboswitchButton;

int Speed1 = 20;
int Speed2 = 50;
int Turbo_OnOff = 0;
int LeftServoCen = 90;
int RightServoCen = 90;

// Servo Trim Value
int Running_Servo_Trim [2];

// create Servo object to control a servo
Servo BucketServo;
Servo RightServo;
Servo LeftServo;



void Move_Forward()
{
  if (Turbo_OnOff == 1)
  {
    LeftServo.write(LeftServoCen + Speed2);
    RightServo.write(RightServoCen - Speed2);
  }
  else
  {
    LeftServo.write(LeftServoCen + Speed1);
    RightServo.write(RightServoCen - Speed1);
  }
}

void Move_Backward()
{
  if (Turbo_OnOff == 1)
  {
    LeftServo.write(LeftServoCen - Speed2);
    RightServo.write(RightServoCen + Speed2);
  }
  else
  {
    LeftServo.write(LeftServoCen - Speed1);
    RightServo.write(RightServoCen + Speed1);
  }
}

void Move_TurnLeft()
{
  if (Turbo_OnOff == 1)
  {
    LeftServo.write(LeftServoCen - Speed2);
    RightServo.write(RightServoCen - Speed2);
  }
  else
  {
    LeftServo.write(LeftServoCen - Speed1);
    RightServo.write(RightServoCen - Speed1);
  }
}

void Move_TurnRight()
{
  if (Turbo_OnOff == 1)
  {
    LeftServo.write(LeftServoCen + Speed2);
    RightServo.write(RightServoCen + Speed2);
  }
  else
  {
    LeftServo.write(LeftServoCen + Speed1);
    RightServo.write(RightServoCen + Speed1);
  }
}

void Motor_Break()
{
  LeftServo.write(LeftServoCen);
  RightServo.write(RightServoCen);
}



void setup() {
  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Read EEPROM Trim
  Running_Servo_Trim[0] = (int8_t)EEPROM.read(0); // LeftServo
  Running_Servo_Trim[1] = (int8_t)EEPROM.read(1); // RightServo

  // Servo PIN Set
  BucketServo.attach(4);
  RightServo.attach(3);
  LeftServo.attach(2);
  delay(100);

  // Initialize Servo
  LeftServoCen = 90 + Running_Servo_Trim[0];
  RightServoCen = 90 + Running_Servo_Trim[1];
  BucketServo.write(90);
  LeftServo.write(LeftServoCen);
  RightServo.write(RightServoCen);

  // Setup the Remote Control's UI canvas
  LRemote.setName("LinkIt 7697 Bulldozer");
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

  // Add an Turbo on/off switch
  turboswitchButton.setText("Turbo");
  turboswitchButton.setPos(1, 1);
  turboswitchButton.setSize(1, 1);
  turboswitchButton.setColor(RC_PINK);
  LRemote.addControl(turboswitchButton);

  // Add a Head slider
  bucketslider.setText("Bucket Up / Down (60 ~ 150)");
  bucketslider.setPos(0, 3);
  bucketslider.setSize(3, 1);
  bucketslider.setColor(RC_YELLOW);
  bucketslider.setValueRange(60, 150, 90);
  LRemote.addControl(bucketslider);

  // Add a Left slider
  leftslider.setText("Left Wheel Trim (-20 ~ 20)");
  leftslider.setPos(0, 4);
  leftslider.setSize(3, 1);
  leftslider.setColor(RC_PINK);
  leftslider.setValueRange(-20, 20, Running_Servo_Trim[0]);
  LRemote.addControl(leftslider);

  // Add a Right slider
  rightslider.setText("Right Wheel Trim (-20 ~ 20)");
  rightslider.setPos(0, 5);
  rightslider.setSize(3, 1);
  rightslider.setColor(RC_PINK);
  rightslider.setValueRange(-20, 20, Running_Servo_Trim[1]);
  LRemote.addControl(rightslider);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("begin() returned");
}



void loop() {
  // BLE central device, e.g. an mobile app
  if (!LRemote.connected())
  {
    Serial.println("waiting for connection");
    delay(1000);
  }
  else
  {
    delay(15);
  }

  // Process the incoming BLE write request
  LRemote.process();


  // Move
  if (forwardbutton.getValue())
  {
    Move_Forward();
  }
  else if (backwardbutton.getValue())
  {
    Move_Backward();
  }
  else if (turnleftbutton.getValue())
  {
    Move_TurnLeft();
  }
  else if (turnrightbutton.getValue())
  {
    Move_TurnRight();
  }
  else
  {
    Motor_Break();
  }


  // Bucket
  if (bucketslider.isValueChanged())
  {
    BucketServo.write(bucketslider.getValue());
  }

  // Left slider
  if (leftslider.isValueChanged())
  {
    Running_Servo_Trim[0] = leftslider.getValue();

    EEPROM.write(0, Running_Servo_Trim[0]);

    LeftServoCen = 90 + Running_Servo_Trim[0];
    LeftServo.write(LeftServoCen);
  }

  // Right slider
  if (rightslider.isValueChanged())
  {
    Running_Servo_Trim[1] = rightslider.getValue();

    EEPROM.write(1, Running_Servo_Trim[1]);

    RightServoCen = 90 + Running_Servo_Trim[1];
    RightServo.write(RightServoCen);
  }


  // Turbo ON/OFF
  if (turboswitchButton.isValueChanged())
  {
    Turbo_OnOff = turboswitchButton.getValue();
  }
}
