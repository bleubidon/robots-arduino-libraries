// TODO make this library more flexible (indeed, it hardcodes many sensor parameters, such as the accelerometer and gyroscope ranges)
// TODO make a better class interface in terms of methods calls order constraints
// TODO handle yaw (in addition to pitch and roll)

#pragma once

#include <Arduino.h>
#include <Wire.h>

class MPU6050
{
public:
    struct human_sensor_data
    {
        float Acc_X_, Acc_Y_, Acc_Z_;
        float Gyr_X_, Gyr_Y_, Gyr_Z_;
    };
    struct absolute_angles
    {
        float pitch_, roll_;
    };

    MPU6050(char i2c_slave_address);
    void setup();
    human_sensor_data compute_human_sensor_data(bool correct_gyro_with_calib=true);
    absolute_angles compute_absolute_angles(float elapsed_time_seconds, bool use_gyroscope=false);

private:
    char i2c_slave_address_;

    // Raw sensor data
    int16_t Acc_rawX_, Acc_rawY_, Acc_rawZ_;
    int16_t Gyr_rawX_, Gyr_rawY_, Gyr_rawZ_;
    int temperature_;

    // Human sensor data
    float Acc_humanX_, Acc_humanY_, Acc_humanZ_;
    float Gyr_humanX_, Gyr_humanY_, Gyr_humanZ_;

    // Absolute angles
    float Acceleration_angle_[2];
    float Gyro_angle_[2];
    float Final_angle_[2] = {0};

    // Coefficient between 0 and 1, used to compute the absolute angles. By increasing this value,
        // - The computed absolute angles are more stable (they contain less error due to vibrations)
        // - The computed absolute angles have a slower response (more time is needed for them to stabilize)
    static constexpr float filter_first_part_coeff = .98;

    // Calibration
    long gyro_x_cal_ = 0, gyro_y_cal_ = 0, gyro_z_cal_ = 0;
    float angle_pitch_acc_cal_ = 0, angle_roll_acc_cal_ = 0;
    static const unsigned int cal_rounds_ = 100;
    
    void calibrate_gyro_and_accel();
    void read_raw_sensor_data();
    void accel_human_to_absolute_angles();
};
