//
// Created by Abiel on 9/5/18.
//

#ifndef BOTBUSTERSREBIRTH_ACCELERATIONMODELDATA_H
#define BOTBUSTERSREBIRTH_ACCELERATIONMODELDATA_H

#include <map>
#include <string>
#include "Utilities/CSVWritable.h"
#include "Math/LinearRegression.h"

class AccelerationModelDataPoint : public CSVWritable {
public:
	AccelerationModelDataPoint(double velocity, double acceleration);

	void setVelocity(double velocity);

	void setAcceleration(double acceleration);

	double getVelocity() const;

	double getAcceleration() const;

	std::string getCSVColumnNames() const override;

	std::string toCSV() const override;

	//Math operators to be able to calculate averages easily
	AccelerationModelDataPoint &operator+=(const AccelerationModelDataPoint &rhs);

	const AccelerationModelDataPoint operator+(const AccelerationModelDataPoint &rhs) const;

	AccelerationModelDataPoint &operator/=(const AccelerationModelDataPoint &rhs);

	const AccelerationModelDataPoint operator/(const AccelerationModelDataPoint &rhs) const;

	AccelerationModelDataPoint &operator/=(double rhs);

	const AccelerationModelDataPoint operator/(double rhs) const;

private:
	double velocity, acceleration;
};

class CharacterizationConstants;

class AccelerationModelData : public CSVWritable {
public:
	void addData(double voltage, const AccelerationModelDataPoint &dataPoint);

	AccelerationModelDataPoint getData(double voltage) const;

	std::map<double, AccelerationModelDataPoint> getAllData() const;

	void clearData();

	LinearRegression getDataModel(const CharacterizationConstants &voltageCharacterization) const;

	CharacterizationConstants characterize(const CharacterizationConstants &voltageCharacterization) const;

	std::string getCSVColumnNames() const override;

	std::string toCSV() const override;

private:
	std::map<double, AccelerationModelDataPoint> data;
};


#endif //BOTBUSTERSREBIRTH_ACCELERATIONMODELDATA_H
