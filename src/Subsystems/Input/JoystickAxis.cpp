//
// Created by Abiel on 9/11/18.
//

#include "JoystickAxis.h"

void JoystickAxis::updateValue(double value) {
	std::lock_guard<std::mutex> lock(joystickMutex);
	this->inValue = value;

	calculateOutput();
}

void JoystickAxis::calculateOutput() {
	;
}

double JoystickAxis::get() const {
	std::lock_guard<std::mutex> lock(joystickMutex);
	return outValue;
}