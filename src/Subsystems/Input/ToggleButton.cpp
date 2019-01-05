//
// Created by Abiel on 9/11/18.
//

#include "ToggleButton.h"

void ToggleButton::calculateOutput() {
	if (inValue && inValue != previousStatus) {
		this->outValue = !outValue;
	}

	this->previousStatus = inValue;
}