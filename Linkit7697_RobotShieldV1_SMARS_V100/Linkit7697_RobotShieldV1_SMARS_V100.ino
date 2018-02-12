// 最後編輯 2018-1-14 by ShinWei Chiou
// Left Motor : P10 P12，Right Motor : P13 P17
// HR-SC04P , Trig : P7 , Echo : P8
// Buzzer : P15

#include <LRemote.h>

// Button
LRemoteLabel label;
LRemoteButton forwardbutton;
LRemoteButton backwardbutton;
LRemoteButton turnleftbutton;
LRemoteButton turnrightbutton;
LRemoteButton buzzerbutton;
LRemoteSwitch ultrasonicswitchButton;

// Ultrasonic SENSOR
const char TRIGGER = 7;
const char ECHO = 8;
int Ultrasonic_OnOff = 0;
int Ultrasonic_Distance = 0;

// Buzzer
const int BuzzerPin = 15;


/*------------------------------------------------------------*/
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


/*------------------------------------------------------------*/
void setup() {

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
  buzzerbutton.setPos(1, 3);
  buzzerbutton.setSize(1, 1);
  buzzerbutton.setColor(RC_YELLOW);
  LRemote.addControl(buzzerbutton);

  // Add an Ultrasonic on/off switch
  ultrasonicswitchButton.setText("AUTO");
  ultrasonicswitchButton.setPos(1, 1);
  ultrasonicswitchButton.setSize(1, 1);
  ultrasonicswitchButton.setColor(RC_PINK);
  LRemote.addControl(ultrasonicswitchButton);

  // Start broadcasting our remote contoller
  LRemote.begin();
  Serial.println("LRemote begin ...");
}


/*------------------------------------------------------------*/
void loop() {

  // BLE central device, e.g. an mobile app
  if (!LRemote.connected())
  {
    Serial.println("Waiting for connection ...");
    delay(500);
  }
  else
  {
    delay(10);
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

}


