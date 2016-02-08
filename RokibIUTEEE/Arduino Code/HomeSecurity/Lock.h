#ifndef LOCK_H_
#define LOCK_H_
#include <Servo.h>

class Lock{
  private:
    byte servo_pin;
    Servo servo;
    unsigned int unlock_angle;
    unsigned int lock_angle;
    bool is_locked;

  public:
    //Overloaded Ctor
    Lock(byte servoPin);
    Lock(void) { is_locked = false; }
    Lock (byte servoPin, unsigned int unlockAngle, unsigned int lockAngle);
    
    bool isLocked(void);
    void attach(int servoPin);
    void attach(void);
    void unlock(void);
    void lock(void);

    void setLockAngle(unsigned int angle);
    void setUnlockAngle(unsigned int angle);
};

//Ctor
Lock::Lock(byte servoPin){
  servo_pin = servoPin;
  is_locked = false;
}

Lock::Lock( byte servoPin, unsigned int unlockAngle, unsigned int lockAngle){
  servo_pin = servoPin;
  unlock_angle = unlockAngle;
  lock_angle = lockAngle;
  is_locked = false;
}

//unlock the lock
void Lock::unlock(void){
  servo.write(unlock_angle);
  is_locked = false;
}

//Close the lock
void Lock::lock(void){
  servo.write(lock_angle);
  is_locked = true;
}

//Returns if the lock is locked or unlock
bool Lock::isLocked(void){
  return is_locked;
}

//Set the lock angle and 
void Lock::setLockAngle(unsigned int angle){
  lock_angle = angle;
}

//Set the unlock angle
void Lock::setUnlockAngle(unsigned int angle){
  unlock_angle = angle;
}

//Attaches the servo
void Lock::attach(int servoPin){
  servo.attach(servoPin);
}

void Lock::attach(void){
  servo.attach(servo_pin);
}


#endif 
