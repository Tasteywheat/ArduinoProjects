
#define SENSOR_COUNT 5

uint8_t sensorPins[] = {A0, A1, A2, A3, A4};
volatile uint16_t sensorValues[] = {0, 0, 0, 0, 0}; 

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  readSensors();
  printSensorValues();
  delay(20);
}

void readSensors(){
  for(int i = 0; i < SENSOR_COUNT; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }
}

void printSensorValues(){
  for(int i = 0; i < SENSOR_COUNT; i++) {
    Serial.print("A");
    Serial.print(i);
    Serial.print(" = ");
    Serial.print(sensorValues[i]);
    Serial.print(" ");
    if (i == SENSOR_COUNT - 1) Serial.print("\n");
  }
}


