#include <PID_v1.h>
#include <Sabertooth.h>
#include <SoftwareSerial.h>
#include "Arduino.h"
#include "LineSensor.h"
//#define DEBUG
#define CENTER 3000
#define MAX_SPD 70
#define MAX_TRN 30

//int RATES[] = {-4000, -2250, -1000, -250, 0, 250, 1000, 2250, 4000};


double linePos = CENTER;
//double line_value = 0;
double turn_power = 0;
double drive_power = 0;
double turnGoal = CENTER;
double driveGoal = CENTER;



int start = 0;
int delta = 0;
int period = 10;
volatile int lastLinePos = 0;
int low_threshold = 690;
int high_threshold = 800;
/*
**** Linear Rates ********* Order of best performance
double kp = 0.0125;
double ki = 0.025;
double kd = 0.0045;

double kp = 0.0125;
double ki = 0.018;
double kd = 0.007;

**** Exponential Rates *****
double kp = 0.0075;
double ki = 0.05;
double kd = 0.007;
*/

double kp = 0.0125;
double ki = 0.025;
double kd = 0.0045;

PID turnPID(&linePos, &turn_power, &turnGoal, kp, ki, kd, DIRECT);
//PID drivePID(&linePos, &drive_power, &driveGoal, kp, ki, kd, DIRECT);

SoftwareSerial SWSerial(NOT_A_PIN, 11); // RX on no pin (unused), TX on pin 11 (to S1).
Sabertooth st(128, SWSerial); // Address 128, and use SWSerial as the serial port.


void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  SWSerial.begin(9600);
  initSensorPins();
  
  turnPID.SetMode(AUTOMATIC);
  turnPID.SetSampleTime(10);
  turnPID.SetOutputLimits(-MAX_TRN + 7, MAX_TRN);
  
 // drivePID.SetMode(AUTOMATIC);
 // drivePID.SetOutputLimits(0, MAX_SPD);
 st.drive(0);
 st.turn(0);
}

void loop(){
  start = millis();
  
  readSensorsRaw();
  
  //If you pickup up the robot it stops
  if(SENSOR_VALS[0] > high_threshold &&
    SENSOR_VALS[1] > high_threshold &&
    SENSOR_VALS[2] > high_threshold &&
    SENSOR_VALS[3] > high_threshold &&
    SENSOR_VALS[4] > high_threshold) {
    
    st.drive(0);
    st.turn(0);

  }else {
  //Assume you are line following
    linePos = getLinePos();
    if(linePos >= 1000 && linePos <= 5000) { //if have a good reading
      lastLinePos = linePos;                  //  use the good reading
    } else {                                  //if Bad reading 
      linePos = lastLinePos;                  //  use last good reading
    }
    
    //line_value = getLineRate(linePos);

   
    
    turnPID.Compute();
 //   drivePID.Compute();
    st.drive(MAX_SPD);
    st.turn(floor(turn_power));
    

  }
  #ifdef DEBUG
    printSensorVals();
    Serial.print(" , ");
    Serial.print(linePos);
    Serial.print(" , ");
    Serial.println(turn_power);
  #endif
  /*
   //Ensure constant loop time 
   delta = start - millis(); 
   if (delta > period) delay(delta - period); 
  */
}


int getLinePos(){
  int largest = getMaxVal(SENSOR_VALS);
  int pos = -5;
  if (largest > low_threshold){
    for (int i = 0; i < SENSOR_COUNT; i++){
      if(SENSOR_VALS[i] == largest) {
        pos = (i + 1) * 1000; 
        if(largest < high_threshold){
          if(lastLinePos > pos) pos += 500;
          if(lastLinePos < pos) pos -= 500;
        }
              
        break;
      }
    }
  }
  
  return pos;
}
/*
int getLineRate(int theLinePos){
  switch(theLinePos){
    case 1000:
      return RATES[0];
    case 1500:
      return RATES[1];
    case 2000:
      return RATES[2];
    case 2500:
      return RATES[3];
    case 3000:
      return RATES[4];
    case 3500:
      return RATES[5];
    case 4000:
      return RATES[6];
    case 4500:
      return RATES[7];
    case 5000:
      return RATES[8];
    default:
      return -1;
  }
}
*/
