//Code for common anode


const int A = 11;
const int B = 7;
const int C = 3;
const int D = 5;
const int E = 6;
const int F = 10;
const int G = 2;

int segments[] = {A, B, C, D, E, F, G};
int digits[] = {12, 9, 8, 13};

void digitOn(byte index){
  digitalWrite(digits[index-1], HIGH);
}

void digitOff(byte index){
  digitalWrite(digits[index-1], LOW);
}


void clear(void){
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  digitalWrite(E, HIGH);
  digitalWrite(F, HIGH);
  digitalWrite(G, HIGH);
}

void eight(void){
  clear();
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
}

void zero(void){
  clear();
  eight();
  digitalWrite(G, HIGH);  
}

void nine(void){
  clear();
  eight();
  digitalWrite(E, HIGH);
}

void three(void){
  clear();
  nine();
  digitalWrite(F, HIGH);
}

void five(void){
  clear();
  three();
  digitalWrite(F, LOW);
  digitalWrite(B, HIGH);
}

void two(void){
  clear();
  five();
  digitalWrite(B, LOW);
  digitalWrite(F, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(C, HIGH);
}

void six(void){
  clear();
  five();
  digitalWrite(E, LOW);
}


void setup() {
  for (int i = 0; i < 7; i++) pinMode(segments[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digits[i], OUTPUT);

  digitOn(4);

  six();
}

void loop() {
  

}
