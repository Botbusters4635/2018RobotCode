//
// Created by Abiel on 9/12/18.
//

#ifndef BOTBUSTERSREBIRTH_CONSTRAINEDOBJECT_H
#define BOTBUSTERSREBIRTH_CONSTRAINEDOBJECT_H

#include "Utilities/CSVWritable.h"
#include <type_traits>

template<class T>
class ConstrainedObject : public CSVWritable {
public:
	ConstrainedObject<T>(const T &objectIn, double distance, double maxVelocity, double maxAcceleration,
	                     double minAcceleration);

	explicit ConstrainedObject<T>(const T &objectIn);

	ConstrainedObject<T>();

	T getObject() const;

	void setDistance(double distance);

	double getDistance() const;

	void setMaxVelocity(double maxVelocity);

	double getMaxVelocity() const;

	void setMaxAcceleration(double maxAcceleration);

	double getMaxAcceleration() const;

	void setMinAcceleration(double minAcceleration);

	double getMinAcceleration() const;

	std::string getCSVColumnNames() const override;

	std::string toCSV() const override;

private:
	T object;

	double distance;
	double maxVelocity;
	double maxAcceleration;
	double minAcceleration;
};


#endif //BOTBUSTERSREBIRTH_CONSTRAINEDOBJECT_H
