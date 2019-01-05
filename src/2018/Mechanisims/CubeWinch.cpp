//
// Created by Abiel on 2/10/18.
//

#include "CubeWinch.h"
#include "Math/MathUtil.h"

CubeWinch::CubeWinch() : EctoSubsystem("CubeWinch"), masterMotor(*motors.getMotor(masterName)),
                         slaveMotor(*motors.getMotor(slaveName)), upWinch(0.2, 0.1), downWinch(0.2, 0.1) {
	log->info("Initializing CubeWinch...");

	settings.setCurrentGroup("Winch");
	maximumUpVelocityWinch = settings.getNumber("MaximumUpVelocityWinch");
	maximumDownVelocityWinch = settings.getNumber("MaximumDownVelocityWinch");

	//Setups slave motor
	motors.setupSlave(slaveMotor, masterMotor);

	masterMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_10_MotionMagic, 20, 20);
	masterMotor.SetStatusFramePeriod(StatusFrameEnhanced::Status_2_Feedback0, 20, 20);

	masterMotor.ConfigClosedloopRamp(0.78, 30);

	//Gets how many native units per rev
	nativeUnitsPerRev = (masterMotor.getEncoderCodes());

	winchDiameter = settings.getNumber("WinchDiameter");
	robotBaseHeight = settings.getNumber("RobotBaseHeight");

	//Gets the max and min height
	maxHeight = settings.getNumber("MaxHeight") + robotBaseHeight;
	minHeight = settings.getNumber("MinHeight") + robotBaseHeight;
	maxHeightNative = EctoUtils::MSToNativeUnits(maxHeight, winchDiameter, nativeUnitsPerRev);
	minHeightNative = EctoUtils::MSToNativeUnits(minHeight, winchDiameter, nativeUnitsPerRev);

	//Gets the forward and reverse soft limits
	forwardSoftLimit = settings.getNumber("ForwardSoftLimit");
	reverseSoftLimit = settings.getNumber("ReverseSoftLimit");
	forwardSoftLimitNative = EctoUtils::MToNativeUnits(forwardSoftLimit, winchDiameter, nativeUnitsPerRev);
	//84749
	forwardSoftLimitNative = 84560;

	reverseSoftLimitNative = EctoUtils::MToNativeUnits(reverseSoftLimit, winchDiameter, nativeUnitsPerRev);

	//Checks if forward and reverse soft limits are enabled
	forwardSoftLimitEnabled = settings.getBool("ForwardSoftLimitEnabled");
	reverseSoftLimitEnabled = settings.getBool("ReverseSoftLimitEnabled");

	//Writes the soft limits to the motor
	masterMotor.ConfigForwardSoftLimitThreshold(forwardSoftLimitNative, 10);
	masterMotor.ConfigReverseSoftLimitThreshold(reverseSoftLimitNative, 10);
	masterMotor.ConfigForwardSoftLimitEnable(true, 10);
	masterMotor.ConfigReverseSoftLimitEnable(true, 10);

	//Checks if manual control is enabled
	manualControl = settings.getBool("Manual");
	manualStep = settings.getNumber("ManualStepNative");

	//Gets the maximum speed and acceleration
	maximumSpeed = settings.getNumber("MaximumSpeed");
	maximumAcceleration = settings.getNumber("MaximumAcceleration");
	maximumNativeSpeed = EctoUtils::MSToNativeUnits(maximumSpeed, winchDiameter, nativeUnitsPerRev);
	maximumNativeAcceleration = EctoUtils::MSToNativeUnits(maximumAcceleration, winchDiameter, nativeUnitsPerRev);

	//Checks if zero switch is present (currently untested)
	hasZeroSwitch = settings.getBool("HasZeroSwitch");
	string zeroSwitchPos = settings.getString("ZeroSwitchPos");

	//Gets the preset heights
	cubeHeight = settings.getNumber("CubeHeight") - robotBaseHeight;
	exchangeHeight = settings.getNumber("ExchangeHeight") - robotBaseHeight;
	switchHeight = settings.getNumber("SwitchHeight") - robotBaseHeight;
	scaleHeight = settings.getNumber("ScaleHeight") - robotBaseHeight;

	//Converts them to native units
	nativeCubeHeight = EctoUtils::MToNativeUnits(cubeHeight, winchDiameter, nativeUnitsPerRev);
	nativeExchangeHeight = EctoUtils::MToNativeUnits(exchangeHeight, winchDiameter, nativeUnitsPerRev);
	nativeSwitchHeight = EctoUtils::MToNativeUnits(switchHeight, winchDiameter, nativeUnitsPerRev);
	nativeScaleHeight = EctoUtils::MToNativeUnits(scaleHeight, winchDiameter, nativeUnitsPerRev);

	currentNativeHeight = nativeCubeHeight;

	log->info("Height: {}, {}, {}, {} Native: {}, {}, {}, {}", cubeHeight, exchangeHeight, switchHeight, scaleHeight,
	          nativeCubeHeight, nativeExchangeHeight, nativeSwitchHeight, nativeScaleHeight);

	//Configs PIDs for the master motor
	masterMotor.SelectProfileSlot(0, 0);

	EctoUtils::PidValues masterMotorPIDS = *EctoUtils::readPIDs("WinchPIDs");

	motors.configPID(masterMotor, masterMotorPIDS, 0);

	//Sets the maximum velocity and maximum acceleration
	//TODO Make this so it uses the already given values
	//masterMotor.ConfigMotionCruiseVelocity(1500 * 2.85, 0);

	//Temporary bodge
	maximumSpeed = 1500 * 6.8;
	maximumAcceleration = 1000 * 5.55;

	masterMotor.ConfigMotionCruiseVelocity(maximumSpeed, 0);
	masterMotor.ConfigMotionAcceleration(maximumAcceleration, 0);

	masterMotor.ConfigNeutralDeadband(0.05, 20);

	//Sets the control mode to the master
	masterMotor.setControlMode(ControlMode::MotionMagic);

	log->trace("Finished setting up CubeWinch");
}

