#include "EctoInput.h"
//TODO Re-implement input checking

EctoInput::EctoInput() {
	log->info("Initializing EctoInput...");
	settings.setCurrentGroup("Input");

	joystickCount = settings.getNumber<int>("StickCount");

	initializeJoysticks();
	initializeAxis();
	initializeButtons();

	updateThread = std::thread(&EctoInput::threadUpdateTask, this);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void EctoInput::threadUpdateTask() {
	frc::DriverStation &driverStation = frc::DriverStation::GetInstance();

	while (true) {
		driverStation.WaitForData(0);

		ectoInputMutex.lock();

		for (std::pair<int, JoystickData> data : joysticks) {
			JoystickData &joyData = data.second;

			for (std::pair<const int, std::vector<EctoButton *>> &buttonData : joyData.buttonsToUpdate) {
				bool buttonState = getButtonState(buttonData.first, data.first);

				for (EctoButton *button : buttonData.second) {
					button->updateStatus(buttonState);
				}
			}

			for (std::pair<const int, std::vector<JoystickAxis *>> &axisData : joyData.axesToUpdate) {
				double axisValue = getJoystickRaw((unsigned int) axisData.first, data.first);

				for (JoystickAxis *axis : axisData.second) {
					axis->updateValue(axisValue);
				}
			}
		}

		ectoInputMutex.unlock();
	}
}

#pragma clang diagnostic pop

void EctoInput::addButton(EctoButton *button, const std::string &buttonName, int joystickId) {
	std::lock_guard<std::mutex> lock(ectoInputMutex);

	cout << joystickId << endl;

	JoystickData &data = joysticks.at(joystickId);

	int buttonId = data.buttonIds.at(buttonName);


	data.buttonsToUpdate[buttonId].emplace_back(button);
}

void EctoInput::addAxis(JoystickAxis *axis, const std::string &axisName, int joystickId) {
	std::lock_guard<std::mutex> lock(ectoInputMutex);

	JoystickData &data = joysticks.at(joystickId);

	int axisId = data.axisIds.at(axisName);

	data.axesToUpdate[axisId].emplace_back(axis);
}

void EctoInput::initializeJoysticks() {
	std::lock_guard<std::mutex> lock(ectoInputMutex);

	log->info("Initializing Sticks...");
	for (int i = 1; i <= joystickCount; ++i) {
		auto stickId = settings.getNumber<int>(baseNameStick + to_string(i));

		JoystickData joystickData;

		joystickData.joystickId = i;
		joystickData.wpiJoystickId = stickId;

		joystickData.joystick = new frc::Joystick(stickId);

		while (joystickData.joystick->GetAxisCount() == 0) {
		};

		joystickData.axisCount = joystickData.joystick->GetAxisCount();
		joystickData.buttonCount = joystickData.joystick->GetButtonCount();

		joysticks.emplace(std::make_pair(i, joystickData));
	}
}

void EctoInput::initializeAxis() {
	std::lock_guard<std::mutex> lock(ectoInputMutex);

	for (std::pair<const int, JoystickData> &data : joysticks) {
		JoystickData &joyData = data.second;

		for (int i = 1; i < joyData.axisCount + 1; ++i) {
			std::string axisName = settings.getString(
					baseNameStickSelector + std::to_string(data.first) + "." + baseNameAxis + to_string(i));

			joyData.axisIds.emplace(std::make_pair(axisName, i));
		}
	}
}

void EctoInput::initializeButtons() {
	std::lock_guard<std::mutex> lock(ectoInputMutex);

	for (std::pair<const int, JoystickData> &data : joysticks) {
		JoystickData &joyData = data.second;

		for (int i = 1; i < joyData.buttonCount; ++i) {
			std::string buttonName = settings.getString(
					baseNameStickSelector + std::to_string(data.first) + "." + baseNameButton + to_string(i));

			cout << buttonName << i << endl;

			joyData.buttonIds.emplace(std::make_pair(buttonName, i));
		}
	}
}

/**
 * Returns button state
 * @param  id Button ID
 * @return
 */
bool EctoInput::getButtonState(int id, int joyId) const {
	return joysticks.at(joyId).joystick->GetRawButton(id);
}


/**
 * Returns joystick raw value
 * @param  joystickId Axis ID
 * @return
 */
double EctoInput::getJoystickRaw(const unsigned int joystickId, int joyId) const {
	return joysticks.at(joyId).joystick->GetRawAxis(joystickId - 1);
}

int EctoInput::getPOV(int joyId) const {
	return joysticks.at(joyId).joystick->GetPOV();
}
