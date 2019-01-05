#include "CubeIntake.h"
#include "Math/MathUtil.h"

CubeIntake::CubeIntake()
		: EctoSubsystem("Cube Intake"),
		  wristControl(0.1, 0.15),
		  intakeControl(0.1, 0.15) {
	log->info("Initializing CubeIntake...");

	leftIntakeMotor = motors.getMotor("LeftIntakeMotor");
	rightIntakeMotor = motors.getMotor("RightIntakeMotor");

	wristMotor = motors.getMotor("WristMotor");

	// Sets brakes to motors
	leftIntakeMotor->setBrake(brakeState);
	rightIntakeMotor->setBrake(brakeState);

	leftIntakeMotor->setControlMode(ControlMode::PercentOutput);
	rightIntakeMotor->setControlMode(ControlMode::PercentOutput);

	leftIntakeMotor->ConfigContinuousCurrentLimit(20, 10);
	rightIntakeMotor->ConfigContinuousCurrentLimit(20, 10);

	leftIntakeMotor->EnableCurrentLimit(true);
	rightIntakeMotor->EnableCurrentLimit(true);

	leftIntakeMotor->ConfigVoltageCompSaturation(12.0, 10);
	leftIntakeMotor->ConfigVoltageCompSaturation(12.0, 10);

	leftIntakeMotor->EnableVoltageCompensation(true);
	rightIntakeMotor->EnableVoltageCompensation(true);

	wristMotor->ConfigClosedloopRamp(1.3, 30);

	settings.setCurrentGroup("Intake");

	intakeSpeedTeleop = settings.getNumber("SpeedTeleopIntake");
	ejectSpeedTeleop = settings.getNumber("SpeedTeleopEject");

	bool currentLimitEnabled = settings.getBool("CurrentLimitEnabled");

	if (currentLimitEnabled) {
		double peakCurrentLimit = settings.getNumber("WristPeakCurrentLimit");
		double peakCurrentDuration = settings.getNumber("WristPeakCurrentDuration");

		double continuousCurrentLimit =
				settings.getNumber("WristContinuousCurrentLimit");

		wristMotor->ConfigPeakCurrentLimit(peakCurrentLimit, 10);
		wristMotor->ConfigPeakCurrentDuration(peakCurrentDuration, 10);
		wristMotor->ConfigContinuousCurrentLimit(continuousCurrentLimit, 10);
		wristMotor->EnableCurrentLimit(true);
	} else {
		wristMotor->EnableCurrentLimit(false);
	}

	double maximumNumberOfTurns = settings.getNumber("WristNumberOfTurns");

	maximumForwardLimit = maximumNumberOfTurns * wristMotor->getEncoderCodes() * 1.25 - 100;
	maximumReverseLimit = 10;

	wristMotor->ConfigForwardSoftLimitThreshold(maximumForwardLimit, 20);
	wristMotor->ConfigReverseSoftLimitThreshold(maximumReverseLimit, 20);

	wristMotor->ConfigForwardSoftLimitEnable(true, 20);
	wristMotor->ConfigReverseSoftLimitEnable(true, 20);

	wristMotor->ConfigVoltageCompSaturation(12.0, 10);
	wristMotor->EnableVoltageCompensation(true);

	leftIntakeMotor->ConfigVoltageCompSaturation(12.0, 10);
	leftIntakeMotor->EnableVoltageCompensation(true);

	rightIntakeMotor->ConfigVoltageCompSaturation(12.0, 10);
	rightIntakeMotor->EnableVoltageCompensation(true);

	wristMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 20);
	wristMotor->setEncoderCodes(4096);

	wristMotor->ConfigMotionCruiseVelocity(900, 0);
	wristMotor->ConfigMotionAcceleration(540, 0);

	wristMotor->SelectProfileSlot(0, 0);

	EctoUtils::PidValues wristPid;

	wristPid.p = 1.2285;
	wristPid.i = 0.00205;
	wristPid.d = 0.00101;
	wristPid.f = 1.01;

	motors.configPID(*wristMotor, wristPid, 0);

	wristMotor->setControlMode(ControlMode::MotionMagic);

	log->trace("Read {} as intake output...", intakeSpeedTeleop);
	log->trace("Read {} as eject output...", ejectSpeedTeleop);

	// log->trace("Setting up slave motors...");
	// motors.setupSlave(*rightIntakeMotor, *leftIntakeMotor);

	light.setOutput("StatusLight", true);
}

