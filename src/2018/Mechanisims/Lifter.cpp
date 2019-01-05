//
// Created by Alberto on 2/18/18.
//

#include "Lifter.h"

Lifter::Lifter() : EctoSubsystem("Lifter") {
	master = motors.getMotor("ElevatorRobotMaster");
	slave1 = motors.getMotor("ElevatorRobotSlave1");
	slave2 = motors.getMotor("ElevatorRobotSlave2");
	slave3 = motors.getMotor("ElevatorRobotSlave3");

	liftElevator = motors.getMotor("LiftElevator");
	motors.setupSlave(*slave1, *master);
	motors.setupSlave(*slave2, *master);
	motors.setupSlave(*slave3, *master);


	master->setBrake(true);
	slave1->setBrake(true);
	slave2->setBrake(true);
	slave3->setBrake(true);

	liftElevator->setBrake(false);
	settings.setCurrentGroup("Lifter");
	upSpeed = settings.getNumber("upSpeed");
	downSpeed = settings.getNumber("downSpeed");

	stick.addButton(&upButton, "rightPush");

}

void Lifter::initTeleop() {
}

void Lifter::updateTeleop() {

	int pov = stick.getPOV();
	int pov2 = stick.getPOV(2);

	bool upBt = upButton.get();
	//bool downBt = stick.getButtonState("leftPush", 2);

	if (upBt || pov2 == 0) {
		liftElevator->Set(upSpeed);
	} else if (pov == 180 || pov2 == 180) {
		master->Set(downSpeed);
	} else if (pov2 == 90) {
		master->Set(downSpeed * -1.0);
	} else {
		liftElevator->Set(0);
		master->Set(0);
	}

}

void Lifter::initAuto() {
	;
}

void Lifter::updateAuto() {

}

void Lifter::initDisabled() {
	;
}

void Lifter::updateDisabled() {
	;
}

void Lifter::sendInitialTelemetry() const {

}

void Lifter::sendTelemetry() const {

}
