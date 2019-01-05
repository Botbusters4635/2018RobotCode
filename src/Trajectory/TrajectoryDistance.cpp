//
// Created by Abiel on 9/2/18.
//

#include "TrajectoryDistance.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"
#include "Math/MathUtil.h"

TrajectoryDistance::TrajectoryDistance(const std::vector<RobotPose2DCurvature> &trajectoryIn) {
	double prevDistance = 0.0;

	for (std::size_t i = 1; i < trajectoryIn.size(); ++i) {
		double distance =
				prevDistance + Point2D::getDistance(trajectoryIn.at(i - 1).getPoint(), trajectoryIn.at(i).getPoint());
		prevDistance = distance;

		this->trajectory.emplace_back(std::make_pair(trajectoryIn.at(i), distance));
	}
}

std::vector<pair<RobotPose2DCurvature, double>> TrajectoryDistance::getDistanceTrajectory() const {
	return trajectory;
}

RobotPose2DCurvature TrajectoryDistance::sample(double distance) const {
	if (distance >= trajectory.back().second)
		return trajectory.back().first;

	if (distance <= 0.0)
		return (*trajectory.begin()).first;

	for (std::size_t i = 1; i < trajectory.size(); ++i) {
		if (trajectory.at(i).second < distance)
			continue;

		if (MathUtil::almostEqual(trajectory.at(i).second, trajectory.at(i - 1).second)) {
			return trajectory.at(i).first;
		} else {
			double t =
					(distance - trajectory.at(i - 1).second) / (trajectory.at(i).second - trajectory.at(i - 1).second);

			double curvature = MathUtil::linearInterpolation(trajectory.at(i - 1).first.getCurvature(),
			                                                 trajectory.at(i).first.getCurvature(), t);
			double dcurvature = MathUtil::linearInterpolation(trajectory.at(i - 1).first.getDCurvature(),
			                                                  trajectory.at(i).first.getDCurvature(), t);

			double headingRadians = MathUtil::linearInterpolation(trajectory.at(i - 1).first.getHeading().getRadians(),
			                                                      trajectory.at(i).first.getHeading().getRadians(), t);
			Rotation2D heading = Rotation2D::fromRadians(headingRadians);

			double x = MathUtil::linearInterpolation(trajectory.at(i - 1).first.getX(), trajectory.at(i).first.getX(),
			                                         t);
			double y = MathUtil::linearInterpolation(trajectory.at(i - 1).first.getY(), trajectory.at(i).first.getY(),
			                                         t);

			return {RobotPose2D(x, y, heading), curvature, dcurvature};
		}
	}
}