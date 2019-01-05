//
// Created by Abiel on 11/1/18.
//

#ifndef ECTOPLOTTER_PATHREADER_H
#define ECTOPLOTTER_PATHREADER_H

#include <string>
#include "json.hpp"
#include "PathContainer.h"

using json = nlohmann::json;

class PathReader {
public:
	explicit PathReader(const std::string &filePath);

	PathContainer readFromFile();

private:
	std::string filePath;
};


#endif //ECTOPLOTTER_PATHREADER_H
