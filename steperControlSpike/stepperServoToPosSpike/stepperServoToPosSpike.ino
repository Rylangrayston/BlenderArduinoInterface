/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int stepPin =  8;
int stepPeriod = 1000/ 2;
int enablePin = 6; 
int directionPin = 7 ;
boolean motorDirection = true; 

int currentPosition  = 0; 
int targetPosition = 0; 

// the setup routine runs once when you press reset:
void setup() {       
Serial.begin(9600);   
  // initialize the digital pin as an output.
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  digitalWrite( enablePin, LOW);  
  
}

void setDirection(){
  if (targetPosition > currentPosition)
  {
    motorDirection = true; 
  }
  else
  {
    motorDirection = false;
  }
  digitalWrite(directionPin, motorDirection); 
}

void sendStep()
{
    digitalWrite(stepPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(stepPeriod);               // wait for a second
  digitalWrite(stepPin, LOW);    // turn the LED off by making the voltage LOW
  delayMicroseconds(stepPeriod);   
}
  

void updateStepper(){
  setDirection(); 
  
  if (targetPosition > currentPosition)
   {
     sendStep(); 
     currentPosition++;
     //Serial.println(currentPosition); 
   }
   
   
     if (targetPosition < currentPosition)
   {
     sendStep(); 
     currentPosition--;
     //Serial.println(currentPosition); 
   }
            // wait for a second

}

// the loop routine runs over and over again forever:
void loop() {

  updateStepper(); 

  if (Serial.available())
  {
    targetPosition = 10 * Serial.read();
    Serial.println(targetPosition);
  }
  

  
}
