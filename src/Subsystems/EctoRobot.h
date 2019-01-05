//
// Created by alber on 12/6/2017.
//

#ifndef BOTBUSTERSREBIRTH_ECTOROBOT_H
#define BOTBUSTERSREBIRTH_ECTOROBOT_H

#include "spdlog/spdlog.h"
#include <frc/WPILib.h>
#include "Utilities/EctoSettings.h"
#include "Subsystems/DriveTrain/EctoTank.h"
#include "Subsystems/Input/InputHandler.h"
#include "Robot/RobotProperties.h"
#include "EctoSubsystem.h"
#include "SubsystemManager.h"

#include <string>

namespace spd = spdlog;

class EctoRobot : public TimedRobot, protected EctoSubsystem {
public:
	explicit EctoRobot(const std::string &robotName);

	/**
	 * Add your subsystems to the manager here, the first thing that is called
	 */
	virtual void initSubsystems() = 0;

	/**
	 * Add all properties to the global robot properties here
	 */
	virtual void addProperties() = 0;

protected:
	SubsystemManager &manager = SubsystemManager::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();
	RobotProperties &properties = RobotProperties::getInstance();

	virtual EctoTankConfig getTankConfig() = 0;

	virtual std::shared_ptr<InputHandler> getStartingInputHandler() = 0;

	std::shared_ptr<InputHandler> initialInputHandler;

	std::shared_ptr<spdlog::logger> log;

private:
	void RobotInit() override;

	void RobotPeriodic() override;

	void DisabledInit() override;

	void DisabledPeriodic() override;

	void TeleopInit() override;

	void TeleopPeriodic() override;

	void AutonomousInit() override;

	void AutonomousPeriodic() override;
};


#endif //BOTBUSTERSREBIRTH_ECTOROBOT_H
