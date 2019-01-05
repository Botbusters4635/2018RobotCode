//
// Created by Abiel on 9/13/18.
//

#include "CSVHumanWriter.h"

CSVHumanWriter::CSVHumanWriter(const CSVWritable &writable, std::ostream &stream) :
		objectToWrite(writable), outputStream(stream) {
	;
}

void CSVHumanWriter::writeObject() {
	std::stringstream headerStream(objectToWrite.getCSVColumnNames());
	std::stringstream dataStream(objectToWrite.toCSV());

	std::vector<std::string> header;
	while (headerStream.good()) {
		std::string substr;
		std::getline(headerStream, substr, ',');
		header.emplace_back(substr);
	}

	while (dataStream.good()) {
		std::string line;
		std::getline(dataStream, line);

		std::stringstream lineStream(line);

		for (const std::string &column : header) {
			std::string lineOut;
			std::getline(lineStream, lineOut, ',');

			outputStream << column << ": " << lineOut << ' ';
		}

		outputStream << std::endl;
	}
}