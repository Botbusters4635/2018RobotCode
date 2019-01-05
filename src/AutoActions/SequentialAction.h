//
// Created by Abiel on 8/25/18.
//

#ifndef BOTBUSTERSREBIRTH_SEQUENTIALACTION_H
#define BOTBUSTERSREBIRTH_SEQUENTIALACTION_H

#include <vector>
#include <memory>
#include <stdexcept>
#include "Action.h"

using std::shared_ptr;

/**
 * Runs various actions in a sequential order
 */
class SequentialAction : public Action {
public:
	explicit SequentialAction(const std::vector<shared_ptr<Action>> &listOfActionsIn);

	void init() override;

	void update() override;

	void finish() override;

private:
	const std::vector<shared_ptr<Action>> &listOfActions;
	std::vector<shared_ptr<Action>>::const_iterator currentAction;
};


#endif //BOTBUSTERSREBIRTH_SEQUENTIALACTION_H
