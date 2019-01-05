//
// Created by Abiel on 9/11/18.
//

#ifndef BOTBUSTERSREBIRTH_TOGGLEBUTTON_H
#define BOTBUSTERSREBIRTH_TOGGLEBUTTON_H

#include "EctoButton.h"

class ToggleButton : public EctoButton {
protected:
	void calculateOutput() override;

private:
	bool previousStatus = false;

	bool toggledState = false;
};


#endif //BOTBUSTERSREBIRTH_TOGGLEBUTTON_H
