//
// Created by Abiel on 10/8/18.
//

#include "RobotDimensions.h"
#include <cmath>

#include "Utilities/EctoSettings.h"

RobotDimensions::RobotDimensions(double robotWidth, double robotHeight, double robotWheelBase, double wheelDiameter) {
	this->robotWidth = robotWidth;
	this->robotHeight = robotHeight;

	this->robotWheelBase = robotWheelBase;

	this->wheelDiameter = wheelDiameter;

	wheelRadius = wheelDiameter / 2.0;
	wheelCircumference = wheelDiameter * M_PI;
}

RobotDimensions::RobotDimensions() : RobotDimensions(0.0, 0.0, 0.0, 0.0) {
	;
}

void RobotDimensions::setRobotWidth(double width) {
	this->robotWidth = width;
}

double RobotDimensions::getRobotWidth() const {
	return robotWidth;
}

void RobotDimensions::setRobotHeight(double height) {
	this->robotHeight = height;
}

double RobotDimensions::getRobotHeight() const {
	return robotHeight;
}

void RobotDimensions::setRobotWheelBase(double wheelbase) {
	this->robotWheelBase = wheelbase;
}

double RobotDimensions::getRobotWheelBase() {
	return robotWheelBase;
}

void RobotDimensions::setWheelDiameter(double wheelDiameter) {
	this->wheelDiameter = wheelDiameter;
	this->wheelRadius = wheelDiameter / 2.0;
	this->wheelCircumference = wheelDiameter * M_PI;
}

double RobotDimensions::getWheelDiameter() const {
	return wheelDiameter;
}

void RobotDimensions::setWheelRadius(double wheelRadius) {
	this->wheelRadius = wheelRadius;
	this->wheelDiameter = wheelRadius * 2.0;
	this->wheelCircumference = wheelDiameter * M_PI;
}

double RobotDimensions::getWheelRadius() const {
	return wheelRadius;
}

double RobotDimensions::getWheelCircumference() const {
	return wheelCircumference;
}

/**
 * Gets robot dimensions from an EctoSettings object
 * @param settings
 * @return
 */
RobotDimensions RobotDimensions::getDimensionsFromSettings(EctoSettings &settings) {
	RobotDimensions dimensions;

	dimensions.setRobotWidth(settings.getNumber("Width"));
	dimensions.setRobotHeight(settings.getNumber("Height"));

	dimensions.setRobotWheelBase(settings.getNumber("WheelBase"));

	dimensions.setWheelDiameter(settings.getNumber("WheelDiameter"));

	return dimensions;
}