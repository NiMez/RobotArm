
//The robotic arm goes through a loop of picking up and dropping a ball while moving in straight lines

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

// Angles
#define lowerLength 110     
#define upperLength 110
float armLength;
float z;
float TopAngle1;
float TopAngleDegrees;
float TopAngle1a;   
float BotAngle;  
float BotAngleDegrees;

// interpolation targets
float zTarget;
float xTarget;


int anglex;
int angley;
int anglez;

int angle0 = 50; //50 min 180 max
int angle00 = 140; // min  140 max

int angle1 = 20; //20 min
int angle11 = 60; // min max

int angle2 = 40; //20 min
int angle22 = 100; // min max

int val;
int pulse;

const int button = 8;

unsigned long previousMillis = 0;
const long interval = 5000;

int stepFlag = 0;

class Interpolation {  
public:
    rampInt myRamp;
    int interpolationFlag = 0;
    int savedValue;    

    int go(int input, int duration) {

      if (input != savedValue) {   // check for new data
          interpolationFlag = 0;
      }
      savedValue = input;          // bookmark the old value  
    
      if (interpolationFlag == 0) {                                        
          myRamp.go(input, duration, LINEAR, ONCEFORWARD);              
          interpolationFlag = 1;
      }
    
      int output = myRamp.update();               
      return output;
    }
};    // end of class

void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(50);
  pinMode(button, INPUT_PULLUP);
  
  // default servo position
  
  pulse = map(angle0,0,180,SERVOMIN, SERVOMAX); 
  pwm.setPWM(0,0,pulse);
  pulse = map(angle1,0,180,SERVOMIN, SERVOMAX); 
  pwm.setPWM(1,0,pulse);
  pulse = map(angle2,0,180,SERVOMIN, SERVOMAX); 
  pwm.setPWM(2,0,pulse);
  delay(5000);
}

void loop (){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 10){ //start timed loop
    int buttonPush = digitalRead(button);
    Serial.println(buttonPush);
    
    if (buttonPush == HIGH && stepFlag == 0){
      anglex = angle00;
      angley = angle11;
      previousMillis = currentMillis;
      stepFlag = 1;
    }
    else if (currentMillis - previousMillis > 1000 && stepFlag == 1){
      anglez = angle22;
      stepFlag = 2;
    }
    else if (currentMillis - previousMillis > 3000 && stepFlag == 2){
      anglez = angle2;
      stepFlag = 0;
    }
   
    else if (buttonPush == LOW && currentMillis - previousMillis > 5000){
      anglex = angle0;
      angley = angle1;
      anglez = angle2;
    }
  pulse = map(anglex,0,180,SERVOMIN, SERVOMAX); 
  pwm.setPWM(0,0,pulse);
  pulse = map(angley,0,180,SERVOMIN, SERVOMAX); 
  pwm.setPWM(1,0,pulse);
  pulse = map(anglez,0,180,SERVOMIN, SERVOMAX); 
  pwm.setPWM(2,0,pulse);
  }

            // start interpolation
          
          z = interpZ.go(zTarget,1000);
          x = interpX.go(xTarget,1000);

          // *** Inverse Kinematics ***
      
          // calculate modification to shoulder angle and arm length
      
          shoulderAngle2a = atan(z/x);
          shoulderAngle2aDegrees = shoulderAngle2a * (180/PI);    // degrees
          shoulderAngle2 = shoulderAngle2a - 0.7853908;  // take away the default 45' offset (in radians)
          shoulderAngle2Degrees = shoulderAngle2 * (180/PI);    // degrees
          shoulderMs2 = shoulderAngle2Degrees * 11;
          
          z2 = x/cos(shoulderAngle2a);     // calc new arm length to feed to the next bit of code below
      
          // ****************
      
          // calculate arm length based on upper/lower length and elbow and shoulder angle
          shoulderAngle1a = (sq(upperLength) + sq(z2) - sq(lowerLength)) / (2 * upperLength * z2);
          shoulderAngle1 = acos(shoulderAngle1a);     // radians
          elbowAngle = PI - (shoulderAngle1 *2);       // radians
      
          // calc degrees from angles
          shoulderAngleDegrees = shoulderAngle1 * (180/PI);    // degrees
          elbowAngleDegrees = elbowAngle * (180/PI);              // degrees 
      
          // calc milliseconds PWM to drive the servo.
          shoulderMs = shoulderAngleDegrees * 11;
          elbowMs = elbowAngleDegrees * 11;

          // *** end of Inverse Kinematics ***
}
  
  
