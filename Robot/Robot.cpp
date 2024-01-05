#include "Robot.h"

#include <PWM.h>

const uint8_t GAUCHE = 0;
const uint8_t DROITE = 1;
const char endl = '\n';

bool Sick::enable = false;

c_Robot Robot;

void c_Robot::setup(c_Robot::Config config)
{
	position.setup(config.odometrie);

	InitTimersSafe();
	moteurs[GAUCHE].setup(config.moteurs[GAUCHE]);
	moteurs[DROITE].setup(config.moteurs[DROITE]);

	num_sicks = config.num_sicks;
	capteurs = new Sick[num_sicks];
	//capteurs = (Sick*)malloc(num_sicks * sizeof(Sick));
	for (int i(0); i < num_sicks; i++)
		capteurs[i].setup(config.sicks[i]);

	min_speed = config.min_speed;
	dist_max_speed = config.dist_max_speed;
    dist_acceleration = config.dist_acceleration;
    rot_max_speed = config.rot_max_speed;
    rot_acceleration = config.rot_acceleration;
    min_cons_moteur = config.min_cons_moteur;
    max_cons_moteur = config.max_cons_moteur;
    precision_dist = config.precision_dist;
    precision_rot = config.precision_rot;

	duration = config.dureeMatch;
    do_limited_duration = config.do_limited_duration;

	dist = config.dist;
	rot = config.rot;
	consigne_pid = false;
    do_asserv = false;
}

void c_Robot::stop()
{
    Serial << "__ROBOT STOP__" << endl;
	moteurs[GAUCHE].consigne(0);
	moteurs[DROITE].consigne(0);

	consigne_pid = false;
    do_asserv = false;
}

void c_Robot::consigne(float _dist, float _rot)
{
	dist.set_consigne(_dist);
	rot.set_consigne(_rot);

	prev_time = millis();
	consigne_pid = true;

	LOG(Serial << "CMD: " << "dist: " << _dist << "," << "rot: " << _rot << endl);
}

void c_Robot::consigne_rel(float _dist, float _rot)
{
	consigne((position.dist() + _dist), position.rot() + _rot);
}

// Helpers
void c_Robot::translate(float _dist, bool blocking)
{
	consigne_rel(_dist, 0);

	if (blocking)
		while (Robot.loop_pid())
			;
}

void c_Robot::rotate(float _angle, bool blocking)
{
	consigne_rel(0, _angle);

	if (blocking)
		while (Robot.loop_pid())
			;
}

void c_Robot::go_to(vec _dest, bool blocking)
{
    // Pivote dans la direction de la destination (bloquant)
	look_at(_dest, true);

	// Avance
	vec dir = _dest - position.pos();
	consigne_rel(vec::dist(position.pos(), _dest), dir.angle() - position.rot());
    
	if (blocking)
		while (Robot.loop_pid())
			;
}

void c_Robot::go_to_bkwd(vec _dest, bool blocking)
{
    // Pivote dans la direction de la destination (bloquant)
//    look_at(_dest, true);

    // Recule
    vec dir = _dest - position.pos();

    Serial << "current angle: " << position.rot() << endl;

    consigne_rel(-vec::dist(position.pos(), _dest), 0); // dir.angle() - position.rot() + 180

    if (blocking)
        while (Robot.loop_pid())
            ;
}

void c_Robot::look_at(vec _point, bool blocking)
{
	Sick::enable = false;
	vec dir = _point - position.pos();
	consigne(position.dist(), dir.angle());

	if (blocking)
		while (Robot.loop_pid())
			;

	Sick::enable = true;
}

// PID
float angle_diff(float a, float b)
{
	if (b < a)
		return -angle_diff(b, a);
	if (b < a + 180)
		return b - a;
	else
		return (b - 360) - a;
}

int c_Robot::scale(float speed)
{
	if (speed > 0)
		return speed + min_speed;
//		return min(speed + min_speed, max_speed);
	else
		return speed - min_speed;
//		return max(speed - min_speed, -max_speed);
}

bool c_Robot::loop_pid()
{
	if (!consigne_pid)
		return false;

	unsigned long stop_start = millis();
	if (Sick::enable)
	{
		int i = 0;
		while (i < num_sicks)
		{

			if (capteurs[i++].is_active())
			{
				Serial << "STOOOOOP" << i << endl;
				moteurs[GAUCHE].consigne(0);
				moteurs[DROITE].consigne(0);
				i = 0;
			}
		}
	}
	


	// Mise à jour de la consigne toute les 10 millisecondes (au moins)
	const unsigned long min_delay = 10;
	unsigned long current_time = millis();
	prev_time += (current_time - stop_start);

	unsigned long dt = current_time - prev_time;
	if (dt < min_delay)
		return true;
	prev_time = current_time;


	// Lecture des capteurs de position et calcul de la consigne grâce aux PIDs
	position.update();
    // LOG(Serial << "position.dist: " << position.dist() << ", position.rot: " << position.rot() << endl;)
    // Serial << "cons: " << dist.consigne - position.dist() << ", dt: " << dt << endl;

	float vitesse_dist = dist.compute(dist.consigne - position.dist(), dt);
	float vitesse_rot = rot.compute(angle_diff(rot.consigne, position.rot()), dt);

    float vg = vitesse_dist + vitesse_rot;
	float vd = vitesse_dist - vitesse_rot;
	
    // Ecrêtage des consignes moteurs
    // vg = constrain(vg, 0, max_speed);
    // vd = constrain(vd, 0, max_speed);

    // moteurs[0].consigne(vg);
    // moteurs[1].consigne(vd);
	

	// Ecrit les données de log sur le port série
	// LOG(Serial << "PID: cons: " << dist.consigne << " " << rot.consigne << "| pos: " <<
    //     position.dist() << " " << position.rot() << "| vit: " <<
	// 	vitesse_dist << " " << vitesse_rot << "| vg: " <<
    //     vg <<  " vd: " << vd
	//     << endl;)
	return true;
}
