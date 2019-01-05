#ifndef PathContainer_H_
#define PathContainer_H_

#include <string>

#include "Math/Splines/QuinticSpline.h"
#include "Math/DataTypes/RobotPose2D.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"

//Data type used to store all the info in a path
struct PathContainer {
	std::string pathName = "Path";

	double stepSize = 0.001;

	std::vector<RobotPose2D> waypoints;
	std::vector<QuinticSpline> splines;

	std::vector<RobotPose2DCurvature> generatedPoints;
};

void from_json(const json &j, PathContainer &container);

void to_json(json &j, const PathContainer &container);

#endif