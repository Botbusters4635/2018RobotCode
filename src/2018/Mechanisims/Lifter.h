//
// Created by Alberto on 2/18/18.
//

#ifndef BOTBUSTERSREBIRTH_LIFTER_H
#define BOTBUSTERSREBIRTH_LIFTER_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/EctoSubsystem.h"
#include "Managers/EctoMotors.h"
#include "Managers/EctoInput.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/EctoUtils.h"
#include "Subsystems/Input/EctoButton.h"

using namespace std;

class Lifter : public EctoSubsystem {
public:
	static Lifter &getInstance() {
		static Lifter singletonInstance;
		return singletonInstance;
	}

	void initTeleop() override;

	void updateTeleop() override;

	void initAuto() override;

	void updateAuto() override;

	void initDisabled() override;

	void updateDisabled() override;

	void sendInitialTelemetry() const override;

	void sendTelemetry() const override;

private:
	explicit Lifter();

	shared_ptr<spd::logger> log = spd::stdout_color_mt("Lifter");
	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();
	shared_ptr<EctoTalon> master, slave1, slave2, slave3, liftElevator;
	double upSpeed, downSpeed;

	EctoButton upButton;
};


#endif //BOTBUSTERSREBIRTH_LIFTER_H
