//
// Created by Alberto on 11/26/2017.
//

#ifndef BOTBUSTERSREBIRTH_PUREPURSUITRUNNER_H
#define BOTBUSTERSREBIRTH_PUREPURSUITRUNNER_H

#include "Utilities/EctoUtils.h"
#include "Subsystems/DriveTrain/EctoTank.h"
#include "PathContainer.h"
#include "Subsystems/TankState.h"
#include "spdlog/spdlog.h"
#include "Utilities/EctoTimer.h"
#include "Utilities/EctoSettings.h"

class PurePursuitRunner {

public:
	explicit PurePursuitRunner(TankState &inState, const PathContainer &inPathContainer, double lookAheadDistance = 1);

	void runPath(EctoTank &driveTrain, double speed);

	bool isFinished();

	double getPercentCompleted() const;


private:
	EctoTimer &timer = EctoTimer::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();
	TankState &state;

	shared_ptr<spd::logger> log = spd::get("log");

	double getTime();

	bool started, finished, reduceSpeedWithAngleError;

	double lookAheadDistance, currentTargetSpeed, secsToAccel;

	double turnProportional, minimumSpeedMultiplier, maximumSpeedMultiplier;

	vector<pair<double, double>> accelerationProfile;

	unsigned int currentTargetWaypoint, speedTimeSteps, speedIndex;

	double startTime = timer.getTime();

	const PathContainer &pathContainer;
};

#endif //BOTBUSTERSREBIRTH_PUREPURSUITRUNNER_H
