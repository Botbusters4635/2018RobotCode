//
// Created by Abiel on 2/28/18.
//

#include "CubeManager.h"
#include "Math/MathUtil.h"

CubeManager::CubeManager() : EctoSubsystem("CubeManager") {
	log->info("Initializing CubeManager...");

	settings.setCurrentGroup("Intake");

	//Gets the preset intake speeds
	intakeSpeedTeleop = settings.getNumber("SpeedTeleopIntake");

	//Gets the preset eject speeds
	ejectSpeedTeleop = settings.getNumber("EjectSpeedTeleop");
	ejectSpeedSwitch = settings.getNumber("EjectSpeedSwitch");
	ejectSpeedExchange = settings.getNumber("EjectSpeedExchange");
	ejectSpeedScale = settings.getNumber("EjectSpeedScale");

	//Gets expo parameters for intake sticks
	singleStickExpoIntake = settings.getNumber("SingleStickExpo");

	leftStickExpoIntake = settings.getNumber("LeftStickExpo");
	rightStickExpoIntake = settings.getNumber("RightStickExpo");

	stopAfterReleasedWinch = settings.getBool("StopAfterReleased");
	offsetToMoveAfterReleasedWinch = settings.getNumber("OffsetToMoveAfterReleased");

	settings.setCurrentGroup("Winch");
	downTriggerExpoWinch = settings.getNumber("DownTriggerExpo");
	upTriggerExpoWinch = settings.getNumber("UpTriggerExpo");

	maximumUpVelocityWinch = settings.getNumber("MaximumUpVelocityWinch");
	maximumDownVelocityWinch = settings.getNumber("MaximumDownVelocityWinch");

	currentSetHeight = winch.getMinNativeHeight();

	upWinch = new JoystickAxisExpo(upTriggerExpoWinch, 0.0);
	downWinch = new JoystickAxisExpo(downTriggerExpoWinch, 0.0);

	upWinchSecond = new JoystickAxisExpo(upTriggerExpoWinch, 0.0);
	downWinchSecond = new JoystickAxisExpo(downTriggerExpoWinch, 0.0);

	stickIntake = new JoystickAxisExpo(singleStickExpoIntake, 0);

	manualStickLeftIntake = new JoystickAxisExpo(leftStickExpoIntake, 0);
	manualStickRightIntake = new JoystickAxisExpo(rightStickExpoIntake, 0);

	stick.addButton(&enableIntakeController, buttonEnableIntake);
	stick.addButton(&enableIntakeSecondController, "A", 2);

	stick.addButton(&shootIntakeController, buttonShootIntake);
	stick.addButton(&shootIntakeSecondController, "B", 2);

	stick.addButton(&moveToSwitch, "X", 2);
	stick.addButton(&forceResetZero, "select", 2);

	stick.addButton(&shootWithoutPistonIntakeController, "rightPush", 2);
	stick.addButton(&enableDoubleStickControlIntake, "leftPush", 2);

	stick.addButton(&forceIntakeUpController, "B");

	stick.addAxis(upWinch, triggerUpWinch);
	stick.addAxis(downWinch, triggerDownWinch);

	stick.addAxis(upWinchSecond, triggerUpWinch, 2);
	stick.addAxis(downWinchSecond, triggerDownWinch, 2);

	stick.addAxis(stickIntake, "leftY", 2);

	stick.addAxis(manualStickLeftIntake, "leftY", 2);
	stick.addAxis(manualStickRightIntake, "rightY", 2);
}

void CubeManager::initTeleop() {
	currentSetHeight = winch.getNativeHeight();
}

