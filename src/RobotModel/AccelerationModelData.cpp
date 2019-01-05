//
// Created by Abiel on 9/5/18.
//

#include "AccelerationModelData.h"
#include "CharacterizationConstants.h"

AccelerationModelDataPoint::AccelerationModelDataPoint(double velocity, double acceleration) {
	this->velocity = velocity;
	this->acceleration = acceleration;
}

void AccelerationModelDataPoint::setVelocity(double velocity) {
	this->velocity = velocity;
}

void AccelerationModelDataPoint::setAcceleration(double acceleration) {
	this->acceleration = acceleration;
}

double AccelerationModelDataPoint::getVelocity() const {
	return velocity;
}

double AccelerationModelDataPoint::getAcceleration() const {
	return acceleration;
}

std::string AccelerationModelDataPoint::getCSVColumnNames() const {
	return "Velocity,Acceleration";
}

std::string AccelerationModelDataPoint::toCSV() const {
	std::stringstream stream;

	stream << getVelocity() << ',' << getAcceleration();

	return stream.str();
}

AccelerationModelDataPoint &AccelerationModelDataPoint::operator+=(const AccelerationModelDataPoint &rhs) {
	this->velocity += rhs.getVelocity();
	this->acceleration += rhs.getAcceleration();

	return *this;
}

const AccelerationModelDataPoint AccelerationModelDataPoint::operator+(const AccelerationModelDataPoint &rhs) const {
	return AccelerationModelDataPoint(*this) += rhs;
}

AccelerationModelDataPoint &AccelerationModelDataPoint::operator/=(const AccelerationModelDataPoint &rhs) {
	this->velocity /= rhs.getVelocity();
	this->acceleration /= rhs.getVelocity();

	return *this;
}

const AccelerationModelDataPoint AccelerationModelDataPoint::operator/(const AccelerationModelDataPoint &rhs) const {
	return AccelerationModelDataPoint(*this) /= rhs;
}

AccelerationModelDataPoint &AccelerationModelDataPoint::operator/=(double rhs) {
	this->velocity /= rhs;
	this->acceleration /= rhs;

	return *this;
}

const AccelerationModelDataPoint AccelerationModelDataPoint::operator/(double rhs) const {
	return AccelerationModelDataPoint(*this) /= rhs;
}

void AccelerationModelData::addData(double voltage, const AccelerationModelDataPoint &dataPoint) {
	auto previousPoint = data.find(voltage);

	if (previousPoint == data.end())
		data.emplace(std::make_pair(voltage, dataPoint));
	else {
		previousPoint->second = ((*previousPoint).second + dataPoint) / 2.0;
	}
}

AccelerationModelDataPoint AccelerationModelData::getData(double voltage) const {
	const auto point = data.find(voltage);

	//Returns empty data point if the key was not found
	if (point == data.end())
		return {0, 0};

	return (*point).second;
}

std::map<double, AccelerationModelDataPoint> AccelerationModelData::getAllData() const {
	return data;
}

void AccelerationModelData::clearData() {
	data.clear();
}

LinearRegression AccelerationModelData::getDataModel(const CharacterizationConstants &voltageCharacterization) const {
	std::vector<std::pair<double, double>> linearRegPoints;

	for (const auto &accelMapPoint : data) {
		const AccelerationModelDataPoint &accelPoint(accelMapPoint.second);
		const double &voltage(accelMapPoint.first);

		std::pair<double, double> dataPoint;

		dataPoint.first = accelPoint.getAcceleration();
		dataPoint.second =
				voltage - (voltageCharacterization.Kv * accelPoint.getVelocity()) - voltageCharacterization.Ks;

		linearRegPoints.emplace_back(dataPoint);
	}

	return LinearRegression(linearRegPoints);
}

CharacterizationConstants AccelerationModelData::characterize(
		const CharacterizationConstants &voltageCharacterization) const {
	LinearRegression accelerationModel = getDataModel(voltageCharacterization);

	CharacterizationConstants output = voltageCharacterization;

	output.Ka = accelerationModel.getSlope();

	return output;
}

std::string AccelerationModelData::getCSVColumnNames() const {
	return "Voltage,Acceleration,Velocity";
}

std::string AccelerationModelData::toCSV() const {
	std::stringstream stream;

	for (const auto &dataPoint : data) {
		stream << dataPoint.first << ',' << dataPoint.second.getAcceleration() << ',' << dataPoint.second.getVelocity()
		       << '\n';
	}

	return stream.str();
}