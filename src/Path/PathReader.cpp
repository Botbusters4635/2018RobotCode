//
// Created by Abiel on 11/1/18.
//

#include "PathReader.h"
#include <fstream>
#include <iostream>

PathReader::PathReader(const std::string &filePath) {
	this->filePath = filePath;
}

PathContainer PathReader::readFromFile() {
	std::ifstream fileStream(filePath);

	json j;

	fileStream >> j;
	fileStream.close();

	PathContainer container = j["path"];

	return container;
}