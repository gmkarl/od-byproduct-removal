#include <Wire.h>
#include "DS3231.h"

RTClib RTC;

// Goal: Over a 60 second loop:
//		pump1 turns on at 0 seconds
//		pump2 turns on at 2 seconds
//		both turn off at 10 seconds
// Assumption: setting P1B high activates pump1
//             setting P2A high activates pump2

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
unsigned long lastMillis = 0;
unsigned long pumpCycleMillis = 0;


// must be long to prevent overflow
const unsigned long ODPump1StartTime = 0; // activate at start of run cycle
const unsigned long ODPump2StartTime = 2000; // 2sec into run cycle
const unsigned long ODPump1StopTime = 10000;  // 10sec run time
const unsigned long ODPump2StopTime = 10000;   // 8sec run time
const unsigned long ODPumpCyclePeriod = 60000; // restart cycle after 60sec


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
  DateTime now = RTC.now(); // not used yet

  unsigned long currentMillis = millis();
  pumpCycleMillis += currentMillis - lastMillis; // prevents 50-day overflow by taking difference
  lastMillis = currentMillis;

  if (pumpCycleMillis >= ODPump1StartTime && pumpCycleMillis < ODPump1StopTime)
  {
    // we are in a time window where PUMP1 should be RUNNING
    if (ODPump1State != HIGH)
    {
      ODPump1State = LOW;
      digitalWrite(P1B, ODPump1State);
    } 
  } else {
    // we are in a time window where PUMP1 should be DISABLED
    if (ODPump1State != LOW)
    {
      ODPump1State = HIGH;
      digitalWrite(P1B, ODPump1State);
    } 
  }

  if (pumpCycleMillis >= ODPump2StartTime && pumpCycleMillis < ODPump2StopTime)
  {
    // we are in a time window where PUMP2 should be RUNNING
    if (ODPump2State != HIGH)
    {
      ODPump2State = LOW;
      digitalWrite(P2A, ODPump2State);
    } 
  } else {
    // we are in a time window where PUMP2 should be DISABLED
    if (ODPump2State != LOW)
    {
      ODPump2State = HIGH;
      digitalWrite(P2A, ODPump2State);
    } 
  }

  // restart after the period is over
  if (pumpCycleMillis >= ODPumpCyclePeriod)
  {
    pumpCycleMillis -= ODPumpCyclePeriod;
  }
}
