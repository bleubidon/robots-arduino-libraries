#include "Asserv.h"

float Asserv::dist_d_freinage, Asserv::rot_d_freinage;

float Asserv::dist_vitesseConsigne, Asserv::dist_distanceConsigne, Asserv::dist_vitesseDesiree;
float Asserv::rot_vitesseConsigne, Asserv::rot_distanceConsigne, Asserv::rot_vitesseDesiree;
float Asserv::depl_distance, Asserv::depl_angle;
bool Asserv::target_reached;

float Asserv::pid_dist, Asserv::pid_rot, Asserv::vg, Asserv::vd;
float Asserv::pid_min_value = 20;  // = 20
float Asserv::pid_max_value = 255; //  = 255

Asserv::Asserv()
{
}

void Asserv::setup()
{
    Timer1.initialize(100000); // 10 Hz
    Timer1.attachInterrupt(handle_interrupt);

    char **c = calloc(2, sizeof(char *));
    c[1] = "100";
    depl_init(2, c);
}

void Asserv::loop()
{
    if (!target_reached)
    {
        dist_vitesseDesiree = Robot.position.dist() < depl_distance - dist_d_freinage ? Robot.dist_max_speed : 0;
        rot_vitesseDesiree = Robot.position.rot() < depl_angle - rot_d_freinage ? Robot.rot_max_speed : 0;
        delay(0);
    }
    else
    {
        dist_vitesseDesiree = 0;
        rot_vitesseDesiree = 0;
    }
}

void Asserv::depl_init(int argc, char **argv)
{
    Serial << "argc: " << argc << " argv[0]: " << argv[0] << " argv[1]: " << argv[1] << endl;
    if (argc == 2)
    {
        target_reached = false;
        depl_distance = atof(argv[1]); //en cm
        depl_angle = 0;                // en °

        Robot.dist_max_speed = round(sqrt(Robot.dist_acceleration * depl_distance));
        Robot.rot_max_speed = round(sqrt(Robot.rot_acceleration * depl_angle));

        // dist_d_freinage = Robot.dist_max_speed * Robot.dist_max_speed / (2 * Robot.dist_acceleration);
        // rot_d_freinage = Robot.rot_max_speed * Robot.rot_max_speed / (2 * Robot.rot_acceleration);
        dist_d_freinage = depl_distance / 2;
        rot_d_freinage = depl_angle / 2;

        Serial << "depl_distance: " << depl_distance << ", dist_d_freinage: " << dist_d_freinage << endl;
        Serial << "depl_angle: " << depl_angle << ", rot_d_freinage: " << rot_d_freinage << endl;

        Robot.do_asserv = true;
    }
}

void Asserv::handle_interrupt()
{
    // Arrêter le robot après une certaine durée
    if (Robot.do_limited_duration && millis() - Robot.start_time > Robot.duration * 1000)
    {
        Robot.stop();
        while (1)
            ;
    }

    // Asservissement
    if (Robot.do_asserv)
    {
        // Generation consigne
        if (dist_vitesseConsigne < dist_vitesseDesiree)
            dist_vitesseConsigne += Robot.dist_acceleration;
        else if (dist_vitesseConsigne > dist_vitesseDesiree)
            dist_vitesseConsigne -= Robot.dist_acceleration;
        dist_distanceConsigne += dist_vitesseConsigne;

        // if (dist_distanceConsigne > depl_distance - Robot.position.dist())
        //     dist_distanceConsigne = depl_distance - Robot.position.dist();
        if (dist_distanceConsigne > depl_distance)
            dist_distanceConsigne = depl_distance;

        if (rot_vitesseConsigne < rot_vitesseDesiree)
            rot_vitesseConsigne += Robot.rot_acceleration;
        else if (rot_vitesseConsigne > rot_vitesseDesiree)
            rot_vitesseConsigne -= Robot.rot_acceleration;
        rot_distanceConsigne += rot_vitesseConsigne;

        // if (rot_distanceConsigne > depl_angle - Robot.position.rot())
        //     rot_distanceConsigne = depl_angle - Robot.position.rot();
        if (rot_distanceConsigne > depl_angle)
            rot_distanceConsigne = depl_angle;

        // PID
        Robot.position.update();
        // DEBUG(Serial << "position.dist: " << Robot.position.dist() << ", position.rot: " << Robot.position.rot() << endl);

        pid_dist = Robot.dist.compute(dist_distanceConsigne - Robot.position.dist(), 100); // /!\ dt value
        pid_rot = Robot.rot.compute(rot_distanceConsigne - Robot.position.rot(), 100);     // /!\ dt value

        // Ecrêtage spécifique
        if (abs(Robot.dist.erreur) < Robot.precision_dist && abs(Robot.rot.erreur) < Robot.precision_rot)
        {
            target_reached = true;
        }
        else
        {
            target_reached = false;
            if (dist_vitesseDesiree == 0 && abs(Robot.dist.erreur) >= Robot.precision_dist ||
                depl_distance * Robot.dist.Kp < Robot.min_cons_moteur && depl_distance > 0)
            {
                pid_dist = constrain_abs(pid_dist, pid_min_value, pid_max_value);
            }

            if (rot_vitesseDesiree == 0 && abs(Robot.rot.erreur) >= Robot.precision_rot ||
                depl_angle * Robot.rot.Kp < Robot.min_cons_moteur && depl_angle > 0)
            {
                pid_rot = constrain_abs(pid_rot, pid_min_value, pid_max_value);
            }
        }

        vg = pid_dist - pid_rot;
        vd = pid_dist + pid_rot;
        // vg = - pid_rot;
        // vd = pid_rot;

        //Ecrêtage consignes moteurs
        /* if (abs(Robot.dist.erreur) < Robot.precision_dist && abs(Robot.rot.erreur) < Robot.precision_rot) {
            target_reached = true;
            vg = constrain_abs(vg, 0, Robot.max_cons_moteur);
            vd = constrain_abs(vd, 0, Robot.max_cons_moteur);
        }
        else {
            target_reached = false;
            vg = constrain_abs(vg, Robot.min_cons_moteur, Robot.max_cons_moteur);
            vd = constrain_abs(vd, Robot.min_cons_moteur, Robot.max_cons_moteur);
        } */

        vg = constrain_abs(vg, 0, Robot.max_cons_moteur);
        vd = constrain_abs(vd, 0, Robot.max_cons_moteur);

        Robot.moteurs[0].consigne(vg);
        Robot.moteurs[1].consigne(vd);

        // Serial << "dist_vitesseDesiree: " << dist_vitesseDesiree <<
        // ", dist_vitesseConsigne: " << dist_vitesseConsigne <<
        // ", dist_distanceConsigne: " << dist_distanceConsigne <<
        // ", pid_dist: " << pid_dist <<
        // ", position.dist: " << Robot.position.dist() <<
        // endl;
        // Serial << "rot_vitesseDesiree: " << rot_vitesseDesiree <<
        // ", rot_vitesseConsigne: " << rot_vitesseConsigne <<
        // ", rot_distanceConsigne: " << rot_distanceConsigne <<
        // ", pid_rot: " << pid_rot <<
        // ", position.rot: " << Robot.position.rot() <<
        // endl;
        Serial << "vg: " << vg << ", vd: " << vd << ", target_reached: " << target_reached << endl;
        // Serial << endl;
    }
}
