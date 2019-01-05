#ifndef BOTBUSTERSREBIRTH_ECTOTANK_H
#define BOTBUSTERSREBIRTH_ECTOTANK_H

#include "Managers/EctoInput.h"
#include "Managers/EctoMotors.h"
#include "Managers/EctoPCM.h"
#include "Subsystems/EctoSubsystem.h"

#include "Gearbox.h"

#include "Subsystems/Input/InputHandler.h"
#include "Subsystems/Input/JoystickAxisExpo.h"

#include "Controllers/EctoTalon.h"

#include "Sensors/NavX.h"

#include "Utilities/EctoSettings.h"
#include "Utilities/MotorValues.h"

#include "Subsystems/Input/ToggleButton.h"

#include "Subsystems/RobotData.h"
#include "Robot/RobotProperties.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <cmath>
#include <memory>
#include <string>

struct EctoTankConfig {
	std::string leftMasterName;
	std::string rightMasterName;

	bool hasSlaves = true;

	std::vector<std::string> leftSlaves;
	std::vector<std::string> rightSlaves;

	bool defaultBrakeState = true;

	GearboxConfig gearboxConfig;

	EctoUtils::PidValues lowGearPID;
	EctoUtils::PidValues highGearPID;

	MotorMode defaultTeleopControlMode = MotorMode::VOLTAGE;

	// Maximum current per motor for current control mode
	double maximumCurrentPerMotor = 25;

	bool enableVoltageCompensation = true;
	double voltageCompensation = 12.0;

	double voltageCompensationFilterSamples = 32;

	double joystickDeadzone = 0.1;
	double joystixExpo = 0.08;

	//In meters * s ^ -2
	double maximumAutonomousAcceleration = 1;

	//In meters * s ^ -3
	double maximumAutonomousJerk = 1;

	double maximumAngularAcceleration = 1;

	double maxiumumAngularJerk = 1;

	//In meters * s ^ -2 (only used to calculate distance to deaccelerate)
	double maximumDeacceleration = 1;
};

class EctoTank : public EctoSubsystem {
public:
	EctoTank() = delete;

	EctoTank(const EctoTankConfig &config,
	         const std::shared_ptr<InputHandler> &initialInputHandler);

	// MotorValues already contains the mode
	void writeMotors(const MotorValues &in);

	void initTeleop() override;

	void updateTeleop() override;

	void updateRobot() override;

	void initAuto() override;

	void updateAuto() override;

	void initDisabled() override;

	void updateDisabled() override;

	void setBrakes(bool state);

	double getYaw();

	double getCurrentMaxSpeed() const;

	MotorValues getCurrentSpeeds();

	MotorValues getCurrentDistances();

	/**
	 * Assumes 100% motor voltage is 12.0V
	 * @return
	 */
	MotorValues getCurrentVoltages();

	double getCurrentMinSpeed() const;

	GearboxConfig getGearboxConfig() const;

	void setGearboxConfig(const GearboxConfig &gearboxConfig);

	void setControlMode(MotorMode motorMode);

	void stop();

	void driveFollowingAngle(double speed, double angle);

	void turnToAngle(double angle);

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("EctoTank");

	EctoTankConfig tankConfig;

	GearboxConfig gearboxConfig;

	// Assumes gearboxes are the same on both sides
	Gearbox driveGearbox;

	std::shared_ptr<InputHandler> currentInputHandler;

	JoystickAxisExpo leftAxis, rightAxis;
	ToggleButton toggleCurrentMode;

	EctoButton highGearButton;
	EctoButton lowGearButton;

	MotorMode currentTeleopMode;

	EctoTank &operator=(const EctoTank &);

	NavX &navx = NavX::getInstance();

	EctoSettings &settings = EctoSettings::getInstance();
	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	EctoPCM &pcm = EctoPCM::getInstance();
	RobotData &robotData = RobotData::getInstance();
	RobotProperties &robotProperties = RobotProperties::getInstance();

	void setupPids();

	void setupSlaves();

	EctoTalon &leftMaster, &rightMaster;

	vector<shared_ptr<EctoTalon>> leftSlaves, rightSlaves;

	EctoUtils::PidValues speedPid;
	EctoUtils::PidValues torquePid;

	double wheelCircumference;
	double wheelBase;

	const bool brakeState = true;

	const unsigned int highGearProfileSlot = 0;
	const unsigned int lowGearProfileSlot = 1;

	unsigned int currentProfile = 0;

	void selectProfileSlots(unsigned int profileSlot);

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

	double previousLeftCurrent = 0.0;
	double previousRightCurrent = 0.0;

	const bool poseido = false;
};

#endif  // BOTBUSTERSREBIRTH_ECTOTANK2_H
