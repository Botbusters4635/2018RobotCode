//
// Created by Abiel on 9/7/18.
//

#ifndef BOTBUSTERSREBIRTH_AUTOROUTINEBASE_H
#define BOTBUSTERSREBIRTH_AUTOROUTINEBASE_H

#include "AutoActions/Action.h"
#include "Subsystems/EctoSubsystem.h"
#include <string>
#include <vector>
#include <memory>

struct ActionData {
	std::shared_ptr<Action> action;
	bool hasFinishedBeenCalled = false;
};

class AutoRoutineBase : public EctoSubsystem {
public:
	explicit AutoRoutineBase(const std::string &baseName);

	//Currently unused
	void initTeleop() override;

	void updateTeleop() override;

	void initDisabled() override;

	void updateDisabled() override;

	void initAuto() override;

	void updateAuto() override;

protected:
	/**
	 * Add all actions here
	 */
	virtual void init() = 0;

	void addAction(std::shared_ptr<Action> action);

private:
	std::vector<ActionData> actions;

	void reloadRoutine();

	void initAction(const ActionData &actionData);

	void runAction(ActionData &actionData);

};


#endif //BOTBUSTERSREBIRTH_AUTOROUTINEBASE_H
