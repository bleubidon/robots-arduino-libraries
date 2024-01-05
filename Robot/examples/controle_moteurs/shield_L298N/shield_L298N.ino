#include <Stepper.h>

#define In1 9
#define In2 8
// #define In3 11
// #define In4 12
#define EnA 10
// #define EnB 10
#define minStrength 40
#define maxStrength 200

const int stepsPerRevolution = 200; // Number of steps per output rotation

Stepper myStepper(stepsPerRevolution, In1, In2, In3, In4); // Create Instance of Stepper library

void setup()
{

  myStepper.setSpeed(150); // set the speed in rpm
  Serial.begin(9600);
  setStrength(maxStrength);
}

void loop()
{
  // step one revolution in one direction:
  Serial.println("clockwise");
  myStepper.step(1 * stepsPerRevolution);
  delay(15);
  holdPosition(3000);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-1 * stepsPerRevolution);
  delay(15);
  holdPosition(3000);
}

void holdPosition(int duration)
{
  setStrength(minStrength);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(duration);
  setStrength(maxStrength);
}

void releaseMotor()
{
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void setStrength(int pwm)
{
  analogWrite(EnA, pwm);
  analogWrite(EnB, pwm);
}
