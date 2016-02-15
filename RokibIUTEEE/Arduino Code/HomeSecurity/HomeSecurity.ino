#include "Lock.h"
#include <SoftwareSerial.h>


//User saved password in mcu
const String pass_phrase = "Rokib";



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
#define led 13

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
enum OPERATION {UNLOCK, LOCK, LOCK_STATUS, VERIFY_PASSWORD, NONE};

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
 * 
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
  
  
}

