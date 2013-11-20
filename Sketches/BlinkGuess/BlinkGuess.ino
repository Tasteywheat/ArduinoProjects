
int redPin = 9;
int greenPin = 10;
int bluePin = 11;

int buttonPin = 2;

int brightRed = 0;
int brightGreen = 0;
int brightBlue = 0;

int buttonState = LOW;


void setup(){
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  randomSeed(map(micros(), 0, sizeof(long)/2, 0, 1023));
  
  analogWrite(bluePin, 255);
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
}

void loop(){
  
  if(digitalRead(buttonPin) == HIGH){
    flash(100, 0, 0, 500, 5); 
    for(int i = 5; i > 0; i--){  
      brightRed = random(0, 100);
      brightGreen = random(0, 100);
      brightBlue = random(0, 100);
    }
    ledOn(brightRed, brightBlue, brightGreen);
    
  }
  printValues();
  delay(15);
}

void printValues(){ 
  Serial.print("Button: ");
  Serial.print(digitalRead(buttonPin));
  Serial.print(", Red: ");
  Serial.print(brightRed);
  Serial.print(", Green: ");
  Serial.print(brightGreen);
  Serial.print(", Blue: ");
  Serial.println(brightBlue);
}

void flash(int red, int green, int blue, int wait, int count){
  ledOff();
  for (int i = count; i > 0; i--){
    ledOn(red, green, blue);
    delay(wait);
    ledOff();
    delay(wait);
  }
  
  
}

void ledOff(){
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}

void ledOn(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
  
