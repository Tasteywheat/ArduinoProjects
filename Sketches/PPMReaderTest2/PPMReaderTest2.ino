
enum motorPins {EN, DIR};

#define DEBUG

#define RC_MAX 2000 // PPM Max
#define RC_MIN 1000 // PPM minimum
#define MIN_SPEED 80 // the minimum power for the motors (otherwise it they just wine and do nothing)
#define spd_pin A0
#define ppm1_pin 5 //channel 3
#define ppm2_pin 6 // channel 4

#define deadzone 10
#define timeout 22000

int SPEED_LIMIT = 150;
int ppm1 = 1500;
int ppm2 = 1500;
int last_ppm1 = 1500;
int last_ppm2 = 1500;

int leftMotorPins[] = {9, 7};
int rightMotorPins[] = {10, 8};

int spd = 0;
int trn = 0;


void setup(){
#ifdef DEBUG  
  Serial.begin(9600);
#endif
  
  pinMode(ppm1_pin, INPUT);
  pinMode(ppm2_pin, INPUT);
  
  pinMode(leftMotorPins[EN], OUTPUT);
  pinMode(leftMotorPins[DIR], OUTPUT); 
  
  pinMode(rightMotorPins[EN], OUTPUT);  
  pinMode(rightMotorPins[DIR], OUTPUT);
  
  pinMode(spd_pin, INPUT);
}

void loop(){
//  SPEED_LIMIT = analogRead(spd_pin) / 4;
  SPEED_LIMIT = analogRead(spd_pin);
  //read_ppm(); 
  //setBackForward();
  //setMotorSpeeds();
  
#ifdef DEBUG
  Serial.print("speed_limit = ");
  Serial.print(SPEED_LIMIT);
  Serial.print(", speed = ");
  Serial.print(spd);
  Serial.print(", direction = ");
  Serial.println(trn);
#endif
}

void setBackForward (){
  
  
  spd = constrain(map(last_ppm1, RC_MIN, RC_MAX, -SPEED_LIMIT, SPEED_LIMIT), -SPEED_LIMIT, SPEED_LIMIT);
  
  if(spd >= -MIN_SPEED && spd <= MIN_SPEED){
    spd = 0;
  }
  
  if (spd > MIN_SPEED){//Forward
    digitalWrite(leftMotorPins[DIR], HIGH);
    digitalWrite(rightMotorPins[DIR], LOW);
  }
  
  if (spd < -MIN_SPEED){//Backward
    digitalWrite(leftMotorPins[DIR], LOW);
    digitalWrite(rightMotorPins[DIR], HIGH);
  }
}

void setMotorSpeeds (){
  
  trn = constrain(map(last_ppm2, RC_MIN, RC_MAX, -SPEED_LIMIT, SPEED_LIMIT), -SPEED_LIMIT, SPEED_LIMIT);
  
  if(trn >= -10 && trn <= 10) {
    trn = 0;
  }
  
  if( trn < -10){
    analogWrite(leftMotorPins[EN], constrain(abs(spd) - abs(trn), 0, SPEED_LIMIT));
    analogWrite(rightMotorPins[EN], constrain(abs(spd), 0, SPEED_LIMIT));  
  }else if (trn > 10){
    analogWrite(leftMotorPins[EN], constrain(abs(spd), 0, SPEED_LIMIT));
    analogWrite(rightMotorPins[EN], constrain(abs(spd) - abs(trn), 0, SPEED_LIMIT));  
  }else{
    analogWrite(rightMotorPins[EN], abs(spd));
    analogWrite(leftMotorPins[EN], abs(spd));
  }
}

void read_ppm(){
  ppm1 = pulseIn(ppm1_pin, HIGH, timeout);
  if(ppm1 > 1000 && ppm1 < 2000){
    if((ppm1 > (last_ppm1 + deadzone)) || (ppm1 < (last_ppm1 - deadzone))){ 
      last_ppm1 = ppm1;
    }
  }
  
  ppm2 = pulseIn(ppm2_pin, HIGH, timeout);
  if(ppm2 > 1000 && ppm2 < 2000){
    if((ppm2 > (last_ppm2 + deadzone)) || (ppm2 < (last_ppm2 - deadzone))){ 
      last_ppm2 = ppm2;
    }
  }
}


  
