//
// Created by Abiel on 9/5/18.
//

#include "ModelAcceleration.h"

ModelAcceleration::ModelAcceleration(EctoTalon &leftSideIn, EctoTalon &rightSideIn,
                                     const CharacterizationConstants &voltageConstantsIn) :
		leftSide(leftSideIn), rightSide(rightSideIn), voltageConstants(voltageConstantsIn) {
	;
}

void ModelAcceleration::init() {
	startTime = timer.getTime();

	leftSide.setControlMode(MotorMode::VOLTAGE);
	rightSide.setControlMode(MotorMode::VOLTAGE);

	leftSide.Set(maximumPower);
	rightSide.Set(maximumPower);

	previousTime = startTime;
}

void ModelAcceleration::update() {
	double currentTime = timer.getTime();

	double leftVelocity = leftSide.getSpeedRPM();
	double rightVelocity = rightSide.getSpeedRPM();

	//Do not run first loop
	if (previousTime == startTime) {
		previousTime = currentTime;

		previousLeftVelocity = leftVelocity;
		previousRightVelocity = rightVelocity;
		return;
	}

	double leftAcceleration = (leftVelocity - previousLeftVelocity) / (currentTime - previousTime);
	double rightAcceleration = (rightVelocity - previousRightVelocity) / (currentTime - previousTime);

	leftAcceleration = MathUtil::almostEqual(leftAcceleration, 0.0) ? 0.0 : leftAcceleration;
	rightAcceleration = MathUtil::almostEqual(rightAcceleration, 0.0) ? 0.0 : rightAcceleration;

	leftData.addData(setVoltage, AccelerationModelDataPoint(leftVelocity, leftAcceleration));
	rightData.addData(setVoltage, AccelerationModelDataPoint(rightVelocity, rightAcceleration));

	if (timer.getTime() >= startTime + timeToTest)
		this->finished = true;
}

void ModelAcceleration::finish() {
	leftSide.Set(0);
	rightSide.Set(0);
}