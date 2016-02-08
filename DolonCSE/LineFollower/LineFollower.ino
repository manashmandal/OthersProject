/*
 * for pos = 3000; forward
 * for pos = 2500; slight left
 * for pos = 1000; hard left
 * for pos = 3500; slight right
 * for pos = 4000; medium right
 * 
 */


#include "Motor.h"
#define baud 9600
#define spd 255
#define del 500
#define mid_del 1000
#define high_del 1500

#define threshold 800

#define __debug__
//#define __analog__



//Motor pins setup
motorPins pins = {3, 6, 10, 9};

//Motors
Motor *motor;


int active_sensors = 0;

int sensors[] = {A0, A1, A2, A3, A4};

int analog_reading[] = {0, 0, 0, 0, 0};
int digital_reading[] = {0, 0, 0, 0, 0};


void read_line(void){
  active_sensors = 0;
  for (int i = 0; i < 5; i++){
    analog_reading[i] = analogRead(sensors[i]);
    if ((analog_reading[i]) > threshold) { digital_reading[i] = 1; active_sensors++; }
    else digital_reading[i] = 0;
  }
}

int get_position(void){
  unsigned int pos = 0;
  read_line();
  for (int i = 0; i < 5; i++){
    pos += digital_reading[i] * (i + 1) * 1000;
  }
  return pos / active_sensors;
}


void line_follow(void){
  int position = get_position();
  if (position == 3000){
    motor->go(spd, spd, FORWARD);
  } else if (position > 2000 && position < 3000){
    motor->go(spd - 20, spd - 20, ANTICLOCKWISE);
  } else if (position > 500 && position < 2000){
    motor->go(spd, spd, ANTICLOCKWISE);
  } else if (position > 3000 && position < 4000){
    motor->go(spd - 20, spd - 20, CLOCKWISE);
  } else if (position > 3500 && position < 5000){
    motor->go(spd, spd, CLOCKWISE);
  } else if (position == -1) motor->go(0, 0, NOWHERE);
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(baud);
  for (int i = 0; i < 5; i++) pinMode(sensors[i], INPUT);
  motor = new Motor(pins);
//  motor->go(spd, spd, FORWARD);
}

void debug(){

  #ifdef __analog__
  
  Serial.println(" ========== ANALOG VALUE ======= ");
  Serial.println(" = = = = begin = = = = ");
  Serial.println();
  for (int i = 0; i < 5; i++){
    Serial.println("sensor [ " + String(i) + " ] = " + String(analogRead(sensors[i])));
  }
  Serial.println(" = = = = end = = = = ");
  Serial.println();
  delay(mid_del);

  #else

  Serial.println(" = = = = Digital Value = = = = ");
  read_line();
  for (int i = 0; i < 5; i++) Serial.print(digital_reading[i] + String(" "));
  Serial.println();
  Serial.println();
  Serial.println("position value : " + String(get_position()));
  Serial.println(" ====== end ======= ");

  delay(mid_del);

  #endif
}

#ifdef __debug__
void loop(){
  debug();
}

#else

void loop() {
  // put your main code here, to run repeatedly:
  line_follow();

}

#endif
