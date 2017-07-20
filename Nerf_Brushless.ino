/*
Brushless Nerf Stryfe Project.
By Jack Wallace
Includes setting motor speed via bluetooth connection.
*/
#include <Servo.h>

//Declare ESCs
Servo ESC1;
Servo ESC2;

//Pin constants
const int ESC1pin = PD5;
const int ESC2pin = PD6;
const int triggerPin = PD2;

//Min and Max PWM values for ESC
const int ESCminspeed = 1100;
const int ESCmaxspeed = 2400;

//Min PWM value for motor to spin
const int minspeed = 1225;
//Max PWM value at flywheel friction ceiling
const int maxspeed = 1800;

//*Note that trigger is pulled HIGH when switch is open*
int trigger = 1;
//Velocity Scale from 1-100
int Velocity = 100;
int setESC = maxspeed;
bool DISARM = 0;

void setup () {
  //Set pin modes
  pinMode(ESC1pin, OUTPUT);
  pinMode(ESC2pin, OUTPUT);
  pinMode(triggerPin, INPUT);
  //Initialise Serial
  Serial.begin(9600);
  //Set up Servo objects
  ESC1.attach(ESC1pin);
  ESC2.attach(ESC2pin);
  //ARM ESCs
  ESC1.writeMicroseconds(ESCminspeed);
  ESC2.writeMicroseconds(ESCminspeed);
  //Wait for arming sequence
  delay(4000);
}

void loop () {
  //Check if trigger is depressed
  trigger = digitalRead(triggerPin);
  
  //TRIGGER ON
  if (trigger == 0 && DISARM == 0) {
    //Set ESCs to current velocity setting
    ESC1.writeMicroseconds(setESC);
    ESC2.writeMicroseconds(setESC);
  }
  
  //TRIGGER OFF
  if (trigger == 1) {
    //Set ESCs to minimum
    ESC1.writeMicroseconds(ESCminspeed);
    ESC2.writeMicroseconds(ESCminspeed);

    if (Serial.available()) {
      Velocity = Serial.parseInt();
      if (Velocity>0 && Velocity<=100){
        setESC = map(Velocity,1,100,minspeed,maxspeed);
        Serial.print("New Motor Setting [1-100]: ");
        Serial.println(Velocity);
        Serial.print("//ESC Value ->");
        Serial.println(setESC);
        if (DISARM == 1) {
          DISARM = 0;
          Serial.println("-- (Weapon Armed.) --");
        }    
      }
      else if (Velocity == 101) {
        DISARM = 1;
        Serial.println("-- (Weapon Disarmed.) --");
      }
      else {
        Serial.println("[Invalid Input]");
      }
    }
  }
     
  //small delay for stability
  delay(5);

}

