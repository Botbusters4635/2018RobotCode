//
// Created by Abiel on 8/25/18.
//

#include "ThreadedParallelAction.h"

ThreadedParallelAction::ThreadedParallelAction(const std::vector<shared_ptr<Action>> &listOfActionsIn) : listOfActions(
		listOfActionsIn) {
	;
}

void ThreadedParallelAction::init() {
	//Starts all threads
	for (const shared_ptr<Action> &action : listOfActions) {
		std::pair<std::packaged_task<void(shared_ptr<Action>)>, std::future<void>> actionTask;

		actionTask.first = std::packaged_task<void(shared_ptr<Action>)>(ThreadedParallelAction::runUntilFinished);
		actionTask.second = (actionTask.first).get_future();

		std::thread thread(std::move(actionTask.first), action);

		runningThreads.emplace_back(std::move(thread));
	}

	//std::packaged_task<void(shared_ptr<Action>)> actionRunTask(ThreadedParallelAction::runUntilFinished);
}

void ThreadedParallelAction::update() {
	;
}

bool ThreadedParallelAction::isFinished() const {
	bool finishedStatus = true;

	for (const std::pair<std::packaged_task<void(
				shared_ptr<Action>)>, std::future<void>> &actionStatus : runningActions) {
		auto status = (actionStatus.second).wait_for(std::chrono::milliseconds(0));

		finishedStatus = finishedStatus && status == std::future_status::ready;
	}

	return finishedStatus;
}

void ThreadedParallelAction::finish() {
	for (std::thread &actionThread : runningThreads) {
		actionThread.join();
	}
}

void ThreadedParallelAction::runUntilFinished(const shared_ptr<Action> &action) {
	action->init();

	while (action->isFinished()) {
		action->update();
	}

	action->finish();
}