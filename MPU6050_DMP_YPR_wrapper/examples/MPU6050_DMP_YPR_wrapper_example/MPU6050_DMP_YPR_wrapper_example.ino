#include <MPU6050_DMP_YPR_wrapper.h>

MPU6050_DMP_YPR_wrapper imu;
float *ypr = nullptr;

void setup()
{
    Serial.begin(115200);

// Either provide custom calibration offsets, or request a calibration now
    // int imu_init_return_code = imu.setup(-1844, 1559, 1330, 55, -19, 10);  // provide custom calibration offsets
    int imu_init_return_code = imu.setup();  // request a calibration now

    if (imu_init_return_code)
    {
        Serial.print("Error while attempting to initialize the IMU (");
        Serial.print(imu_init_return_code);
        Serial.println(")");
        while (1)
            delay(1000);
    }
}

void loop()
{
    // Get and print yaw, pitch and roll angles in degrees

    if ((ypr = imu.get_ypr()) != nullptr)
    {
        Serial.print("ypr\t");
        Serial.print(ypr[0]);
        Serial.print("\t");
        Serial.print(ypr[1]);
        Serial.print("\t");
        Serial.println(ypr[2]);
    }
    
    delay(100);
}
