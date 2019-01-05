#include "EctoMotors.h"

EctoMotors::EctoMotors() {
	log->info("Initializing EctoMotors...");
	settings.setCurrentGroup("Motors");
	motorQuantity = settings.getNumber("MotorQuantity");

	initializeMotors();
	writeInversions();
	// writeClosedLoopInversions();
	writeEncoderCodes();
	writeEncoderInversions();
}

/**
 * Initializes all motors using config file
 */
void EctoMotors::initializeMotors() {
	log->info("Initializing Motors...");

	for (unsigned int i = 1; i <= motorQuantity; ++i) {
		// Gets motor name from motorId.#
		std::string name = settings.getString(baseStringName + to_string(i));

		// Gets inverted state from motorInvert.#
		bool invertedTemp = settings.getBool(baseStringInvert + to_string(i));

		// Gets encoder codes from motorEncoderCodes.#
		uint16_t encoderTurns = static_cast<uint16_t>(
				settings.getNumber(baseStringEncoderCodes + to_string(i)));

		// Gets encoder inversions from motorEncoderInvert.#
		bool encoderInversion =
				settings.getBool(baseStringEncoderInvert + to_string(i));

		bool closedLoopInversion =
				settings.getBool(baseStringClosedLoopInvert + to_string(i));
		// Gets motor id
		unsigned int id = static_cast<unsigned int>(
				settings.getNumber(baseStringId + to_string(i)));
		ids.emplace_back(id);

		// Stores motor IDs using names as keys

		idName.emplace(name, id);

		// Stores motor names using IDs as keys
		names.emplace(id, name);

		// Creates empty motor objects using id
		motors.emplace(id, motor());

		// Stores motor inversions
		inverted.emplace(id, invertedTemp);

		// Stores closed loop inversions
		closedLoopInversions.emplace(id, closedLoopInversion);

		// Stores encoder codes per rotation
		encoderCodes.emplace(id, encoderTurns);

		// Stores encoder inverted status
		encoderInversions.emplace(id, encoderInversion);
	}

	for (unsigned int id : ids) {
		motors[id].id = id;
		motors[id].motorName = names[id];
		motors[id].motor.reset(new EctoTalon(id));
	}

	motorsMapSize = motors.size();
}

void EctoMotors::setupSlave(EctoTalon &slaveMotor, EctoTalon &masterMotor) {
	slaveMotor.setControlMode(ControlMode::Follower);
	slaveMotor.Set(masterMotor.GetDeviceID());
}

/**
 * Sets inversions for motors as defined by the config file
 */
void EctoMotors::writeInversions() {
	log->info("Writing motor inversions...");
	for (unsigned int id : ids) {
		motors[id].motor->SetInverted(inverted[id]);
	}
}

/**
 * Sets inversions for closed loop for motor as defined by the config file
 */
void EctoMotors::writeClosedLoopInversions() {
	// Maybe not needed anymore

	log->info("Writing motor closed loop inversions...");
	for (unsigned int id : ids) {
		motors[id].motor->SetInverted(closedLoopInversions[id]);
	}
}

/**
 * Sets the encoder codes as defined by the config file
 */
void EctoMotors::writeEncoderCodes() {
	log->info("Writing encoder codes per rev...");
	for (unsigned int id : ids) {
		motors[id].motor->setFeedbackDevice(FeedbackDevice::QuadEncoder);
		motors[id].motor->SetSelectedSensorPosition(0, 0, 10);
		motors[id].motor->setEncoderCodes(encoderCodes[id]);
	}
}

/**
 * Sets inversions for encoders as defined by the config file
 */
void EctoMotors::writeEncoderInversions() {
	log->info("Writing encoder inversions...");
	for (unsigned int id : ids) {
		motors[id].motor->SetSensorPhase(encoderInversions[id]);
	}
}

/**
 * Checks if given ID is smaller or equal to the amount of motors.
 * @param id
 * @return
 */
inline bool EctoMotors::checkIfValidId(const unsigned int id) {
	return id <= motorsMapSize;
}

/**
 * Check if name exists.
 * @param name
 * @return
 */
inline bool EctoMotors::checkIfValidId(const string &name) {
	return idName.count(name) > 0;
}

/**
 * Checks if given value is valid, if it is not, throws an exception.
 * @param id
 */
inline void EctoMotors::idCheck(const unsigned int id) {
	if (!checkIfValidId(id)) {
		std::string error = "Attempted to use invalid motor id: " + std::to_string(id);
		throw runtime_error(error);
	}
}

/**
 * Check if given name exists, if it is not, throws exception.
 * @param name
 */
inline void EctoMotors::idCheck(const string &name) {
	if (!checkIfValidId(name)) {
		std::string error = "Attempted to use invalid name: " + name;
		log->error(error);
		throw runtime_error(error);
	}
}

/**
 * Returns the CANTalon object
 * @param id
 * @return
 */
shared_ptr<EctoTalon> EctoMotors::getMotor(unsigned int id) {
	idCheck(id);
	return motors[id].motor;
}

/**
 * Returns the CANTalon object
 * @param name
 * @return
 */
shared_ptr<EctoTalon> EctoMotors::getMotor(const string &name) {
	// log->trace("Retrieving motor: {}", name);
	return getMotor(getId(name));
}

/**
 * Gets the motor ID with the given name.
 * @param  input Motor Name
 * @return
 */
unsigned int EctoMotors::getId(const string &input) {
	idCheck(input);
	return idName[input];
}

/**
 * Writes PID Values to motor.
 * @param id
 * @param pid
 * @param profileSlot
 */
void EctoMotors::configPID(EctoTalon &motor, const EctoUtils::PidValues &pid,
                           const unsigned int profileSlot) {
	configPID(motor, pid.p, pid.i, pid.d, pid.f, profileSlot);
}

/**
 * Writes PID Values to motor.
 * @param id          Motor ID
 * @param p           P Value
 * @param i           I Value
 * @param d           D Value
 * @param f           F Value
 * @param profileSlot Profile Slot
 */
void EctoMotors::configPID(EctoTalon &motor, const double p, const double i,
                           const double d, const double f,
                           const unsigned int profileSlot) {
	log->info("Writing PIDs to: {} with slot: {}, {},{},{},{}", motor.getId(),
	          profileSlot, p, i, d, f);

	motor.Config_kP(profileSlot, p, 10);
	motor.Config_kI(profileSlot, i, 10);
	motor.Config_kD(profileSlot, d, 10);
	motor.Config_kF(profileSlot, f, 10);

	motor.ConfigNominalOutputForward(0, 0);
	motor.ConfigNominalOutputReverse(0, 0);
}

EctoUtils::PidValues EctoMotors::readPID(EctoTalon &motor,
                                         unsigned int profileSlot) {
	EctoUtils::PidValues temp;

	// motor.SelectProfileSlot(profileSlot, 0);

	// TODO find out what the second parameter is
	temp.p =
			motor.ConfigGetParameter(ParamEnum::eProfileParamSlot_P, profileSlot, 0);
	temp.i =
			motor.ConfigGetParameter(ParamEnum::eProfileParamSlot_I, profileSlot, 0);
	temp.d =
			motor.ConfigGetParameter(ParamEnum::eProfileParamSlot_D, profileSlot, 0);
	temp.f =
			motor.ConfigGetParameter(ParamEnum::eProfileParamSlot_F, profileSlot, 0);

	return temp;
}
