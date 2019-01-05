//
// Created by Abiel on 9/12/18.
//

#include "ConstrainedObject.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"

template
class ConstrainedObject<RobotPose2DCurvature>;

template<class T>
ConstrainedObject<T>::ConstrainedObject(const T &objectIn, double distance, double maxVelocity, double maxAcceleration,
                                        double minAcceleration) {
	this->object = objectIn;

	this->distance = distance;
	this->maxVelocity = maxVelocity;

	this->maxAcceleration = maxAcceleration;
	this->minAcceleration = minAcceleration;
}

template<class T>
ConstrainedObject<T>::ConstrainedObject(const T &objectIn) : ConstrainedObject<T>(objectIn, 0.0, 0.0, 0.0, 0.0) {
	;
}

template<class T>
ConstrainedObject<T>::ConstrainedObject() : ConstrainedObject<T>(T()) {
	;
}

template<class T>
T ConstrainedObject<T>::getObject() const {
	return object;
}

template<class T>
void ConstrainedObject<T>::setDistance(double distance) {
	this->distance = distance;
}

template<class T>
double ConstrainedObject<T>::getDistance() const {
	return distance;
}

template<class T>
void ConstrainedObject<T>::setMaxVelocity(double maxVelocity) {
	this->maxVelocity = maxVelocity;
}

template<class T>
double ConstrainedObject<T>::getMaxVelocity() const {
	return maxVelocity;
}

template<class T>
void ConstrainedObject<T>::setMaxAcceleration(double maxAcceleration) {
	this->maxAcceleration = maxAcceleration;
}

template<class T>
double ConstrainedObject<T>::getMaxAcceleration() const {
	return maxAcceleration;
}

template<class T>
void ConstrainedObject<T>::setMinAcceleration(double minAcceleration) {
	this->minAcceleration = minAcceleration;
}

template<class T>
double ConstrainedObject<T>::getMinAcceleration() const {
	return minAcceleration;
}

template<class T>
std::string ConstrainedObject<T>::getCSVColumnNames() const {
	std::stringstream stream;

	stream << "Distance,MaxVelocity,MaxAcceleration,MinAcceleratoin";

	if (std::is_base_of<CSVWritable, T>::value)
		stream << ',' << object.getCSVColumnNames();

	return stream.str();
}

template<class T>
std::string ConstrainedObject<T>::toCSV() const {
	std::stringstream stream;

	stream << distance << ',' << maxVelocity << ',' << maxAcceleration << ',' << minAcceleration;

	if (std::is_base_of<CSVWritable, T>::value)
		stream << ',' << object.toCSV();

	stream << '\n';

	return stream.str();
}