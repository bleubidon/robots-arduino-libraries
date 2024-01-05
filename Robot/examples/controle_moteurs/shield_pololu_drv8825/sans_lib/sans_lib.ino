/*Example sketch to control a stepper motor with A4988/DRV8825 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */

#define UseLeftMotor
// #define UseRightMotor

// Define stepper motor connections and steps per revolution:
#define dirPinLeft 7
#define stepPinLeft 6
#define dirPinRight 3
#define stepPinRight 2
#define stepsPerRevolution 400

void setup() {
  // Declare pins as output:
  pinMode(dirPinLeft, OUTPUT);
  pinMode(stepPinLeft, OUTPUT);
  pinMode(dirPinRight, OUTPUT);
  pinMode(stepPinRight, OUTPUT);
}

void loop() {
  // Set the spinning direction clockwise:
  #ifdef UseLeftMotor
    digitalWrite(dirPinLeft, LOW);
  #endif
  #ifdef UseRightMotor
    digitalWrite(dirPinRight, HIGH);
  #endif

  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < stepsPerRevolution; i++) {
    // These four lines result in 1 step:
    #ifdef UseLeftMotor
      digitalWrite(stepPinLeft, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinLeft, LOW);
      delayMicroseconds(1000);
    #endif
    #ifdef UseRightMotor
      digitalWrite(stepPinRight, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinRight, LOW);
      delayMicroseconds(1000);
    #endif
  }

  delay(1000);
}
