//
// Created by Abiel on 9/13/18.
//

#ifndef BOTBUSTERSREBIRTH_CSVHUMANWRITER_H
#define BOTBUSTERSREBIRTH_CSVHUMANWRITER_H

#include "CSVWritable.h"
#include <iostream>
#include <vector>

/**
 * Writes csv to stream in a human readable way
 */
class CSVHumanWriter {
public:
	explicit CSVHumanWriter(const CSVWritable &writable, std::ostream &stream = std::cout);

	void writeObject();

private:
	const CSVWritable &objectToWrite;
	std::ostream &outputStream;
};


#endif //BOTBUSTERSREBIRTH_CSVHUMANWRITER_H
