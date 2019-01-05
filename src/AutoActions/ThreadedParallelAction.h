//
// Created by Abiel on 8/25/18.
//

#ifndef BOTBUSTERSREBIRTH_THREADEDPARALLELACTION_H
#define BOTBUSTERSREBIRTH_THREADEDPARALLELACTION_H

#include <vector>
#include <memory>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <future>
#include "Action.h"

using std::shared_ptr;

/**
 * SUPER EXPERIMENTAL, USE WITH CAUTION
 */
class ThreadedParallelAction : public Action {
public:
	explicit ThreadedParallelAction(const std::vector<shared_ptr<Action>> &listOfActionsIn);

	void init() override;

	void update() override;

	bool isFinished() const override;

	void finish() override;

private:
	static void runUntilFinished(const shared_ptr<Action> &action);

	const std::vector<shared_ptr<Action>> &listOfActions;

	std::vector<std::pair<std::packaged_task<void(shared_ptr<Action>)>, std::future<void>>> runningActions;
	std::vector<std::thread> runningThreads;
};


#endif //BOTBUSTERSREBIRTH_THREADEDPARALLELACTION_H
