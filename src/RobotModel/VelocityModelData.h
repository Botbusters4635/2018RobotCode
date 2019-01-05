//
// Created by Abiel on 9/4/18.
//

#ifndef BOTBUSTERSREBIRTH_ROBOTMODELDATA_H
#define BOTBUSTERSREBIRTH_ROBOTMODELDATA_H

#include <vector>
#include <map>
#include <string>
#include "Utilities/CSVWritable.h"
#include "Math/LinearRegression.h"

class VelocityModelData : public CSVWritable {
public:
	//Adds data to the class
	void addData(const std::pair<double, double> &inData);

	//Gets the given data at voltage from the class
	double getData(double voltage) const;

	//Gets all data
	std::map<double, double> getAllData() const;

	LinearRegression getDataModel() const;

	void clearData();

	std::string getCSVColumnNames() const override;

	std::string toCSV() const override;

private:
	static constexpr auto dataLabel = "VelocityModelData";

	//Voltage to speed
	std::map<double, double> data;
};

#endif //BOTBUSTERSREBIRTH_ROBOTMODELDATA_H
