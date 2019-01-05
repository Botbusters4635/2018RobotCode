/*
* Settings
* Group: Shooter
* Output: Normal Speed
* MaxOutput: Maximum Speed
* OutputRevolver: Revolver Speed
* WheelDiameterMeters: Diameter of Wheel
*/

//DEPRECIATED CLASS!!!

#include "Controllers/EctoTalon.h"
#include "Managers/EctoMotors.h"
#include "Managers/EctoInput.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/EctoUtils.h"
#include "PIDs/SimplePID.h"
#include "Subsystems/Input/EctoButton.h"
#include "Subsystems/Input/JoystickAxisExpo.h"
#include <frc/WPILib.h>
#include <iostream>

using namespace std;

/**
 * Controls the shooter using PID
 */
class EctoShooter {
public:
	void update();

	void shootAuto();

	static EctoShooter &getInstance() {
		static EctoShooter singletonInstance;
		return singletonInstance;
	}

private:
	EctoButton xButton, otherButton;
	JoystickAxisExpo leftTrigger;

	void shoot(double speed, int direction);

	EctoShooter();

	EctoShooter(const EctoShooter &);

	EctoShooter &operator=(const EctoShooter &);

	SimplePIDSource speed;
	frc::PIDController speedPID;
	SimplePIDOutput outputPID;

	double maxOutput;

	double output, outputRevolver, wheelDiameter;
	EctoMotors &motors = EctoMotors::getInstance();

	EctoTalon &motor1, &motor2, &motorRevolver;
	EctoInput &stick = EctoInput::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();

	frc::Timer clock;
};
