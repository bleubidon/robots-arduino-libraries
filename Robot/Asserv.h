#pragma once

#include <helpers.h>
#include <Robot.h>
#include <TimerOne.h> // Contrôle des interrupts

class Asserv
{
public:
    Asserv();

    void setup();
    void loop();
    void depl_init(int argc, char **argv);

private:
    static void handle_interrupt();

    static float dist_d_freinage, rot_d_freinage;

    static float dist_vitesseConsigne, dist_distanceConsigne, dist_vitesseDesiree;
    static float rot_vitesseConsigne, rot_distanceConsigne, rot_vitesseDesiree;
    static float depl_distance, depl_angle;
    static bool target_reached;

    static float pid_dist, pid_rot, vg, vd;
    static float pid_min_value, pid_max_value;
};
