// Compute PID based on a simulated negatively-fedback-measure, and stop when the measure is close enough to the setpoint

#include <PID.h>

PID pid;

float kp = 1.6F;
float ki = .05F;
float kd = 10.F;

float setpoint = 100.F;
float threshold = .5F;
float measure = 0.F;

float pid_debug[3];
unsigned long time, time_prev;

void setup()
{
    Serial.begin(115200);

    pid.set_coefficients(kp, ki, kd);
    pid.set_setpoint(setpoint);
    time_prev = millis();
}

void loop()
{
    time = millis();
    pid.compute(measure, time - time_prev);

    Serial.print("Current error: ");
    Serial.print(pid.get_error());

    pid.get_pid_debug(pid_debug);
    Serial.print("  | p: ");
    Serial.print(pid_debug[0]);
    Serial.print(", i: ");
    Serial.print(pid_debug[1]);
    Serial.print(", d: ");
    Serial.println(pid_debug[2]);

    measure += pid.get_pid();

    if (abs(pid.get_error()) < threshold)
    {
        Serial.println("Measure reached a value that is close enough to the setpoint");
        while (1)
            delay(1000);
    }

    time_prev = time;
    delay(100);
}
