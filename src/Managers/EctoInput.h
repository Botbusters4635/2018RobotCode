/*
* Settings
* Group: Input
* StickPort: Stick Port (probably 0)
* Deadzone: Deadzone for Joystick
* ButtonName# - ButtonName#: Button Name
* AxisName# - AxisName#: Axis Name
*/

#ifndef BOTBUSTERSREBIRTH_ECTOINPUT_H
#define BOTBUSTERSREBIRTH_ECTOINPUT_H

#include "Subsystems/Input/EctoButton.h"
#include "Subsystems/Input/JoystickAxis.h"
#include "Utilities/EctoSettings.h"
#include <frc/WPILib.h>
#include <frc/DriverStation.h>
#include <frc/Joystick.h>
#include <map>
#include <utility>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>

/**
 * Class responsible for handling all inputs.
 * @return
 */
class EctoInput final {
public:
	static EctoInput &getInstance() {
		static EctoInput singletonInstance;
		return singletonInstance;
	}

	void addButton(EctoButton *button, const std::string &buttonName, int joystickId = 1);

	void addAxis(JoystickAxis *axis, const std::string &axisName, int joystickId = 1);

	int getPOV(int joyId = 1) const;

private:
	struct JoystickData {
		frc::Joystick *joystick;

		int joystickId;
		int wpiJoystickId;

		int axisCount;
		int buttonCount;

		//Stores names to ids
		std::map<std::string, int> buttonIds;
		std::map<std::string, int> axisIds;

		std::map<int, std::vector<EctoButton *>> buttonsToUpdate;
		std::map<int, std::vector<JoystickAxis *>> axesToUpdate;
	};

	bool getButtonState(int id, int joyId = 1) const;

	double getJoystickRaw(unsigned int joystickId, int joyId = 1) const;

	shared_ptr<spd::logger> log = spd::stdout_color_mt("EctoInput");
	EctoSettings &settings = EctoSettings::getInstance();

	EctoInput();

	EctoInput &operator=(const EctoInput &);

	void initializeJoysticks();

	void initializeButtons();

	void initializeAxis();

	const std::string baseNameStick = "StickPort";
	const std::string baseNameStickSelector = "Stick";
	const std::string baseNameAxis = "AxisName";
	const std::string baseNameButton = "ButtonName";

	int joystickCount;

	std::map<int, JoystickData> joysticks;

	mutable std::mutex ectoInputMutex;

	std::thread updateThread;

	void threadUpdateTask();
};

#endif