#include <SoftwareSerial.h>

#include <AFMotor.h>


SoftwareSerial bt(50, 51);

#include <Servo.h>
#define GRIPPER_PIN 9
#define ARM_PIN 10

#define baud 9600

enum controlType {DC_MOTOR, GRIPPER, ARM, NONE};

//Default stop
controlType control = controlType::NONE;

String command = "";

//Servos
Servo gripper;
Servo arm;


#define FRONT_LEFT 2
#define FRONT_RIGHT 1
#define BACK_LEFT 3
#define BACK_RIGHT 4

#define spd 255


AF_DCMotor frontLeft(FRONT_LEFT);
AF_DCMotor frontRight(FRONT_RIGHT);
AF_DCMotor backLeft(BACK_LEFT);
AF_DCMotor backRight(BACK_RIGHT);

void forward(void){
  frontLeft.run(FORWARD);
  frontRight.run(FORWARD);
  backLeft.run(FORWARD);
  backRight.run(FORWARD);
}

void backward(void){
  frontLeft.run(BACKWARD);
  frontRight.run(BACKWARD);
  backLeft.run(BACKWARD);
  backRight.run(BACKWARD);
}

void left(void){
  frontRight.run(FORWARD);
  backRight.run(FORWARD);
  frontLeft.run(BACKWARD);
  backLeft.run(BACKWARD);
}

void right(void){
  frontRight.run(BACKWARD);
  backRight.run(BACKWARD);
  frontLeft.run(FORWARD);
  backLeft.run(FORWARD);
}


void stop(void){
  frontLeft.run(RELEASE);
  frontRight.run(RELEASE);
  backLeft.run(RELEASE);
  backRight.run(RELEASE);
}



void setup() {
  Serial.begin(baud);
  bt.begin(baud);
  gripper.attach(GRIPPER_PIN);
  arm.attach(ARM_PIN);

    //Setting speed
  frontLeft.setSpeed(spd);
  frontRight.setSpeed(spd);
  backLeft.setSpeed(spd);
  backRight.setSpeed(spd);
}

int getInt(String s){
  return s.substring(1, s.length()).toInt();
}

void loop() {
  if (bt.available() > 0){
    String compositeData = bt.readStringUntil('\n');
    command = compositeData;
    if (compositeData.startsWith("A")) control = controlType::ARM;
    else if (compositeData.startsWith("G")) control = controlType::GRIPPER;
    else control = controlType::DC_MOTOR;
    
    Serial.println(compositeData);
  }

  if (control == controlType::ARM){
    Serial.println("ARM CONTROLLING");
    Serial.println(getInt(command));
    control = controlType::NONE;
  }

  else if (control == controlType::DC_MOTOR){
    if (command.equals("F")) forward();
    else if (command.equals("B")) backward();
    else if (command.equals("L")) left();
    else if (command.equals("R")) right();
    else stop();

    control = controlType::NONE;
  }

  else if (control == controlType::GRIPPER){
    int angle = 180 - getInt(command);
    gripper.write(angle);
    Serial.println("GRIP Controlling");
    Serial.println(angle);
    control = controlType::NONE;
  }

  else if (control == controlType::ARM){
    int angle = getInt(command);
    arm.write(angle);
    control = controlType::NONE;
  }

}
