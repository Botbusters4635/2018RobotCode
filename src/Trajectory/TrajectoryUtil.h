//
// Created by Abiel on 9/2/18.
//

#ifndef BOTBUSTERSREBIRTH_TRAJECTORYUTIL_H
#define BOTBUSTERSREBIRTH_TRAJECTORYUTIL_H

#include <vector>
#include "Timing/TimedObject.h"

class RobotPose2D;

class RobotPose2DCurvature;

class Spline2D;

namespace TrajectoryUtil {
	std::vector<RobotPose2DCurvature>
	trajectoryFromWaypoints(const std::vector<RobotPose2D> &waypoints, double maxDx, double maxDy, double maxDTheta);

	std::vector<TimedObject<RobotPose2DCurvature>>
	generatePath(const std::vector<RobotPose2DCurvature> &trajectory, double startVelocity, double endVelocity,
	             double maxVelocity, double maxAcceleration, bool reverse);
}


#endif //BOTBUSTERSREBIRTH_TRAJECTORYUTIL_H
