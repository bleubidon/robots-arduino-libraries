/*Example sketch to control a stepper motor with DRV8825 stepper motor driver, AccelStepper library and Arduino: continuous rotation. More info: https://www.makerguides.com */
// Include the AccelStepper library:
#include <AccelStepper.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPinLeft 7
#define stepPinLeft 6
#define dirPinRight 3
#define stepPinRight 2
#define motorInterfaceType 1
// Create a new instance of the AccelStepper class:
AccelStepper stepperLeft = AccelStepper(motorInterfaceType, stepPinLeft, dirPinLeft);
AccelStepper stepperRight = AccelStepper(motorInterfaceType, stepPinRight, dirPinRight);
void setup() {
  // Set the maximum speed in steps per second:
  stepperLeft.setMaxSpeed(1000);
  stepperRight.setMaxSpeed(1000);

  stepperLeft.setSpeed(400);
  stepperRight.setSpeed(-400);
}
void loop() {  
  // Step the motor with a constant speed as set by setSpeed():
  stepperLeft.runSpeed();
  stepperRight.runSpeed();
}
