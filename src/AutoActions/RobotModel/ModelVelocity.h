//
// Created by Abiel on 9/4/18.
//

#ifndef BOTBUSTERSREBIRTH_MODELVELOCITY_H
#define BOTBUSTERSREBIRTH_MODELVELOCITY_H

#include <memory>
#include "AutoActions/Action.h"
#include "RobotModel/VelocityModelData.h"
#include "Controllers/EctoTalon.h"
#include "Utilities/EctoTimer.h"

enum class ModelAction {

};

class ModelVelocity : public Action {
public:
	/**
	 * NOTE: Assumes maximum voltage = 12.0V
	 * Ramp rate is set value per run loop.
	 * waitTimeForMotor is the time to wait to log the velocity value
	 * @param leftSide
	 * @param rightSide
	 * @param minimumSet
	 * @param maximumSet
	 * @param rampRate
	 * @param waitTimeForMotor
	 */
	ModelVelocity(EctoTalon &leftSide, EctoTalon &rightSide, double minimumSet = 0.0, double maximumSet = 1.0,
	              double rampRate = 0.01, double waitTimeForMotor = 0.1);

	void init() override;

	void update() override;

	void finish() override;

	LinearRegression getLeftRegression() const;

	LinearRegression getRightRegression() const;

private:
	const double maximumVoltage = 12.0;

	EctoTalon &leftSide;
	EctoTalon &rightSide;

	EctoTimer &timer = EctoTimer::getInstance();

	double maximumSet, minimumSet, rampRate;

	double waitTimeForMotor;

	double currentSet;

	double timeToWait = 0;

	VelocityModelData leftData;
	VelocityModelData rightData;
};


#endif //BOTBUSTERSREBIRTH_MODELVELOCITY_H
