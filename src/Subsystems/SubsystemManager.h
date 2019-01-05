//
// Created by Abiel on 10/13/18.
//

#ifndef BOTBUSTERSREBIRTH_SUBSYSTEMMANAGER_H
#define BOTBUSTERSREBIRTH_SUBSYSTEMMANAGER_H

#include "Telemetry/EctoTelemetry.h"
#include "EctoSubsystem.h"
#include <memory>

class EctoSubsystem;

class SubsystemManager : private EctoTelemetry {
	//friend class MasterOfPuppets;

public:
	static SubsystemManager &getInstance() {
		static SubsystemManager singletonInstance;
		return singletonInstance;
	}

	void addSubsystem(const std::shared_ptr<EctoSubsystem> &newSubsystem);

	void initRobot();

	void updateRobot();

	void initDisabled();

	void updateDisabled();

	void initTeleop();

	void updateTeleop();

	void initAuto();

	void updateAuto();

	void printRegisteredSubsystems();

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

private:
	SubsystemManager();

	shared_ptr<spd::logger> log = spd::stdout_color_mt("SubsystemManager");

	EctoSettings &settings = EctoSettings::getInstance();
	bool managerEnabled = false;
	vector<shared_ptr<EctoSubsystem>> subsystems;
};


#endif //BOTBUSTERSREBIRTH_SUBSYSTEMMANAGER_H
