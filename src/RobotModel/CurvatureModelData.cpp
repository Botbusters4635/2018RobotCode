//
// Created by Abiel on 9/7/18.
//

#include "CurvatureModelData.h"

CurvatureModelDataPoint::CurvatureModelDataPoint(double linearVelocity, double angularVelocity, double leftVoltage,
                                                 double rightVoltage) {
	this->linearVelocity = linearVelocity;
	this->angularVelocity = angularVelocity;

	this->leftVoltage = leftVoltage;
	this->rightVoltage = rightVoltage;
}

CurvatureModelDataPoint::CurvatureModelDataPoint() : CurvatureModelDataPoint(0.0, 0.0, 0.0, 0.0) {
	;
}

void CurvatureModelDataPoint::setLinearVelocity(double linearVelocity) {
	this->linearVelocity = linearVelocity;
}

double CurvatureModelDataPoint::getLinearVelocity() const {
	return linearVelocity;
}

void CurvatureModelDataPoint::setAngularVelocity(double angularVelocity) {
	this->angularVelocity = angularVelocity;
}

double CurvatureModelDataPoint::getAngularVelcoty() const {
	return angularVelocity;
}

void CurvatureModelDataPoint::setLeftVoltage(double leftVoltage) {
	this->leftVoltage = leftVoltage;
}

double CurvatureModelDataPoint::getLeftVoltage() const {
	return leftVoltage;
}

void CurvatureModelDataPoint::setRightVoltgae(double rightVoltage) {
	this->rightVoltage = rightVoltage;
}

double CurvatureModelDataPoint::getRightVoltage() const {
	return rightVoltage;
}

void CurvatureModelDataPoint::setVoltages(std::pair<double, double> voltages) {
	setVoltages(voltages.first, voltages.second);
}

void CurvatureModelDataPoint::setVoltages(double leftVoltage, double rightVoltage) {
	this->leftVoltage = leftVoltage;
	this->rightVoltage = rightVoltage;
}

std::pair<double, double> CurvatureModelDataPoint::getVoltages() const {
	return std::make_pair(leftVoltage, rightVoltage);
}