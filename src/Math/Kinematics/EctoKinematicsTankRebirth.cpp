//
// Created by Abiel on 9/14/18.
//

#include "EctoKinematicsTankRebirth.h"
#include "Math/DataTypes/Twist2D.h"
#include "Math/DataTypes/Rotation2D.h"
#include "Math/DataTypes/RobotPose2D.h"

Twist2D
EctoKinematicsTankRebirth::forwardKinematics(double leftWheelDelta, double rightWheelDelta, double driveWheelTrackWidth,
                                             double tankScrubFactor) {
	double deltaRotation = (rightWheelDelta - leftWheelDelta) / (driveWheelTrackWidth * tankScrubFactor);
	return forwardKinematics(leftWheelDelta, rightWheelDelta, deltaRotation);
}

Twist2D EctoKinematicsTankRebirth::forwardKinematics(double leftWheelDelta, double rightWheelDelta,
                                                     double deltaRotation) {
	const double dx = (leftWheelDelta + rightWheelDelta) / 2.0;
	return {dx, 0.0, deltaRotation};
}

Twist2D EctoKinematicsTankRebirth::forwardKinematics(const Rotation2D &prevHeading, const Rotation2D &currentHeading,
                                                     double leftWheelDelta, double rightWheelDelta) {
	const double dx = (leftWheelDelta + rightWheelDelta) / 2.0;
	return {dx, 0.0, prevHeading.inverse().rotateBy(currentHeading).getRadians()};
}

RobotPose2D EctoKinematicsTankRebirth::integrateForwardKinematics(const RobotPose2D &currentPose,
                                                                  const Twist2D &forwardKinematics) {
	return currentPose.transformBy(RobotPose2D::exp(forwardKinematics));
}