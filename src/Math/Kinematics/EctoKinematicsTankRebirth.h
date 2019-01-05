//
// Created by Abiel on 9/14/18.
//

#ifndef BOTBUSTERSREBIRTH_ECTOKINEMATICSTANKREBIRTH_H
#define BOTBUSTERSREBIRTH_ECTOKINEMATICSTANKREBIRTH_H

class RobotPose2D;

class Twist2D;

class Rotation2D;

namespace EctoKinematicsTankRebirth {
	Twist2D forwardKinematics(double leftWheelDelta, double rightWheelDelta, double driveWheelTrackWidth,
	                          double tankScrubFactor);

	Twist2D forwardKinematics(double leftWheelDelta, double rightWheelDelta, double deltaRotation);

	Twist2D forwardKinematics(const Rotation2D &prevHeading, const Rotation2D &currentHeading, double leftWheelDelta,
	                          double rightWheelDelta);

	RobotPose2D integrateForwardKinematics(const RobotPose2D &currentPose, const Twist2D &forwardKinematics);
}


#endif //BOTBUSTERSREBIRTH_ECTOKINEMATICSTANKREBIRTH_H
