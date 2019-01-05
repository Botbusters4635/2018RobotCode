//
// Created by alberto on 13/12/17.
//
#include "EctoTalon.h"
#include "spdlog/sinks/stdout_color_sinks.h"

const shared_ptr<spd::logger> EctoTalon::log = spd::stdout_color_mt("EctoTalon");

EctoTalon::EctoTalon(int id) : TalonSRX(id), BaseMotorController(id | 0x02040000),
                               currentMode(ControlMode::PercentOutput) {
	this->motorId = id;
}

/**
 * Sets talon to requested value using previously set mode.
 * @param requestedValue
 */
void EctoTalon::Set(double requestedValue) {
	TalonSRX::Set(EctoTalon::currentMode, requestedValue);

	currentSetValueEcto = requestedValue;
}

/**
 * Returns the motor id.
 * @return
 */
int EctoTalon::getMotorId() {
	return this->motorId;
}

/**
 * Returns the currently set value.
 * @return
 */
double EctoTalon::getSetValue() const {
	return currentSetValueEcto;
}

/**
 * Sets the feedback device for the talon.
 * @param device
 * @param deviceId
 * @param timeout
 */
void EctoTalon::setFeedbackDevice(FeedbackDevice device, int deviceId, int timeout) {
	ConfigSelectedFeedbackSensor(device, deviceId, timeout);
}

void EctoTalon::setControlMode(MotorMode mode) {
	setControlMode(convertMotorValueTypeToCTR(mode));
}

/**
 * Changes the talon control mode. Note, this is not sent to the talon until
 * a set command is ran.
 * @param mode
 */
void EctoTalon::setControlMode(ctre::phoenix::motorcontrol::ControlMode mode) {
	log->trace("Setting control mode to: {}", (int) mode);
	currentMode = mode;
}

/**
 * Returns the currently set control mode. Note, if a set command
 * has not been sent to the Talon when the control mode was changed,
 * this may not the control mode that the talon is currently set to.
 * @return
 */
ControlMode EctoTalon::getCurrentControlMode() const {
	return currentMode;
}

/**
 * Gets the sensor velocity and converts the native units to RPM using the currently set encoder codes.
 * @return
 */
double EctoTalon::getSpeedRPM() {
	//TODO Find out why sensor 0 has to be selected
	int nativeUnits = GetSelectedSensorVelocity(0);

	if (encoderCodes == 0) {
		log->error("Tried to get RPM of encoder without encoder codes: {} on Talon: {}", encoderCodes, GetDeviceID());
		throw logic_error("Tried to get RPM of encoder without encoder codes being defined " + this->motorId);
	} else {
		return EctoUtils::nativeUnitsToRPM((double) nativeUnits, (double) encoderCodes);
	}
}

/**
 * Calculates speed in M/S given wheelDiameter (in meters)
 * @param wheelDiameter
 * @return
 */
double EctoTalon::getSpeedMS(double wheelDiameter) {
	return EctoUtils::RPMToMS(getSpeedRPM(), wheelDiameter);
}

double EctoTalon::getSpeedMSCircumference(double wheelCircumference) {
	return (getSpeedRPM() * wheelCircumference) / 60.0;
}

double EctoTalon::getPosition() {
	//TODO Find out why sensor 0 has to be selected
	//TODO Test this
	int nativeUnits = GetSelectedSensorPosition(0);

	if (encoderCodes == 0) {
		throw logic_error("Tried to get rotations without encoder codes being defined");
	} else {
		return nativeUnits / (double) encoderCodes;
	}
}

/**
 * Sets encoder codes to be used in native unit conversions
 * @param codes
 */
void EctoTalon::setEncoderCodes(uint16_t codes) {
	log->trace("Set encoder codes to: {}", codes);
	encoderCodes = codes;
}

/**
 * Returns currently set encoder codes
 * @return
 */
uint16_t EctoTalon::getEncoderCodes() const {
	return encoderCodes;
}

/**
 * If brake state is true, brakes are enabled, if false coast mode is enabled
 * @param brakeState
 */
void EctoTalon::setBrake(bool brakeState) {
	if (brakeState)
		this->SetNeutralMode(NeutralMode::Brake);
	else
		this->SetNeutralMode(NeutralMode::Coast);
}

int EctoTalon::getId() {
	return GetDeviceID();
}

ControlMode EctoTalon::convertMotorValueTypeToCTR(MotorMode typeIn) {
	switch (typeIn) {
		case (MotorMode::VOLTAGE):
			return ControlMode::PercentOutput;

		case (MotorMode::POSITION):
			return ControlMode::Position;

		case (MotorMode::VELOCITY):
			return ControlMode::Velocity;

		case (MotorMode::CURRENT):
			return ControlMode::Current;

		case (MotorMode::FOLLOWER):
			return ControlMode::Follower;

		case (MotorMode::MOTION_PROFILE):
			return ControlMode::MotionProfile;

		case (MotorMode::MOTION_MAGIC):
			return ControlMode::MotionMagic;

		case (MotorMode::MOTION_PROFILE_ARC):
			return ControlMode::MotionProfileArc;

		case (MotorMode::DEFAULT):
			return ControlMode::Disabled;
	}
}