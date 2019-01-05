//
// Created by abiel on 11/29/17.
//

#include "FeedForwardCalculator.h"

FeedForwardCalculator::FeedForwardCalculator() {
	log->info("Initializing FeedForwardCalculator...");
	timer.reset(new frc::Timer());
}

void FeedForwardCalculator::setMotor(const std::string &motorName, uint16_t codesPerRev) {
	log->info("Setting motor: {} with codesPerVev: {}...", motorName, codesPerRev);
	this->motorName = motorName;
	this->motor = motors.getMotor(motorName);

	log->trace("Using Motor ID: {}", this->motor->getMotorId());

	this->codesPerRev = codesPerRev;

	motor->setEncoderCodes(codesPerRev);

	log->trace("Setting encoder codes to {}", codesPerRev);

	//Resets values
	maxSpeed = 0.0;
	fGain = 0.0;

	finished = false;
	running = false;

	log->trace("Getting previous Talon Control Mode...");
	previousControlMode = motor->getCurrentControlMode();

	log->trace("Resetting timer...");
	timer->Reset();
	log->trace("Finished resetting timer");
}

/*
 * Sets the talon to run at max speed
 */
void FeedForwardCalculator::setMaxSpeed() {
	log->trace("Setting Talon to max speed: {}...", maxTalonSpeed);
	motor->setControlMode(ControlMode::PercentOutput);

	motor->EctoTalon::Set(maxTalonSpeed);

	log->trace("Set max speed: {}", motor->getSetValue());
}

/*
 * Gets the speed at which the talon is running
 */
double FeedForwardCalculator::getMaxSpeed() {
	double rpm = motor->getSpeedRPM();
	log->trace("Getting max speed rpm from Talon, got: {}", rpm);
	return rpm;
}

/*
 * Calculates the F-gain based on the talon's max speed
 */
double FeedForwardCalculator::calculateFgain() {
	double fGain = (4092.0) / getMaxNativeSpeed();
	log->trace("Calculating F-gain, got: {}", fGain);
	return fGain;
}

/*
 * Resets talon to previous state
 */
void FeedForwardCalculator::resetTalon() {
	log->trace("Resetting Talon to previous mode...");
	motor->Set(0);
	motor->setControlMode(previousControlMode);
}

/*
 * Needs to be called periodically in order to calculate f-gain
 */
bool FeedForwardCalculator::periodic() {
	//log->trace("Started: {}, Finished: {}, Running {}", started, finished, running);
	if (!started) {
		log->info("Started calculating F-Gain");
		started = true;
	}
	if (!finished) {

		if (!running) {
			timer->Start();
			setMaxSpeed();
			running = true;
		} else if (timer->HasPeriodPassed(timeToWaitToAccelerate)) {
			log->info("Finished FeedForwardCalculator");
			maxSpeed = getMaxSpeed();
			resetTalon();

			fGain = calculateFgain();

			finished = true;

			printValues();
		}

	}

	return finished;
}

double FeedForwardCalculator::getMaxSpeedRPM() {
	log->trace("Getting max rpm from Talon, got: {}", maxSpeed);
	return maxSpeed;
	//return EctoUtils::nativeUnitsToRPM(maxSpeed, (double) codesPerRev);
}

double FeedForwardCalculator::getMaxNativeSpeed() {
	double maxNativeSpeed = EctoUtils::RPMToNativeUnits(maxSpeed, (double) codesPerRev);
	log->trace("Getting max native speed from Talon, got: {}", maxNativeSpeed);

	return maxNativeSpeed;
}

double FeedForwardCalculator::getFgain() {
	return fGain;
}

void FeedForwardCalculator::printValues() {
	log->trace("Getting Max RPM");
	log->info("Max RPM: {}", getMaxSpeedRPM());
	log->info("Max Native Speed: {}", getMaxNativeSpeed());
	log->info("Fgain: {}", getFgain());
}