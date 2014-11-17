/*
Author:          Jonathan Irwin
Date:            17 November 2014
Purpose:         Control arduino 4WD robot using L293D Intergrated circuit
                  The robot also has a ultrasonic sensor on the front to tell when it is going to hit an object - all very simple stuff for now
*/

//import ultra-sonic sensor library
#include <NewPing.h>

#define TRIGGER_PIN  7  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     6  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

//setup all the pins for the IC
const int controlPin1 = 2; // connected to pin 7 on the H-bridge
const int controlPin2 = 3; // connected to pin 2 on the H-bridge
const int controlPin3 = 12; 
const int controlPin4 = 13; 
const int enablePin = 9;   // connected to pin 1 on the H-bridge
const int enablePin1 = 10;  
const int directionSwitchPin = 4;  // connected to the switch for direction
const int onOffSwitchStateSwitchPin = 5; // connected to the switch for turning the motor on and off
const int potPin = A0;  // connected to the potentiometer's output

// create some variables to hold values from your inputs
int onOffSwitchState = 0;  // current state of the On/Off switch
int previousOnOffSwitchState = 0; // previous position of the on/off switch
int directionSwitchState = 0;  // current state of the direction switch
int previousDirectionSwitchState = 0;  // previous state of the direction switch

int motorEnabled = 0; // Turns the motor on/off
int motorSpeed = 0; // speed of the motor
int motorDirection = 1; // current direction of the motor

void setup() {
  Serial.begin(115200); // Start the serial monitor
  
  // intialize the inputs and outputs
  pinMode(directionSwitchPin, INPUT);
  pinMode(onOffSwitchStateSwitchPin, INPUT);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(controlPin3, OUTPUT);
  pinMode(controlPin4, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(enablePin1, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(enablePin, LOW);
  digitalWrite(enablePin1, LOW);
}

void loop() {
    unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
    unsigned int cm = (uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
    
    //if close to something just switch off
    if (cm < 13){
      driveBackwards("left");
      delay(1600);
      stopDriving(4000);
      driveForwards();
    }
    
  // read the value of the on/off switch
  onOffSwitchState = digitalRead(onOffSwitchStateSwitchPin);
  delay(1);

  // read the value of the direction switch
  directionSwitchState = digitalRead(directionSwitchPin);

  // read the value of the pot and divide by 4 to get
  // a value that can be used for PWM
  motorSpeed = analogRead(potPin) / 4;

  // if the on/off button changed state since the last loop()
  if (onOffSwitchState != previousOnOffSwitchState) {
    // change the value of motorEnabled if pressed
    if (onOffSwitchState == HIGH) {
      motorEnabled = !motorEnabled;
    }
  }

  // if the direction button changed state since the last loop()
  if (directionSwitchState != previousDirectionSwitchState) {
    // change the value of motorDirection if pressed
    if (directionSwitchState == HIGH) {
      changeDirection();
    }
  }

  // change the direction the motor spins by talking
  // to the control pins on the H-Bridge
  if (motorDirection == 1) {
    driveForwards();
  }
  else {
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
    digitalWrite(controlPin3, LOW);
    digitalWrite(controlPin4, HIGH);
  }

  // if the motor is supposed to be on
  if (motorEnabled == 1) {
    // PWM the enable pin to vary the speed
    analogWrite(enablePin, motorSpeed);
    analogWrite(enablePin1, motorSpeed);
  }
  else { // if the motor is not supposed to be on
    //turn the motor off
    analogWrite(enablePin, 0);
    analogWrite(enablePin1, 0);
  }
  // save the current On/Offswitch state as the previous
  previousDirectionSwitchState = directionSwitchState;
  // save the current switch state as the previous
  previousOnOffSwitchState = onOffSwitchState;
}




