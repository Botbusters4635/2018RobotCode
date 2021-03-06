//
// Created by Abiel on 8/21/18.
//

#ifndef BOTBUSTERSREBIRTH_TWIST2D_H
#define BOTBUSTERSREBIRTH_TWIST2D_H

#include <cmath>
#include "Utilities/CSVWritable.h"

class Point2D;

/**
 * A movement along an arc with a constant curvature and velocity
 */
class Twist2D : public CSVWritable {
public:
	Twist2D(double dx, double dy, double dtheta);

	Twist2D scaled(double scale) const;

	double norm() const;

	double curvature() const;

	void setDx(double dx);

	void setDy(double dy);

	void setDtheta(double dtheta);

	double getDx() const;

	double getDy() const;

	double getDtheta() const;

	std::string getCSVColumnNames() const override;

	std::string toCSV() const override;

private:
	double dx, dy;
	double dtheta;
};


#endif //BOTBUSTERSREBIRTH_TWIST2D_H
