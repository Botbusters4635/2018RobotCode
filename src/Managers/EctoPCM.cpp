#include "EctoPCM.h"

EctoPCM::EctoPCM() {
	// BOOST_LOG_TRIVIAL(trace) <<"EctoPCM initializing...";
	log->info("Initializing EctoPCM...");
	settings.setCurrentGroup("PCM");
	defaultPCMId = settings.getNumber<int>("DefaultPcmId");
	compressor = new frc::Compressor(settings.getNumber("CompressorId"));

	compressor->ClearAllPCMStickyFaults();
	compressor->SetClosedLoopControl(true);

	initializePistons();
}

/**
 * Reads piston IDs from settings.
 */
void EctoPCM::initializePistons() {
	log->info("Initializing pistons...");

	// BOOST_LOG_TRIVIAL(trace) <<"Pistons initializing...";
	pistonQuantity = static_cast<unsigned int>(settings.getNumber("PistonQuantity"));

	for (unsigned int i = 1; i <= pistonQuantity; ++i) {
		vector<unsigned int> ids;
		pair<unsigned int, unsigned int> id;

		std::string name = settings.getString(basePistonName + to_string(i));

		ids = settings.getArrayNumbers<unsigned int>(basePiston + to_string(i));

		auto pcmID = settings.getNumber<int>(basePistonPCMId + to_string(i));

		//If no pcmId is found, use default value
		if (pcmID == -1) {
			pcmID = defaultPCMId;
		}

		id.first = ids[0];
		id.second = ids[1];

		pistonIds.emplace(i, id);
		pistonPCMIds.emplace(i, pcmID);
		names.emplace(i, name);
		idNames.emplace(name, i);

		shared_ptr<frc::DoubleSolenoid> piston = make_shared<frc::DoubleSolenoid>(pcmID, id.first, id.second);
		pistons.emplace(i, piston);
	}
}

/**
 * Sets compressor power state.
 * @param state Compressor Power
 */
void EctoPCM::compressorPower(const bool state) {
	if (state) {
		compressorStatus = true;
		compressor->Start();
	} else {
		compressorStatus = false;
		compressor->Stop();
	}
}

/**
 * Sets piston using individual IDs.
 * @param id1   Solenoid ID
 * @param id2   Solenoid ID
 * @param state Piston State
 */
void EctoPCM::setPiston(const unsigned int id, const bool state) {
	if (state) {
		pistons[id]->Set(frc::DoubleSolenoid::Value::kForward);
	} else {
		pistons[id]->Set(frc::DoubleSolenoid::Value::kReverse);
	}
}

/**
 * Sets piston state.
 * @param name  Piston Name
 * @param state Piston State
 */
void EctoPCM::setPiston(const string name, const bool state) {
	//BOOST_LOG_TRIVIAL(trace) <<"Updating piston " << name << " to " << !state ;
	unsigned int id = idNames[name];
	setPiston(id, state);
}

/**
 * Toggles piston.
 * @param name Piston Name
 */
void EctoPCM::togglePiston(const string name) {
	//BOOST_LOG_TRIVIAL(trace) <<"Toggling piston " << name;
	unsigned int id = idNames[name];
	invertState[id] = !invertState[id];
	setPiston(name, invertState[id]);
}

/**
 * Toggles piston.
 * @param id Piston ID
 */
void EctoPCM::togglePiston(const unsigned id) {
	// BOOST_LOG_TRIVIAL(warning) << "Please try using named pistons instead";
	// BOOST_LOG_TRIVIAL(warning) <<"Toggling piston " << id;
	invertState[id] = !invertState[id];
	setPiston(id, invertState[id]);
}

void EctoPCM::initPistonsState(const string name, bool state) {
	// BOOST_LOG_TRIVIAL(info) << "Initializing piston " << name << "...";
	unsigned int id = idNames[name];
	invertState[id] = state;
}