//
// Created by Abiel on 9/5/18.
//

#ifndef BOTBUSTERSREBIRTH_MODELACCELERATION_H
#define BOTBUSTERSREBIRTH_MODELACCELERATION_H

#include "AutoActions/Action.h"
#include "RobotModel/AccelerationModelData.h"
#include "RobotModel/CharacterizationConstants.h"
#include "Utilities/EctoTimer.h"
#include "Controllers/EctoTalon.h"
#include "Math/MathUtil.h"

class ModelAcceleration : Action {
public:
	ModelAcceleration(EctoTalon &leftSideIn, EctoTalon &rightSideIn,
	                  const CharacterizationConstants &voltageConstantsIn);

	void init() override;

	void update() override;

	void finish() override;

private:
	static constexpr double maximumVoltage = 12.0;
	static constexpr double maximumPower = 0.5;

	static constexpr double setVoltage = maximumVoltage * maximumPower;

	const double timeToTest = 3.0;

	double startTime = 0.0;

	double previousTime = 0.0;

	double previousLeftVelocity = 0.0;
	double previousRightVelocity = 0.0;

	EctoTimer &timer = EctoTimer::getInstance();

	EctoTalon &leftSide, &rightSide;

	CharacterizationConstants const &voltageConstants;

	AccelerationModelData leftData;
	AccelerationModelData rightData;
};


#endif //BOTBUSTERSREBIRTH_MODELACCELERATION_H
