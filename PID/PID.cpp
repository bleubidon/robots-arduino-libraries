#include "PID.h"

PID::PID(float Kp, float Ki, float Kd) : Kp_(Kp), Ki_(Ki), Kd_(Kd) {}

void PID::set_coefficients(float Kp, float Ki, float Kd)
{
    Kp_ = Kp;
    Ki_ = Ki;
    Kd_ = Kd;
}

void PID::set_setpoint(float setpoint)
{
    setpoint_ = setpoint;
}

float PID::get_pid()
{
    return pid_;
}

void PID::get_pid_debug(float pid_debug[])
{
    pid_debug[0] = pid_p_;
    pid_debug[1] = pid_i_;
    pid_debug[2] = pid_d_;
}

float PID::get_error()
{
    return error_;
}

void PID::compute(float measure, float dt)
{
    float error = setpoint_ - measure;

    pid_p_ = Kp_ * error;
    pid_i_ += Ki_ * error;

    if (first_iteration || dt <= 0)
        first_iteration = false;
    else
        pid_d_ = Kd_ * ((error - error_) / dt);
    
    pid_ = pid_p_ + pid_i_ + pid_d_;
    error_ = error;
}
