#include "LiquidCrystal.h"

int temperature = 0;
int speed = 0;

#define RS 2
#define EN 3
#define D0 4
#define D1 5
#define D2 6
#define D3 7

#define LM35 A0

int motor_left = 11;
int motor_right = 9;

void fan(int speed){
  analogWrite(motor_left, speed);
  digitalWrite(motor_right, LOW);
}


LiquidCrystal lcd(RS, EN, D0, D1, D2, D3);

void show_temperature(void) {
  temperature = analogRead(LM35);
  int temp = temperature * 0.4882815;
  lcd.setCursor(12, 0);
  lcd.print((temp));
  speed = map(temp, 15 , 37, 0, 255);
  Serial.println(speed);
  fan(speed);
  delay(1000);
}



void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Temperature: ");
  lcd.setCursor(12, 0);
  lcd.print("      ");
  lcd.print("20");


  //I/O Definition
  pinMode(LM35, INPUT);
  pinMode(motor_left, OUTPUT);
  pinMode(motor_right, OUTPUT);

}

void loop()
{
  show_temperature();



 
  

  /* add main program code here */

}

