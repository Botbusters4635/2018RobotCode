#include "GearReceiver.h"

GearReceiver::GearReceiver() : EctoSubsystem("Gear Receiver") {
	currentState = false;

	stick.addButton(&buttonState, "B");

	close();
}

void GearReceiver::open() {
	currentState = true;
	pcm.setPiston("Gear", currentState);
}

void GearReceiver::set(bool target) {
	if (target) {
		open();
	} else {
		close();
	}
}

void GearReceiver::close() {
	currentState = false;
	pcm.setPiston("Gear", currentState);
}

void GearReceiver::toggle() {
	currentState = !currentState;
	pcm.setPiston("Gear", currentState);
}

bool GearReceiver::getState() const {
	return currentState;
}

void GearReceiver::initTeleop() {

}

void GearReceiver::updateTeleop() {
	set(buttonState.get());
}

void GearReceiver::initAuto() {
	close();
}

void GearReceiver::updateAuto() {

}

void GearReceiver::initDisabled() {

}

void GearReceiver::updateDisabled() {

}

void GearReceiver::sendInitialTelemetry() const {
	frc::SmartDashboard::PutBoolean("Gear Intake Enabled", getState());
}

void GearReceiver::sendTelemetry() const {
	frc::SmartDashboard::PutBoolean("Gear Intake Enabled", getState());
}




