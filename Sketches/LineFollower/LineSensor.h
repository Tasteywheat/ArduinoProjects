
#ifndef LineSensor_h
#define LineSensor_h
#include "Arduino.h"

#define SENSOR_COUNT 5
#define SAMPLE_SIZE 5

uint8_t SENSOR_PINS[] = {A0, A1, A2, A3, A4};
volatile int SENSOR_OFFSETS[] = {0, 0, 0, 0, 0};
volatile int SENSOR_VALS[] = {0, 0, 0, 0, 0};



void initSensorPins(){
  for (int i = 0; i < SENSOR_COUNT; i++){
    pinMode(SENSOR_PINS[i], INPUT);
  }
}

void readSensorsRaw(){
  for (int i = 0; i < SENSOR_COUNT; i++){
    SENSOR_VALS[i] = analogRead(SENSOR_PINS[i]);
  }
}

void readSensors(){
  for (int i = 0; i < SENSOR_COUNT; i++){
    SENSOR_VALS[i] = analogRead(SENSOR_PINS[i]) - SENSOR_OFFSETS[i];
  }
}

void printSensorVal(int pinIndex){
  Serial.print("A");
  Serial.print(pinIndex);
  Serial.print(" = ");
  Serial.print(SENSOR_VALS[pinIndex]);
}

void printSensorVals(){

  for (int i = 0; i < SENSOR_COUNT - 1; i++){
    printSensorVal(i);
    Serial.print(", ");
  }
  printSensorVal(SENSOR_COUNT - 1);
  
}

void printSensorOffsets(){

  for (int i = 0; i < SENSOR_COUNT; i++){
    Serial.print(SENSOR_OFFSETS[i]);
    Serial.print(", ");
  }

  Serial.println("");
}

int getMinVal(volatile int vals[]){
  int lesser = 1100;
  for (int i = 0; i < SENSOR_COUNT; i++){
    lesser = min(lesser, vals[i]);
  }
  
  return lesser;
}

int getMaxVal(volatile int vals[]){
  int higher = -100;
  for (int i = 0; i < SENSOR_COUNT; i++){
    higher = max(higher, vals[i]);
  }
  
  return higher;
}


void calibrateSensors(){
  for (int i = SAMPLE_SIZE; i >= 0; i--) {
    readSensorsRaw();
    for(int k = 0; k < SENSOR_COUNT; k++){
      SENSOR_OFFSETS[k] += SENSOR_VALS[k];
    }
  }
  
  for (int i = 0; i < SENSOR_COUNT; i++){
    SENSOR_OFFSETS[i] = SENSOR_OFFSETS[i] / SAMPLE_SIZE;
  }
  
  int lowest = getMinVal(SENSOR_OFFSETS);
  
  for (int i = 0; i < SENSOR_COUNT; i++){
    SENSOR_OFFSETS[i] = SENSOR_OFFSETS[i] - lowest;
  }
  
}


#endif
