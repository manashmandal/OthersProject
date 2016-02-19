//#define __DEBUG__

#define threshold 200
#define baud 9600
#define short_del 500
#define long_del 2000

unsigned int new_pin1 = A4;
unsigned int new_pin2 = A5;

const unsigned int red[] = {10, 7, new_pin1, 13}; //r1, r2, r3, r4
const unsigned int yellow[] = {9, 6, new_pin2, 12};
const unsigned int green[] = {8, 5, 4, 11};

int ir1 = 0;
int ir2 = 1;
int ir3 = 2;
int ir4 = 3;

int ir[] = {ir1, ir2, ir3, ir4}; 

bool reading[] = {false, false, false, false};

enum LIGHT {FIRST = 0, SECOND = 1, THIRD = 2, FOURTH = 3, NONE = 4};

LIGHT light;

int index = 4;

void debug(void){
  Serial.println (" == DEBUG BEGIN == ");
  for (int i = 0; i < 4; i++){
    Serial.println("IR [ " + String(i) + "] = " + String(analogRead(ir[i])));
    delay(short_del);
  }
  Serial.println (" == DEBUG END == ");
  Serial.println();
  delay(long_del);
}

void setup() {
  Serial.begin(baud);
  for (int i = 0; i < 4; i++){
    pinMode(red[i], OUTPUT);
    pinMode(yellow[i], OUTPUT);
    pinMode(green[i], OUTPUT);
    pinMode(ir[i], INPUT);
  }
}

int whichLight(void){
  if (analogRead(ir[0]) < threshold) return FIRST;
  else if (analogRead(ir[1]) < threshold) return SECOND;
  else if (analogRead(ir[2]) < threshold) return THIRD;
  else if (analogRead(ir[3]) < threshold) return FOURTH;
  else return NONE;
}

void offAll (void){
  for (int i = 0; i < 4; i++){
    digitalWrite (red[i], LOW);
    digitalWrite (yellow[i], LOW);
    digitalWrite (green[i], LOW);
  }
}

void firstLight(void){
  offAll();
  digitalWrite (green[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[FOURTH], HIGH);
}

void secondLight(void){
  offAll();
  digitalWrite(green[SECOND], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[FOURTH], HIGH);
}


void thirdLight(void){
  offAll();
  digitalWrite(green[THIRD], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[FOURTH], HIGH);
}

void fourthLight(void){
  offAll();
  digitalWrite(green[FOURTH], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[SECOND], HIGH);
}


void onAll(void){
  for (int i = 0; i < 4; i++) {
    digitalWrite(red[i], HIGH);
    digitalWrite(yellow[i], HIGH);
    digitalWrite(green[i], HIGH);
  }
}


void onIdle(void){

  index = whichLight();
  
  // For first light
  digitalWrite(green[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[FOURTH], HIGH);
  delay(long_del);
  digitalWrite(green[FIRST], LOW);
  digitalWrite(red[SECOND], LOW);
  digitalWrite(red[THIRD], LOW);
  digitalWrite(red[FOURTH], LOW);
  
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
  delay(short_del);
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

  index = whichLight();

  //For second
  digitalWrite(green[SECOND], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[FOURTH], HIGH);
  delay(long_del);
  digitalWrite(green[SECOND], LOW);
  digitalWrite(red[FIRST], LOW);
  digitalWrite(red[THIRD], LOW);
  digitalWrite(red[FOURTH], LOW);
  
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
  delay(short_del);
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

  index = whichLight();

  //For third
  digitalWrite(green[THIRD], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[FOURTH], HIGH);
  delay(long_del);
  digitalWrite(green[THIRD], LOW);
  digitalWrite(red[FIRST], LOW);
  digitalWrite(red[SECOND], LOW);
  digitalWrite(red[FOURTH], LOW);
  
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
  delay(short_del);
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

  index = whichLight();

  //For fourth
  digitalWrite(green[FOURTH], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[THIRD], HIGH);
  delay(long_del);
  digitalWrite(green[FOURTH], LOW);
  digitalWrite(red[FIRST], LOW);
  digitalWrite(red[SECOND], LOW);
  digitalWrite(red[THIRD], LOW);

  index = whichLight();
  
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
  delay(short_del);
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

}

#ifdef __DEBUG__
void loop() { onAll(); }

#else



void loop(){
  
  int index = 4;

  index = whichLight();
  
  while(index != NONE){
    index = whichLight();
    switch(index){
      case FIRST:
        firstLight();
        break;
      case SECOND:
        secondLight();
        break;

      case THIRD:
        thirdLight();
        break;

      case FOURTH:
        fourthLight();
        break;

      default:
        offAll();
        break;
    }
  }

  while(whichLight() == NONE){
    
  if (whichLight() != NONE){
    offAll();
    break;
  }

      // For first light
  digitalWrite(green[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[FOURTH], HIGH);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  delay(long_del);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  digitalWrite(green[FIRST], LOW);
  digitalWrite(red[SECOND], LOW);
  digitalWrite(red[THIRD], LOW);
  digitalWrite(red[FOURTH], LOW);
  index = whichLight();
  
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
  index = whichLight();
  delay(short_del);
  index = whichLight();
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

  index = whichLight();

  //For second
  digitalWrite(green[SECOND], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[THIRD], HIGH);
  digitalWrite(red[FOURTH], HIGH);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  delay(long_del);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  digitalWrite(green[SECOND], LOW);
  digitalWrite(red[FIRST], LOW);
  digitalWrite(red[THIRD], LOW);
  digitalWrite(red[FOURTH], LOW);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  delay(short_del);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

    if (whichLight() != NONE){
    offAll();
    break;
  }

  //For third
  digitalWrite(green[THIRD], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[FOURTH], HIGH);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  delay(long_del);
  digitalWrite(green[THIRD], LOW);
  digitalWrite(red[FIRST], LOW);
  digitalWrite(red[SECOND], LOW);
  digitalWrite(red[FOURTH], LOW);

    if (whichLight() != NONE){
    offAll();
    break;
  }
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  delay(short_del);
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);
    if (whichLight() != NONE){
    offAll();
    break;
  }


  //For fourth
  digitalWrite(green[FOURTH], HIGH);
  digitalWrite(red[FIRST], HIGH);
  digitalWrite(red[SECOND], HIGH);
  digitalWrite(red[THIRD], HIGH);
  
    if (whichLight() != NONE){
    offAll();
    break;
  }
  
  delay(long_del);
  digitalWrite(green[FOURTH], LOW);
  digitalWrite(red[FIRST], LOW);
  digitalWrite(red[SECOND], LOW);
  digitalWrite(red[THIRD], LOW);

    if (whichLight() != NONE){
    offAll();
    break;
  }
  
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], HIGH);
    if (whichLight() != NONE){
    offAll();
    break;
  }
  delay(short_del);
  for (int i = 0; i < 4; i++) digitalWrite(yellow[i], LOW);

    if (whichLight() != NONE){
    offAll();
    break;
  }
    
  }
}


#endif

