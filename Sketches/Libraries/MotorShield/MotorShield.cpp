#include "Arduino.h"
#include "MotorShield.h"

MotorShield::MotorShield() {
  pinMode(M1E, OUTPUT);
  pinMode(M2E, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  m1Dir = LOW;
  m2Dir = HIGH;
  digitalWrite(M1, m1Dir);
  digitalWrite(M2, m2Dir);
  digitalWrite(M1E, LOW);
  digitalWrite(M2E, LOW);
  motorsStop();
}

void MotorShield::motorsRun(int m1Spd, int m2Spd){	
  
  if(m1Spd < 0)
	m1Spd = m1Spd - M1OFFSET;

  if(m1Spd > 0)
	m1Spd = m1Spd + M1OFFSET;

  if(m2Spd < 0)
	m2Spd = m2Spd - M2OFFSET;

  if(m2Spd > 0)
	m2Spd = m2Spd + M2OFFSET;
  
  if(abs(m1Spd) > MAX_SPEED) {
    if(m1Spd < 0) {
	  m1Spd = MAX_SPEED * -1;
	} else {
	  m1Spd = MAX_SPEED;
	}
  }

  if(abs(m2Spd) > MAX_SPEED) {
    if(m2Spd < 0) {
	  m2Spd = MAX_SPEED * -1;
	} else {
	  m2Spd = MAX_SPEED;
	}
  }
  
  m1Dir = (m1Spd < 0);
  m2Dir = (m2Spd > 0);

  
  
  analogWrite(M1E, abs(m1Spd));
  analogWrite(M2E, abs(m2Spd));  
  
}

void MotorShield::motorsStop(){
  analogWrite(M1E, 0);
  analogWrite(M2E, 0);
}
