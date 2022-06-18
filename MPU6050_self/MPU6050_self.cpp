#include "MPU6050_self.h"

MPU6050::MPU6050(char i2c_slave_address) : i2c_slave_address_(i2c_slave_address) {}

void MPU6050::setup()
{
    Wire.begin();
    Wire.beginTransmission(i2c_slave_address_);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);

    calibrate_gyro_and_accel();  // Note / to fix: for the accelerometer, a calibration is not necessarily desired
}

MPU6050::human_sensor_data MPU6050::compute_human_sensor_data(bool correct_gyro_with_calib)
{
    read_raw_sensor_data();

    if (correct_gyro_with_calib)
    {
        Gyr_rawX_ -= gyro_x_cal_;
        Gyr_rawY_ -= gyro_y_cal_;
    }

    Acc_humanX_ = Acc_rawX_ / 16384.;
    Acc_humanY_ = Acc_rawY_ / 16384.;
    Acc_humanZ_ = Acc_rawZ_ / 16384.;

    Gyr_humanX_ = Gyr_rawX_ / 131.;
    Gyr_humanY_ = Gyr_rawY_ / 131.;
    Gyr_humanZ_ = Gyr_rawZ_ / 131.;

    return (human_sensor_data)
    {
        Acc_humanX_, Acc_humanY_, Acc_humanZ_,
        Gyr_humanX_, Gyr_humanY_, Gyr_humanZ_
    };
}

MPU6050::absolute_angles MPU6050::compute_absolute_angles(float elapsed_time_seconds, bool use_gyroscope)
{
    compute_human_sensor_data();

    // Compute accelerometer absolute angles
    accel_human_to_absolute_angles();
    Acceleration_angle_[0] -= angle_pitch_acc_cal_;
    Acceleration_angle_[1] -= angle_roll_acc_cal_;

    if (use_gyroscope)
    {
        // Compute gyroscope absolute angles
        Gyro_angle_[0] = Gyr_humanX_ *elapsed_time_seconds;
        Gyro_angle_[1] = Gyr_humanY_ *elapsed_time_seconds;
    }
    else
    {
        Gyro_angle_[0] = 0;
        Gyro_angle_[1] = 0;
    }

    // Compute final absolute angles, with a filter that:
        // - takes into account the last previous computation
        // - uses a linear combination of accelerometer absolute angles and gyroscope absolute angles
    Final_angle_[0] = filter_first_part_coeff *(Final_angle_[0] + Gyro_angle_[0]) + (1 - filter_first_part_coeff) *Acceleration_angle_[0];
    Final_angle_[1] = filter_first_part_coeff *(Final_angle_[1] + Gyro_angle_[1]) + (1 - filter_first_part_coeff) *Acceleration_angle_[1];

    return (absolute_angles) {Final_angle_[0], Final_angle_[1]};
}

void MPU6050::calibrate_gyro_and_accel()
{
    // Calibration consists of considering the current position as a zero-angle reference for all upcoming measurements

    for (unsigned int _ = 0; _ < cal_rounds_ ; _++)
    {
        compute_human_sensor_data(false);

        gyro_x_cal_ += Gyr_rawX_;
        gyro_y_cal_ += Gyr_rawY_;
        gyro_z_cal_ += Gyr_rawZ_;

        accel_human_to_absolute_angles();
        angle_pitch_acc_cal_ += Acceleration_angle_[0];
        angle_roll_acc_cal_ += Acceleration_angle_[1];

        delay(3);
    }
    gyro_x_cal_ /= cal_rounds_;
    gyro_y_cal_ /= cal_rounds_;
    gyro_z_cal_ /= cal_rounds_;

    angle_pitch_acc_cal_ /=  cal_rounds_;
    angle_roll_acc_cal_ /=  cal_rounds_;
}

void MPU6050::read_raw_sensor_data()
{
    Wire.beginTransmission(i2c_slave_address_);
    Wire.write(0x3B);
    Wire.endTransmission();
    Wire.requestFrom(i2c_slave_address_, 14);
    while(Wire.available() < 14);
    Acc_rawX_ = Wire.read()<<8|Wire.read();
    Acc_rawY_ = Wire.read()<<8|Wire.read();
    Acc_rawZ_ = Wire.read()<<8|Wire.read();
    temperature_ = Wire.read()<<8|Wire.read();
    Gyr_rawX_ = Wire.read()<<8|Wire.read();
    Gyr_rawY_ = Wire.read()<<8|Wire.read();
    Gyr_rawZ_ = Wire.read()<<8|Wire.read();
}

void MPU6050::accel_human_to_absolute_angles()
{
    Acceleration_angle_[0] = atan((Acc_humanY_)/sqrt(pow((Acc_humanX_),2) + pow((Acc_humanZ_),2)))*RAD_TO_DEG;  // accel pitch
    Acceleration_angle_[1] = atan(-1*(Acc_humanX_)/sqrt(pow((Acc_humanY_),2) + pow((Acc_humanZ_),2)))*RAD_TO_DEG;  // accel roll
}
