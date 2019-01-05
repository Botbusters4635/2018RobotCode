
#ifndef BOTBUSTERSREBIRTH_MECHANISM_H
#define BOTBUSTERSREBIRTH_MECHANISM_H

#include "Subsystems/Telemetry/EctoTelemetry.h"
#include "Utilities/EctoSettings.h"
#include <string>
#include <memory>
#include "spdlog/spdlog.h"

namespace spd = spdlog;

class EctoSubsystem : public EctoTelemetry {
public:
	explicit EctoSubsystem(const string &name);

	virtual void initRobot();

	virtual void updateRobot();

	virtual void initTeleop();

	virtual void updateTeleop();

	virtual void initAuto();

	virtual void updateAuto();

	virtual void initDisabled();

	virtual void updateDisabled();

	std::string getName() const;

private:
	std::string name;
};


#endif

