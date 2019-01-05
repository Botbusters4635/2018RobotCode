//
// Created by Abiel on 9/4/18.
//

#include "ModelVelocity.h"

ModelVelocity::ModelVelocity(EctoTalon &leftSideOut, EctoTalon &rightSideOut, double minimumSet, double maximumSet,
                             double rampRate, double waitTimeForMotor) : leftSide(leftSideOut),
                                                                         rightSide(rightSideOut) {
	this->maximumSet = maximumSet;
	this->minimumSet = minimumSet;

	this->waitTimeForMotor = waitTimeForMotor;

	this->rampRate = rampRate;

	currentSet = minimumSet;
}

void ModelVelocity::init() {
	leftSide.setControlMode(MotorMode::VOLTAGE);
	rightSide.setControlMode(MotorMode::VOLTAGE);

	leftSide.Set(0.0);
	rightSide.Set(0.0);
}

void ModelVelocity::update() {
	if (timer.getTime() >= timeToWait) {
		leftData.addData(std::make_pair(maximumVoltage * currentSet, leftSide.getSpeedRPM()));
		rightData.addData(std::make_pair(maximumVoltage * currentSet, rightSide.getSpeedRPM()));

		leftSide.Set(currentSet);
		rightSide.Set(currentSet);

		currentSet += rampRate;
		timeToWait = timer.getTime() + waitTimeForMotor;

		this->finished = currentSet >= maximumSet;
	}
}

void ModelVelocity::finish() {
	leftSide.Set(0);
	rightSide.Set(0);
}

LinearRegression ModelVelocity::getLeftRegression() const {
	return leftData.getDataModel();
}

LinearRegression ModelVelocity::getRightRegression() const {
	return rightData.getDataModel();
}