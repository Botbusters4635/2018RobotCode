#include "EctoKinematicsTank.h"

EctoKinematicsTank::EctoKinematicsTank() {
	settings.setCurrentGroup("Tank");

	wheelWidth = settings.getNumber("WheelWidth");
	//TODO Find out what this is
	tankScrubFactor = settings.getNumber("TankScrubFactor");
}

//TODO Check the following functions

/**
 * Calculate speed to move the motors at given velocity.
 * @param  vX       X Velocity
 * @param  vY       Y Velocity (Not used)
 * @param  rotation Rotation Value
 * @return
 */
MotorValues EctoKinematicsTank::inverseKinematics(double vX, double vY, double rotation) {
	if (vY != 0)
		throw logic_error("Attempted to calculate inverse kinematics for tank with a Y value! This is not possible.");

	MotorValues temp;

	//If values are 0
	if (abs(rotation) < std::numeric_limits<double>::epsilon()) {
		temp.left = vX;
		temp.right = vX;
	}

	//TODO Implement scrub values if needed
	double delta_v = rotation / 2.0;
	temp.left = vX - delta_v;
	temp.right = vX + delta_v;

	return temp;
}

MotorValues EctoKinematicsTank::inverseKinematics(double vX,
                                                  double vY,
                                                  double rotation,
                                                  double radius) {
	MotorValues calc = inverseKinematics(vX, vY, rotation);

	calc.left = calc.left * M_PI_2 * radius;
	calc.right = calc.right * M_PI_2 * radius;

	return calc;
}

MotorValues EctoKinematicsTank::inverseKinematics(const RobotPose2D &in) {
	return inverseKinematics(in.getX(), in.getY(), in.getTheta());
}

MotorValues EctoKinematicsTank::inverseKinematics(const RobotPose2D &in, double radius) {
	return inverseKinematics(in.getX(), in.getY(), in.getTheta(), radius);
}

//TODO Check if formulas are correct
const RobotPose2D EctoKinematicsTank::kinematics(const MotorValues &in, double heading) {
	double dx = (in.left + in.right) / 2.0;

	return RobotPose2D(dx, 0, heading);
}
