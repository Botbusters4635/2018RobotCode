//
// Created by Abiel on 9/2/18.
//

#include "TimedObject.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"

template
class TimedObject<RobotPose2DCurvature>;

template<class T>
TimedObject<T>::TimedObject(const T &inObject, double time, double velocity, double acceleration) : object(inObject) {
	this->time = time;
	this->velocity = velocity;
	this->acceleration = acceleration;
}

template<class T>
TimedObject<T>::TimedObject(const T &inObject) : TimedObject(inObject, 0.0, 0.0, 0.0) {
	;
}

template<class T>
T TimedObject<T>::getObject() const {
	return object;
}

template<class T>
void TimedObject<T>::setTime(double time) {
	this->time = time;
}

template<class T>
double TimedObject<T>::getTime() const {
	return this->time;
}

template<class T>
void TimedObject<T>::setVelocity(double velocity) {
	this->velocity = velocity;
}

template<class T>
double TimedObject<T>::getVelocity() const {
	return this->velocity;
}

template<class T>
void TimedObject<T>::setAcceleration(double acceleration) {
	this->acceleration = acceleration;
}

template<class T>
double TimedObject<T>::getAcceleration() const {
	return this->acceleration;
}

template<class T>
std::string TimedObject<T>::getCSVColumnNames() const {
	std::stringstream stream;

	stream << "Time,Velocity,Acceleration";

	if (std::is_base_of<CSVWritable, T>::value)
		stream << ',' << object.getCSVColumnNames();

	return stream.str();
}

template<class T>
std::string TimedObject<T>::toCSV() const {
	std::stringstream stream;

	stream << time << ',' << velocity << ',' << acceleration;

	if (std::is_base_of<CSVWritable, T>::value)
		stream << ',' << object.toCSV();

	stream << '\n';

	return stream.str();
}