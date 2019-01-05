//
// Created by Abiel on 8/29/18.
//

#include "SplineUtils.h"
#include "Math/Splines/QuinticSpline.h"
#include "Math/DataTypes/Point2D.h"
#include "Math/DataTypes/Rotation2D.h"
#include "Math/DataTypes/RobotPose2D.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"
#include "Math/DataTypes/Twist2D.h"

std::vector<RobotPose2DCurvature> SplineUtils::parametrizeSpline(const QuinticSpline &spline, double t0, double t1,
                                                                 double maxDx, double maxDy, double maxDTheta) {
	std::vector<RobotPose2DCurvature> output;

	output.emplace_back(spline.getRobotPose2DCurvature(0.0));

	double dt = t1 - t0;

	for (double t = 0; t <= t1; t += dt / minSampleSize) {
		getSegmentArc(output, spline, t, t + dt / minSampleSize, maxDx, maxDy, maxDTheta);
	}

	return output;
}

std::vector<RobotPose2DCurvature>
SplineUtils::parametrizeSplines(const std::vector<QuinticSpline> &splines, double maxDx,
                                double maxDy, double maxDTheta) {
	std::vector<RobotPose2DCurvature> output;

	if (splines.empty())
		return output;

	output.emplace_back(splines.at(0).getRobotPose2DCurvature(0.0));

	for (const QuinticSpline &spline : splines) {
		std::vector<RobotPose2DCurvature> samples = parametrizeSpline(spline, 0.0, 1.0, maxDx, maxDy, maxDTheta);
		samples.erase(samples.begin());

		//Combine output and samples
		output.insert(
				output.end(),
				std::make_move_iterator(samples.begin()),
				std::make_move_iterator(samples.end())
		);
	}

	return output;
}

void SplineUtils::getSegmentArc(std::vector<RobotPose2DCurvature> &returnArc, const QuinticSpline &spline, double t0,
                                double t1, double maxDx, double maxDy, double maxDTheta) {

	Point2D p0 = spline.getPoint(t0);
	Point2D p1 = spline.getPoint(t1);

	Rotation2D r0 = spline.getHeading(t0);
	Rotation2D r1 = spline.getHeading(t1);

	RobotPose2D trans(Point2D(p0, p1).rotateBy(r0.inverse()), r1.rotateBy(r0.inverse()));
	Twist2D twist = RobotPose2D::log(trans);

	if (twist.getDy() > maxDy || twist.getDx() > maxDx || twist.getDtheta() > maxDTheta) {
		getSegmentArc(returnArc, spline, t0, (t0 + t1) / 2.0, maxDx, maxDy, maxDTheta);
		getSegmentArc(returnArc, spline, (t0 + t1) / 2.0, t1, maxDx, maxDy, maxDTheta);
	} else {
		returnArc.emplace_back(spline.getRobotPose2DCurvature(t1));
	}
}