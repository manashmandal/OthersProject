#include <SoftwareSerial.h>

#define SHORT_DELAY 200
#define LONG_DELAY 5000

#define RX 10 //SoftwareSerial RX pin
#define TX 11 //SoftwareSerial TX pin
#define BAUD 9600 //Baudrate for Serial and SoftwareSerial

//Bluetooth object
SoftwareSerial bluetooth(RX, TX);

#define ledPin 13 //choose the pin for LED
#define inputPin 7 // choose the input pin (for PIR Sensor)

int pirState = LOW; //Assuming no motion detected at starting point

int val = 0; //Variable for reading the pin status

void setup(void){
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  bluetooth.begin(BAUD);
  Serial.begin(BAUD);
}

//Strings for motion detection and threat alert
char* motionDetected = "Motion detected!";
char* potentialThreat = "Potential Threat Alert!";
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println(motionDetected);
      bluetooth.println(motionDetected);
      // We only want to print on the output change, not state
      pirState = HIGH;
      delay(LONG_DELAY);
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println(potentialThreat);
      bluetooth.println(potentialThreat);
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }

  delay(SHORT_DELAY);
}
