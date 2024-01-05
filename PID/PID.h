#pragma once

class PID
{
public:
    PID(float Kp=1, float Ki=0, float Kd=0);

    void set_coefficients(float Kp, float Ki, float Kd);
    void set_setpoint(float setpoint);
    float get_pid();
    void get_pid_debug(float pid_debug[]);
    float get_error();
    void compute(float measure, float dt);

private:
    float Kp_, Ki_, Kd_;  // pid coefficients
    float pid_p_=0, pid_i_=0, pid_d_=0;  // each pid component
    float pid_=0;  // sum of all pid components
    float setpoint_=0, error_=0;
    bool first_iteration=true;
};
