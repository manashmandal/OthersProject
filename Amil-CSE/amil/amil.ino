#include <LiquidCrystal.h>
#include <Servo.h>

int set_position = 0;

//Baud rate for serial communication
#define BAUD_RATE 9600


//LCD Object

//Liquid Crystal Pin Definitions
#define RS 7
#define EN 8
#define D0 9
#define D1 10
#define D2 11
#define D3 12
#define COL 16
#define ROW 2

LiquidCrystal lcd(RS, EN, D0, D1, D2, D3);

//Global car counting variable 
unsigned int car_count = 0;

//Servo object and bar pin 
Servo bar;

#define BAR_PIN 3
#define CLOSE_ANGLE 130
#define OPEN_ANGLE 180

bool bar_status = false;

//Ir definitions
#define ENTRY_IR A0
#define EXIT_IR A1

#define THRESHOLD 1000

//Delay for interval in ms
#define INTERVAL 3000

//Delay for bar open and closing interval
#define BAR_INTERVAL 1500

bool entry_object_detected(void){
  if (analogRead(ENTRY_IR) < THRESHOLD){
    return true;
  } else return false;
}

bool exit_object_detected(void){
  if (analogRead(EXIT_IR) < THRESHOLD){
    return true;
  } else return false;
}

void close(void){
  bar.write(CLOSE_ANGLE);
  bar_status = false;
}

void open(void){
  bar.write(OPEN_ANGLE);
  bar_status = true;
}



void update_count(void){
  if (entry_object_detected() == true && exit_object_detected() == false){
    if (bar_status == false) {
      open();
      delay(1000);
      if (exit_object_detected() == true){
        car_count++;
        lcd.setCursor(10, 1);
        lcd.print("     ");
        lcd.setCursor(10, 1);
        lcd.print(car_count);
        Serial.println(car_count);
        delay(3000);
      }
    }
    
  } 

  else {delay(1000); close(); }
}

void setup() {
  //Initializing Serial communication
  Serial.begin(BAUD_RATE);

  //Initializing LiquidCrystal 
  lcd.begin(COL, ROW);
  lcd.begin(COL, ROW);
  lcd.print("Automated Garage");
  lcd.setCursor(0, 1);
  lcd.print("Total Car: ");

  //Initializing servo to the bar_pin [3]
  bar.attach(BAR_PIN);
  bar.write(CLOSE_ANGLE);

  //Initializing rx
  pinMode(ENTRY_IR, INPUT);
  pinMode(EXIT_IR, INPUT);

  lcd.setCursor(10, 1);
  lcd.print("     ");
  lcd.setCursor(10, 1);
  lcd.print(0);
  

  delay(1000);

  close();
}


void loop() {
//  update_count();
  set_position++;

  if (set_position == 1) close();

  else {
    update_count();
  }

  set_position = 2;
//
//  Serial.print("first :");
//  Serial.println(analogRead(A0));
//  delay(1000);
//  Serial.print("second :");
//  Serial.println(analogRead(A1));
//  delay(1000);
//char cmd = 'a';
//
//  if (Serial.available() > 0){
//    cmd = Serial.read();
//  }
//
//  if (cmd == 'o') open();
//  else if (cmd == 'c') close();
}
