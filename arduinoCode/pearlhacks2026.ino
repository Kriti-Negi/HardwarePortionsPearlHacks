#include <Servo.h>
#define TRIG_PIN  9 // The Arduino UNO R4 pin connected to the ultrasonic sensor's TRIG pin
#define ECHO_PIN  10 // The Arduino UNO R4 pin connected to the ultrasonic sensor's ECHO pin

float duration_us, distance_cm;

Servo myservo;  // create Servo object to control a servo
// twelve Servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int const_init_time_interval = 20; // adjust as needed
int incomingByte = 0;
int currSpeedPercent = 100;

void setup() {
  myservo.attach(11);
  Serial.begin (9600);

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT); 
}

void loop() {

  // check for any written bits
  int percentSpeed = 1;
  if (Serial.available() > 0){
      percentSpeed = ((int)Serial.read() - 48);
      if(percentSpeed >= 0 && percentSpeed <= 9){
        currSpeedPercent = (percentSpeed * 10);
        if(percentSpeed == 0){
          currSpeedPercent = 100;
        }
      }
  }

  int time_interval = const_init_time_interval * 0.01 * currSpeedPercent;

  for (pos = 0; pos <= 45; pos += 1) { 
    myservo.write(pos);     
    delay(time_interval);                       
  }
  for (pos = 45; pos >= 0; pos -= 1) { 
    myservo.write(pos);             
    delay(time_interval);   
  }

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.println(distance_cm);

  delay(time_interval);
}
