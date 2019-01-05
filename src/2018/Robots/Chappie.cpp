//
// Created by Abiel on 8/28/18.
//

#include "Chappie.h"

Chappie::Chappie() : EctoRobot("Chappie2018"), settings(CHAPPIE_FILENAME) {
	;
}

void Chappie::initSubsystems() {
	initialInputHandler = getStartingInputHandler();
	tank = std::make_shared<EctoTank>(getTankConfig(), initialInputHandler);

	state = std::make_shared<TankState>();
	gyroService = std::make_shared<GyroService>();

	manager.addSubsystem(gyroService);
	manager.addSubsystem(tank);
	manager.addSubsystem(state);
}

void Chappie::addProperties() {
	RobotDimensions dimensions;

	settings.setCurrentGroup("Dimensions");
	dimensions = RobotDimensions::getDimensionsFromSettings(settings);

	properties.setDimensions(dimensions);
}

void Chappie::initRobot() {
	;
}

void Chappie::updateRobot() {
	cout << NavX::getInstance().GetYaw() << endl;
}

void Chappie::initAuto() {
	;
}

void Chappie::updateAuto() {
	;
}

void Chappie::initTeleop() { ; }

void Chappie::updateTeleop() {
	;
}

void Chappie::initDisabled() { ; }

void Chappie::updateDisabled() { ; }

void Chappie::sendInitialTelemetry() const { ; }

void Chappie::sendTelemetry() const { ; }

EctoTankConfig Chappie::getTankConfig() {
	EctoTankConfig config;

	config.leftMasterName = "LeftMaster";
	config.rightMasterName = "RightMaster";

	config.hasSlaves = true;

	config.leftSlaves = std::vector<std::string>{"LeftSlave1", "LeftSlave2"};
	config.rightSlaves = std::vector<std::string>{"RightSlave1", "RightSlave2"};

	config.defaultBrakeState = true;

	// TODO Define correct values for the gearbox
	GearboxConfig gearboxConfig;

	config.gearboxConfig = gearboxConfig;

	// TODO Define PIDs
	EctoUtils::PidValues lowGearPID;

	lowGearPID.p = 1.1;
	lowGearPID.i = 0;
	lowGearPID.d = 0;
	lowGearPID.f = 0.0;

	config.lowGearPID = lowGearPID;
	// config.highGearPID

	config.defaultTeleopControlMode = MotorMode::VOLTAGE;

	// TODO Set the maximum current to a correct value
	config.maximumCurrentPerMotor = 10;

	config.enableVoltageCompensation = true;
	config.voltageCompensation = 12.0;

	config.joystixExpo = 0.15;
	config.joystickDeadzone = 0.18;

	return config;
}

std::shared_ptr<InputHandler> Chappie::getStartingInputHandler() {
	TankDifferentialDriveConfig config;

	return std::shared_ptr<InputHandler>(new TankDifferentialDrive(config));
}