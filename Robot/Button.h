#pragma once

// Gestion des boutons

#include <Arduino.h>

enum class State
{
	Up,
	Pressed,
	Down,
	Released
};

class Button
{
public:
	void setup(uint8_t _pin, unsigned long debounce_delay = 50);
	State loop();
	State state;

private:
	uint8_t pin;
	uint8_t last_state;
	unsigned long last_time, trigger;
};