void CubeWinch::initTeleop() {
	currentNativeHeight = determineNativeHeight(currentPosition);
	currentSetHeight = getNativeHeight();

	stick.addAxis(&upWinch, "rightTrig", 2);
	stick.addAxis(&downWinch, "leftTrig", 2);
}

void CubeWinch::updateTeleop() {
	//Gets the output to be set using the currentPosition
	//currentOutput = determineNativeHeight(currentPosition);

	//log->info("Setting: {}", currentOutput);

	//cout << this->getCurrentHeight() << endl;

	if (this->magicMotionEnabled) {
		motors.getMotor("CubeWinchMaster")->setControlMode(ControlMode::MotionMagic);
		motors.getMotor("CubeWinchMaster")->Set(currentNativeHeight);
	} else {
		motors.getMotor("CubeWinchMaster")->setControlMode(ControlMode::Position);
		motors.getMotor("CubeWinchMaster")->Set(currentNativeHeight);
	}

	/**
	 * Winch Control
	 */

	//cout << "MaxUp: " << maximumUpVelocityWinch << endl;
	//cout << "MaxDown: " << maximumDownVelocityWinch << endl;

	double upSpeed = upWinch.get() * maximumUpVelocityWinch;
	double downSpeed = downWinch.get() * maximumUpVelocityWinch;

	frc::SmartDashboard::PutNumber("UpNumber", upSpeed);
	frc::SmartDashboard::PutNumber("DownNumber", downSpeed);
	frc::SmartDashboard::PutNumber("Result", upSpeed - downSpeed);

	double speed = upSpeed - downSpeed;
	//double speed = 0;
	currentSetHeight += speed;

	//cout << "Speed: " << speed << " Set Height: " << currentSetHeight << endl;

	bool isRosasHacked = true;

	//If stop after released is enabled and no trigger input is received
	if (speed == 0.0) {
		double currentHeight = this->getCurrentNativeHeight();
		//cout << "Current Height: " << currentHeight << endl;

		double direction = 0;

		//Determines the direction the winch was previously going in
		if (currentSetHeight - currentHeight > 0.0)
			direction = 1.0;
		else
			direction = -1.0;

		//cout << "Direction: " << direction << endl;

		auto currentVelocity = this->getCurrentVelocity();

		//cout << "Current Velocity " << currentVelocity << endl;

		//Blame Joel if this doesn't work

		//cout << "A: " << -1.0 * std::pow(currentVelocity, 2.0) << " B: " << 2.0 * this->getMaximumAcceleration() << " C: " << -1.0 * std::pow(currentVelocity, 2.0) / (2.0 * this->getMaximumAcceleration()) << endl;

		//offsetToMoveAfterReleasedWinch = -1.0 * std::pow(currentVelocity, 2.0) / (2.0 * this->getMaximumAcceleration());
		offsetToMoveAfterReleasedWinch = 0;
		//(Reyna, 2018)

		//cout << "OffsetToMoveWinch " << offsetToMoveAfterReleasedWinch << endl;

		//cout << "Set Height: " << std::abs(currentSetHeight - currentHeight) << endl;

		//if (std::abs(currentSetHeight - currentHeight) > offsetToMoveAfterReleasedWinch)
		//	currentSetHeight = currentHeight + (offsetToMoveAfterReleasedWinch * direction);
	}

	//cout << "Without Limit: " << currentSetHeight << endl;
	currentSetHeight = MathUtil::limitNumber(currentSetHeight, this->getMinNativeHeight(), this->getMaxNativeHeight());
	masterMotor.setControlMode(MotorMode::MOTION_MAGIC);
	masterMotor.Set(currentSetHeight);
	//cout << "Actual Height: " << currentSetHeight << endl;

	//this->setNativeHeight(currentSetHeight);
	//SmartDashboard::PutNumber("NativeHeight", currentNativeHeight);
	//SmartDashboard::PutNumber("CurrentNativeHeight", masterMotor.getPosition());

	//currentPosition = this->determinePositionFromNativeHeight(currentSetHeight);
	//this->setPosition(currentPosition);

	frc::SmartDashboard::PutNumber("SetPosition", currentSetHeight);
	frc::SmartDashboard::PutNumber("CurrentPosition", masterMotor.getPosition());
}

