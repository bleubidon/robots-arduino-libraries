#include "MPU6050_DMP_YPR_wrapper.h"

#include "I2Cdev.h"
#include "Wire.h"

MPU6050_DMP_YPR_wrapper::MPU6050_DMP_YPR_wrapper(uint8_t address) : mpu_(MPU6050(address)) {}

int MPU6050_DMP_YPR_wrapper::setup(
    int16_t x_accel_offset, int16_t y_accel_offset, int16_t z_accel_offset,
    int16_t x_gyro_offset, int16_t y_gyro_offset, int16_t z_gyro_offset)
{
    Wire.begin();
    Wire.setClock(400000);

    mpu_.initialize();
    if (!mpu_.testConnection())
        return -1;

    if (uint8_t devStatus = mpu_.dmpInitialize())
    {
        // ERROR:
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)

        return devStatus;
    }

    // Considering that all offsets being null means that they should be ignored
    if (x_accel_offset || y_accel_offset || z_accel_offset || x_gyro_offset || y_gyro_offset || z_gyro_offset)
    {
        mpu_.setXAccelOffset(x_accel_offset);
        mpu_.setYAccelOffset(y_accel_offset);
        mpu_.setZAccelOffset(z_accel_offset);
        mpu_.setXGyroOffset(x_gyro_offset);
        mpu_.setYGyroOffset(y_gyro_offset);
        mpu_.setZGyroOffset(z_gyro_offset);
    }
    else
    {
        mpu_.CalibrateAccel(6);
        mpu_.CalibrateGyro(6);
    }
    
    mpu_.setDMPEnabled(true);
    dmpReady_ = true;

    return 0;
}

float *MPU6050_DMP_YPR_wrapper::get_ypr()
{
    if (!dmpReady_ || !mpu_.dmpGetCurrentFIFOPacket(fifoBuffer_))
        return nullptr;

    mpu_.dmpGetQuaternion(&q_, fifoBuffer_);
    mpu_.dmpGetGravity(&gravity_, &q_);
    mpu_.dmpGetYawPitchRoll(ypr_, &q_, &gravity_);

    ypr_[0] *= RAD_TO_DEG;
    ypr_[1] *= RAD_TO_DEG;
    ypr_[2] *= RAD_TO_DEG;
    return ypr_;
}
