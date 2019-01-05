/*
* Settings
* Group: Tank
* WheelWidth: Wheel Width
* TankScrubFactor: ???
*/

#ifndef BOTBUSTERSREBIRTH_ECTOKINEMATICSTANK_H
#define BOTBUSTERSREBIRTH_ECTOKINEMATICSTANK_H

#include "Math/DataTypes/RobotPose2D.h"
#include "Utilities/MotorValues.h"
#include "Utilities/EctoSettings.h"
#include <memory>

class EctoKinematicsTank final {
public:
	static EctoKinematicsTank &getInstance() {
		static EctoKinematicsTank singletonInstance;
		return singletonInstance;
	}

	/**
	 * Calculates inverse kinematics, returns values from -1 to 1, unitless.
	 * @param  vX       X Velocity, preferably from -1 to 1
	 * @param  vY       Y Velocity, preferably from -1 to 1
	 * @param  rotation Rotation speed in radians
	 * @return
	 */
	MotorValues inverseKinematics(double vX, double vY, double rotation);

	/**
	 * Calculates inverse kinematics, returns values in necessary wheel speeds in unit/unit (t).
	 * @param  vX       X Velocity, in unit/unit(t)
	 * @param  vY       Y Velocity, in unit/unit(t)
	 * @param  rotation Rotation speed in radians
	 * @param  radius   Radius of wheel in previously used units
	 * @return
	 */
	MotorValues inverseKinematics(double vX, double vY, double rotation, double radius);

	/**
	 * Calculates inverse kinematics, unitless.
	 * @param  in Twist2d object with velocities
	 * @return
	 */
	MotorValues inverseKinematics(const RobotPose2D &in);

	/**
	 * Calculates inverse kinematics, returns values of necessary wheel speeds in unit/unit (t).
	 * @param  in     Twist2d object with velocities
	 * @param  radius Radius of wheel in previously used units
	 * @return
	 */
	MotorValues inverseKinematics(const RobotPose2D &in, double radius);

	/**
	 * Returns velocities
	 * @param  in     Motor Velocities
	 * @param  radius Wheel Radius
	 * @return
	 */
	const RobotPose2D kinematics(const MotorValues &in, double radius);


private:
	EctoKinematicsTank();

	EctoKinematicsTank &operator=(const EctoKinematicsTank &);

	EctoSettings &settings = EctoSettings::getInstance();

	double wheelWidth = 0;
	double tankScrubFactor = 0;
};

#endif
