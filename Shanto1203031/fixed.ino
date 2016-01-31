#include <SevenSeg.h>

#include <Keypad.h>


char inputs[] = {
  '0', '0', '0', '0'
};


SevenSeg disp(11, 7, 3, 5, 6, 10, 2);

const int numOfDigits = 4;

int digitPins[numOfDigits] = {13, 8, 9, 12};



const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4' , '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//int keys[ROWS][COLS] = {
//  {1, 2, 3, 'A'},
//  {4 , 5, 6, 'B'},
//  {7, 8, 9, 'C'},
//  {'*', 0, '#', 'D'}
//};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {28, 29, 30, 31};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


int k = 0;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  disp.setCommonAnode();
  disp.setDigitPins(numOfDigits, digitPins);
  disp.write(1000);

  
}

int value = 0;

void loop() {

  
//  Serial.println(atoi("1234"));


  k++;

  if (k == 1){
   for (int i = 0; i < 4; i++){
    char in = kpd.waitForKey();
    inputs[i] = in;
  }
  
  value = atoi(inputs);
  }

  char otherKey = kpd.getKey();

  if (otherKey == 'A') { k = 0; Serial.println("reset");}

  disp.write(value);
  
  
} 
