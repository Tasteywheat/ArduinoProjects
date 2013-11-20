#include <Servo.h>
#include <math.h>
//measuredValue, currentEstimate, previousEstimate, currentErrorEstimate, previousErrorEstimate, gain/blend factor, processVariance, measurementVariance
const int attribCount = 8;
enum kalmanValues {z ,xhat, xhatminus, p, pminus, k, q, r };
float kalmanThrottle[attribCount];

int throttle = 4;
int throttleValue = 0;
int motorPin = 5;

Servo motor;
int lastVal = 0;



void setup(){
  motor.attach(motorPin);
  pinMode(throttle, INPUT);
  Serial.begin(115200);
  for (int i = 0; i < attribCount; i++){
    kalmanThrottle[i] = 0;      
  }
  
  kalmanThrottle[r] = 0.1;
  kalmanThrottle[q] = 0.05;
  kalmanThrottle[p] = 1;

}

void loop(){
  /*
  kalmanThrottle[z] = constrain(pulseIn(throttle, HIGH, 19500), 1045, 1900);
  kalmanThrottle[z] = map(kalmanThrottle[z], 1045, 1900, 8, 175); 
  applyKalmanFilter(kalmanThrottle);
  
  motor.write(kalmanThrottle[xhat]);
  
  Serial.print(" KalmanRaw = ");
  Serial.print(kalmanThrottle[z]);
  Serial.print(" : Kalman = ");
  Serial.print(kalmanThrottle[xhat]);
  Serial.println();
 */
 while(digitalRead(throttle) == LOW){
   continue;
 }
 int startTime = micros();
 while(digitalRead(throttle) == HIGH){
  continue;
 }
 int delta = micros() - startTime;
 Serial.println(delta);
   
// Serial.println(pulseIn(throttle, HIGH, 20000));
 delay(5);
 
}

void applyKalmanFilter(float* kalmanArray){
  //**** Time Update *****
  //previousEstimate = currentEstimate ( before current is recalculated )
  kalmanArray[xhatminus] = kalmanArray[xhat];
  //remember our previous error estimate
  kalmanArray[pminus] = kalmanArray[p]+kalmanArray[q];
  
  
  //**** Measurement Update *****
  //Calculate the kalman blend this will cause the more accurate prediction to be more heavily weighted
  kalmanArray[k] = kalmanArray[pminus]/(kalmanArray[pminus]+kalmanArray[r]);
  //Calculate our new prediction
  kalmanArray[xhat] = floor(kalmanArray[xhatminus]+(kalmanArray[k]*(kalmanArray[z]-kalmanArray[xhatminus])));
  //Calculate the new error estimate
  kalmanArray[p] = (1-kalmanArray[k])*kalmanArray[pminus];
}

