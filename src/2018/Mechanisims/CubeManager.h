//
// Created by Abiel on 2/28/18.
//

#ifndef BOTBUSTERSREBIRTH_CUBEMANAGER_H
#define BOTBUSTERSREBIRTH_CUBEMANAGER_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/EctoSubsystem.h"
#include "Managers/EctoInput.h"

#include "Subsystems/Input/EctoButton.h"
#include "Subsystems/Input/JoystickAxisExpo.h"

#include "CubeIntake.h"
#include "CubeWinch.h"

using namespace std;

class CubeManager : public EctoSubsystem {
public:
	static CubeManager &getInstance() {
		static CubeManager singletonInstance;
		return singletonInstance;
	}

	void initTeleop() override;

	void updateTeleop() override;

	void initAuto() override;

	void updateAuto() override;

	void initDisabled() override;

	void updateDisabled() override;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

private:
	explicit CubeManager();

	EctoButton moveToSwitch;
	EctoButton forceResetZero;

	EctoButton enableIntakeController;
	EctoButton enableIntakeSecondController;

	EctoButton shootIntakeController;
	EctoButton shootIntakeSecondController;

	EctoButton shootWithoutPistonIntakeController;

	EctoButton enableDoubleStickControlIntake;

	EctoButton forceIntakeUpController;

	JoystickAxisExpo *upWinch;
	JoystickAxisExpo *downWinch;

	JoystickAxisExpo *upWinchSecond;
	JoystickAxisExpo *downWinchSecond;

	JoystickAxisExpo *stickIntake;

	JoystickAxisExpo *manualStickLeftIntake;
	JoystickAxisExpo *manualStickRightIntake;

	double determineIntakeEjectSpeed(CurrentPosition in) const;

	bool determineIntakeSwivelPosition(CurrentPosition in) const;

	double determineIntakeEjectSpeedNative(double in) const;

	bool determineIntakeSwivelPositionNative(double in) const;

	shared_ptr<spd::logger> log = spd::stdout_color_mt("Cube Manager");

	EctoSettings &settings = EctoSettings::getInstance();
	EctoInput &stick = EctoInput::getInstance();

	CubeIntake intake;
	CubeWinch winch;

	//Intake buttons
	static constexpr auto buttonEnableIntake = "rightJoy";
	static constexpr auto buttonShootIntake = "leftJoy";

	//Previous intake button state (used for toggles)
	bool previousStateEnableIntake = false;
	bool previousStateShootIntake = false;
	bool previousStateDoubleStickControlIntake = false;

	//Stop winch after triggers are released control
	bool stopAfterReleasedWinch = true;
	static const bool offsetCalculationEnabled = true;
	double offsetToMoveAfterReleasedWinch = 1000.0;

	//Winch Buttons
	static constexpr auto buttonScaleWinch = "leftPush";
	static constexpr auto buttonSwitchWinch = "rightPush";
	static constexpr auto buttonExchangeWinch = "Y";

	//Winch Buttons for new control style
	static constexpr auto triggerUpWinch = "rightTrig";
	static constexpr auto triggerDownWinch = "leftTrig";

	static constexpr auto buttonReturnZero = "leftPush";
	static constexpr auto buttonSnapWinch = "rightPush";

	//New winch control style configuration parameters
	//Expo control for winch triggers
	double downTriggerExpoWinch = 0;
	double upTriggerExpoWinch = 0;

	//Maximum native velocity at which to move winch with triggers
	double maximumUpVelocityWinch = 0;
	double maximumDownVelocityWinch = 0;

	//Single intake stick expo paramater
	double singleStickExpoIntake = 0;

	//Separate intake stick expo parameter
	double leftStickExpoIntake = 0;
	double rightStickExpoIntake = 0;

	//When true, intake is enabled
	bool intakeEnabled = false;

	bool intakeDoubleStickEnabled = false;

	//Default speed at which to intake at
	double intakeSpeedTeleop = 0, ejectSpeedTeleop = 0;

	//Speed at which to eject at at set position
	double ejectSpeedSwitch, ejectSpeedExchange, ejectSpeedScale;

	//Current winch position
	CurrentPosition currentPosition = CurrentPosition::CubeHeight;

	double currentSetHeight;
};


#endif //BOTBUSTERSREBIRTH_CUBEMANAGER_H
