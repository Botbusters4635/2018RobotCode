//
// Created by Abiel on 4/6/18.
//

#ifndef BOTBUSTERSREBIRTH_PATHUTIL_H
#define BOTBUSTERSREBIRTH_PATHUTIL_H

#include "Utilities/EctoCSV.h"
#include "Waypoint.h"
#include <string>
#include <vector>

using namespace std;

namespace PathUtil {
	constexpr static auto X_ColumnLabel = "X";
	constexpr static auto Y_ColumnLabel = "Y";
	constexpr static auto Time_ColumnLabel = "Time";

	constexpr static auto LeftDistance_ColumnLabel = "LeftDistance";
	constexpr static auto RightDistance_ColumnLabel = "RightDistance";

	//bool writeDistancePathtoEctoCSV(EctoCSV &csv, const DistancePath &distancePath, const string &pathName = "DistancePath");
	//DistancePath loadDistancePathFromEctoCSV(EctoCSV &csv, const string &pathName = "DistancePath");

	Waypoint interpolateBetweenWaypoints(const Waypoint &firstPoint, const Waypoint &lastPoint, double currentTime);

	//DistancePoint interpolateBetweenDistancePoints(const DistancePoint &firstPoint, const DistancePoint &lastPoint, double currentTime);
}

#endif //BOTBUSTERSREBIRTH_PATHUTIL_H
