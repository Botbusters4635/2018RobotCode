#include "MotorValues.h"

MotorValues::MotorValues(double leftValue, double rightValue, MotorMode type) {
	this->left = leftValue;
	this->right = rightValue;

	this->valueType = type;
}

MotorValues::MotorValues() : MotorValues(0, 0, MotorMode::DEFAULT) {
	;
}

double MotorValues::calculateMax() const {
	return std::max(left, right);
}

double MotorValues::getLeft() const {
	return left;
}

double MotorValues::getRight() const {
	return right;
}

void MotorValues::setLeft(double left) {
	this->left = left;
}

void MotorValues::setRight(double right) {
	this->right = right;
}

MotorMode MotorValues::getValueType() const {
	return valueType;
}

void MotorValues::setValueType(MotorMode valueType) {
	this->valueType = valueType;
}

std::string MotorValues::getValueTypeName(MotorMode valueType) {
	switch (valueType) {
		case (MotorMode::VOLTAGE):
			return "Voltage";

		case (MotorMode::VELOCITY):
			return "Velocity";

		case (MotorMode::POSITION):
			return "Position";

		default:
			return "Default";
	}
}

void MotorValues::printValues() {
	std::cout << "Left: " << left << " Right: " << right << std::endl;
}

std::string MotorValues::getMotorModeName(MotorMode mode) {
	switch (mode) {
		case MotorMode::VOLTAGE:
			return "Voltage";

		case MotorMode::POSITION:
			return "Position";

		case MotorMode::VELOCITY:
			return "Velocity";

		case MotorMode::CURRENT:
			return "Current";

		case MotorMode::FOLLOWER:
			return "Follower";

		case MotorMode::MOTION_PROFILE:
			return "Motion Profile";

		case MotorMode::MOTION_PROFILE_ARC:
			return "Motion Profile Arc";

		case MotorMode::DEFAULT:
			return "Default";

		default:
			return "Unknown";
	}
}