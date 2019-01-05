#ifndef BOTBUSTERSREBIRTH_TIMER_H
#define BOTBUSTERSREBIRTH_TIMER_H

#include <frc/Timer.h>

using namespace std;

class EctoTimer {
public:
	static EctoTimer &getInstance() {
		static EctoTimer singletonInstance;
		return singletonInstance;
	}

	double getTime();

private:
	EctoTimer();

	EctoTimer(const EctoTimer &);

	EctoTimer &operator=(const EctoTimer &);

};


#endif