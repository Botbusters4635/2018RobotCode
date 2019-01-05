#include "EctoClimber.h"

EctoClimber::EctoClimber() : climbMotor(*motors.getMotor("ClimbMotor")), EctoSubsystem("Climber") {
	climbMotor.setControlMode(ControlMode::PercentOutput);
	climbMotor.Set(offSpeed);

	stick.addButton(&enableClimber, "Y");
}

/**
 * Returns the current motor state.
 * @return
 */
double EctoClimber::getClimberState() const {
	double motorState = climbMotor.getSetValue();
	return motorState;
}


void EctoClimber::initTeleop() {
}

void EctoClimber::updateTeleop() {
	double output = enableClimber.get() ? onSpeed : offSpeed;
	climbMotor.Set(output);
}

void EctoClimber::initAuto() {

}

void EctoClimber::updateAuto() {

}

void EctoClimber::initDisabled() {

}

void EctoClimber::updateDisabled() {

}

void EctoClimber::sendInitialTelemetry() const {
	frc::SmartDashboard::PutNumber("Climber State", getClimberState());
}

void EctoClimber::sendTelemetry() const {
	frc::SmartDashboard::PutNumber("Climber State", getClimberState());
}



