// This is a wrapper class for the MPU6050_DMP library; it exposes yaw, pitch and roll angles measures
//   through a user-friendly, straight-forward interface

#include "MPU6050_6Axis_MotionApps20.h"

class MPU6050_DMP_YPR_wrapper
{
public:
    MPU6050_DMP_YPR_wrapper(uint8_t address=MPU6050_DEFAULT_ADDRESS);

    // returns 0 if success; returns non-zero if error
    int setup(int16_t x_accel_offset=0, int16_t y_accel_offset=0, int16_t z_accel_offset=0,
               int16_t x_gyro_offset=0, int16_t y_gyro_offset=0, int16_t z_gyro_offset=0);

    // returns pointer to yaw, pitch, roll in degrees if data available, nullptr otherwise
    float *get_ypr();

private:
    MPU6050 mpu_;

    bool dmpReady_ = false;  // set true if DMP init was successful
    uint8_t fifoBuffer_[64]; // FIFO storage buffer

    // orientation/motion vars
    Quaternion q_;           // [w, x, y, z]         quaternion container
    VectorFloat gravity_;    // [x, y, z]            gravity vector
    float ypr_[3];           // [yaw, pitch, roll]   yaw/pitch/roll container
};
