#pragma once

// Fonctions relatives au déplacement et au timing du robot

#include "PID_old_and_bad.h"
#include "Sick.h"
#include "Moteur.h"
#include "Odometrie.h"


class c_Robot
{
	public:
		struct Config
		{
			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];
			int num_sicks, *sicks;
			unsigned int dureeMatch;
            bool do_limited_duration;

			unsigned int min_speed, dist_max_speed, dist_acceleration, rot_max_speed, rot_acceleration;
            int min_cons_moteur, max_cons_moteur;
            float precision_dist, precision_rot;

			PID dist;
			PID rot;
		};

		void setup(c_Robot::Config config);

		void start();
		void stop();

		// dist: centimètres, rot: degrés
		void consigne(float _dist, float _rot);
		void consigne_rel(float _dist, float _rot);

		// helpers
		void translate(float _dist, bool blocking = true);
		void rotate(float _angle, bool blocking = true);
		void go_to(vec _dest, bool blocking = true);
		void go_to_bkwd(vec _dest, bool blocking = true);
		void look_at(vec _point, bool blocking = true);

		int scale(float speed);
		bool loop_pid();

		Odometrie& pos() { return position; }
		PID& dist_pid() { return dist; }
		PID& rot_pid() { return rot; }


		Odometrie position;
		Moteur moteurs[2];

		int num_sicks;
		Sick *capteurs;

		unsigned long start_time;
		unsigned int duration, min_speed, dist_max_speed, dist_acceleration, rot_max_speed, rot_acceleration;
        int min_cons_moteur, max_cons_moteur;
        float precision_dist, precision_rot;
        bool do_limited_duration;

		// PID
		PID dist, rot;
		bool consigne_pid, do_asserv;
		unsigned long prev_time;

		int initial_dist;
};

extern c_Robot Robot;