void CubeIntake::initRobot() {
	wristMotor->setBrake(true);

	stick.addAxis(&wristControl, "leftY", 2);
	stick.addAxis(&intakeControl, "rightY", 2);
	stick.addButton(&releasePiston, "rightPush", 2);
}

void CubeIntake::initTeleop() {
	light.setOutput("StatusLight", false);
	wristMotor->setBrake(true);
}

void CubeIntake::updateTeleop() {
	//cout << intakeControl.get() << endl;
	double intakeInput = intakeControl.get();

	intake(intakeInput);

	currentWristTarget += wristControl.get() * wristJoystickVelocity;
	currentWristTarget = MathUtil::limitNumber(currentWristTarget, maximumReverseLimit, maximumForwardLimit);

	cout << currentWristTarget << endl;

	double wristTarget = -std::abs(std::abs(std::abs(wristControl.get()) - 1) * 0.25 * 1.25 *
	                               wristMotor->getEncoderCodes());
	wristMotor->Set(currentWristTarget);

	//cout << releasePiston.get() << endl;
}

void CubeIntake::initAuto() {
	wristMotor->setBrake(true);

	wristMotor->Set(0.25 * wristMotor->getEncoderCodes() * 1.25 - 100);
}

void CubeIntake::updateAuto() { ; }

void CubeIntake::initDisabled() { wristMotor->setBrake(false); }

void CubeIntake::updateDisabled() { ; }

void CubeIntake::sendInitialTelemetry() const { ; }

void CubeIntake::sendTelemetry() const {
	frc::SmartDashboard::PutBoolean("Enabled", enabled);
}

/**
 * Intakes using the given speed
 * @param speed
 */
void CubeIntake::intake(double speed) {
	double setSpeed = MathUtil::limitNumber(speed, -maxIntakeSpeed, 1.0);

	leftIntakeMotor->Set(speed);
	rightIntakeMotor->Set(speed);

	if (speed < 0)
		pcm.setPiston("Intake", false);
	else if (speed > 0)
		pcm.setPiston("Intake", true);
	else
		pcm.setPiston("Intake", false);

	if (releasePiston.get()) {
		pcm.setPiston("Intake", true);
	}

	enabled = true;
}

/**
 * Ejects cube using the given speed
 * @param speed
 */
void CubeIntake::eject(double speed, bool activateGripPiston) {
	if (speed > 0) {
		log->error(
				"Sending positive value for ejecting on intake, multiplying by -1");
		speed *= -1;
	}

	leftIntakeMotor->Set(speed);
	rightIntakeMotor->Set(speed);

	pcm.setPiston("Intake", activateGripPiston);
	enabled = true;
}

void CubeIntake::writeToMotors(double speed, bool activateGripPiston) {
	leftIntakeMotor->Set(speed);
	rightIntakeMotor->Set(speed);
	// light.setOutput("StatusLight", true);

	pcm.setPiston("Intake", activateGripPiston);
	enabled = true;
}

void CubeIntake::writeToIndividualMotors(double speedLeft, double speedRight,
                                         bool activateGripPiston) {
	leftIntakeMotor->Set(speedLeft);
	rightIntakeMotor->Set(speedRight);

	pcm.setPiston("Intake", activateGripPiston);
	enabled = true;
}

/**
 * Stops the intake
 */
void CubeIntake::stop() {
	leftIntakeMotor->Set(0);
	rightIntakeMotor->Set(0);

	light.setOutput("StatusLight", false);
	pcm.setPiston("Intake", true);
	enabled = false;
}

/**
 * Returns the current intaking status
 * @return
 */
bool CubeIntake::getIntakingStatus() const { return intaking; }