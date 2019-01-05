#include "PathWriter.h"

PathWriter::PathWriter(const std::string &filePath, const PathContainer &container) : pathContainer(container) {
	this->filePath = filePath;
	this->pathContainer = pathContainer;
}

std::vector<RobotPose2DCurvature> PathWriter::generatePoints() const {
	std::vector<RobotPose2DCurvature> output;

	for (const QuinticSpline &spline : pathContainer.splines) {
		for (double t = 0.0; t <= 1.0; t += pathContainer.stepSize) {
			output.emplace_back(RobotPose2DCurvature(spline.getPoint(t), spline.getHeading(t), spline.getCurvature(t),
			                                         spline.getDCurvature(t)));
		}
	}

	return output;
}

void PathWriter::writeToFile() {
	if (pathContainer.generatedPoints.empty()) {
		std::cout << "GeneratingPoints" << std::endl;
		pathContainer.generatedPoints = generatePoints();
	}

	json outputJson;

	outputJson["path"] = pathContainer;

	std::ofstream file;
	file.open(filePath, std::ios::trunc);

	file << outputJson;
	file.close();

	std::cout << "Finished Writing" << std::endl;
}