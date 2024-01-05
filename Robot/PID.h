#pragma once

#include <helpers.h>

class PID
{
	public:
		PID(float _Kp = 0, float _Ki = 0, float _Kd = 0);

		void set_consigne(float _consigne);
		void set_coefs(float _Kp, float _Ki, float _Kd);

		float compute(float erreur, float dt);

		float Kp, Ki, Kd;
        float pid_p, pid_d, pid;
		float consigne, erreur, vitesse, integrale;
};
