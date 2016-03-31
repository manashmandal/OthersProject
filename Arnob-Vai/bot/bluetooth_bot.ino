// ****** CONNECT THE LEFT TWO MOTORS TO CHANNEL 1 AND 3........... CONNECT THE RIGHT TWO MOTORS TO CHANNEL 2 AND 4***********
// ****** IF MOTORS DO NOT MOVE IN THE DESIRED DIRECTION, CHECK AND REVERSE THE WIRING IF NEEDED***************

// Using Joystick Commander V5.0 and AndroTest V2.0 with 6 buttons + Joystick control
//#include <Servo.h>
//#include "SoftwareSerial.h"

// Arduino pins for the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// 8-bit bus after the 74HC595 shift register
// (not Arduino pins)
// These are used to set the direction of the bridge driver.
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4
#define MOTOR3_A 5
#define MOTOR3_B 7
#define MOTOR4_A 0
#define MOTOR4_B 6

// Arduino pins for the PWM signals.
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3
#define MOTOR3_PWM 6
#define MOTOR4_PWM 5
#define SERVO1_PWM 10
#define SERVO2_PWM 9

// Codes for the motor function.
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4


//int SERVO3_PWM = 22;
// Declare classes for Servo connectors of the MotorShield.
//Servo servo_1;
//Servo servo_2;
//Servo servo_3;

int inByte = 0;

//int operate = 0;

//int pos1 = 90;
//int pos2 = 90;
//int pos3 = 90;

//int ser1;
//int ser2;
//int ser3;

int speed_val = 255;

#define    STX          0x02
#define    ETX          0x03
#define    ledPin       13
#define    SLOW         750                            // Datafields refresh rate (ms)
#define    FAST         250                             // Datafields refresh rate (ms)

//SoftwareSerial Serial(6, 7);                          // BlueTooth module: pin#2=TX pin#3=RX
byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};                 // bytes received
byte buttonStatus = 0;                                  // first Byte sent to Android device
long previousMillis = 0;                                // will store last time Buttons status was updated
long sendInterval = SLOW;                               // interval between Buttons status transmission (milliseconds)
String displayStatus = "xxxx";                          // message to Android device

void setup()  {
  Serial.begin(9600);
  //Serial.begin(38400);                                // 38400 = max value for softserial
  pinMode(ledPin, OUTPUT);
  //Serial.println(VERSION);
  Serial.flush();
  while (Serial.available())  Serial.read();        // empty RX buffer

  // Use the default "Servo" library of Arduino.
  // Attach the pin number to the servo library.
  // This might also set the servo in the middle position.
  //servo_1.attach(SERVO1_PWM);
  //servo_2.attach(SERVO2_PWM);
  //servo_3.attach(SERVO3_PWM);

  //servo_1.write(pos1);
  //servo_2.write(pos2);
  //servo_3.write(pos3);

  //turn motors off by default
  motor(1, RELEASE, 0);
  motor(2, RELEASE, 0);
  motor(3, RELEASE, 0);
  motor(4, RELEASE, 0);
  delay(500);
}

void loop() {
  if (Serial.available())
  { // data received from smartphone
    delay(2);
    cmd[0] =  Serial.read();
    if (cmd[0] == STX)
    {
      int i = 1;
      while (Serial.available())
      {
        delay(1);
        cmd[i] = Serial.read();
        if (cmd[i] > 127 || i > 7)                 break; // Communication error
        if ((cmd[i] == ETX) && (i == 2 || i == 7))   break; // Button or Joystick data
        i++;
      }
      if     (i == 2)          getButtonState(cmd[1]);  // 3 Bytes  ex: < STX "C" ETX >
      else if (i == 7)          getJoystickState(cmd);  // 6 Bytes  ex: < STX "200" "180" ETX >
    }
  }
  sendBlueToothData();
}

