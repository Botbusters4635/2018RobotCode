//
// Created by Abiel on 11/9/17.
//

#ifndef BOTBUSTERSREBIRTH_WAYPOINT_H
#define BOTBUSTERSREBIRTH_WAYPOINT_H

#include "Math/DataTypes/Point2D.h"

/**
 * Represents a coordinate in a trajectory
 */

class Waypoint : public Point2D {
public:
	Waypoint(double x, double y, double time);

	double getTime() const;

protected:
	double time;
};

#endif //BOTBUSTERSREBIRTH_WAYPOINT_H
