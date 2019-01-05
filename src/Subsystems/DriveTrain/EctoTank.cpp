//
// Created by Abiel on 10/16/17.
//

#include "EctoTank.h"
#include "Managers/InputUtils.h"
#include "Math/MathUtil.h"

/**
 * Constructor, initializer list requires rightMaster and leftMaster motor
 * reference, also the name of the EctoSubsystem
 */
EctoTank::EctoTank(const EctoTankConfig &config,
                   const std::shared_ptr<InputHandler> &initialInputHandler)
		: EctoSubsystem("EctoTank"),
		  tankConfig(config),
		  driveGearbox(config.gearboxConfig),
		  rightMaster(*motors.getMotor(tankConfig.rightMasterName)),
		  leftMaster(*motors.getMotor(tankConfig.leftMasterName)),
		  currentTeleopMode(config.defaultTeleopControlMode),
		  leftAxis(config.joystixExpo, config.joystickDeadzone),
		  rightAxis(config.joystixExpo, config.joystickDeadzone) {
	log->info("Initializing EctoTank...");

	currentInputHandler = initialInputHandler;

	setupPids();
	setupSlaves();

	setBrakes(tankConfig.defaultBrakeState);

	this->wheelCircumference = robotProperties.getDimensions().getWheelCircumference();
	this->wheelBase = robotProperties.getDimensions().getRobotWheelBase();

	if (tankConfig.voltageCompensation) {
		leftMaster.ConfigVoltageCompSaturation(tankConfig.voltageCompensation, 10);
		leftMaster.EnableVoltageCompensation(true);

		rightMaster.ConfigVoltageCompSaturation(tankConfig.voltageCompensation, 10);
		rightMaster.EnableVoltageCompensation(true);
	} else {
		leftMaster.EnableVoltageCompensation(false);
		rightMaster.EnableVoltageCompensation(false);
	}

	log->info("Finished Initializing");
}

/**
 * Writes PIDs to motors
 */
void EctoTank::setupPids() {
	// motors.configPID(leftMaster, tankConfig.highGearPID, highGearProfileSlot);
	motors.configPID(leftMaster, tankConfig.lowGearPID, lowGearProfileSlot);

	// motors.configPID(rightMaster, tankConfig.highGearPID, highGearProfileSlot);
	motors.configPID(rightMaster, tankConfig.lowGearPID, lowGearProfileSlot);

	leftMaster.setFeedbackDevice(FeedbackDevice::QuadEncoder);
	rightMaster.setFeedbackDevice(FeedbackDevice::QuadEncoder);

	leftMaster.setControlMode(ControlMode::Velocity);
	rightMaster.setControlMode(ControlMode::Velocity);

	leftMaster.SelectProfileSlot(lowGearProfileSlot, 0);
	rightMaster.SelectProfileSlot(lowGearProfileSlot, 0);

	leftMaster.Set(0);
	rightMaster.Set(0);
}

void EctoTank::setupSlaves() {
	if (!tankConfig.hasSlaves) return;

	for (const std::string &leftSlaveName : tankConfig.leftSlaves) {
		motors.setupSlave(*motors.getMotor(leftSlaveName), leftMaster);
	}

	for (const std::string &rightSlaveName : tankConfig.rightSlaves) {
		motors.setupSlave(*motors.getMotor(rightSlaveName), rightMaster);
	}
}

// leftMaster.SelectProfileSlot(profileSlot, 0);

/**
 * Enables the intent to set the piston shift on speed, will only do so if speed
 * is above certain threshold
 */
/*
void EctoTank::enableShift() {
 speedShift = true;

 if (isInSpeedTimeout) {
         double timeSinceTimeout = timer.getTime() - lastSpeedChange;
         if (timeSinceTimeout > speedChangeTimeoutSec) {
                 isInSpeedTimeout = false;
         }
 } else {
         if ((abs(getRightMotorsSpeed()) + abs(getLeftMotorsSpeed())) / 2.0 >
minSpeedShiftEnabled) { auto previousMode = rightMaster.getCurrentControlMode();

                 rightMaster.setControlMode(ControlMode::PercentOutput);
                 leftMaster.setControlMode(ControlMode::PercentOutput);

                 setBrakes(false);
                 setRightMotorsSpeed(0);
                 setLeftMotorsSpeed(0);
                 isInSpeedTimeout = true;
                 lastSpeedChange = timer.getTime();

                 pcm.setPiston("Shift", speedShift);
                 setSpeedMode();
                 speedPistonEnabled = true;
                 setBrakes(true);

                 rightMaster.setControlMode(previousMode);
                 leftMaster.setControlMode(previousMode);

         } else {
                 pcm.setPiston("Shift", false);
                 speedPistonEnabled = false;

                 setTorqueMode();
         }
 }

}
*/

/**
 * Writes to motors.
 * @param in
 */
void EctoTank::writeMotors(const MotorValues &in) {
	leftMaster.setControlMode(in.getValueType());
	rightMaster.setControlMode(in.getValueType());

	leftMaster.Set(in.left);
	rightMaster.Set(in.right);
}