void sendBlueToothData()
{
  static long previousMillis = 0;
  long currentMillis = millis();
  if (currentMillis - previousMillis > sendInterval) // send data back to smartphone
  {
    previousMillis = currentMillis;

    // Data frame transmitted back from Arduino to Android device:
    // < 0X02   Buttons state   0X01   DataField#1   0x04   DataField#2   0x05   DataField#3    0x03 >
    // < 0X02      "01011"      0X01     "120.00"    0x04     "-4500"     0x05  "Motor enabled" 0x03 >    // example

    Serial.print((char)STX);                                             // Start of Transmission
    Serial.print(getButtonStatusString());  Serial.print((char)0x1);   // buttons status feedback
    Serial.print(GetdataInt1());            Serial.print((char)0x4);   // datafield #1
    Serial.print(GetdataFloat2());          Serial.print((char)0x5);   // datafield #2
    Serial.print(displayStatus);                                         // datafield #3
    Serial.print((char)ETX);                                             // End of Transmission
  }
}

String getButtonStatusString()
{
  String bStatus = "";
  for (int i = 0; i < 6; i++)
  {
    if (buttonStatus & (B100000 >> i))      bStatus += "1";
    else                                  bStatus += "0";
  }
  return bStatus;
}

int GetdataInt1()
{ // Data dummy values sent to Android device for demo purpose
  static int i = -30;             // Replace with your own code
  i ++;
  if (i > 0)    i = -30;
  return i;
}

float GetdataFloat2()
{ // Data dummy values sent to Android device for demo purpose
  static float i = 50;             // Replace with your own code
  i -= .5;
  if (i < -50)    i = 50;
  return i;
}

void getJoystickState(byte data[8])
{
  int joyX = (data[1] - 48) * 100 + (data[2] - 48) * 10 + (data[3] - 48); // obtain the Int from the ASCII representation
  int joyY = (data[4] - 48) * 100 + (data[5] - 48) * 10 + (data[6] - 48);
  joyX = joyX - 200;                                                  // Offset to avoid
  joyY = joyY - 200;                                                  // transmitting negative numbers

  if (joyX < -100 || joyX > 100 || joyY < -100 || joyY > 100)     return; // commmunication error

  // Your code here ...

  if (joyY > 15)
  {
    joyY = map(joyY, 0, 100, 0, 255);
    mfor(joyY);
  }
  else if (joyX < -15)
  {
    joyX = map((joyX) * (-1), 0, 100, 0, 255);
    mleft(joyX);
  }
  else if (joyX > 15)
  {
    joyX = map(joyX, 0, 100, 0, 255);
    mright(joyX);
  }
  else if (joyY < -15 )
  {
    joyY = map((joyY) * (-1), 0, 100, 0, 255);
    mback(joyY);
  }
  else if (joyX == 0 && joyY == 0)
  {
    mstop();
  }
  Serial.print("Joystick position:  ");
  Serial.print(joyX);
  Serial.print(", ");
  Serial.println(joyY);
}

