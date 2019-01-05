//
// Created by Abiel on 10/13/18.
//

#include "SubsystemManager.h"

SubsystemManager::SubsystemManager() {
	settings.setCurrentGroup("Main");
	log->info("Initializing SubsystemManager...");
}

void SubsystemManager::addSubsystem(const shared_ptr<EctoSubsystem> &newSubsystem) {
	log->trace("Adding {} to vector...", newSubsystem->getName());
	subsystems.emplace_back(newSubsystem);
}

void SubsystemManager::initRobot() {
	for (const auto &subsystem : subsystems) {
		subsystem->initRobot();
	}
}

void SubsystemManager::updateRobot() {
	for (const auto &subsystem : subsystems) {
		subsystem->updateRobot();
	}
}

void SubsystemManager::initDisabled() {
	for (const auto &subsystem : subsystems) {
		subsystem->initDisabled();
	}
}

void SubsystemManager::updateDisabled() {
	for (const auto &subsystem : subsystems) {
		subsystem->updateDisabled();
	}
}

void SubsystemManager::initTeleop() {
	for (const auto &subsystem : subsystems) {
		subsystem->initTeleop();
	}

}

void SubsystemManager::updateTeleop() {
	for (const auto &subsystem : subsystems) {
		subsystem->updateTeleop();
	}
}

void SubsystemManager::initAuto() {
	for (const auto &subsystem : subsystems) {
		subsystem->initAuto();
	}
}

void SubsystemManager::sendInitialTelemetry() const {
	for (const auto &subsystem : subsystems) {
		subsystem->sendInitialTelemetry();
	}
}

void SubsystemManager::sendTelemetry() const {
	for (const auto &subsystem : subsystems) {
		subsystem->sendTelemetry();
	}
}

void SubsystemManager::updateAuto() {
	for (const auto &subsystem : subsystems) {
		subsystem->updateAuto();
	}
}

void SubsystemManager::printRegisteredSubsystems() {
	log->info("Registered Subsystems are: ");
	for (const auto &subsystem : subsystems) {
		log->info(subsystem->getName());
	}
}

