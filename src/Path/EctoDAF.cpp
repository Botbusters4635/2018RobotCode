//
// Created by Abiel on 12/11/18.
//

#include "EctoDAF.h"

EctoDAF::EctoDAF(const PathContainer &inContainer, double lookaheadDistance, double endDistanceThreshold,
                 double minimumAngleThreshold, double maximumAngleThreshold) : container(inContainer) {
	this->lookaheadDistance = lookaheadDistance;
	this->endDistanceThreshold = endDistanceThreshold;
	this->minimumAngleThreshold = minimumAngleThreshold;
	this->maximumAngleThreshold = maximumAngleThreshold;
}

void EctoDAF::init() {
	if (container.generatedPoints.empty()) {
		throw std::logic_error("Tried to run an ungenerated path!");
	}

	if (RobotPose2D::getDistanceBetweenPoints(container.generatedPoints.front(), container.generatedPoints.back()) <=
	    lookaheadDistance) {
		throw std::logic_error("Distance between start point and end point is smaller than the lookahead distance!");
	}

	currentPose = getCurrentPose();
	previousAngularVelocity = 0.0;

	currentTarget = lookForNextPose(currentPose, container.generatedPoints.begin());

	closestPose = getClosestPoint(currentPose, container.generatedPoints.begin(), currentTarget);
	previousClosestPose = container.generatedPoints.begin();

	endPose = std::prev(container.generatedPoints.end());

	lastLoopTime = std::chrono::high_resolution_clock::now();

	robotData.addData("EctoDAFFinished", false);
}

void EctoDAF::update() {
	currentPose = getCurrentPose();

	//Update lookahead
	currentTarget = lookForNextPose(currentPose, currentTarget);

	//Update closest pose
	closestPose = getClosestPoint(currentPose, previousClosestPose, currentTarget);

	double angleBetweenPoints = 0.0;

	if (RobotPose2D::getDistanceBetweenPoints(currentPose, *endPose) < endDistanceThreshold) {
		robotData.addData("EctoDAFFinished", true);
		return;
	}

	//Check if there are any intermediate points or if the last point is within lookahead distance
	if (RobotPose2D::getDistanceBetweenPoints(currentPose, (*endPose)) <= lookaheadDistance or
	    std::next(closestPose) == endPose) {
		//Drive in straight line to end pose
		angleBetweenPoints = Point2D::getAngleFromPoints(currentPose.getPoint(), (*endPose).getPoint());

	} else {
		//Look for the largest triangle
		auto largestTriangle = lookForTheTallestTriangle(currentPose, closestPose, currentTarget, *currentTarget);

		//double circleRadius = calculateCirculeRadius(largestTriangle.second, lookaheadDistance);
		Point2D circleCenter = calculateCircleCenter(currentPose.getPoint(), (*largestTriangle.first).getPoint(),
		                                             (*currentTarget).getPoint());


		Point2D dPoint;

		dPoint.setX(circleCenter.getX() - currentPose.getX());
		dPoint.setY(circleCenter.getY() - currentPose.getY());

		angleBetweenPoints = Point2D::getAngleFromPoints(currentPose.getPoint(), dPoint);
	}

	//Checks angle lower angle threshold
	angleBetweenPoints = std::abs(angleBetweenPoints) < minimumAngleThreshold ? 0.0 : angleBetweenPoints;

	//Checks higher angle threshold
	angleBetweenPoints =
			std::abs(angleBetweenPoints) > maximumAngleThreshold ? maximumAngleThreshold : angleBetweenPoints;

	//timeStep in seconds
	double timeStep = (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - lastLoopTime)).count();

	double newAngularVelocity =
			previousAngularVelocity + (angleBetweenPoints - currentPose.getHeading().getRadians()) / timeStep;

	robotData.addData("EctoDAF_AngularVelocity", newAngularVelocity);

	previousAngularVelocity = newAngularVelocity;
	previousClosestPose = closestPose;

	lastLoopTime = std::chrono::high_resolution_clock::now();
}

void EctoDAF::finish() {
	;
}

std::vector<RobotPose2DCurvature>::const_iterator EctoDAF::lookForNextPose(const RobotPose2DCurvature &currentPose,
                                                                           const std::vector<RobotPose2DCurvature>::const_iterator &poseToStartLookingFrom) const {

	auto nextPose = poseToStartLookingFrom;

	while (nextPose < container.generatedPoints.end()) {
		double distanceFromCurrentPose = RobotPose2D::getDistanceBetweenPoints(currentPose, (*nextPose));

		if (distanceFromCurrentPose < lookaheadDistance) {
			nextPose++;
		} else {
			break;
		}
	}

	return std::prev(nextPose);
}