void CubeManager::updateTeleop() {
	/**
	 * Winch Buttons
	 */

	//If second controller is not 0, use second controller input
	upWinch = upWinchSecond != 0 ? upWinchSecond : upWinch;
	downWinch = downWinchSecond != 0 ? downWinchSecond : downWinch;

	/**
	 * Winch Control
	 */
	if (moveToSwitch.get())
		currentSetHeight = winch.determineNativeHeight(CurrentPosition::SwitchHeight);

	double upSpeed = upWinch->get() * maximumUpVelocityWinch;
	double downSpeed = downWinch->get() * maximumDownVelocityWinch;

	double speed = upSpeed - downSpeed;

	currentSetHeight += speed;

	//If stop after released is enabled and no trigger input is received
	if (stopAfterReleasedWinch && speed == 0) {
		double currentHeight = winch.getCurrentNativeHeight();

		double direction;

		//Determines the direction the winch was previously going in
		if (currentSetHeight - currentHeight > 0)
			direction = 1.0;
		else
			direction = -1.0;

		if (offsetCalculationEnabled) {
			auto currentVelocity = winch.getCurrentVelocity();

			//Blame Joel if this doesn't work
			offsetToMoveAfterReleasedWinch = -1.0 * pow(currentVelocity, 2.0) / 2.0 * winch.getMaximumAcceleration();
			//(Reyna, 2018)
		}

		if (abs(currentSetHeight - currentHeight) > offsetToMoveAfterReleasedWinch)
			currentSetHeight = currentHeight + (offsetToMoveAfterReleasedWinch * direction);
	}

	currentSetHeight = MathUtil::limitNumber(currentSetHeight, winch.getMinNativeHeight(), winch.getMaxNativeHeight());

	winch.setNativeHeight(currentSetHeight);

	currentPosition = winch.determinePositionFromNativeHeight(currentSetHeight);
	winch.setPosition(currentPosition);

	if (forceResetZero.get())
		this->winch.resetZeroEncoderPos();

	/**
 	* Intake Buttons
 	*/
	//Enables intake
	bool enableIntake = enableIntakeController.get() || enableIntakeSecondController.get();

	//Ejects cube from intake
	bool shootIntake = shootIntakeController.get() || shootIntakeSecondController.get();

	//Ejects cube without activating piston
	bool shootWithoutPistonIntake = shootWithoutPistonIntakeController.get();

	/**
	 * Intake Control
	 */
	//Toggles intake
	if (enableIntake && enableIntake != previousStateEnableIntake)
		intakeEnabled = !intakeEnabled;

	double intakeSpeed = intakeSpeedTeleop;
	double ejectSpeed = ejectSpeedTeleop;

	//Determines at which speed should the cube eject at
	ejectSpeed = determineIntakeEjectSpeed(winch.getCurrentPosition());

	//Toggle double stick enabled
	if (enableDoubleStickControlIntake.get() &&
	    enableDoubleStickControlIntake.get() != previousStateDoubleStickControlIntake)
		intakeDoubleStickEnabled = !intakeDoubleStickEnabled;

	if (intakeDoubleStickEnabled) {
		double averageOfBoth = (manualStickLeftIntake->get() + manualStickRightIntake->get()) / 2.0;

		bool activatePiston = averageOfBoth <= 0.0;

		intake.writeToIndividualMotors(manualStickLeftIntake->get(), manualStickRightIntake->get(), activatePiston);
	} else {
		//Sets the intake motors
		if (stickIntake->get() == 0.0) {
			if (stickIntake->get() > 0)
				this->intake.writeToMotors(stickIntake->get(), false);
			else if (stickIntake->get() < 0)
				this->intake.writeToMotors(stickIntake->get(), true);
		} else {
			if (intakeEnabled)
				this->intake.intake(intakeSpeed);
			else if (shootIntake)
				this->intake.eject(ejectSpeed);
			else if (shootWithoutPistonIntake)
				this->intake.eject(ejectSpeed, false);
			else
				this->intake.stop();
		}
	}

	//TODO Update this for offseason
	//Forces intake up, usually only used to setup initial position
	//bool forceIntakeUp = forceIntakeUpController.get() || RobotController::GetUserButton();
	//if (forceIntakeUp)
	//	EctoPCM::getInstance().setPiston("ClawSwivel", false);

	//This usually always goes last
	previousStateEnableIntake = enableIntake;
	previousStateShootIntake = shootIntake;
	previousStateDoubleStickControlIntake = enableDoubleStickControlIntake.get();
}

void CubeManager::initAuto() {
	;
}

void CubeManager::updateAuto() {
	//Determines the intake swivel status
	;
}

void CubeManager::initDisabled() {
	;
}

void CubeManager::updateDisabled() {
	;
}

void CubeManager::sendInitialTelemetry() const {
	;
}

void CubeManager::sendTelemetry() const {
	;
}

/**
 * Determines at which speed should the intake be ejecting at given current winch position
 * @param in
 * @return
 */
double CubeManager::determineIntakeEjectSpeed(CurrentPosition in) const {
	switch (in) {
		case CubeHeight:
			return (ejectSpeedTeleop);

		case ExchangeHeight:
			return (ejectSpeedExchange);

		case SwitchHeight:
			return (ejectSpeedScale);

		case ScaleHeight:
			return (ejectSpeedScale);

		default:
			return (ejectSpeedTeleop);
	}
}

/**
 * Determines if swivel should be raised or lowered
 * (true is raised false is lowered)
 * @param in
 * @return
 */
bool CubeManager::determineIntakeSwivelPosition(CurrentPosition in) const {
	switch (in) {
		case CubeHeight:
			return (false);

		case ExchangeHeight:
			return (false);

		case SwitchHeight:
			return (false);

		case ScaleHeight:
			return (false);

		default:
			return (false);
	}
}