/**
 * Sets the brakes state
 * @param state
 */
void EctoTank::setBrakes(const bool state) {
	leftMaster.setBrake(state);
	rightMaster.setBrake(state);
}

/**
 * Returns the max speed the robot can be expected to reach on the current speed
 * mode
 * @return
 */
double EctoTank::getCurrentMaxSpeed() const {
	driveGearbox.getCurrentMaximumSpeed();
}

MotorValues EctoTank::getCurrentSpeeds() {
	return {leftMaster.getSpeedMSCircumference(wheelCircumference),
	        rightMaster.getSpeedMSCircumference(wheelCircumference),
	        MotorMode::VELOCITY};
}

MotorValues EctoTank::getCurrentDistances() {
	return {leftMaster.getPosition() * wheelCircumference,
	        rightMaster.getPosition() * wheelCircumference, MotorMode::POSITION};
}

MotorValues EctoTank::getCurrentVoltages() {
	return {leftMaster.getSetValue() * 12.0, rightMaster.getSetValue() * 12.0,
	        MotorMode::VOLTAGE};
}

/**
 * Returns the min speed the robot can be expected to move at on the current
 * speed mode
 * @return
 */
double EctoTank::getCurrentMinSpeed() const {
	// TODO Implement something less arbitrary
	return getCurrentMaxSpeed() * .15;
}

/**
 * Returns the yaw of the robot
 * @return
 */
double EctoTank::getYaw() { return navx.GetYaw(); }

/**
 * Initializes disabled
 */
void EctoTank::initDisabled() { ; }

/**
 * Update disabled
 */
void EctoTank::updateDisabled() {
	// TODO Reset and ZeroYaw navx every once in a while (while the robot is
	// disabled)
}

/**
 * Initializes teleop
 */
void EctoTank::initTeleop() {
	stick.addAxis(&leftAxis, "leftY");
	stick.addAxis(&rightAxis, "rightY");

	stick.addButton(&toggleCurrentMode, "A", 1);

	stick.addButton(&highGearButton, "rightPush", 1);
	stick.addButton(&lowGearButton, "leftPush", 1);

	currentInputHandler->init();
}

/**
 * Update tank teleop function.
 */
void EctoTank::updateTeleop() {
	InputValue value;

	value.setLeftVel(-leftAxis.get());
	value.setRightVel(-rightAxis.get());

	// value.setLeftVel(MathUtil::limitNumber(value.getLeftVel(), -0.6, 0.6));
	// value.setRightVel(MathUtil::limitNumber(value.getRightVel(), -0.6, 0.6));

	InputValue outputValue = currentInputHandler->calculateInputs(value);

	if (highGearButton.get())
		pcm.setPiston("Shift", true);
	else if (lowGearButton.get())
		pcm.setPiston("Shift", false);

	// TODO Implement speed shift
	switch (currentTeleopMode) {
		case MotorMode::VOLTAGE: {
			writeMotors(InputUtils::convertInputValueToMotorValue(
					outputValue, this->currentTeleopMode));
			break;
		}

		case MotorMode::VELOCITY: {
			double metersPerSec = 1.4;  //
			// meters per 100 ms .14
			// Rot per 100 ms .29
			double actualVelocity = EctoUtils::RPMToNativeUnits(
					EctoUtils::MSToRPM(1.4, robotProperties.getDimensions().getWheelDiameter()),
					(double) leftMaster.getEncoderCodes());

			outputValue *= actualVelocity;

			writeMotors(InputUtils::convertInputValueToMotorValue(
					outputValue, this->currentTeleopMode));
			break;
		}

		case MotorMode::CURRENT: {
			InputValue limitedOutputValue;

			limitedOutputValue.setLeftVel(EctoUtils::map(
					std::abs(outputValue.getLeftVel()), 0.4, 1.0, previousLeftCurrent,
					tankConfig.maximumCurrentPerMotor));

			if (outputValue.getLeftVel() < 0)
				limitedOutputValue.setLeftVel(limitedOutputValue.getLeftVel() * -1.0);

			limitedOutputValue.setRightVel(EctoUtils::map(
					std::abs(outputValue.getRightVel()), 0.4, 1.0, previousRightCurrent,
					tankConfig.maximumCurrentPerMotor));

			if (outputValue.getRightVel() < 0)
				limitedOutputValue.setRightVel(limitedOutputValue.getRightVel() * -1.0);

			outputValue *= tankConfig.maximumCurrentPerMotor;

			SmartDashboard::PutNumber("RightCurrentTarget",
			                          outputValue.getRightVel());
			SmartDashboard::PutNumber("LeftCurrentTarget", outputValue.getLeftVel());

			double leftCurrent = leftMaster.GetOutputCurrent();
			double rightCurrent = rightMaster.GetOutputCurrent();

			SmartDashboard::PutNumber(
					"RightCurrentDelta",
					std::abs(std::abs(outputValue.getRightVel()) - rightCurrent));
			SmartDashboard::PutNumber(
					"LeftCurrentDelta",
					std::abs(std::abs(outputValue.getLeftVel()) - leftCurrent));

			writeMotors(InputUtils::convertInputValueToMotorValue(
					outputValue, this->currentTeleopMode));
			break;
		}

		default:
			throw std::runtime_error("Tried to use an invalid teleop control mode!");
	}
}

