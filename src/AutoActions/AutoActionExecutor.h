//
// Created by Abiel on 9/12/18.
//

#ifndef BOTBUSTERSREBIRTH_AUTOACTIONEXECUTOR_H
#define BOTBUSTERSREBIRTH_AUTOACTIONEXECUTOR_H

#include "Action.h"
#include "Subsystems/EctoSubsystem.h"
#include <vector>
#include <memory>
#include <utility>

class AutoActionExecutor : public EctoSubsystem {
public:
	explicit AutoActionExecutor(std::vector<Action *> &actionsIn);

	void initAuto() override;

	void updateAuto() override;

private:
	std::vector<Action *> &actions;
	std::map<Action *, bool> ranAction;
};


#endif //BOTBUSTERSREBIRTH_AUTOACTIONEXECUTOR_H
