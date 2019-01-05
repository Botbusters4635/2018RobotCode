//
// Created by Abiel on 2/7/18.
//

/*

#ifndef BOTBUSTERSREBIRTH_MASTEROFPUPPETS_H
#define BOTBUSTERSREBIRTH_MASTEROFPUPPETS_H

#include "Subsystems/Mechanisims/CubeIntake.h"
#include "Subsystems/Mechanisims/CubeWinch.h"
#include "Subsystems/DriveTrain/EctoTank.h"
#include "Subsystems/Path/PathParser.h"
#include "Subsystems/Path/PurePursuitRunner.h"
#include "EctoSubsystem.h"
#include "Utilities/MatchData.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/EctoTimer.h"

using namespace OpenRIO::PowerUp;

class MasterOfPuppets : public EctoSubsystem {
public:
	static MasterOfPuppets &getInstance() {
		static MasterOfPuppets singletonInstance;
		return singletonInstance;
	}

	void initTeleop() override;

	void updateTeleop() override;

	void initAuto() override;

	void updateAuto() override;

	void initDisabled() override;

	void updateDisabled() override;

	void sendTelemetry() const override;

	void sendInitialTelemetry() const override;

	bool getIntakingStatus() const;

private:
	bool overrideTrySwitch = true;
	bool overrideTryScale = false;
	bool trySecondCube = false;
	bool onlyReturn = true;

	string overridePosition = "Center";

	void limitTankSpeedWithWinch();

	shared_ptr<spd::logger> log = spd::stdout_color_mt("MasterOfPuppets");

	bool trySwitch, tryScale;

	bool hasRun = false, hasSetup = false, hasFinished = false;
	double pathSpeed = .45, lookAheadDistance = 0.5, startTime = 0, outSpeedIntakeScale = 0, outSpeedIntakeSwitch = 0, intakeSpeed = 0;
	double simpleMovementSpeed = 0.0, simpleMovementAcceleration = 0.0;

	bool movedBack = false;

	bool pilePathSetup = false;
	bool wentToCubePath = false;
	bool droveBack = false;
	bool returnToSwitchSetup = false;
	bool returnedToSwitch = false;

	double distanceToMoveBackFromSwitch, distanceToMoveFromSwitchToCenterCube, distanceToGrabCube;

	//In seconds
	double timeToWaitToGrabCube;

	string pathName, posStart;

	CubeIntake &cubeIntake = CubeIntake::getInstance();
	CubeWinch &cubeWinch = CubeWinch::getInstance();
	EctoTank &ectoTank = EctoTank::getInstance();

	explicit MasterOfPuppets();

	MasterOfPuppets &operator=(const MasterOfPuppets &);

	EctoSettings &settings = EctoSettings::getInstance();

	PathParser parser = PathParser("/home/lvuser/paths");
	shared_ptr<Path> path;
	shared_ptr<PurePursuitRunner> runner;

	SendableChooser<string> *initialPosChooser;

	EctoTimer &timer = EctoTimer::getInstance();
};


#endif //BOTBUSTERSREBIRTH_MASTEROFPUPPETS_H
*/