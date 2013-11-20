
#include <Stepper.h>

volatile long chops = 0;
Stepper stepper(96, 4,5,6,7);
int speedPin = A5;
int currentIndex = 0; 
int val = 0;
int previous = 0;
void setup(){
  
  Serial.begin(9600);
  //attachInterrupt(0, count, FALLING);
  pinMode(speedPin, INPUT);
 
}

void loop(){

  val = map(analogRead(speedPin), 0, 1023, -180, 180);
  stepper.setSpeed(abs(val));

  if(val < -20){
    stepper.step(-10);      
  }else if (val > 20) {
    stepper.step(10);
  }else{
    stepper.step(0);    
  } 
  Serial.println(val);
  

  
}

void count(){
  chops++;
}


