//
// Created by Abiel on 9/11/18.
//

#ifndef BOTBUSTERSREBIRTH_JOYSTICKAXISEXPO_H
#define BOTBUSTERSREBIRTH_JOYSTICKAXISEXPO_H

#include "JoystickAxis.h"

class JoystickAxisExpo : public JoystickAxis {
public:
	JoystickAxisExpo(double expoValue, double deadZone);

protected:
	void calculateOutput() override;

private:
	double expoValue = 1.0;
	double deadZone = 0.0;
};


#endif //BOTBUSTERSREBIRTH_JOYSTICKAXISEXPO_H