void getButtonState(int bStatus)
{
  switch (bStatus) {
    // -----------------  BUTTON #1  -----------------------
    case 'A':
      buttonStatus |= B000001;        // ON
      Serial.println("\n** Button_1: ON **");
      displayStatus = "DRIVE MODE";
      Serial.println(displayStatus);
      //operate = 1;
      break;
    case 'B':
      buttonStatus &= B111110;        // OFF
      Serial.println("\n** Button_1: OFF **");
      displayStatus = "ARM MODE";
      Serial.println(displayStatus);
      //operate = 0;
      break;

    // -----------------  BUTTON #2  -----------------------
    case 'C':
      buttonStatus |= B000010;        // ON
      Serial.println("\n** Button_2: ON **");
      //operate = 0;
      displayStatus = "GOKU";
      Serial.println(displayStatus);
      break;
    case 'D':
      buttonStatus &= B111101;        // OFF
      Serial.println("\n** Button_2: OFF **");
      //operate = 2;
      displayStatus = "GOHAN";
      Serial.println(displayStatus);
      break;

    // -----------------  BUTTON #3  -----------------------
    case 'E':
      buttonStatus |= B000100;        // ON
      //Serial.println("\n** Button_3: ON **");
      // your code...
      displayStatus = "Motor #1 enabled"; // Demo text message
      //Serial.println(displayStatus);
      break;
    case 'F':
      buttonStatus &= B111011;      // OFF
      //Serial.println("\n** Button_3: OFF **");
      // your code...
      displayStatus = "Motor #1 stopped";
      //Serial.println(displayStatus);
      break;

    // -----------------  BUTTON #4  -----------------------
    case 'G':
      buttonStatus |= B001000;       // ON
      Serial.println("\n** Button_4: ON **");
      // your code...
      displayStatus = "Datafield update <FAST>";
      Serial.println(displayStatus);
      sendInterval = FAST;
      break;
    case 'H':
      buttonStatus &= B110111;    // OFF
      Serial.println("\n** Button_4: OFF **");
      // your code...
      displayStatus = "Datafield update <SLOW>";
      Serial.println(displayStatus);
      sendInterval = SLOW;
      break;

    // -----------------  BUTTON #5  -----------------------
    case 'I':           // configured as momentary button
      //      buttonStatus |= B010000;        // ON
      Serial.println("\n** Button_5: ++ pushed ++ **");
      // your code...
      displayStatus = "Button5: <pushed>";
      break;
    //   case 'J':
    //     buttonStatus &= B101111;        // OFF
    //     // your code...
    //     break;

    // -----------------  BUTTON #6  -----------------------
    case 'K':
      buttonStatus |= B100000;        // ON
      Serial.println("\n** Button_6: ON **");
      // your code...
      displayStatus = "Button6 <ON>"; // Demo text message
      break;
    case 'L':
      buttonStatus &= B011111;        // OFF
      Serial.println("\n** Button_6: OFF **");
      // your code...
      displayStatus = "Button6 <OFF>";
      break;
  }
  // ---------------------------------------------------------------
}

// Initializing
// ------------
// There is no initialization function.
//
// The shiftWrite() has an automatic initializing.
// The PWM outputs are floating during startup,
// that's okay for the Motor Shield, it stays off.
// Using analogWrite() without pinMode() is valid.
//


// ---------------------------------
// motor
//
// Select the motor (1-4), the command,
// and the speed (0-255).
// The commands are: FORWARD, BACKWARD, BRAKE, RELEASE.
//
void motor(int nMotor, int command, int speed)
{
  int motorA, motorB;

  if (nMotor >= 1 && nMotor <= 4)
  {
    switch (nMotor)
    {
      case 1:
        motorA   = MOTOR1_A;
        motorB   = MOTOR1_B;
        break;
      case 2:
        motorA   = MOTOR2_A;
        motorB   = MOTOR2_B;
        break;
      case 3:
        motorA   = MOTOR3_A;
        motorB   = MOTOR3_B;
        break;
      case 4:
        motorA   = MOTOR4_A;
        motorB   = MOTOR4_B;
        break;
      default:
        break;
    }

    switch (command)
    {
      case FORWARD:
        motor_output (motorA, HIGH, speed);
        motor_output (motorB, LOW, -1);     // -1: no PWM set
        break;
      case BACKWARD:
        motor_output (motorA, LOW, speed);
        motor_output (motorB, HIGH, -1);    // -1: no PWM set
        break;
      case BRAKE:
        // The AdaFruit library didn't implement a brake.
        // The L293D motor driver ic doesn't have a good
        // brake anyway.
        // It uses transistors inside, and not mosfets.
        // Some use a software break, by using a short
        // reverse voltage.
        // This brake will try to brake, by enabling
        // the output and by pulling both outputs to ground.
        // But it isn't a good break.
        motor_output (motorA, LOW, 255); // 255: fully on.
        motor_output (motorB, LOW, -1);  // -1: no PWM set
        break;
      case RELEASE:
        motor_output (motorA, LOW, 0);  // 0: output floating.
        motor_output (motorB, LOW, -1); // -1: no PWM set
        break;
      default:
        break;
    }
  }
}


