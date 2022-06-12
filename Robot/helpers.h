#pragma once

// Fonctions utilitaires; gestion des logs

//#define NDEBUG	// Commenter pour activer les messages de type debug
//#define NLOG		// Commenter pour activer les messages de type log

#include <Arduino.h>

extern const uint8_t GAUCHE;
extern const uint8_t DROITE;

extern const char endl;

inline int clamp(int _min, int _val, int _max)
{
	return max(_min, min(_val, _max));
}

inline int constrain_abs(int x, int a, int b) {
    return x > 0 ? constrain(x, a, b): -constrain(-x, a, b);
}

template <class T>
inline Print &operator<<(Print &obj, T arg)
{
	obj.print(arg);
	return obj;
}

#ifdef NDEBUG
# define DEBUG(x)
#else
# define DEBUG(x) x
#endif

#undef NLOG

#ifdef NLOG
# define LOG(x)
#else
# define LOG(x) x
#endif
