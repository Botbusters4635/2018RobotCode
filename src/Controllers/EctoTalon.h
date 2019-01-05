//
// Created by alberto on 13/12/17.
//

#ifndef BOTBUSTERSREBIRTH_ECTOTALON_H
#define BOTBUSTERSREBIRTH_ECTOTALON_H

#include "ctre/Phoenix.h"
#include "spdlog/spdlog.h"
#include <iostream>

#include "Utilities/EctoUtils.h"
#include "Utilities/MotorValues.h"

namespace spd = spdlog;

class EctoTalon : public TalonSRX {
public:
	explicit EctoTalon(int id);

	void Set(double requestedValue);

	int getMotorId();

	double getSetValue() const;

	void setFeedbackDevice(FeedbackDevice device, int deviceId = 0, int timeout = 10);

	void setControlMode(MotorMode mode);

	//DEPRECIATED, use MotorMode pls
	void setControlMode(ControlMode mode);

	ControlMode getCurrentControlMode() const;

	//This function should be const, but can't because CTR
	double getSpeedRPM();

	//DEPRECIAED, use getSpeedMSCircumference instead
	double getSpeedMS(double wheelDiameter);

	double getSpeedMSCircumference(double wheelCircumference);

	/**
	 * Should return wheel rotations
	 * @return
	 */
	double getPosition();

	void setBrake(bool brakeState);

	uint16_t getEncoderCodes() const;

	void setEncoderCodes(uint16_t codes);

	int getId();

private:
	static ControlMode convertMotorValueTypeToCTR(MotorMode typeIn);

	int motorId;

	const static shared_ptr<spd::logger> log;

	//Has to keep track of set value because apparently CTR doesn't let you check for it
	double currentSetValueEcto = 0;

	//ctre::phoenix::motorcontrol::ControlMode currentMode;
	ControlMode currentMode;

	uint16_t encoderCodes = 1;

};


#endif //BOTBUSTERSREBIRTH_ECTOTALON_H
