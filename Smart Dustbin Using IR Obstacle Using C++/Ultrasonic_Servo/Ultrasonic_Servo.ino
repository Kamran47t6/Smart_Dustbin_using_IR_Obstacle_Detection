
#include <Servo.h>
const int analogInPin = A0; 
int sensorValue = 0;               
int servoPin = 9;
Servo servo;

void setup() {
 
servo.attach(servoPin);
}

void loop() {
 
  sensorValue = analogRead(analogInPin);           
 
  if(sensorValue < 600){
   
    servo.write(90);
    
   }
   else
   {
    servo.write(0);
   }
  }