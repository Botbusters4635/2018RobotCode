//
// Created by Abiel on 2/10/18.
//

#ifndef BOTBUSTERSREBIRTH_CUBEWINCH_H
#define BOTBUSTERSREBIRTH_CUBEWINCH_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/EctoSubsystem.h"
#include "Managers/EctoMotors.h"
#include "Managers/EctoInput.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/EctoUtils.h"
#include "Subsystems/Input/JoystickAxisExpo.h"
#include "Subsystems/Input/EctoButton.h"

using namespace std;

enum CurrentPosition {
	CubeHeight, //Used to pick up cubes
	ExchangeHeight, //Used to deposit cubes in the exchange
	SwitchHeight, //Used to place cubes in the switch
	ScaleHeight //Used to place cubes in the scale
};

class CubeWinch : public EctoSubsystem {
public:
	CubeWinch();

	void initTeleop() override;

	void updateTeleop() override;

	void initAuto() override;

	void updateAuto() override;

	void initDisabled() override;

	void updateDisabled() override;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

	double getCurrentHeight() const;

	CurrentPosition getCurrentPosition() const;

	void setPosition(CurrentPosition pos);

	void setNativeHeight(double nativeHeight);

	double getNativeHeight() const;

	double getCurrentNativeHeight();

	int getCurrentVelocity();

	double getMaxHeight() const;

	double getMaxNativeHeight() const;

	double getMinHeight() const;

	double getMinNativeHeight() const;

	void resetZeroEncoderPos();

	double determineNativeHeight(CurrentPosition pos) const;

	CurrentPosition determinePositionFromNativeHeight(double nativeHeight) const;

	void setMagicMotionStatus(bool status);

	bool getMagicMotionStatus() const;

	double getMaximumAcceleration() const;

private:
	JoystickAxisExpo upWinch, downWinch;
	EctoButton forceResetZero;

	double maximumUpVelocityWinch, maximumDownVelocityWinch;

	shared_ptr<spd::logger> log = spd::stdout_color_mt("Cube Winch");

	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();

	static constexpr auto masterName = "CubeWinchMaster";
	static constexpr auto slaveName = "CubeWinchSlave";

	static constexpr double defaultHeight = 0.0;
	double currentOutput = defaultHeight;

	CurrentPosition currentPosition = CurrentPosition::CubeHeight;
	double currentNativeHeight = 0;

	EctoTalon &masterMotor;
	EctoTalon &slaveMotor;

	double robotBaseHeight = 0;

	double nativeUnitsPerRev = 0;

	double winchDiameter = 0;

	double maxHeight, minHeight;
	double maxHeightNative, minHeightNative;

	double forwardSoftLimit, reverseSoftLimit;
	double forwardSoftLimitNative, reverseSoftLimitNative;

	bool forwardSoftLimitEnabled = false;
	bool reverseSoftLimitEnabled = false;

	bool manualControl = false;

	double maximumSpeed, maximumAcceleration;
	double maximumNativeSpeed, maximumNativeAcceleration;

	bool hasZeroSwitch = false;
	ctre::phoenix::ParamEnum zeroSwitchPos;

	double cubeHeight, elevatedCubeHeight, exchangeHeight, switchHeight, scaleHeight, manualHeight, manualStep = 1;
	double nativeCubeHeight, nativeElevatedCubeHeight, nativeExchangeHeight, nativeSwitchHeight, nativeScaleHeight;

	bool prevValueLeftShoulder = false, prevValueRightShoulder = false, prevValueYButton = false;

	bool magicMotionEnabled = true;

	double currentSetHeight = 0.0;

	double offsetToMoveAfterReleasedWinch = 1000.0;

};


#endif //BOTBUSTERSREBIRTH_CUBEWINCH_H
