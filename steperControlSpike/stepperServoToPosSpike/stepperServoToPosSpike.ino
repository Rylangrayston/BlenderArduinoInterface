/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

//float framePeriod = 1/60 * 1000000; 
long stepsToDo = 0; 
long microSecondsBetweenSteps = 1000;
long microSecondsPerFrame = 16666 * 1.8; 
long slow = 0; 

long lastTargetPosition = 0 ;

unsigned long timeOfLastStep = 0; 
unsigned long currentTime = 0;
boolean start = false; 

int stepPin =  8;


int minStepPeriod = 100;
int enablePin = 6; 
int directionPin = 7 ;
boolean motorDirection = true; 
int toFastPin = 10;
int wayToFastPin = 9;

int currentPosition  = 0; 
int targetPosition = 0; 

// the setup routine runs once when you press reset:
void setup() {       
Serial.begin(115200);   
  // initialize the digital pin as an output.
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(toFastPin, OUTPUT);
  pinMode(wayToFastPin, OUTPUT);
  digitalWrite( enablePin, LOW);  
  
}


void setStepSpeed()
  {
    stepsToDo = abs(targetPosition-currentPosition);
    microSecondsBetweenSteps = microSecondsPerFrame/(stepsToDo + 1);
    microSecondsBetweenSteps += slow; 
    if (microSecondsBetweenSteps < minStepPeriod)
    {
      digitalWrite(toFastPin,HIGH);
      microSecondsBetweenSteps = minStepPeriod; 
    }
    else if (microSecondsBetweenSteps < 0)
    {
      digitalWrite(wayToFastPin,HIGH);
    }
    else 
    {
      digitalWrite(toFastPin,LOW);
    }
   // microSecondsBetweenSteps = minStepPeriod; 
  }



int readTwoBytes(){
  return  (Serial.read() + (Serial.read() << 8));
}

void writeTwoBytes(int x){
  Serial.write((x&0xFF));
  Serial.write((x>>8));
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
  //delayMicroseconds(1);               // wait for a second
  digitalWrite(stepPin, LOW);    // turn the LED off by making the voltage LOW
  //delayMicroseconds(1);   
}
  

void updateStepper(){

  
  if (targetPosition > currentPosition)
   {
     sendStep(); 
     currentPosition++;
     digitalWrite(wayToFastPin, LOW);
     //Serial.println(currentPosition); 
   }
   
   
    else if (targetPosition < currentPosition)
   {
     sendStep(); 
     currentPosition--;
     digitalWrite(wayToFastPin, LOW);
     //Serial.println(currentPosition); 
   }
            // wait for a second
            
   else
   {
     digitalWrite(wayToFastPin, HIGH);
   }

}


 void startUp()
 {
   for (int i = 0; i < 200 * 8; i++)
   {
     delayMicroseconds(10000);
     sendStep();
   }
 }
   
   
// the loop routine runs over and over again forever:
void loop() {

  if (start)
  {
    start = false;
    startUp(); 
  } 
  
  currentTime = micros(); 
  
  if (currentTime - timeOfLastStep > microSecondsBetweenSteps)
  {
    timeOfLastStep = currentTime;
  updateStepper(); 
  }
  
  if (Serial.available() >10)
  {
    digitalWrite(wayToFastPin, HIGH);
  }
  
  if (Serial.available() > 1)
  {
    lastTargetPosition = targetPosition;
    targetPosition =  readTwoBytes();
    
    if (lastTargetPosition != targetPosition)
    {
    setStepSpeed();
      setDirection(); 
    }
    //writeTwoBytes(currentPosition);
    //Serial.println(targetPosition);s
  }
  

  
}
