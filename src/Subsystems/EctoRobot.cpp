//
// Created by alber on 12/6/2017.
//

#include "EctoRobot.h"

EctoRobot::EctoRobot(const std::string &robotName) : EctoSubsystem(robotName) {
	spdlog::init_thread_pool(8192, 1);

	settings.setCurrentGroup("Main");
	log = spd::stdout_color_mt("log");

	int logLevel;
	logLevel = (int) settings.getNumber("consoleLogLevel");
	spd::level::level_enum levelRead;
	bool validLevel = false;

	if (logLevel >= 0 && logLevel <= 6) {
		validLevel = true;
		levelRead = spd::level::level_enum(logLevel);
	} else {
		levelRead = spd::level::trace;
		logLevel = 0;
	}

	spd::set_level(levelRead);
	if (!validLevel)
		log->critical("Couldn't read logLevel or invalid entry from settings.ini, defaults to trace (0)");
	log->info("Log level set to: {}", logLevel);

	log->info("Botbusters 4635 | Almost 2019 Code");

	settings.changeFile(settings.getString("TankConfigFile"));
}

void EctoRobot::RobotInit() {
	this->addProperties();

	this->initSubsystems();

	this->initRobot();
	manager.initRobot();
	manager.sendInitialTelemetry();
}

void EctoRobot::RobotPeriodic() {
	this->updateRobot();
	manager.updateRobot();
	manager.sendTelemetry();
}

void EctoRobot::DisabledInit() {
	this->initDisabled();
	manager.initDisabled();
}

void EctoRobot::DisabledPeriodic() {
	this->updateDisabled();
	manager.updateDisabled();
}

void EctoRobot::TeleopInit() {
	this->initTeleop();
	manager.initTeleop();
}

void EctoRobot::TeleopPeriodic() {
	this->updateTeleop();
	manager.updateTeleop();
}

void EctoRobot::AutonomousInit() {
	this->initAuto();
	manager.initAuto();
}

void EctoRobot::AutonomousPeriodic() {
	this->updateAuto();
	manager.updateAuto();
}