//
// Created by Abiel on 8/29/18.
//

#ifndef BOTBUSTERSREBIRTH_SPLINEUTILS_H
#define BOTBUSTERSREBIRTH_SPLINEUTILS_H

#include <vector>

class QuinticSpline;

class RobotPose2DCurvature;

namespace SplineUtils {
	//TODO Set default maximums
	const double maxDx = .08;
	const double maxDy = 0.001;
	const double maxDTheta = 0.1;
	const double minSampleSize = 1.0;

	std::vector<RobotPose2DCurvature> parametrizeSpline(const QuinticSpline &spline,
	                                                    double t0 = 0.0, double t1 = 1.0, double maxDx = maxDx,
	                                                    double maxDy = maxDy, double maxDTheta = maxDTheta);

	std::vector<RobotPose2DCurvature>
	parametrizeSplines(const std::vector<QuinticSpline> &splines, double maxDx = maxDx, double maxDy = maxDy,
	                   double maxDTheta = maxDTheta);

	void getSegmentArc(std::vector<RobotPose2DCurvature> &returnArc, const QuinticSpline &spline, double t0, double t1,
	                   double maxDx, double maxDy, double maxDTheta);
}

#endif //BOTBUSTERSREBIRTH_SPLINEUTILS_H
