//
// Created by Abiel on 3/5/18.
//

#ifndef BOTBUSTERSREBIRTH_STATUSLIGHT_H
#define BOTBUSTERSREBIRTH_STATUSLIGHT_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/EctoSubsystem.h"
#include <frc/DigitalOutput.h>

using namespace std;

class StatusLight {
public:
	static StatusLight &getInstance() {
		static StatusLight singletonInstance;
		return singletonInstance;
	}

	void setOutput(const string &name, bool state);

	void toggleOutput(const string &name);

	bool getSetOutput(const string &name) const;

private:
	EctoSettings &settings = EctoSettings::getInstance();

	explicit StatusLight();

	void initializeOutputs();

	const string baseDigitalOutputId = "DigitalOutputId.";
	const string baseDigitalOutputName = "DigitalOutputName.";

	int outputCount;

	map<int, frc::DigitalOutput *> outputs;
	map<string, int> names;

	map<int, bool> setOutputs;
};


#endif //BOTBUSTERSREBIRTH_STATUSLIGHT_H
