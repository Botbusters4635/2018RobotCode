//
// Created by Abiel on 12/11/18.
//

#ifndef BOTBUSTERSREBIRTH_ECTODAF_H
#define BOTBUSTERSREBIRTH_ECTODAF_H

#include "PathContainer.h"
#include "Subsystems/RobotData.h"
#include "AutoActions/Action.h"
#include <vector>
#include <chrono>

class EctoDAF : public Action {
public:
	EctoDAF(const PathContainer &inContainer, double lookaheadDistance, double endDistanceThreshold,
	        double minimumAngleThreshold, double maximumAngleThreshold);

	void init() override;

	void update() override;

	void finish() override;

private:
	RobotData &robotData = RobotData::getInstance();

	const PathContainer &container;
	double lookaheadDistance;

	double endDistanceThreshold;
	double minimumAngleThreshold;
	double maximumAngleThreshold;

	RobotPose2DCurvature currentPose;
	double previousAngularVelocity;

	std::vector<RobotPose2DCurvature>::const_iterator closestPose;
	std::vector<RobotPose2DCurvature>::const_iterator previousClosestPose;

	std::vector<RobotPose2DCurvature>::const_iterator currentTarget;
	std::vector<RobotPose2DCurvature>::const_iterator endPose;

	std::chrono::time_point<std::chrono::high_resolution_clock> lastLoopTime;

	std::vector<RobotPose2DCurvature>::const_iterator lookForNextPose(const RobotPose2DCurvature &currentPose,
	                                                                  const std::vector<RobotPose2DCurvature>::const_iterator &poseToStartLookingFrom) const;

	static double calculateHeightOfTriangleBetweenPoints(const RobotPose2DCurvature &startingPose,
	                                                     const RobotPose2DCurvature &intermediatePose,
	                                                     const RobotPose2DCurvature &endPose);

	std::pair<std::vector<RobotPose2DCurvature>::const_iterator, double>
	lookForTheTallestTriangle(const RobotPose2DCurvature &currentPose,
	                          const std::vector<RobotPose2DCurvature>::const_iterator &startPose,
	                          const std::vector<RobotPose2DCurvature>::const_iterator &endPose,
	                          const RobotPose2DCurvature &localGoal) const;

	static double calculateCirculeRadius(double triangleHeight, double lookaheadDistance);

	static Point2D
	calculateCircleCenter(const Point2D &currentPoint, const Point2D &triangleHeightPoint, const Point2D &localGoal);

	static double
	calculateCircleAngle(const Point2D &currentPoint, const Point2D &triangleHeightPoint, const Point2D &localGoal,
	                     double triangleHeight, double lookaheadDistance);

	RobotPose2DCurvature getCurrentPose() const;

	std::vector<RobotPose2DCurvature>::const_iterator getClosestPoint(const RobotPose2DCurvature &currentPoint,
	                                                                  const std::vector<RobotPose2DCurvature>::const_iterator &startPose,
	                                                                  const std::vector<RobotPose2DCurvature>::const_iterator &endPose) const;
};


#endif //BOTBUSTERSREBIRTH_ECTODAF_H
