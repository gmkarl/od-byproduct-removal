#include <Wire.h>
#include "DS3231.h"

RTClib RTC;

// *** CONSTANTS *** //
const byte ODSensor = A14;
const byte ODSensorVCC = 51;
const byte ODSensorGND = 50;
const byte MotorDriver1 = 52;
const byte MotorDriver1STBY = 36;
const byte MotorDriver2 = 53;
const byte MotorDriver2STBY = 37;
const byte P1A = 35;
const byte P1B = 33;  // Will be pin that is changed
const byte P1PWM = 8;
const byte P2A = 39;  // Will be pin that is changed
const byte P2B = 41;
const byte P2PWM = 45;


// *** VARIABLES *** //
bool ODPump1State = LOW;
bool ODPump2State = LOW;
unsigned long previousMillisPump1 = 0;
unsigned long previousMillisPump2 = 0;
unsigned long nMillisTimer = 0;


// must be long to prevent overflow
const unsigned long ODPump1INTERVAL = 30000; // (60sec) time between starting
const unsigned long ODPump1RunTime = 10000;  // 10sec run time
const unsigned long ODPump2INTERVAL = 12000; // (12sec) time between starting
const unsigned long ODPump2RunTime = 8000;   // 8sec run time


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  // put your setup code here, to run once:
  pinMode (ODSensorVCC, OUTPUT);
  pinMode (ODSensorGND, OUTPUT);
  pinMode (ODSensor, INPUT);
  pinMode (MotorDriver1, OUTPUT);
  pinMode (MotorDriver1STBY, OUTPUT);
  pinMode (MotorDriver2, OUTPUT);
  pinMode (MotorDriver2STBY, OUTPUT);
  pinMode (P1A, OUTPUT);
  pinMode (P1B, OUTPUT);
  pinMode (P1PWM, OUTPUT);
  pinMode (P2A, OUTPUT);
  pinMode (P2B, OUTPUT);
  pinMode (P2PWM, OUTPUT);

  digitalWrite (ODSensorGND, LOW);
  digitalWrite (ODSensorVCC, HIGH);
  digitalWrite (MotorDriver1, HIGH);
  digitalWrite (MotorDriver1STBY, HIGH);
  digitalWrite (MotorDriver2, HIGH);
  digitalWrite (MotorDriver2STBY, HIGH);

  digitalWrite(P1A, LOW);
  analogWrite(P1PWM, 170);
  digitalWrite(P2B, LOW);
  analogWrite(P2PWM, 170);
}

void loop()
{
  DateTime now = RTC.now();
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisPump1 > ODPump1INTERVAL)
  {
    //store the time of this change
    previousMillisPump1 += ODPump1INTERVAL;
    ODPump1State = !ODPump1State; // Toggle
    digitalWrite(P1B, ODPump1State);

  }

  //if (previousMillisPump1 - currentMillis > ODPump2INTERVAL) {
  // if (currentMillis - previousMillisPump1 > ODPump2INTERVAL) {
  if (currentMillis - previousMillisPump2 > ODPump2INTERVAL) {
    previousMillisPump2 += ODPump2INTERVAL;
    ODPump2State = !ODPump2State;  // Toggle
    digitalWrite (P2A, ODPump2State);
  }

}
