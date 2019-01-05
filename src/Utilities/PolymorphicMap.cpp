//
// Created by Abiel on 10/16/18.
//

#include "PolymorphicMap.h"

void PolymorphicMap::writeData(const std::string &key, const boost::spirit::hold_any &data) {
	std::unique_lock<std::shared_timed_mutex> lock(dataMutex);

	auto iter = this->data.find(key);

	if (iter != this->data.end()) {
		//Data value already exists

		if (iter->second.type() != data.type())
			throw std::logic_error(
					"Tried to add a " + std::string(data.type().name()) + " to key: " + key + " with data type: " +
					std::string(iter->second.type().name()));

		iter->second = data;
	} else {
		//No data value

		this->data.emplace(std::make_pair(key, data));
	}
}