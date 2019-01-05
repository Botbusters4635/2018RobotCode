//
// Created by Abiel on 9/12/18.
//

#include "AutoActionExecutor.h"

AutoActionExecutor::AutoActionExecutor(std::vector<Action *> &actionsIn) : EctoSubsystem("AutoActionExecutor"),
                                                                           actions(actionsIn) {
	;
}

void AutoActionExecutor::initAuto() {
	for (Action *action : actions) {
		action->init();
	}
}

void AutoActionExecutor::updateAuto() {
	for (Action *action : actions) {
		if (action->isFinished() && !ranAction[action]) {
			action->finish();
			ranAction[action] = true;
		} else {
			action->update();
		}
	}
}