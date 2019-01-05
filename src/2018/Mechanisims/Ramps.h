
#ifdef BOTBUSTERSREBIRTH_RAMP_H
#define BOTBUSTERSREBIRTH_RAMP_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/Manager/EctoInput.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "Subsystems/Manager/EctoPCM.h"
#include "Utilities/EctoSettings.h"
#include "Subsystems/EctoSubsystem.h"
#include <vector>
#include <string>

using namespace std;

class Ramps : public EctoSubsystem {
public:
	static Ramps &getInstance() {
		static Ramps singletonInstance;
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
	explicit Ramps();

	shared_ptr<spd::logger> log = spd::stdout_color_mt("Ramp");

	shared_ptr<EctoTalon> rampRMotor;

	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();
	EctoPCM &pcm = EctoPCM::getInstance();
	double output;
};

#endif
