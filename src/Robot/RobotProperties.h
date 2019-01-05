//
// Created by Abiel on 10/8/18.
//

#ifndef BOTBUSTERSREBIRTH_ROBOTPROPERTIES_H
#define BOTBUSTERSREBIRTH_ROBOTPROPERTIES_H

#include "RobotDimensions.h"

class EctoRobot;

class RobotProperties {
public:
	static RobotProperties &getInstance() {
		static RobotProperties singletonInstance;
		return singletonInstance;
	}

	RobotDimensions getDimensions() const;

	void setDimensions(const RobotDimensions &dimensions);

	RobotProperties &operator=(const RobotProperties &) = delete;

private:
	RobotProperties();

	RobotDimensions dimensions;

	bool haveDimensionsBeenSet = false;
};


#endif //BOTBUSTERSREBIRTH_ROBOTPROPERTIES_H
