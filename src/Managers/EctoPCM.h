/*
* Settings
* Group: PCM
* PcmId: PCM ID
* CompressorId: Compressor ID
* PistonQuantity: Piston Quantity
* piston1 - piston#: Piston IDs
* pistonId.1 - pistonId.#: Optional Piston Names
 */
#ifndef BOTBUSTERSREBIRTH_ECTOPCM_H
#define BOTBUSTERSREBIRTH_ECTOPCM_H

#include <frc/WPILib.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Compressor.h>
#include "Utilities/EctoSettings.h"
#include <map>
#include <utility>
#include <string>

using namespace std;

/**
 * Class used to control pistons.
 * @return
 */
class EctoPCM final {
public:

	static EctoPCM &getInstance() {
		static EctoPCM singletonInstance;
		return singletonInstance;
	}

	void compressorPower(bool state);

	void setPiston(unsigned int id, bool state);

	void setPiston(string name, bool state);

	void togglePiston(string name);

	void togglePiston(unsigned id);

	void initPistonsState(string name, bool state);

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("EctoPCM");

	EctoPCM();

	EctoPCM &operator=(const EctoPCM &);

	EctoSettings &settings = EctoSettings::getInstance();

	int defaultPCMId = 0;

	void initializePistons();

	frc::Compressor *compressor;
	bool compressorStatus = false;

	unsigned int pistonQuantity;

	const string basePiston = "piston";
	const string basePistonPCMId = "pistonPCMId.";
	const string basePistonName = "pistonId.";

	map<unsigned int, pair<unsigned int, unsigned int>> pistonIds;
	map<unsigned int, int> pistonPCMIds;
	map<unsigned int, shared_ptr<frc::DoubleSolenoid>> pistons;
	map<unsigned int, std::string> names;
	map<std::string, unsigned int> idNames;

	map<unsigned int, bool> invertState;
};

#endif