void CubeWinch::initAuto() {
	;
}

void CubeWinch::updateAuto() {
	//Gets the output to be set using the currentPosition
	currentOutput = determineNativeHeight(currentPosition);

	motors.getMotor("CubeWinchMaster")->Set(currentOutput);
}

void CubeWinch::initDisabled() {
	;
}

void CubeWinch::updateDisabled() {
	;
}

void CubeWinch::sendInitialTelemetry() const {
	frc::SmartDashboard::PutNumber("Winch1Current", motors.getMotor("CubeWinchMaster")->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("Winch2Current", motors.getMotor("CubeWinchSlave")->GetOutputCurrent());
}

void CubeWinch::sendTelemetry() const {
	frc::SmartDashboard::PutNumber("Winch1Current", motors.getMotor("CubeWinchMaster")->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("Winch Pos", motors.getMotor("CubeWinchMaster")->getPosition());
	frc::SmartDashboard::PutNumber("Winch2Current", motors.getMotor("CubeWinchSlave")->GetOutputCurrent());
	frc::SmartDashboard::PutNumber("WinchHeight", getCurrentHeight());
	frc::SmartDashboard::PutNumber("WinchRawPosition", masterMotor.getPosition());
}

/**
 * Gets the current height of the winch in meters, (measured from the floor to the claw)
 * @return
 */
double CubeWinch::getCurrentHeight() const {
	double pos = masterMotor.getPosition();
	return (pos * winchDiameter * M_PI) + robotBaseHeight;
}

/**
 * Sets the current position of the winch
 * @param pos
 */
void CubeWinch::setPosition(CurrentPosition pos) {
	currentPosition = pos;
}

/**
 * Returns the current position of the winch
 * @return
 */
CurrentPosition CubeWinch::getCurrentPosition() const {
	return currentPosition;
}

/**
 * Gets max height of the winch
 * @return
 */
double CubeWinch::getMaxHeight() const {
	return maxHeight;
}

/**
 * Returns the native height given a current position
 * @param pos
 * @return
 */
double CubeWinch::determineNativeHeight(const CurrentPosition pos) const {
	switch (pos) {
		case (CubeHeight):
			return nativeCubeHeight;

		case (ExchangeHeight):
			return nativeExchangeHeight;

		case (SwitchHeight):
			return nativeSwitchHeight;

		case (ScaleHeight):
			return nativeScaleHeight;

		default:
			log->error("Tried to get a native height of undefined position: {}, returning CubeHeight", (int) pos);
			return CubeHeight;
	}
}

void CubeWinch::resetZeroEncoderPos() {
	motors.getMotor("CubeWinchMaster")->SetSelectedSensorPosition(4000, 0, 0);
}

void CubeWinch::setNativeHeight(double nativeHeight) {
	this->currentNativeHeight = nativeHeight;
}

double CubeWinch::getNativeHeight() const {
	return this->currentNativeHeight;
}

/**
 * Gets the current native height
 * @return
 */
double CubeWinch::getCurrentNativeHeight() {
	return motors.getMotor("CubeWinchMaster")->GetSelectedSensorPosition(0);
}

int CubeWinch::getCurrentVelocity() {
	return motors.getMotor("CubeWinchMaster")->GetSelectedSensorVelocity(0);
}

double CubeWinch::getMaxNativeHeight() const {
	return forwardSoftLimitNative;
}

double CubeWinch::getMinHeight() const {
	return minHeight;
}

double CubeWinch::getMinNativeHeight() const {
	return reverseSoftLimitNative;
}

CurrentPosition CubeWinch::determinePositionFromNativeHeight(double nativeHeight) const {
	if (nativeHeight <= nativeCubeHeight + 4000)
		return CurrentPosition::CubeHeight;
	else if (nativeHeight <= nativeSwitchHeight)
		return CurrentPosition::SwitchHeight;
	else if (nativeHeight <= nativeScaleHeight)
		return CurrentPosition::ScaleHeight;
}

void CubeWinch::setMagicMotionStatus(bool status) {
	this->magicMotionEnabled = status;
}

bool CubeWinch::getMagicMotionStatus() const {
	return this->magicMotionEnabled;
}

double CubeWinch::getMaximumAcceleration() const {
	return 1000 * 5.55;
}