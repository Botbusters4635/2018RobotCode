//
// Created by Abiel on 9/14/18.
//

#include "TankState.h"
#include "Math/Kinematics/EctoKinematicsTankRebirth.h"
#include "Utilities/EctoMath.h"

TankState::TankState() : EctoSubsystem("TankState"), leftMotor(*motors.getMotor(leftMotorName)),
                         rightMotor(*motors.getMotor(rightMotorName)), data(bufferSize) {
	this->dimensions = properties.getDimensions();
}

void TankState::initRobot() {
	watchdog = new EctoWatchdog("TankState", std::chrono::milliseconds(2000), std::chrono::milliseconds(10),
	                            std::bind(&TankState::updateState, this));
	watchdog->start();
}

void TankState::updateRobot() {

}

void TankState::sendInitialTelemetry() const {
	;
}

void TankState::sendTelemetry() const {
	auto lastPose = robotData.getData<std::pair<double, RobotPose2D>>("TankStateLastPoint");

	SmartDashboard::PutNumber("RobotX", lastPose.second.getX());
	SmartDashboard::PutNumber("RobotY", lastPose.second.getY());
	SmartDashboard::PutNumber("RobotTimeDuration", lastPose.first);
	SmartDashboard::PutNumber("RobotTheta", lastPose.second.getHeading().getRadians());
}

std::pair<double, RobotPose2D> TankState::getLastMeasurement() const {
	return data.getLastElement();
}

Twist2D TankState::generateOdometryFromSensors(double leftDeltaDistance, double rightDeltaDistance,
                                               const Rotation2D &gyroAngle) const {

	RobotPose2D lastMeasurement = getLastMeasurement().second;

	Twist2D delta = EctoKinematicsTankRebirth::forwardKinematics(lastMeasurement.getHeading(), gyroAngle,
	                                                             leftDeltaDistance, rightDeltaDistance);

	distanceDriven += delta.getDx();
	return delta;
}

void TankState::updateState() {

	double leftPosition = leftMotor.getPosition();
	double rightPosition = rightMotor.getPosition();

	double deltaLeftPosition = leftPosition - previousLeftDistance;
	double deltaRightPosition = rightPosition - previousRightDistance;

	SmartDashboard::PutNumber("LeftPosition", deltaLeftPosition);
	SmartDashboard::PutNumber("RightPosition", deltaRightPosition);

	double gyroReadingRaw = robotData.getData<double>("GyroYaw");
	Rotation2D gyroReading(EctoMath::degreesToRadians(gyroReadingRaw));

	double distanceTraveled = (deltaLeftPosition + deltaRightPosition) / 2;
	double distanceTraveledMeter = distanceTraveled * properties.getDimensions().getWheelCircumference();

	double changeX = distanceTraveledMeter * gyroReading.getCos();
	double changeY = distanceTraveledMeter * gyroReading.getSin();

	RobotPose2D lastPos = getLastMeasurement().second;

	lastPos.setX(lastPos.getX() + changeX);
	lastPos.setY(lastPos.getY() + changeY);


	std::pair<double, RobotPose2D> newPos = std::make_pair(timer.getTime(), lastPos);
	data.addElement(newPos);

	// data.addElement(std::make_pair(timer.getTime(),
	//                                EctoKinematicsTankRebirth::integrateForwardKinematics(getLastMeasurement().second,
	//                                                                                      generateOdometryFromSensors(
	// 		                                                                                     deltaLeftPosition,
	// 		                                                                                     deltaRightPosition,
	// 		                                                                                     gyroReading))));
	previousLeftDistance = leftPosition;
	previousRightDistance = rightPosition;

	robotData.addData("TankStateLastPoint", newPos);
}

std::ostream &operator<<(std::ostream &os, const std::pair<double, RobotPose2D> &data) {
	return os << data.first << ',' << data.second;
}

std::istream &operator>>(std::istream &in, std::pair<double, RobotPose2D> data) {
	in >> data.first;
	in.ignore();
	in >> data.second;

	return in;
}