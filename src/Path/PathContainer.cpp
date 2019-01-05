//
// Created by Abiel on 11/1/18.
//

#include "PathContainer.h"

using json = nlohmann::json;

void to_json(json &j, const PathContainer &container) {
	j = json{{"pathName",        container.pathName},
	         {"stepSizes",       container.stepSize},
	         {"generatedPoints", container.generatedPoints},
	         {"waypoints",       container.waypoints}};
}

void from_json(const json &j, PathContainer &container) {
	j.at("pathName").get_to(container.pathName);
	j.at("stepSizes").get_to(container.stepSize);
	j.at("generatedPoints").get_to(container.generatedPoints);
	j.at("waypoints").get_to(container.waypoints);
}