//
// Created by Abiel on 8/25/18.
//

#ifndef BOTBUSTERSREBIRTH_WAITACTION_H
#define BOTBUSTERSREBIRTH_WAITACTION_H

#include <frc/Timer.h>
#include "Action.h"

class WaitAction : public Action {
public:
	explicit WaitAction(double timeToWait);

	void init() override;

	void update() override;

	void finish() override;

private:
	double timeToWait = 0.0;
	double startTime = 0.0;
};


#endif //BOTBUSTERSREBIRTH_WAITACTION_H
