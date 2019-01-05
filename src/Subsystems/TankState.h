//
// Created by Abiel on 9/14/18.
//

#ifndef BOTBUSTERSREBIRTH_TANKSTATEREBIRTH_H
#define BOTBUSTERSREBIRTH_TANKSTATEREBIRTH_H

#include "EctoSubsystem.h"
#include "Managers/EctoMotors.h"
#include "Utilities/CircularBuffer.h"
#include "Math/DataTypes/RobotPose2D.h"
#include "Math/DataTypes/Twist2D.h"
#include "Math/DataTypes/Rotation2D.h"

#include "Utilities/EctoTimer.h"
#include "Sensors/NavX.h"

#include "Robot/RobotProperties.h"
#include "RobotData.h"
#include "Utilities/EctoWatchdog.h"

class TankState : public EctoSubsystem {
public:
	TankState();

	void initRobot() override;

	void updateRobot() override;

	std::pair<double, RobotPose2D> getLastMeasurement() const;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

private:
	EctoWatchdog *watchdog;


	static constexpr auto leftMotorName = "LeftMaster";
	static constexpr auto rightMotorName = "RightMaster";
	static const std::size_t bufferSize = 200;

	RobotDimensions dimensions;

	mutable double distanceDriven = 0.0;

	EctoMotors &motors = EctoMotors::getInstance();
	EctoTimer &timer = EctoTimer::getInstance();
	RobotProperties &properties = RobotProperties::getInstance();
	RobotData &robotData = RobotData::getInstance();

	NavX &navX = NavX::getInstance();

	EctoTalon &leftMotor, &rightMotor;

	Twist2D
	generateOdometryFromSensors(double leftDeltaDistance, double rightDeltaDistance, const Rotation2D &gyroAngle) const;

	void updateState();

	CircularBuffer<std::pair<double, RobotPose2D>> data;

	double previousLeftDistance = 0.0;
	double previousRightDistance = 0.0;
};

#endif //BOTBUSTERSREBIRTH_TANKSTATEREBIRTH_H
