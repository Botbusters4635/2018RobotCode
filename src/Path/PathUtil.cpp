//
// Created by Abiel on 4/6/18.
//

#include "PathUtil.h"
//g

//DistancePath PathUtil::loadDistancePathFromEctoCSV(EctoCSV &csv, const string &pathName) {
//	if(!csv.checkIfTableExists(pathName))
//		throw runtime_error("Tried to read from a table that does't exist " + pathName);
//
//	DistancePath path;
//
//	csv.setTable(pathName);
//
//	size_t maxLength = csv.getMaxColumnLengthInTable(pathName);
//
//	for(size_t i = 0; i < maxLength; ++i){
//		double leftDistance = csv.getElement(LeftDistance_ColumnLabel, i);
//		double rightDistance = csv.getElement(RightDistance_ColumnLabel, i);
//
//		double time = csv.getElement(Time_ColumnLabel, i);
//
//		path.addPoint(DistancePoint(leftDistance, rightDistance, time));
//	}
//
//	return path;
//}

Waypoint PathUtil::interpolateBetweenWaypoints(const Waypoint &firstPoint, const Waypoint &lastPoint,
                                               double currentTime) {
	if (currentTime > lastPoint.getTime())
		throw runtime_error(
				"Tried to interpolate between two waypoints when current time is more than last recorded time");

	double differenceInTime = lastPoint.getTime() - firstPoint.getTime();

	double deltaX = firstPoint.getX() - lastPoint.getX();
	double deltaY = firstPoint.getY() - lastPoint.getY();

	double distanceBetweenPoints = sqrt(pow(deltaX, 2.0) + pow(deltaY, 2.0));
	double angleOfLine = atan2(deltaY, deltaX);

	double newDistance = (distanceBetweenPoints / differenceInTime) * (lastPoint.getTime() - currentTime);

	return {newDistance * cos(angleOfLine), newDistance * sin(angleOfLine), currentTime};
}

//DistancePoint PathUtil::interpolateBetweenDistancePoints(const DistancePoint &firstPoint,
//                                                         const DistancePoint &lastPoint, double currentTime) {
//	double differenceInTime = lastPoint.getTime() - firstPoint.getTime();
//
//	double newLeftDistance = firstPoint.getLeftDistance() + (currentTime - firstPoint.getTime()) * (lastPoint.getLeftDistance() - firstPoint.getLeftDistance()) / differenceInTime;
//	double newRightDistance = firstPoint.getRightDistance() + (currentTime - firstPoint.getTime()) * (lastPoint.getRightDistance() - firstPoint.getRightDistance()) / differenceInTime;
//
//	return DistancePoint(newLeftDistance, newRightDistance, currentTime);
//
//}