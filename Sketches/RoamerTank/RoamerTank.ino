#include <Servo.h>


#define M1E 6
#define M2E 5
#define M1 8
#define M2 7
#define SPD_PIN A0
#define TURRET_PIN 10
#define OBJ_SENSOR 9

#define M1_OFFSET 0
#define M2_OFFSET 5

const float TURN_DELAY_MILLIS = 500;
const float REVERSE_DELAY_MILLIS = 500;
const float TURRET_MID = 70;
const float TURRET_MAX = 160;
const float TURRET_MIN = 1;
const float TURRET_MILLIS_PER_DEGREE = 2;

float currentTurretPosition = TURRET_MID;

int spd = 50;

volatile byte obstacleState = HIGH;

Servo turret; 

void setup(){
  pinMode(M1E, OUTPUT);
  pinMode(M2E, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(SPD_PIN, INPUT);
  pinMode(TURRET_PIN, OUTPUT);
  pinMode(OBJ_SENSOR, INPUT);
  
  analogWrite(M1E, 0);
  analogWrite(M2E, 0);
  digitalWrite(OBJ_SENSOR, HIGH);
  
  turret.attach(TURRET_PIN);
  turret.write(TURRET_MID);
  
  Serial.begin(9600);
  
}

void loop(){
  spd = map(analogRead(SPD_PIN), 0, 1023, 0, 255);
  CentreTurret();
  obstacleState = digitalRead(OBJ_SENSOR);
 
  while(obstacleState == HIGH){
    spd = map(analogRead(SPD_PIN), 0, 1023, 0, 255);
    Forward();
    TurretSweep();
//    Serial.print(spd);
//    Serial.print(" : ");
//    Serial.println(obstacleState);
   
  }
  MotorsOff();
  
  
  if(currentTurretPosition < TURRET_MID){ //left blocked
    Right();
    delay(floor(((1 - (TURRET_MID - currentTurretPosition)/TURRET_MID))*TURN_DELAY_MILLIS));
    Serial.print(floor(((1 - (TURRET_MID - currentTurretPosition)/TURRET_MID))*TURN_DELAY_MILLIS));
    Serial.print(" = right_delay : ");
  }
  
  if(currentTurretPosition > TURRET_MID){ //right blocked
    Left();
    delay(floor(((TURRET_MAX - currentTurretPosition)/TURRET_MID)*TURN_DELAY_MILLIS));
    Serial.print(floor(((TURRET_MAX - currentTurretPosition)/TURRET_MID)*TURN_DELAY_MILLIS));
    Serial.print(" = left_delay : ");
  }
  
  if(currentTurretPosition == TURRET_MID){ //straight through blocked
    Backward();
    delay(REVERSE_DELAY_MILLIS);
    Left();
    delay(TURN_DELAY_MILLIS);
    
  }
  
  MotorsOff();
  
  
  Serial.print(spd);
  Serial.print(" = speed : ");
  Serial.print(currentTurretPosition);
  Serial.print(" = turret_pos : ");
  Serial.println(obstacleState);
  delay(25);
 
}

void MotorsOn(){
  analogWrite(M1E, spd + M1_OFFSET);
  analogWrite(M2E, spd + M2_OFFSET);
}

void MotorsOff(){
  analogWrite(M1E, 0);
  analogWrite(M2E, 0); 
}

void Forward(){
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  MotorsOn();
}

void Backward(){
  MotorsOff();
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  MotorsOn();
}

void Left(){
  MotorsOff();
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  MotorsOn();
}

void Right(){
  MotorsOff();
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  MotorsOn();
}

void TurretSweep(){
  turret.write(TURRET_MIN);
  delay((currentTurretPosition - TURRET_MIN) * TURRET_MILLIS_PER_DEGREE);
  for(int i = TURRET_MIN; i <= TURRET_MAX; i++){
      turret.write(i);
      delay(TURRET_MILLIS_PER_DEGREE);
      obstacleState = digitalRead(OBJ_SENSOR);
      currentTurretPosition = i;
      if(obstacleState == LOW){
        break;
      }
      
  }
  
  if(obstacleState != LOW) {
    for(int i = TURRET_MAX; i > TURRET_MIN; i--){
        turret.write(i);
        delay(TURRET_MILLIS_PER_DEGREE);
        obstacleState = digitalRead(OBJ_SENSOR);
        currentTurretPosition = i;
        if(obstacleState == LOW){
          break;
        }
        
    }
  }
}

void CentreTurret(){
  turret.write(TURRET_MID);
  delay((max(TURRET_MID,currentTurretPosition) - min(TURRET_MID,currentTurretPosition)) * TURRET_MILLIS_PER_DEGREE);
  currentTurretPosition = TURRET_MID;
}