// ---------------------------------
// motor_output
//
// The function motor_ouput uses the motor driver to
// drive normal outputs like lights, relays, solenoids,
// DC motors (but not in reverse).
//
// It is also used as an internal helper function
// for the motor() function.
//
// The high_low variable should be set 'HIGH'
// to drive lights, etc.
// It can be set 'LOW', to switch it off,
// but also a 'speed' of 0 will switch it off.
//
// The 'speed' sets the PWM for 0...255, and is for
// both pins of the motor output.
//   For example, if motor 3 side 'A' is used to for a
//   dimmed light at 50% (speed is 128), also the
//   motor 3 side 'B' output will be dimmed for 50%.
// Set to 0 for completelty off (high impedance).
// Set to 255 for fully on.
// Special settings for the PWM speed:
//    Set to -1 for not setting the PWM at all.
//
void motor_output (int output, int high_low, int speed)
{
  int motorPWM;

  switch (output)
  {
    case MOTOR1_A:
    case MOTOR1_B:
      motorPWM = MOTOR1_PWM;
      break;
    case MOTOR2_A:
    case MOTOR2_B:
      motorPWM = MOTOR2_PWM;
      break;
    case MOTOR3_A:
    case MOTOR3_B:
      motorPWM = MOTOR3_PWM;
      break;
    case MOTOR4_A:
    case MOTOR4_B:
      motorPWM = MOTOR4_PWM;
      break;
    default:
      // Use speed as error flag, -3333 = invalid output.
      speed = -3333;
      break;
  }

  if (speed != -3333)
  {
    // Set the direction with the shift register
    // on the MotorShield, even if the speed = -1.
    // In that case the direction will be set, but
    // not the PWM.
    shiftWrite(output, high_low);

    // set PWM only if it is valid
    if (speed >= 0 && speed <= 255)
    {
      analogWrite(motorPWM, speed);
    }
  }
}


// ---------------------------------
// shiftWrite
//
// The parameters are just like digitalWrite().
//
// The output is the pin 0...7 (the pin behind
// the shift register).
// The second parameter is HIGH or LOW.
//
// There is no initialization function.
// Initialization is automatically done at the first
// time it is used.
//
void shiftWrite(int output, int high_low)
{
  static int latch_copy;
  static int shift_register_initialized = false;

  // Do the initialization on the fly,
  // at the first time it is used.
  if (!shift_register_initialized)
  {
    // Set pins for shift register to output
    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);

    // Set pins for shift register to default value (low);
    digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORCLK, LOW);
    // Enable the shift register, set Enable pin Low.
    digitalWrite(MOTORENABLE, LOW);

    // start with all outputs (of the shift register) low
    latch_copy = 0;

    shift_register_initialized = true;
  }

  // The defines HIGH and LOW are 1 and 0.
  // So this is valid.

  bitWrite(latch_copy, output, high_low);

  // Use the default Arduino 'shiftOut()' function to
  // shift the bits with the MOTORCLK as clock pulse.
  // The 74HC595 shiftregister wants the MSB first.
  // After that, generate a latch pulse with MOTORLATCH.
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, HIGH);
  delayMicroseconds(5);    // For safety, not really needed.
  digitalWrite(MOTORLATCH, LOW);
}

void mfor(int spd)
{
  motor(1, FORWARD, spd);
  motor(2, FORWARD, spd);
  motor(3, FORWARD, spd);
  motor(4, FORWARD, spd);
  delay(20);
}

void mback(int spd)
{
  motor(1, BACKWARD, spd);
  motor(2, BACKWARD, spd);
  motor(3, BACKWARD, spd);
  motor(4, BACKWARD, spd);
  delay(20);
}

void mleft(int spd)
{
  motor(1, BACKWARD, spd);
  motor(2, FORWARD, spd);
  motor(3, BACKWARD, spd);
  motor(4, FORWARD, spd);
  delay(20);
}

void mright(int spd)
{
  motor(1, FORWARD, spd);
  motor(2, BACKWARD, spd);
  motor(3, FORWARD, spd);
  motor(4, BACKWARD, spd);
  delay(20);
}

void mstop()
{
  motor(1, BRAKE, 0);
  motor(2, BRAKE, 0);
  motor(3, BRAKE, 0);
  motor(4, BRAKE, 0);
  delay(20);
}
