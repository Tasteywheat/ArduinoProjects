
#define DEBUG

#define SPEED_LIMIT 100
#define CHAN_BACK_FORWARD 3
#define CHAN_LEFT_RIGHT 2
#define MOTOR_PIN_COUNT 2

enum motorPin { EN, DIR};

int left[MOTOR_PIN_COUNT] = {9, 7}; 
int right[MOTOR_PIN_COUNT] = {10, 8};

//Channel 3 throttle interrupt settings
volatile int spdLastState = LOW;
volatile unsigned long spdUpTime = 0;
volatile unsigned long spdDownTime = 0;

//Channel 4 turn interrupt settings
volatile int trnLastState = LOW;
volatile unsigned long trnUpTime = 0;
volatile unsigned long trnDownTime = 0;

int rawSpd = 0;
int spd = 0;
int rawTrn;
int trn = 0;


void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  digitalWrite(CHAN_BACK_FORWARD, HIGH);
  digitalWrite(CHAN_LEFT_RIGHT, HIGH);
  
  attachInterrupt(1, readSpeed, RISING);
  attachInterrupt(0, readTurn, RISING);
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  for (int i = 0; i< MOTOR_PIN_COUNT; i++){
   pinMode(left[i], OUTPUT); 
   pinMode(right[i], OUTPUT);
  }
}

void loop(){
  rawSpd = spdDownTime - spdUpTime;
  rawTrn = trnDownTime - trnUpTime;
  
  if ((rawSpd > 1000 && rawSpd < 2000) && (rawTrn > 1000 && rawTrn < 2000)) {
    spd = constrain(map(rawSpd, 1060, 1900, -SPEED_LIMIT, SPEED_LIMIT), -SPEED_LIMIT, SPEED_LIMIT);
    trn = constrain(map(rawTrn, 1072, 1900, -SPEED_LIMIT, SPEED_LIMIT), -SPEED_LIMIT, SPEED_LIMIT);
   
    if(spd < 10 && spd > -10) 
      spd = 0;
  
    if(trn < 10 && trn > -10)
      trn = 0;
  
//    setBackForward();
//    setMotorSpeeds(); 
  }
  #ifdef DEBUG
    Serial.print("rawSpd = ");
    Serial.print(rawSpd);
    Serial.print(", spd = ");
    Serial.print(spd);
    Serial.print(", rawTrn = ");
    Serial.print(rawTrn);
    Serial.print(", trn = ");
    Serial.println(trn);
  #endif
  
  delay(25);

}
void setMotorSpeeds(){
 
  if (trn > 10){ //Left
    analogWrite(left[EN], constrain((abs(spd) - trn), 0, SPEED_LIMIT));
    analogWrite(right[EN], constrain(abs(spd) + trn, 0, SPEED_LIMIT));
  }else if(trn < 10){ //Right
    analogWrite(right[EN], constrain((abs(spd) + trn), 0, SPEED_LIMIT));
    analogWrite(left[EN], constrain(abs(spd) - trn, 0, SPEED_LIMIT));
  }else{ //Straight
     analogWrite(left[EN], abs(spd));
     analogWrite(right[EN],abs(spd));
  }
}
void setBackForward(){
  if(spd > 0){
    digitalWrite(left[DIR], LOW);
    digitalWrite(right[DIR], HIGH);
  }
  
  if(spd < 0){
    digitalWrite(left[DIR], HIGH);
    digitalWrite(right[DIR], LOW);

  }
}

void readTurn(){
  if(trnLastState == LOW){
    trnUpTime = micros();
    trnLastState = HIGH;
    attachInterrupt(0, readTurn, FALLING);
  }else{
    trnDownTime = micros();
    trnLastState = LOW;
    attachInterrupt(0, readTurn, RISING);
  }
  
}

void readSpeed(){
  if(spdLastState == LOW){
    spdUpTime = micros();
    spdLastState = HIGH;
    attachInterrupt(1, readSpeed, FALLING);
  }else{
    spdDownTime = micros();
    spdLastState = LOW;
    attachInterrupt(1, readSpeed, RISING);
  }

}
