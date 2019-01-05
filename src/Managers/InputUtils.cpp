//
// Created by Abiel on 8/27/18.
//
#include "InputUtils.h"
#include "Subsystems/Input/InputValue.h"

double InputUtils::applyDeadzone(double input, double deadzone) {
	return std::abs(input) >= deadzone ? input : 0.0;
}

double InputUtils::applyExponentialCurve(double input, double expoValue) {
	return (expoValue * std::pow(input, 3.0) + (1 - expoValue) * input);
}

MotorValues InputUtils::convertInputValueToMotorValue(const InputValue &value, MotorMode valueType) {
	return {value.getLeftVel(), value.getRightVel(), valueType};
}

