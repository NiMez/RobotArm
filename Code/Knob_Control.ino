//This code allows for control of arm with potentiometers

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  630 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 0;
int potpin1 = A3;
int potpin2 = A2;
int potpin3 = A1;

int angle0 = 50; //50 min 180 max
int angle00 = 140; // min  140 max

int angle1 = 20; //20 min
int angle11 = 60; // min max

int angle2 = 40; //20 min
int angle22 = 100; // min max

int val;
int pulse;

const int button = 8;

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(50);
  pinMode(button, INPUT_PULLUP);
}

void loop() {

   if(digitalRead(button) == LOW){
       
        val = analogRead(potpin1); 
        val = map(val,0,1023,SERVOMIN, SERVOMAX); 
        pwm.setPWM(0,0,val);  
      delay(15);
      val = analogRead(potpin2); 
        val = map(val,0,1023,SERVOMIN, SERVOMAX); 
        pwm.setPWM(1,0,val);  
      delay(15);
      val = analogRead(potpin3); 
        val = map(val,0,1023,SERVOMIN, SERVOMAX); 
        pwm.setPWM(2,0,val);  
      delay(15);
   }

   else if(digitalRead(button) == HIGH){
     pulse = map(angle2,0,180,SERVOMIN, SERVOMAX); 
        pwm.setPWM(2,0,pulse);
        delay(1000);
      pulse = map(angle0,0,180,SERVOMIN, SERVOMAX); 
        pwm.setPWM(0,0,pulse);
        pulse = map(angle1,0,180,SERVOMIN, SERVOMAX); 
        pwm.setPWM(1,0,pulse);
        delay(5000);

   }
      
       
}
  
  
