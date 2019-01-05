//
// Created by Abiel on 8/21/18.
//

#include "Twist2D.h"

Twist2D::Twist2D(double dx, double dy, double dtheta) {
	this->dx = dx;
	this->dy = dy;
	this->dtheta = dtheta;
}


void Twist2D::setDx(double dx) {
	this->dx = dx;
}

void Twist2D::setDy(double dy) {
	this->dy = dy;
}

void Twist2D::setDtheta(double dtheta) {
	this->dtheta = dtheta;
}

double Twist2D::getDx() const {
	return dx;
}

double Twist2D::getDy() const {
	return dy;
}

double Twist2D::getDtheta() const {
	return dtheta;
}

Twist2D Twist2D::scaled(double scale) const {
	return {dx * scale, dy * scale, dtheta * scale};
}

double Twist2D::norm() const {
	if (dy == 0.0)
		return std::abs(dx);

	return hypot(dx, dy);
}

double Twist2D::curvature() const {
	if (std::abs(dtheta) < 0 && norm() < 0)
		return 0.0;

	return dtheta / norm();
}

std::string Twist2D::getCSVColumnNames() const {
	return std::string("dX,dY,dTheta");
}

std::string Twist2D::toCSV() const {
	std::stringstream stream;

	stream << dx << ',' << dy << ',' << dtheta << '\n';

	return stream.str();
}