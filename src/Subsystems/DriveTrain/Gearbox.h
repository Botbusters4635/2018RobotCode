//
// Created by Abiel on 8/26/18.
//

#ifndef BOTBUSTERSREBIRTH_GEARBOX_H
#define BOTBUSTERSREBIRTH_GEARBOX_H

#include <cmath>

enum GearboxState {
	lowGear = 0,
	highGear = 1
};

struct GearboxConfig {
	//The maximum speed of the gearbox (under load), measured in gearbox shaft revolutions per second
	//This value is ignored if hasShift == true
	double maximumSpeed = 100.0;

	bool hasShift = false;

	//The maximum speed for high and low gear (under load), measured in gearbox shaft revolutions per second
	double maximumLowGearSpeed = 100.0;
	double maximumHighGearSpeed = 400.0;

	GearboxState defaultState = GearboxState::lowGear;

	bool hasEncoder = true;

	//How many encoder ticks per encoder revolution
	unsigned int encoderTicksPerEncoderRev = 1440;

	//How many shaft rotations per encoder rotation
	unsigned int shaftRotationsPerEncoderTurn = 3;
};

class Gearbox {
public:
	explicit Gearbox(const GearboxConfig &config);

	void setConfig(const GearboxConfig &config);

	GearboxConfig getConfig() const;

	//Gets the current maximum speed
	double getCurrentMaximumSpeed() const;

	//Gets the maximum speed for the selected gearbox state
	double getMaximumSpeed(GearboxState state) const;

	bool hasShifts() const;

	bool hasEncoder() const;

	//Converts encoder ticks to shaft ticks
	inline unsigned int encoderTicksToShaftTicks(unsigned int encoderTicks) const;

	//Converts shaft ticks to encoder ticks
	inline unsigned int shaftTicksToEncoderTicks(unsigned int shaftTicks) const;

	//Converts encoder ticks into encoder rotations
	inline double encoderTicksToEncoderRotations(unsigned int encoderTicks) const;

	//Convert encoder rotations to encoder ticks
	inline unsigned int encoderRotationsToEncoderTicks(double encoderRotations) const;

	//Converts encoder ticks into shaft rotations
	inline double encoderTicksToShaftRotations(unsigned int encoderTicks) const;

	//Converts shaft rotations into encoder ticks
	inline unsigned int shaftRotationsToEncoderTicks(double shaftRotations) const;


private:
	GearboxConfig config;

	GearboxState currentState;

	unsigned int encoderTicksPerShaftRotation = 0;
};


#endif //BOTBUSTERSREBIRTH_GEARBOX_H
