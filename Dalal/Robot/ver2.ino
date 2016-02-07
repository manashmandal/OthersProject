#include <SoftwareSerial.h>
#include <Motor.h>
#include <NewPing.h>

#define buzzer 2
#define trig 8
#define echo 7
#define max_dist 100

NewPing frontSonar(trig, echo, max_dist);

int speed = 100;

bool button_control = false;
bool desktop = false;

motorpins pins = {6, 9, 3, 5};

Motor motor(pins);

int led = 13;

SoftwareSerial bt(10, 11); //rx tx

float values = -100;
float yValues = -100;

int x_value = 0;
int y_value = 0;

int direction = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

int getX(String value){
  String x = value.substring(1, value.indexOf(','));
  return x.toInt();
}

int getY(String value){
  String y = value.substring(value.indexOf(',') + 1, value.indexOf(')'));
  return y.toInt();
}

String incomingByte = "";

void loop() {
  // put your main code here, to run repeatedly:
  if (bt.available() > 0){
    //values = bt.parseFloat();
    //yValues = bt.parseFloat();
    
    //Serial.println("x values: " + String(values) + "\n y values: " + String(yValues));
    String compositeData = bt.readStringUntil('\n');
    incomingByte = compositeData;

    Serial.println(incomingByte);
    Serial.println(incomingByte.equals("W"));
    
    if (compositeData.startsWith("~")) {
      button_control = true;
      desktop = false;
      if (compositeData.equals("~FORWARD")) {Serial.println("fwd"); direction = 9000;}
      else if (compositeData.equals("~BACKWARD")) {Serial.println("back"); direction = 10000;}
      else if (compositeData.equals("~LEFT")) {Serial.println("left"); direction = 11000;}
      else if (compositeData.equals("~RIGHT")) {Serial.println("right") ; direction = 12000;}
      else if (compositeData.equals("~STOP")) {Serial.println("stop"); direction = 13000;}
      

    } 

    
    else if (compositeData.startsWith("W") ||
        compositeData.startsWith("S") ||
        compositeData.startsWith("A") ||
        compositeData.startsWith("D") ||
        compositeData.startsWith("q")){
          Serial.println("data got");
          desktop = true; 
        }
    
    
    
    else {
      button_control = false;
      desktop = false;
      x_value = getX(compositeData);
      y_value = getY(compositeData); 
    }
    
//    Serial.println("x: " +  String(x_value));
//    Serial.println(x);
    
    delay(10);
  }

  if (button_control == false){


  if (x_value > -8 && x_value < -3){
   

    Serial.println("FORWARD");
    motor.go(speed, speed, FORWARD);
  }

    else if (x_value > 3 && x_value < 8){
     Serial.println("BACKWARD");
    motor.go(speed, speed, BACKWARD);
  }

    else if (y_value > 3 && y_value < 8){
    Serial.println("LEFT");
    motor.go(0, speed, LEFT);
  }


  else if (y_value > -8 && y_value < -3){
    

    Serial.println("RIGHT");
    motor.go(speed, 0, RIGHT);
  }

  else {
    motor.go(0, 0, NOWHERE);
  }

  } else {

    if (direction == 9000){
      motor.go(speed, speed, FORWARD);
    } else if (direction == 10000){
      motor.go(speed, speed, BACKWARD);
    } else if (direction == 11000){
      
      motor.go(speed, speed, RIGHT);
    } else if (direction == 12000){
      motor.go(speed, speed, LEFT);
    } else if (direction == 13000){
      motor.go(0, 0, NOWHERE);
    }
  }


  if (desktop == true){
    if (incomingByte.equals("W")) { motor.go(speed, speed, FORWARD); Serial.println("forward");}
    else if (incomingByte.equals("S")) { motor.go(speed, speed, BACKWARD); Serial.println("backward"); }
    else if (incomingByte.equals("A")) { motor.go(speed, speed, LEFT); Serial.println("left"); }
    else if (incomingByte.equals("D")) {motor.go(speed, speed, RIGHT); Serial.println("right");}
    else  motor.go(0, 0, NOWHERE);
  }

  if (frontSonar.ping_cm() > 2 && frontSonar.ping_cm() < 10){
    digitalWrite(buzzer, HIGH);
  } else digitalWrite(buzzer, LOW);
  
  
}
