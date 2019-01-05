//
// Created by Abiel on 8/25/18.
//

#include "SequentialAction.h"

SequentialAction::SequentialAction(const std::vector<shared_ptr<Action>> &listOfActionsIn) : listOfActions(
		listOfActionsIn) {
	for (const shared_ptr<Action> &action : listOfActions) {
		if (action == nullptr)
			throw std::runtime_error("Sequential Action given a nullptr");
	}
}

void SequentialAction::init() {
	this->currentAction = listOfActions.begin();

	//Runs init on the first action
	(*currentAction)->init();
}

void SequentialAction::update() {
	if (!finished) {
		if ((*currentAction)->isFinished()) {
			(*currentAction)->finish();

			currentAction++;

			if (currentAction == listOfActions.end()) {
				finished = true;
				return;
			} else
				(*currentAction)->init();
		}

		(*currentAction)->update();
	}
}

void SequentialAction::finish() {
	;
}