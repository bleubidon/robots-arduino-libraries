#include "PID.h"

PID::PID(float _Kp, float _Ki, float _Kd)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;

    erreur = 0.0f;
    pid_p = 0.0f;
    pid_d = 0.0f;
    pid = 0.0f;
}

void PID::set_consigne(float _consigne)
{
	consigne = _consigne;
	erreur = 0.0f;
	vitesse = 0.0f;
	integrale = 0.0f;
}

void PID::set_coefs(float _Kp, float _Ki, float _Kd)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
}

float PID::compute(float err, float dt)
{
    pid_p = Kp * err;
    pid_d = Kd * ((err - erreur) * 1000 / dt);

    // Serial << "proportionnel: " << proportionnel << ", intégrale: " << integrale << "; derivée: " << derivee << endl;

	pid	= pid_p + pid_d;

	erreur = err;
	return pid;
}
