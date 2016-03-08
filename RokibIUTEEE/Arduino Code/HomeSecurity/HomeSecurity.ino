#include "Lock.h"
#include <SoftwareSerial.h>


//User saved password in mcu
const String pass_phrase = "Rokib";


//Command for LED
String ledOneOn = "-1ON";
String ledOneOff = "-1OFF";
String ledTwoOn = "-2ON";
String ledTwoOff = "-2OFF";
String ledThreeOn = "-3ON";
String ledThreeOff = "-3OFF";


/*
 * 
 * LED Pins
 */

#define led1 5
#define led2 6
#define led3 7

/*  =============================
 *   Serial Communication
 * ================================
 */

#define RX 10
#define TX 11
#define baud 9600

/*
 * =============================
 *  LOCK
 *  =========================
 *  Ctor:
 *  servo pin, unlock angle, lock angle
 * 
 */

#define UNLOCK_ANGLE 0
#define LOCK_ANGLE 90
#define SERVO_PIN 9 //Use a PWM Pin
#define led 8

Lock locker(SERVO_PIN, UNLOCK_ANGLE, LOCK_ANGLE);


/*
 * ========================
 * PASSWORD
 * =====================
 * 
 */


void unLockIt(void){
  locker.unlock();
}

void lockIt(void){
  locker.lock();
}

String lockStatus(void){
  bool locked = locker.isLocked();
  if (locked == true) return "LOCKED";
  else return "UNLOCKED";
}

//Operations
enum OPERATION {UNLOCK, LOCK, LOCK_STATUS, VERIFY_PASSWORD, NONE, LED_ONE_ON, LED_ONE_OFF, LED_TWO_ON, LED_TWO_OFF, LED_THREE_ON, LED_THREE_OFF};

OPERATION operation;


SoftwareSerial bluetooth(RX, TX);

void setup(void){
  Serial.begin(baud);
  bluetooth.begin(baud);
  bluetooth.println("Initialized");
  Serial.println("Initialized");
  locker.attach();
  locker.lock();
  operation = OPERATION::NONE;
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

/*
 * =====================================
 *          Recognizing the string 
 * =====================================
 * 
 * if incomingString starts with '~' -> Set Password Mode enabled
 * incomingString starts with '#' -> Get status mode enabled
 * incomingString starts with '/' -> Verify password [if matched then opend if not stay closed]
 * incomingString starts with '*' -> 
 * incomingString starts with '-' -> LED operation
 */

String input = "";

void loop(void){

  if (bluetooth.available() > 0){
    String incomingString = bluetooth.readStringUntil('\n');
    
    input = incomingString;    
    Serial.println(input);
    
    if (incomingString.startsWith("/")){
      Serial.println("Operation Unlock");
      operation = OPERATION::UNLOCK;
    } 
    
    else if (incomingString.startsWith("*")){
      Serial.println("Checking lock status");
      operation = OPERATION::LOCK_STATUS;
    } 
    
    else if (incomingString.startsWith("=")){
      Serial.println("Locking");
      operation = OPERATION::LOCK;
    }

    else if (incomingString.startsWith("-")){
      if (incomingString.equals(ledOneOn)){
        operation = LED_ONE_ON;
      } else if (incomingString.equals(ledOneOff)){
        operation = LED_ONE_OFF;
      } else if (incomingString.equals(ledTwoOn)) {
        operation = LED_TWO_ON;
      } else if (incomingString.equals(ledTwoOff)){
        operation = LED_TWO_OFF;
      } else if (incomingString.equals(ledThreeOn)){
        operation = LED_THREE_ON;
      } else if (incomingString.equals(ledThreeOff)){
        operation = LED_THREE_OFF;
      }
    }

    else operation = OPERATION::NONE;
  }


  //Print out lock status
  else if (operation == LOCK_STATUS){
    bluetooth.println(lockStatus());
    operation = OPERATION::NONE;
  }

  //If unlocked lock it
  else if (operation == LOCK){
    if (lockStatus().equals("UNLOCKED")) {
      lockIt();
      bluetooth.println("Locking...");
    }
    operation = OPERATION::NONE;
    digitalWrite(led, LOW); //Turn of the light
  }

  //First check the password if matched open the lock
  else if (operation == UNLOCK){
    String password = input.substring(1);
    if (password.equals(pass_phrase) && lockStatus().equals("LOCKED")) {
      unLockIt();
      Serial.println("Pass phrase matched & unlocking");
      bluetooth.println("Pass phrase matched and unlocking");
      digitalWrite(led, HIGH);
    }
    else {
      bluetooth.println("Pass phrase not matched");
      digitalWrite(led, LOW);
    }
    operation = OPERATION::NONE;
  }

  else if (operation == LED_ONE_ON){
    digitalWrite(led1, HIGH);
    operation = NONE;
  } 

  else if (operation == LED_ONE_OFF){
    digitalWrite(led1, LOW);
    operation = NONE;
  }

  else if (operation == LED_TWO_ON){
    digitalWrite(led2, HIGH);
    operation = NONE;
  }

  else if (operation == LED_TWO_OFF){
    digitalWrite(led2, LOW);
    operation = NONE;
  }

  else if (operation == LED_THREE_ON){
    digitalWrite(led3, HIGH);
    operation = NONE;
  }

  else if (operation == LED_THREE_OFF){
    digitalWrite(led3, LOW);
    operation = NONE;
  }
  
  
}

