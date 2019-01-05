//
// Created by Abiel on 8/25/18.
//

#include "WaitAction.h"

WaitAction::WaitAction(double timeToWait) {
	this->timeToWait = timeToWait;
}

void WaitAction::init() {
	this->startTime = frc::Timer::GetFPGATimestamp();
}

void WaitAction::update() {
	if (frc::Timer::GetFPGATimestamp() - startTime >= timeToWait)
		this->finished = true;
}

void WaitAction::finish() {
	;
}