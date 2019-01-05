//
// Created by Abiel on 9/7/18.
//

#include "AutoRoutineBase.h"

AutoRoutineBase::AutoRoutineBase(const std::string &baseName) : EctoSubsystem(baseName) {
	;
}

void AutoRoutineBase::initTeleop() {
	;
}

void AutoRoutineBase::updateTeleop() {
	;
}

void AutoRoutineBase::initDisabled() {
	;
}

void AutoRoutineBase::updateDisabled() {
	;
}

void AutoRoutineBase::initAuto() {
	reloadRoutine();
}

void AutoRoutineBase::updateAuto() {
	for (ActionData &action : actions) {
		runAction(action);
	}
}

void AutoRoutineBase::reloadRoutine() {
	actions.clear();
	init();

	for (ActionData &actionData : actions) {
		initAction(actionData);
	}
}

void AutoRoutineBase::initAction(const ActionData &actionData) {
	actionData.action->init();
}

void AutoRoutineBase::runAction(ActionData &actionData) {
	if (!actionData.action->isFinished() && !actionData.hasFinishedBeenCalled) {
		actionData.action->update();
	} else {
		actionData.action->finish();
		actionData.hasFinishedBeenCalled = true;
	}
}