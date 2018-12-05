/*
  Input Pull-up Serial

  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on pin 2 and prints the results to the Serial Monitor.

  The circuit:
  - momentary switch attached from pin 2 to ground
  - built-in LED on pin 13

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 5V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012
  by Scott Fitzgerald

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/InputPullupSerial
*/
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;

void setup() {
  //start serial connection
  //Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(10);
  myservo3.attach(11);

}

int pin2 ;
int pin3 ;

int motor_bit2; // bit[2]
int motor_bit3; // bit[1]
int motor_bits; // bit[1..0]

void loop() {
  pin2 = digitalRead(2);
  pin3 = digitalRead(3);

  if ( pin2 ) { // set binary value for bit[2]
    motor_bit2 = 0x2; // 3b'10
  }
  else{
    motor_bit2 = 0x0; // 3b'00
  }

  if ( pin3 ) {
    motor_bit3 = 0x1; // 3b'01
  }
  else {
    motor_bit3 = 0x0; // 3b'00
  }


  motor_bits = motor_bit2 | motor_bit3; 

  if ( motor_bits == 0x0 ){ // set all motors to initial position
    myservo1.write(0);
    myservo2.write(0);
    myservo3.write(0);
  }
  else if ( motor_bits == 0x1 ){ // set motor 1 to unlock position
    myservo1.write(180);
    myservo2.write(0);
    myservo3.write(0);
  }
  else if ( motor_bits == 0x2 ){ // set motor 1 & 2 to unlock position
    myservo1.write(180);
    myservo2.write(180);
    myservo3.write(0);
  }
  else if ( motor_bits == 0x3 ){ // set motor 1 & 2 to unlock position
    myservo1.write(180);
    myservo2.write(180);
    myservo3.write(180);
  }


  
//  //read the pushbutton value into a variable
//  int sensorVal = digitalRead(2);
//  //print out the value of the pushbutton
//  Serial.println(sensorVal);
//
//  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
//  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
//  // button's pressed, and off when it's not:
//  if (sensorVal == HIGH) {
//    digitalWrite(13, LOW);
//  } else {
//    digitalWrite(13, HIGH);
//  }
}
