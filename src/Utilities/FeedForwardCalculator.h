//
// Created by abiel on 11/29/17.
//

#ifndef BOTBUSTERSREBIRTH_FEEDFORWARDCALCULATOR_H
#define BOTBUSTERSREBIRTH_FEEDFORWARDCALCULATOR_H

#include <frc/Timer.h>
#include "Controllers/EctoTalon.h"
#include "Managers/EctoMotors.h"
#include "EctoUtils.h"
#include <string>
#include "spdlog/spdlog.h"

#include "ctre/phoenix/motorcontrol/ControlMode.h"

using namespace std;
namespace spd = spdlog;

using ctre::phoenix::motorcontrol::ControlMode;

class FeedForwardCalculator final {
public:

	static FeedForwardCalculator &getInstance() {
		static FeedForwardCalculator singletonInstance;
		return singletonInstance;
	}

	void setMotor(const std::string &motorName, uint16_t codesPerRev = 480);

	bool periodic();

	double getMaxSpeedRPM();

	double getMaxNativeSpeed();

	double getFgain();

	void printValues();

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("FeedForwardCalculator");

	FeedForwardCalculator();

	FeedForwardCalculator &operator=(const FeedForwardCalculator &);

	EctoMotors motors = EctoMotors::getInstance();

	ControlMode previousControlMode;

	void setMaxSpeed();

	double getMaxSpeed();

	double calculateFgain();

	void resetTalon();

	string motorName;

	shared_ptr<EctoTalon> motor;

	uint16_t codesPerRev = 0;

	double maxSpeed;

	double fGain;

	shared_ptr<frc::Timer> timer;

	bool running = false;
	bool finished = false;
	bool started = false;

	const double maxTalonSpeed = 1;
	const double timeToWaitToAccelerate = 5.0;
	const double talonMaxRequestedSpeed = 1023;
};

#endif //BOTBUSTERSREBIRTH_FEEDFORWARDCALCULATOR_H
