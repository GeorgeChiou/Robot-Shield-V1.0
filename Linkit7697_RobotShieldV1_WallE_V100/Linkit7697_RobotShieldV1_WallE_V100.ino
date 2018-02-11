// 最後編輯 2017-10-24 by ShinWei Chiou
// Left Arm : P9 ，Right Arm : P8，Head : P7
// Left Motor : P10 P12，Right Motor : P13 P17
// HR-SC04P , Trig : P2 , Echo : P3

#include <Servo.h>
#include <LRemote.h>
#include <Ultrasonic.h>

LRemoteLabel label;
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteSlider headslider;
LRemoteSlider armleftslider;
LRemoteSlider armrightslider;
LRemoteSwitch ultrasonicswitchButton;

int Ultrasonic_OnOff = 0;
int AutoDemoCounter = 0;

// create Ultrasonic object
Ultrasonic ultrasonic(2, 3);

// create Servo object to control a servo
Servo HeadServo;
Servo ArmRightServo;
Servo ArmLeftServo;


void Move_Forward()
{
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
}

void Move_Backward()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
}

void Move_TurnLeft()
{
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(17, LOW);
  digitalWrite(13, HIGH);
}

void Move_TurnRight()
{
  digitalWrite(12, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(17, HIGH);
  digitalWrite(13, LOW);
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


void setup() {
  // Motor PIN Set
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(17, OUTPUT);

  // Servo PIN Set
  HeadServo.attach(7);      // attaches the servo on pin 7 to the servo object
  ArmRightServo.attach(8);  // attaches the servo on pin 8 to the servo object
  ArmLeftServo.attach(9);   // attaches the servo on pin 9 to the servo object

  // Initialize Servo
  HeadServo.write(90);
  ArmRightServo.write(90);
  ArmLeftServo.write(90);

  // Initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // Setup the Remote Control's UI canvas
  LRemote.setName("WALL-E");
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

  // Add an Ultrasonic on/off switch
  ultrasonicswitchButton.setText("AUTO");
  ultrasonicswitchButton.setPos(1, 1);
  ultrasonicswitchButton.setSize(1, 1);
  ultrasonicswitchButton.setColor(RC_PINK);
  LRemote.addControl(ultrasonicswitchButton);

  // Add a Head slider
  headslider.setText("Head Slider(60 ~ 120)");
  headslider.setPos(0, 3);
  headslider.setSize(3, 1);
  headslider.setColor(RC_YELLOW);
  headslider.setValueRange(60, 120, 90);
  LRemote.addControl(headslider);

  // Add a Left Arm slider
  armleftslider.setText("Left Arm Slider(60 ~ 110)");
  armleftslider.setPos(0, 4);
  armleftslider.setSize(3, 1);
  armleftslider.setColor(RC_YELLOW);
  armleftslider.setValueRange(60, 110, 90);
  LRemote.addControl(armleftslider);

  // Add a Right Arm slider
  armrightslider.setText("Right Arm Slider(75 ~ 120)");
  armrightslider.setPos(0, 5);
  armrightslider.setSize(3, 1);
  armrightslider.setColor(RC_YELLOW);
  armrightslider.setValueRange(75, 120, 90);
  LRemote.addControl(armrightslider);

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

  // Process the incoming BLE write request
  LRemote.process();


  // Motor
  if (forwardbutton.getValue())
  {
    HeadServo.write(90);
    Move_Forward();
  }
  else if (backwardbutton.getValue())
  {
    HeadServo.write(90);
    Move_Backward();
  }
  else if (turnleftbutton.getValue())
  {
    HeadServo.write(120);
    Move_TurnLeft();
  }
  else if (turnrightbutton.getValue())
  {
    HeadServo.write(60);
    Move_TurnRight();
  }
  else
  {
    Motor_Break();
  }


  // Servo
  if (headslider.isValueChanged())
  {
    HeadServo.write(headslider.getValue());
  }

  if (armleftslider.isValueChanged())
  {
    ArmLeftServo.write(armleftslider.getValue());
  }

  if (armrightslider.isValueChanged())
  {
    ArmRightServo.write(armrightslider.getValue());
  }


  // Ultrasonic ON/OFF
  if (ultrasonicswitchButton.isValueChanged())
  {
    Ultrasonic_OnOff = ultrasonicswitchButton.getValue();
  }

  // Ultrasonic Auto Demo
  if (Ultrasonic_OnOff == 1)
  {
    if (ultrasonic.distanceRead() <= 20)
    {
      AutoDemoCounter = 0;

      HeadServo.write(90);
      ArmLeftServo.write(110);
      ArmRightServo.write(75);
      delay(400);
      ArmRightServo.write(120);
      delay(400);
      ArmRightServo.write(75);
      delay(400);
      ArmRightServo.write(120);
      delay(400);
      ArmRightServo.write(75);
      delay(400);
      ArmRightServo.write(120);
      delay(400);
      ArmRightServo.write(90);
    }

    if (AutoDemoCounter >= 50)
    {
      AutoDemoCounter = 0;

      HeadServo.write(70);
      delay(1000);
      HeadServo.write(90);
      delay(1000);
      HeadServo.write(110);
      delay(1000);
      HeadServo.write(90);
    }
    else
    {
      AutoDemoCounter++;
    }
  }

}
