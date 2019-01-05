//
// Created by Abiel on 10/8/18.
//

#include "RobotProperties.h"
#include <stdexcept>

RobotProperties::RobotProperties() {
	;
}

void RobotProperties::setDimensions(const RobotDimensions &dimensions) {
	haveDimensionsBeenSet = true;
	this->dimensions = dimensions;
}

RobotDimensions RobotProperties::getDimensions() const {
	if (!haveDimensionsBeenSet)
		throw std::runtime_error("Tried to get dimensions without them being set");
	return dimensions;
}