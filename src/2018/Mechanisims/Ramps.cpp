#ifdef BOTBUSTERSREBIRTH_RAMP_H
#include "Ramps.h"

Ramps::Ramps() : EctoSubsystem("Ramps") {
	log->info("Setting up Ramps...");
	rampRMotor = motors.getMotor("RampR");
	settings.setCurrentGroup("Ramps");
	output = settings.getNumber("speed");

}

void Ramps::initTeleop() {

}

void Ramps::updateTeleop() {
	if (stick.getButtonState("Y")) {
		rampRMotor->Set(output);
	} else if (stick.getButtonState("B")) {
		rampRMotor->Set(-output);
	} else {
		rampRMotor->Set(0);
	}
}

void Ramps::initAuto() {

}

void Ramps::updateAuto() {

}

void Ramps::initDisabled() {

}

void Ramps::updateDisabled() {

}

void Ramps::sendInitialTelemetry() const {

}

void Ramps::sendTelemetry() const {
}
#endif