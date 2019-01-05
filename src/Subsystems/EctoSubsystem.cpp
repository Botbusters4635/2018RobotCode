//
// Created by Alberto on 13/12/17.
//
#include "EctoSubsystem.h"

EctoSubsystem::EctoSubsystem(const string &name) {
	this->name = name;
	//manager.addSubsystem(this);
}

string EctoSubsystem::getName() const {
	return name;
}

void EctoSubsystem::initRobot() {
	;
}

void EctoSubsystem::updateRobot() {
	;
}

void EctoSubsystem::initDisabled() {
	;
}

void EctoSubsystem::updateDisabled() {
	;
}

void EctoSubsystem::initTeleop() {
	;
}

void EctoSubsystem::updateTeleop() {
	;
}

void EctoSubsystem::initAuto() {
	;
}

void EctoSubsystem::updateAuto() {
	;
}
