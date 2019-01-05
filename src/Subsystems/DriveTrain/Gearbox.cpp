//
// Created by Abiel on 8/26/18.
//

#include "Gearbox.h"

Gearbox::Gearbox(const GearboxConfig &config) {
	this->config = config;

	if (config.hasShift)
		this->currentState = config.defaultState;

	if (config.hasEncoder)
		this->encoderTicksPerShaftRotation = config.encoderTicksPerEncoderRev / config.shaftRotationsPerEncoderTurn;
	else
		this->encoderTicksPerShaftRotation = 0;
}

void Gearbox::setConfig(const GearboxConfig &config) {
	this->config = config;
}

GearboxConfig Gearbox::getConfig() const {
	return config;
}

double Gearbox::getCurrentMaximumSpeed() const {
	if (config.hasShift) {
		switch (currentState) {
			case (GearboxState::lowGear):
				return config.maximumLowGearSpeed;

			case (GearboxState::highGear):
				return config.maximumHighGearSpeed;
		}
	}

	return config.maximumSpeed;
}

double Gearbox::getMaximumSpeed(GearboxState state) const {
	if (config.hasShift) {
		switch (state) {
			case (GearboxState::lowGear):
				return config.maximumLowGearSpeed;

			case (GearboxState::highGear):
				return config.maximumHighGearSpeed;
		}
	}

	return config.maximumSpeed;
}

bool Gearbox::hasShifts() const {
	return config.hasShift;
}

bool Gearbox::hasEncoder() const {
	return config.hasEncoder;
}

unsigned int Gearbox::encoderTicksToShaftTicks(unsigned int encoderTicks) const {
	return encoderTicks / config.shaftRotationsPerEncoderTurn;
}

unsigned int Gearbox::shaftTicksToEncoderTicks(unsigned int shaftTicks) const {
	return shaftTicks * config.shaftRotationsPerEncoderTurn;
}

double Gearbox::encoderTicksToEncoderRotations(unsigned int encoderTicks) const {
	return encoderTicks * config.encoderTicksPerEncoderRev;
}

unsigned int Gearbox::encoderRotationsToEncoderTicks(double encoderRotations) const {
	return std::round(encoderRotations / config.encoderTicksPerEncoderRev);
}

double Gearbox::encoderTicksToShaftRotations(unsigned int encoderTicks) const {
	return encoderTicksToEncoderRotations(encoderTicks) * config.shaftRotationsPerEncoderTurn;
}

unsigned int Gearbox::shaftRotationsToEncoderTicks(double shaftRotations) const {
	return encoderRotationsToEncoderTicks((shaftRotations / config.shaftRotationsPerEncoderTurn));
}