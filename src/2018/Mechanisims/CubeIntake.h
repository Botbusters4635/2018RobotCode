#ifndef BOTBUSTERSREBIRTH_CUBEINTAKE_H
#define BOTBUSTERSREBIRTH_CUBEINTAKE_H

#include "Utilities/EctoSettings.h"
#include "Managers/EctoInput.h"
#include "Managers/EctoMotors.h"
#include "Managers/EctoPCM.h"
#include "Subsystems/EctoSubsystem.h"
#include "2018/Mechanisims/StatusLight.h"
#include "Subsystems/Input/JoystickAxisExpo.h"
#include "Subsystems/Input/EctoButton.h"
#include <vector>
#include <string>

using namespace std;

class CubeIntake : public EctoSubsystem {
public:
	CubeIntake();

	void initRobot() override;

	void initTeleop() override;

	void updateTeleop() override;

	void initAuto() override;

	void updateAuto() override;

	void initDisabled() override;

	void updateDisabled() override;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

	void lowerIntake();

	void raiseIntake();

	void intake(double speed);

	void eject(double speed, bool activateGripPiston = true);

	void writeToMotors(double speed, bool activateGripPiston);

	void writeToIndividualMotors(double speedLeft, double speedRight, bool activateGripPiston);

	void stop();

	bool getIntakingStatus() const;

private:
	StatusLight &light = StatusLight::getInstance();

	shared_ptr<spd::logger> log = spd::stdout_color_mt("Cube Intake");

	static constexpr auto brakeState = true;

	shared_ptr<EctoTalon> wristMotor;
	shared_ptr<EctoTalon> leftIntakeMotor, rightIntakeMotor;

	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();

	EctoSettings &settings = EctoSettings::getInstance();
	EctoPCM &pcm = EctoPCM::getInstance();

	double maxIntakeSpeed = 0.5;

	bool enabled = false;
	bool intaking = false;
	bool reversed = false;

	EctoButton releasePiston;
	JoystickAxisExpo wristControl, intakeControl;

	double intakeSpeedTeleop = 0, ejectSpeedTeleop = 0;

	double currentWristTarget = 0;

	double maximumForwardLimit, maximumReverseLimit;

	double wristJoystickVelocity = 50;
};

#endif
