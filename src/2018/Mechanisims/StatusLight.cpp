//
// Created by Abiel on 3/5/18.
//

#include "StatusLight.h"

StatusLight::StatusLight() {
	settings.setCurrentGroup("StatusLight");

	outputCount = settings.getNumber<int>("StatusLightCount");

	initializeOutputs();
}

void StatusLight::initializeOutputs() {
	for (int i = 1; i <= outputCount; ++i) {
		pair<int, frc::DigitalOutput *> output;
		pair<string, int> idName;
		pair<int, bool> setOutput;

		output.first = i;

		auto id = settings.getNumber<int>(baseDigitalOutputId + to_string(i));
		string name = settings.getString(baseDigitalOutputName + to_string(i));

		output.first = i;
		output.second = new frc::DigitalOutput(id);

		idName.first = name;
		idName.second = i;

		outputs.emplace(output);
		names.emplace(idName);

		setOutput.first = i;
		setOutput.second = false;

		setOutputs.emplace(setOutput);

		cout << name << id << i << endl;
	}
}

bool StatusLight::getSetOutput(const string &name) const {
	try {
		return setOutputs.at(names.at(name));
	}
	catch (const out_of_range &oor) {
		return false;
	}
}

void StatusLight::setOutput(const string &name, bool state) {
	try {
		int outId = names.at(name);

		setOutputs.at(outId) = state;
		outputs.at(outId)->Set(state);
	}
	catch (const out_of_range &oor) {
		cerr << "With name: " << name << endl;
		cerr << "Out of range in set output" << endl;
	}
}

void StatusLight::toggleOutput(const string &name) {
	try {
		int outId = names.at(name);

		bool stateToSet = !getSetOutput(name);

		setOutput(name, stateToSet);
	}
	catch (const out_of_range &oor) { ;
	}
}