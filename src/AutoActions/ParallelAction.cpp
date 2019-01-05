//
// Created by Abiel on 8/25/18.
//

#include "ParallelAction.h"

ParallelAction::ParallelAction(const std::vector<shared_ptr<Action>> &listOfActionsIn) : listOfActions(
		listOfActionsIn) {
	for (const shared_ptr<Action> &action : listOfActions) {
		if (action == nullptr)
			throw std::runtime_error("Parallel Action given a nullptr");
	}
}

void ParallelAction::init() {
	for (const shared_ptr<Action> &action : listOfActions) {
		action->init();
	}
}

void ParallelAction::update() {
	for (const shared_ptr<Action> &action : listOfActions) {
		action->update();
	}
}

bool ParallelAction::isFinished() const {
	//Initially set isFinished to true
	bool isFinished = true;

	for (const auto &action : listOfActions) {
		isFinished = action->isFinished() && isFinished;
	}

	return isFinished;
}

void ParallelAction::finish() {
	for (const shared_ptr<Action> &action : listOfActions) {
		action->finish();
	}
}