double EctoDAF::calculateHeightOfTriangleBetweenPoints(const RobotPose2DCurvature &startingPose,
                                                       const RobotPose2DCurvature &intermediatePose,
                                                       const RobotPose2DCurvature &endPose) {
	double sideA = Point2D::getDistance(startingPose.getPoint(), intermediatePose.getPoint());
	double sideB = Point2D::getDistance(intermediatePose.getPoint(), endPose.getPoint());
	double sideC = Point2D::getDistance(startingPose.getPoint(), endPose.getPoint());

	double ss = (sideA + sideB + sideC) / 2.0;

	double An = std::sqrt(ss * (ss - sideA) * (ss - sideB) * (ss - sideC));

	return (An * 2.0) / sideC;
}

std::pair<std::vector<RobotPose2DCurvature>::const_iterator, double>
EctoDAF::lookForTheTallestTriangle(const RobotPose2DCurvature &currentPose,
                                   const std::vector<RobotPose2DCurvature>::const_iterator &startPose,
                                   const std::vector<RobotPose2DCurvature>::const_iterator &endPose,
                                   const RobotPose2DCurvature &localGoal) const {
	if (startPose > endPose) {
		throw std::logic_error("Start point is bigger than the end point");
	}

	auto indexPose = startPose;

	//Largest point and height
	std::pair<std::vector<RobotPose2DCurvature>::const_iterator, double> largestPoint;

	while (indexPose <= endPose) {
		double height = calculateHeightOfTriangleBetweenPoints(currentPose, *indexPose, localGoal);

		if (largestPoint.second < height) {
			largestPoint.first = indexPose;
			largestPoint.second = height;
		}

		indexPose++;
	}

	return largestPoint;
}

double EctoDAF::calculateCirculeRadius(double triangleHeight, double lookaheadDistance) {
	return triangleHeight / 2.0 + std::pow(lookaheadDistance, 2.0) / 8.0 * triangleHeight;
}

Point2D EctoDAF::calculateCircleCenter(const Point2D &currentPoint, const Point2D &triangleHeightPoint,
                                       const Point2D &localGoal) {

	double ma =
			(triangleHeightPoint.getY() - currentPoint.getY()) / (triangleHeightPoint.getX() - currentPoint.getX());

	double mb =
			(localGoal.getY() - triangleHeightPoint.getY()) / (localGoal.getX() - triangleHeightPoint.getX());


	double firstComponent = ma * mb * (currentPoint.getY() - localGoal.getY());

	double secondComponent = mb * (currentPoint.getX() + triangleHeightPoint.getX());

	double thirdComponent = ma * (triangleHeightPoint.getX() + localGoal.getX());

	double fourthComponent = 2.0 * (mb - ma);

	double Cx = (firstComponent + secondComponent - thirdComponent) / fourthComponent;

	double firstYComponent = -1.0 * (1.0 / ma);
	double secondYComponent = Cx - (currentPoint.getX() + triangleHeightPoint.getX()) / 2.0;
	double thirdYComponent = (currentPoint.getY() + triangleHeightPoint.getY()) / 2.0;

	double Cy = firstYComponent * secondYComponent + thirdYComponent;

	return {Cx, Cy};
}

double
EctoDAF::calculateCircleAngle(const Point2D &currentPoint, const Point2D &triangleHeightPoint, const Point2D &localGoal,
                              double triangleHeight, double lookaheadDistance) {
	double radius = calculateCirculeRadius(triangleHeight, lookaheadDistance);

	Point2D circleCenter = calculateCircleCenter(currentPoint, triangleHeightPoint, localGoal);

	double drX = circleCenter.getX() - currentPoint.getX();
	double drY = circleCenter.getY() - currentPoint.getY();

	return (std::atan2(drY, drX) + M_PI * 0.5);
}

RobotPose2DCurvature EctoDAF::getCurrentPose() const {
	auto pos = robotData.getData<RobotPose2D>("TankStateLastPoint");
	return (RobotPose2DCurvature(pos, 0.0));
}

std::vector<RobotPose2DCurvature>::const_iterator EctoDAF::getClosestPoint(const RobotPose2DCurvature &currentPoint,
                                                                           const std::vector<RobotPose2DCurvature>::const_iterator &startPose,
                                                                           const std::vector<RobotPose2DCurvature>::const_iterator &endPose) const {
	if (startPose > endPose) {
		throw std::logic_error("Start point is bigger than the end point");
	}

	auto indexPose = startPose;

	//Largest point and distance
	std::pair<std::vector<RobotPose2DCurvature>::const_iterator, double> largestPoint;

	while (indexPose <= endPose) {
		double distance = RobotPose2D::getDistanceBetweenPoints(currentPoint, *indexPose);

		if (largestPoint.second < distance) {
			largestPoint.first = indexPose;
			largestPoint.second = distance;
		}

		indexPose++;
	}

	return largestPoint.first;
}