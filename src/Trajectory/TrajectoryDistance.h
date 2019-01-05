//
// Created by Abiel on 9/2/18.
//

#ifndef BOTBUSTERSREBIRTH_TRAJECTORYDISTANCE_H
#define BOTBUSTERSREBIRTH_TRAJECTORYDISTANCE_H

#include <vector>

class RobotPose2DCurvature;

using std::pair;

/**
 * Stores a trajectory and the distance at each point
 */
class TrajectoryDistance {
public:
	explicit TrajectoryDistance(const std::vector<RobotPose2DCurvature> &trajectoryIn);

	std::vector<pair<RobotPose2DCurvature, double>> getDistanceTrajectory() const;

	//Gets the point closest to the given distance
	RobotPose2DCurvature sample(double distance) const;

private:
	//Stores the point and the distance from that point to the first point
	std::vector<pair<RobotPose2DCurvature, double>> trajectory;
};


#endif //BOTBUSTERSREBIRTH_TRAJECTORYDISTANCE_H
