// Note: for the measured absolute angles values to be alright, MPU6050::compute_absolute_angles needs to be run repeatedly with a relatively high frequency.
    // Consequently, a high serial baud rate is crucial.
    // This is due to the filter formula that is used to compute the absolute angles.
    // This applies both when using the gyroscope to compute those absolute angles and when ignoring it.

// Note: known flaw: it takes some significant time for the absolute angles values to settle to their final, stable value when transitioning quickly (i.e. slow response)

// Note: known flaw: when the orientation of the IMU is changed abruptly, the measured absolute angles have an oscillation phenomenon,
    // that may start with a peak in the same direction and opposite way relatively to the direction and way of the actual movement that occurred (overshoot).
    // This is probably due to the internal working of the accelerometer sensor


#define MPU_ADDRESS 0x68  // MPU-6050 I2C slave address

#include <MPU6050_self.h>

MPU6050 my_mpu6050(MPU_ADDRESS);
struct MPU6050::absolute_angles my_absolute_angles;

unsigned long start_time, time_prev;
float elapsed_time_seconds;

void setup()
{
    Serial.begin(250000);
    my_mpu6050.setup();
    start_time = micros();
}

void loop()
{
    time_prev = start_time;
    start_time = micros();
    elapsed_time_seconds = (start_time - time_prev) / 1e6;

    // Use only the accelerometer (ignore the gyroscope) to compute absolute angles
    // This seems to produce the best absolute angles results (less overshoot, faster response)
    my_absolute_angles = my_mpu6050.compute_absolute_angles(elapsed_time_seconds);

    // // Alternatively:
    // // Use both the accelerometer and the gyroscope to compute absolute angles
    // // This seems to yield to more overshoot and a slower response
    // my_absolute_angles = my_mpu6050.compute_absolute_angles(elapsed_time_seconds, true);

    Serial.print("(°) pitch: ");
    Serial.print(my_absolute_angles.pitch_);
    Serial.print(", roll: ");
    Serial.println(my_absolute_angles.roll_);
}
