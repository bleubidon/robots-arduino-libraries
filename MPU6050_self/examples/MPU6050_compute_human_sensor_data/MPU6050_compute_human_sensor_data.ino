#define MPU_ADDRESS 0x68  // MPU-6050 I2C slave address

#include <MPU6050_self.h>

MPU6050 my_mpu6050(MPU_ADDRESS);
struct MPU6050::human_sensor_data my_human_sensor_data;

void setup()
{
    Serial.begin(9600);
    my_mpu6050.setup();
}

void loop()
{
    my_human_sensor_data = my_mpu6050.compute_human_sensor_data();

    Serial.print("(g) Acc X | Acc Y | Acc Z: ");
    Serial.print(my_human_sensor_data.Acc_X_);
    Serial.print(" | ");
    Serial.print(my_human_sensor_data.Acc_Y_);
    Serial.print(" | ");
    Serial.print(my_human_sensor_data.Acc_Z_);

    Serial.print("  ||  (°/s) Gyr X | Gyr Y | Gyr Z: ");
    Serial.print(my_human_sensor_data.Gyr_X_);
    Serial.print(" | ");
    Serial.print(my_human_sensor_data.Gyr_Y_);
    Serial.print(" | ");
    Serial.println(my_human_sensor_data.Gyr_Z_);
    Serial.println();

    delay(100);
}