void EctoTank::updateRobot() {
	double leftSpeed = leftMaster.getSpeedMSCircumference(wheelCircumference);
	double rightSpeed = rightMaster.getSpeedMSCircumference(wheelCircumference);

	double linearSpeed = (leftSpeed + rightSpeed) / 2.0;

	double angularSpeed = (rightSpeed - leftSpeed) / (2.0 * wheelBase);

	robotData.addData("CurrentLeftWheelSpeed", leftSpeed);
	robotData.addData("CurrentRightWheelSpeed", rightSpeed);
	robotData.addData("CurrentLinearSpeed", linearSpeed);
	robotData.addData("CurrentAngularSpeed", angularSpeed);
//	double timeToDeaccelerateLeft = leftSpeed / tankConfig.maximumDeacceleration;
//	double timeToDeaccelerateRight = rightSpeed / tankConfig.maximumDeacceleration;
//
//	robotData.addData("LeftWheelTimeToDeaccelerate", timeToDeaccelerateLeft);
//	robotData.addData("RightWheelTimeToDeaccelerate", timeToDeaccelerateRight);
//
//	double leftDeaccelerationDistance = leftSpeed * timeToDeaccelerateLeft + .5 * -tankConfig.maximumDeacceleration * std::pow(timeToDeaccelerateLeft, 2.0);
//	double rightDeaccelerationDistance = rightSpeed * timeToDeaccelerateRight + .5 * -tankConfig.maximumDeacceleration * std::pow(timeToDeaccelerateRight, 2.0);
//
//	robotData.addData("LeftDistanceToDeaccelerate", leftDeaccelerationDistance);
//	robotData.addData("RightDistanceToDeaccelerate", rightDeaccelerationDistance);
}

/**
 * Initializes autonomous
 */
void EctoTank::initAuto() { ; }

/**
 * Updates autonomous function
 */
void EctoTank::updateAuto() { ; }

/**
 * Sets up initial telemetry for Tank
 */
void EctoTank::sendInitialTelemetry() const {
	// TODO Implement initial telemetry
	;
}

/**
 * Updates telemetry for Tank
 */
void EctoTank::sendTelemetry() const {
	SmartDashboard::PutNumber("EctoTank/LeftCurrent", leftMaster.GetOutputCurrent());
	SmartDashboard::PutNumber("EctoTank/RightCurrent", rightMaster.GetOutputCurrent());

	SmartDashboard::PutString("EctoTank/CurrentTeleopMode", MotorValues::getMotorModeName(this->currentTeleopMode));
}

GearboxConfig EctoTank::getGearboxConfig() const { return gearboxConfig; }

void EctoTank::setGearboxConfig(const GearboxConfig &gearboxConfig) {
	this->gearboxConfig = gearboxConfig;
}

void EctoTank::setControlMode(MotorMode motorMode) {
	this->currentTeleopMode = motorMode;
}

void EctoTank::stop() { writeMotors(MotorValues(0, 0, MotorMode::VOLTAGE)); }

void EctoTank::driveFollowingAngle(double speed, double angle) {
	double angleError = EctoUtils::shortestAngleTurn(getYaw(), angle);
	double outputMultiplier =
			EctoUtils::map(std::abs(angleError), 0.0, 20.0, 0.0, 1.0);

	outputMultiplier = MathUtil::limitNumber(outputMultiplier, 0.0, 1.0);

	if (std::abs(angleError) > 0.1) {
		if (angleError > 0.0) {
			writeMotors(MotorValues(speed, speed - (speed * outputMultiplier),
			                        MotorMode::VOLTAGE));
		} else if (angleError < 0.0) {
			writeMotors(MotorValues(speed - (speed * outputMultiplier), speed,
			                        MotorMode::VOLTAGE));
		}
	} else {
		writeMotors(MotorValues(speed, speed, MotorMode::VOLTAGE));
	}
}

/**
 * Turn to angle, used for testing, uses angles, needs to be run continuously 
 * @param angle
 */
void EctoTank::turnToAngle(double angle) {
	double angleError = EctoUtils::shortestAngleTurn(getYaw(), angle);

	if (std::abs(angleError) > 1.0) {
		angleError = EctoUtils::shortestAngleTurn(getYaw(), angle);

		//0.011111 so that when the angle error is 90, the output has a delta of 1
		double output = MathUtil::limitNumber(angleError * 0.011111, 0.2, 1.0);

		writeMotors(MotorValues(output, -output, MotorMode::VOLTAGE));
	}

	writeMotors(MotorValues(0.0, 0.0, MotorMode::VOLTAGE));
}
