//
// Created by Abiel on 9/4/18.
//

#include "VelocityModelData.h"

void VelocityModelData::addData(const std::pair<double, double> &inData) {
	if (data.find(inData.first) == data.end())
		data.emplace(inData);
	else {
		double average = (data.at(inData.first) + inData.second) / 2.0;

		data.emplace(std::make_pair(inData.first, average));
	}
}

double VelocityModelData::getData(double voltage) const {
	return data.at(voltage);
}

std::map<double, double> VelocityModelData::getAllData() const {
	return this->data;
}

void VelocityModelData::clearData() {
	data.clear();
}

std::string VelocityModelData::getCSVColumnNames() const {
	std::stringstream stream;

	stream << "Voltage," << dataLabel;

	return stream.str();
}

std::string VelocityModelData::toCSV() const {
	std::stringstream stream;

	for (const std::pair<double, double> dataValue : data) {
		stream << dataValue.first << ',' << dataValue.second << std::endl;
	}

	return stream.str();
}

LinearRegression VelocityModelData::getDataModel() const {
	std::vector<std::pair<double, double>> temporaryData;

	for (const auto &dataValue : this->data) {
		temporaryData.emplace_back(std::make_pair(dataValue.first, dataValue.second));
	}

	return LinearRegression(temporaryData);
}