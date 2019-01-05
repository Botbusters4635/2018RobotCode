#ifndef PathWriter_H_
#define PathWriter_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "PathContainer.h"
#include "Math/DataTypes/RobotPose2DCurvature.h"

#include "json.hpp"

using json = nlohmann::json;

class PathWriter {
public:
	PathWriter(const std::string &filePath, const PathContainer &container);

	void writeToFile();

private:
	std::string filePath;

	PathContainer pathContainer;

	std::vector<RobotPose2DCurvature> generatePoints() const;
};

#endif