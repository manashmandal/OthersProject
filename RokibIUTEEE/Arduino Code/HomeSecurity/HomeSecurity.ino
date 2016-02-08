#include "Lock.h"

#include <SoftwareSerial.h>



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

Lock locker(SERVO_PIN, UNLOCK_ANGLE, LOCK_ANGLE);


/*
 * ========================
 * PASSWORD
 * =====================
 * 
 */

String pass_phrase = "";

void setPassPhrase(String key){
  pass_phrase = key;
}

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


SoftwareSerial bluetooth(RX, TX);

void setup(void){
  Serial.begin(baud);
  bluetooth.begin(baud);
  locker.attach();
  locker.lock();
}

void loop(void){

  if (bluetooth.available() > 0){
    String incomingString = bluetooth.readStringUntil('\n');

    if (incomingString.startsWith("~")){
      setPassPhrase(incomingString);
    } else if (incomingString.startsWith("/")){
      bluetooth.println("Lock status " + lockStatus());
    } else if (incomingString.startsWith("*")){
      if (incomingString.equals(pass_phrase)) locker.unLock()
    }
  }
  
}

