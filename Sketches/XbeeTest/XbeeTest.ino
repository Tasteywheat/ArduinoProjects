#include <SoftwareSerial.h>

SoftwareSerial softSerial(7,6);
int received = 0;
void setup(){
  Serial.begin(9600);
  softSerial.begin(9600);
}

void loop(){
  received = softSerial.available();
  while (received >0){
    char b = softSerial.read();
    if(b == 13){
      Serial.println();
    }else{
        Serial.print(b);
    }
    received--;
  }
  received = Serial.available();
  while (received >0){
    byte b = Serial.read();
    softSerial.write(b);
    received--;
  }
  delay(15);
    
  
}
