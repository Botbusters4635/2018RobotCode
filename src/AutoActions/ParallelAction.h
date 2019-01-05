//
// Created by Abiel on 8/25/18.
//

#ifndef BOTBUSTERSREBIRTH_PARALLELACTION_H
#define BOTBUSTERSREBIRTH_PARALLELACTION_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "Action.h"

using std::shared_ptr;

class ParallelAction : public Action {
public:
	explicit ParallelAction(const std::vector<shared_ptr<Action>> &listOfActionsIn);

	void init() override;

	void update() override;

	bool isFinished() const override;

	void finish() override;

private:
	const std::vector<shared_ptr<Action>> &listOfActions;
};


#endif //BOTBUSTERSREBIRTH_PARALLELACTION_H
