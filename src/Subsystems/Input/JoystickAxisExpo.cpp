//
// Created by Abiel on 9/11/18.
//

#include "JoystickAxisExpo.h"
#include <cmath>

JoystickAxisExpo::JoystickAxisExpo(double expoValue, double deadZone) {
	this->expoValue = expoValue;
	this->deadZone = deadZone;
}

void JoystickAxisExpo::calculateOutput() {
	double tempValue = std::abs(inValue) > deadZone ? inValue : 0;
	this->outValue = expoValue * std::pow(tempValue, 3.0) + (1.0 - expoValue) * tempValue;
}