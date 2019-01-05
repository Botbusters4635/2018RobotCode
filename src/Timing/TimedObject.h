//
// Created by Abiel on 9/2/18.
//

#ifndef BOTBUSTERSREBIRTH_TIMEDOBJECT_H
#define BOTBUSTERSREBIRTH_TIMEDOBJECT_H

#include "Utilities/CSVWritable.h"
#include <type_traits>

template<class T>
class TimedObject : CSVWritable {
public:
	TimedObject<T>(const T &inObject, double time, double velocity, double acceleration);

	explicit TimedObject<T>(const T &inObject);

	T getObject() const;

	void setTime(double time);

	double getTime() const;

	void setVelocity(double velocity);

	double getVelocity() const;

	void setAcceleration(double acceleration);

	double getAcceleration() const;

	std::string getCSVColumnNames() const override;

	std::string toCSV() const override;

private:
	T object;

	double time;
	double velocity;
	double acceleration;
};


#endif //BOTBUSTERSREBIRTH_TIMEDOBJECT_H
