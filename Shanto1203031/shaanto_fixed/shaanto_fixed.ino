//#include <Keypad.h>
//#include "SevenSegment.h"
//
//SevenSeg disp(11, 7, 3, 5, 6, 10, 2);
//
//const int numOfDigits = 4;
//
//int digitPins[numOfDigits] = {13, 8, 9, 12};
//
//const byte ROWS = 4;
//const byte COLS = 4;
//
//char inputs[] = {
//  '0', '0', '0', '0'
//};
//
//
//char keys[ROWS][COLS] = {
//  {'1', '2', '3', 'A'},
//  {'4' , '5', '6', 'B'},
//  {'7', '8', '9', 'C'},
//  {'*', '0', '#', 'D'}
//};
//
//byte rowPins[ROWS] = {15, 16, 17, 18};
//byte colPins[COLS] = {19, 20, 21, 22};
//
//Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//
//
//int k = 0;
//
//int value = 0;
//
//void setup() {
//  // put your setup code here, to run once:
//  disp.setCommonAnode();
//  disp.setDigitPins(numOfDigits, digitPins);
//  disp.write(1000);
//  delay(4000);
//}
//
//void loop() {
//    k++;
//
//  if (k == 1){
//   for (int i = 0; i < 4; i++){
//    char in = kpd.waitForKey();
//    inputs[i] = in;
//  }
//  
//  value = atoi(inputs);
//  }
//
//  char otherKey = kpd.getKey();
//
//  if (otherKey == 'A') { k = 0;}
//
//  disp.write(value);
//  
//}


#include "SevenSegment.h"
#include <Keypad.h>

#define ROWS 4
#define COLS 4

SevenSeg disp(11, 7, 3, 5, 6, 10, 2);

const int numOfDigits = 4;
//
int digitPins[numOfDigits] = {13, 8, 9, 12};

char inputs[] = {
  '0', '0', '0', '0'
};


char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4' , '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//byte rowPins[ROWS] = {15, 16, 17, 18};
//byte colPins[COLS] = {19, 20, 21, 22};

byte rowPins[ROWS] = {24, 25, 26, 27};
byte colPins[COLS] = {28, 29, 30, 31};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


int k = 0;

int value = 0;


void setup() {
  // put your setup code here, to run once:
  disp.setCommonAnode();
  disp.setDigitPins(numOfDigits, digitPins);
}

char key = '1';

void loop(){
      k++;

  if (k == 1){
   for (int i = 0; i < 4; i++){
    char in = kpd.waitForKey();
    inputs[i] = in;
  }
  
  value = atoi(inputs);
  }

  char otherKey = kpd.getKey();

  if (otherKey == 'A') { k = 0;}

  disp.write(value);

//  disp.write(9999);

}


