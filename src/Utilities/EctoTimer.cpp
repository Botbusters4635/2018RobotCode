#include "EctoTimer.h"

EctoTimer::EctoTimer() {
	;
}

double EctoTimer::getTime() {
	return frc::Timer::GetFPGATimestamp();
}