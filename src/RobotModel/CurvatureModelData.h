//
// Created by Abiel on 9/7/18.
//

#ifndef BOTBUSTERSREBIRTH_CURVATUREMODELDATA_H
#define BOTBUSTERSREBIRTH_CURVATUREMODELDATA_H

#include <utility>
#include <map>
#include "Utilities/CSVWritable.h"
#include "Math/LinearRegression.h"

class CurvatureModelDataPoint : public CSVWritable {
public:
	CurvatureModelDataPoint(double linearVelocity, double angularVelocity, double leftVoltage, double rightVoltage);

	CurvatureModelDataPoint();

	void setLinearVelocity(double linearVelocity);

	double getLinearVelocity() const;

	void setAngularVelocity(double angularVelocity);

	double getAngularVelcoty() const;

	void setLeftVoltage(double leftVoltage);

	double getLeftVoltage() const;

	void setRightVoltgae(double rightVoltage);

	double getRightVoltage() const;

	void setVoltages(std::pair<double, double> voltages);

	void setVoltages(double leftVoltage, double rightVoltage);

	std::pair<double, double> getVoltages() const;

private:
	double linearVelocity;
	double angularVelocity;
	double leftVoltage;
	double rightVoltage;
};

class CurvatureModelData : public CSVWritable {
public:
	/*
	void addData(double voltage, const CurvatureModelDataPoint &dataPoint);
	CurvatureModelDataPoint getData(double voltage) const;

	std::map<double, CurvatureModelDataPoint> getAllData() const;
	void clearData();
	*/
private:
	std::map<double, CurvatureModelDataPoint> data;
};


#endif //BOTBUSTERSREBIRTH_CURVATUREMODELDATA_H
