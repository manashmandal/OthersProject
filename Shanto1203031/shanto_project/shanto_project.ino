 #include <Keypad.h>

#define ROWS 4
#define COLS 4

#include "SevenSegment.h"

SevenSeg disp (11 ,7 ,3 , 5 ,6 ,10 ,2);

const int numOfDigits =7;
int digitPins [ numOfDigits ]={12 ,9 ,8 ,13 ,14 ,15 ,16};



char inputs[] = {
  '0', '0', '0', '0', '0', '0', '0','\0'
};


byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4' , '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//byte rowPins[ROWS] = {15, 16, 17, 18};
//byte colPins[COLS] = {19, 20, 21, 22};



Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);




void setup () {
 //setRefreshRate (1000) ;
 disp.setDigitPins ( numOfDigits , digitPins );
}







int k = 0;

int value = 0;



char key = '1';

void loop(){
      k++;

  if (k == 1){
   for (int i = 0; i < 7; i++){
    char in = kpd.waitForKey();
    inputs[i] = in;
  }
  }

  char otherKey = kpd.getKey();

  if (otherKey == 'A') { k = 0;}

  disp.write(inputs);

//  disp.write(9999);


}
