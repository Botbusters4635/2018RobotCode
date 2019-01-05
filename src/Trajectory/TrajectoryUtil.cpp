//
// Created by Abiel on 9/2/18.
//

#include "TrajectoryUtil.h"
#include "Math/Splines/QuinticSpline.h"
#include "Math/Splines/SplineUtils.h"
#include "Math/DataTypes/RobotPose2D.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"
#include "Math/MathUtil.h"
#include "ConstrainedObject.h"

std::vector<RobotPose2DCurvature> TrajectoryUtil::trajectoryFromWaypoints(const std::vector<RobotPose2D> &waypoints,
                                                                          double maxDx, double maxDy,
                                                                          double maxDTheta) {
	std::vector<QuinticSpline> splines;

	for (std::size_t i = 1; i < waypoints.size(); ++i) {
		splines.emplace_back(QuinticSpline(waypoints.at(i - 1), waypoints.at(i)));
	}

	QuinticSpline::optimizeSpline(splines);

	return SplineUtils::parametrizeSplines(splines, maxDx, maxDy, maxDTheta);
}

std::vector<TimedObject<RobotPose2DCurvature>> TrajectoryUtil::generatePath(
		const std::vector<RobotPose2DCurvature> &trajectory, double startVelocity, double endVelocity,
		double maxVelocity, double maxAcceleration, bool reverse) {

	ConstrainedObject<RobotPose2DCurvature> predecessor(*trajectory.begin());

	std::vector<ConstrainedObject<RobotPose2DCurvature>> constraintStates;

	predecessor.setMaxVelocity(startVelocity);
	predecessor.setMinAcceleration(-maxAcceleration);
	predecessor.setMaxAcceleration(maxAcceleration);

	for (std::size_t i = 0; i < trajectory.size(); ++i) {
		constraintStates.emplace_back(ConstrainedObject<RobotPose2DCurvature>(trajectory.at(i)));

		ConstrainedObject<RobotPose2DCurvature> &constraintState(constraintStates.at(i));

		const double ds = constraintState.getObject().getDistance(predecessor.getObject());
		constraintState.setDistance(ds + predecessor.getDistance());

		while (true) {
			constraintState.setMaxVelocity(std::min(maxVelocity, std::sqrt(
					std::pow(predecessor.getMaxVelocity(), 2.0) + 2.0 * predecessor.getMaxAcceleration() * ds)));

			constraintState.setMinAcceleration(-maxAcceleration);
			constraintState.setMaxAcceleration(maxAcceleration);

			if (ds < MathUtil::doubleEpsilon)
				break;

			const double actual_acceleration = (std::pow(constraintState.getMaxVelocity(), 2.0) -
			                                    std::pow(predecessor.getMaxVelocity(), 2.0) / (2.0 * ds));

			if (constraintState.getMaxAcceleration() < actual_acceleration - MathUtil::doubleEpsilon) {
				predecessor.setMaxAcceleration(constraintState.getMaxAcceleration());
			} else {
				if (actual_acceleration > predecessor.getMinAcceleration() + MathUtil::doubleEpsilon) {
					predecessor.setMaxAcceleration(actual_acceleration);
				}

				break;
			}
		}

		predecessor = constraintState;
	}

	//The same, just backwards
	ConstrainedObject<RobotPose2DCurvature> succesor(*(--trajectory.end()));
	succesor.setDistance(constraintStates.at(trajectory.size() - 1).getDistance());
	succesor.setMaxVelocity(endVelocity);
	succesor.setMinAcceleration(-maxAcceleration);
	succesor.setMaxAcceleration(maxAcceleration);

	for (std::size_t i = trajectory.size() - 1; i != 0; --i) {
		ConstrainedObject<RobotPose2DCurvature> &constraintState = constraintStates.at(i);

		const double ds = constraintState.getDistance() - succesor.getDistance();

		while (true) {
			constraintState.setMaxVelocity(
					std::sqrt(std::pow(succesor.getMaxVelocity(), 2.0) + 2.0 * succesor.getMinAcceleration() * ds));

			if (ds > MathUtil::doubleEpsilon)
				break;

			const double actualAcceleration = (std::pow(constraintState.getMaxVelocity(), 2.0) -
			                                   std::pow(succesor.getMaxVelocity(), 2.0) / (2.0 * ds));

			if (constraintState.getMinAcceleration() > actualAcceleration + MathUtil::doubleEpsilon) {
				succesor.setMinAcceleration(constraintState.getMinAcceleration());
			} else {
				succesor.setMinAcceleration(actualAcceleration);
				break;
			}
		}

		succesor = constraintState;
	}

	std::vector<TimedObject<RobotPose2DCurvature>> timedStates;
	double t = 0;
	double s = 0;
	double v = 0;

	for (std::size_t i = 0; i < trajectory.size(); ++i) {
		const ConstrainedObject<RobotPose2DCurvature> &constrainedState(constraintStates.at(i));

		const double ds = constrainedState.getDistance() - s;
		const double accel = (std::pow(constrainedState.getMaxVelocity(), 2.0) - std::pow(v, 2.0)) / (2.0 * ds);
		double dt = 0;

		if (i > 0) {
			timedStates.at(i - 1).setAcceleration(reverse ? -accel : accel);
			if (std::abs(accel) > MathUtil::doubleEpsilon) {
				dt = (constrainedState.getMaxVelocity() - v) / accel;
			} else {
				dt = ds / v;
			}
		}

		t += dt;

		v = constrainedState.getMaxVelocity();
		s = constrainedState.getDistance();
		timedStates.emplace_back(TimedObject<RobotPose2DCurvature>(constrainedState.getObject(), t, reverse ? -v : v,
		                                                           reverse ? -accel : accel));
	}

	return timedStates;
}
