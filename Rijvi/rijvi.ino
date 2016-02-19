#include "Servo.h"
int x,y,z;

unsigned long T2;
Servo myservo;
void setup()
{
  Serial.begin(9600);
   pinMode( A1,INPUT);
   pinMode( A2,INPUT);
   pinMode( A3,INPUT);
   pinMode( 15,OUTPUT);
     pinMode( 16,OUTPUT);
    myservo.attach(13);
}
void reading()
{
  x=digitalRead(A1);
  y=digitalRead(A2);
  z=digitalRead(A3);
    
//    Serial.println(" Sensor1: " + String(x));
//    Serial.println(" Sensor2: " + String(y));
//    Serial.println(" Sensor3: " + String(z));
//   
  }
void(*resetFunc)(void)=0;
void loop()
{
  reading();

   while(x==1 && y!=1)
   {
    T2=millis();
    Serial.println("T2:"+String(T2));
   break;}
   if(T2<3000&&y==1&&x==1)
    {
     digitalWrite(15,HIGH);
     digitalWrite(16,LOW);
     myservo.write(70);
     delay(3000);
        
    }
   if(y==0&&z==0)
   {digitalWrite(15,LOW);
   digitalWrite(16,HIGH);
   delay(3);
   myservo.write(177);
   } 
   if(z==1 && x==0 && y==0)
   {
    resetFunc();
   }
   
}


