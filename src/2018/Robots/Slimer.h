//
// Created by Abiel on 8/28/18.
//

#ifndef BOTBUSTERSREBIRTH_SLIMER_H
#define BOTBUSTERSREBIRTH_SLIMER_H

#include "Subsystems/EctoRobot.h"

#include "Utilities/EctoSettings.h"

#include "Subsystems/Input/InputHandlers/TankDifferentialDrive.h"

#include "2018/Mechanisims/CubeIntake.h"
#include "2018/Mechanisims/CubeWinch.h"

#include "Managers/EctoInput.h"
#include "Subsystems/Input/ToggleButton.h"

#include "Managers/EctoPCM.h"
#include "Managers/EctoMotors.h"

#include "Subsystems/TankState.h"
#include "Sensors/NavX.h"

#include "Subsystems/TankState.h"
#include "Subsystems/GyroService.h"

class Slimer : public EctoRobot {
public:
	Slimer();

	void addProperties() override;

	void initSubsystems() override;

	void initRobot() override;

	void updateRobot() override;

	void initAuto() override;

	void updateAuto() override;

	void initTeleop() override;

	void updateTeleop() override;

	void initDisabled() override;

	void updateDisabled() override;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

	EctoTankConfig getTankConfig() override;

	std::shared_ptr<InputHandler> getStartingInputHandler() override;

private:
	static constexpr auto SLIMER_FILENAME = "/home/lvuser/Slimer2018.ini";

	EctoInput &stick = EctoInput::getInstance();
	EctoPCM &pcm = EctoPCM::getInstance();
	EctoMotors &motors = EctoMotors::getInstance();
	EctoSettings settings;

	NavX &navx = NavX::getInstance();

	std::shared_ptr<EctoTank> tank;
	std::shared_ptr<TankState> state;
	std::shared_ptr<GyroService> gyroService;
	// std::shared_ptr<CubeIntake> intake;
	// std::shared_ptr<CubeWinch> winch;
	// std::shared_ptr<AutonomoDisney> autonomo;

	// ToggleButton firstPiston;
	// ToggleButton secondPiston;

	// ToggleButton elevatorOn;
	// ToggleButton elevatorRetract;
};

#endif  // BOTBUSTERSREBIRTH_SLIMER_H
