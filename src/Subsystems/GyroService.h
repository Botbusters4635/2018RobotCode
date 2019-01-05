//
// Created by Abiel on 12/9/18.
//

#ifndef BOTBUSTERSREBIRTH_GYROSERVICE_H
#define BOTBUSTERSREBIRTH_GYROSERVICE_H

#include "Sensors/NavX.h"
#include "Subsystems/RobotData.h"
#include "Subsystems/EctoSubsystem.h"
#include "Utilities/EctoWatchdog.h"

class GyroService : public EctoSubsystem {
public:
	GyroService();

	void initRobot() override;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

private:
	EctoWatchdog *watchdog;

	void loop();

	NavX &navx = NavX::getInstance();
	RobotData &data = RobotData::getInstance();
};


#endif //BOTBUSTERSREBIRTH_GYROSERVICE_H
