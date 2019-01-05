//
// Created by Abiel on 10/16/18.
//

#ifndef BOTBUSTERSREBIRTH_POLYMORPHICMAP_H
#define BOTBUSTERSREBIRTH_POLYMORPHICMAP_H

#include <boost/spirit/home/support/detail/hold_any.hpp>
#include <unordered_map>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <exception>

class PolymorphicMap {
public:
	/**
	 * Updates or creates a new data value
	 * @param key
	 * @param data
	 */
	void writeData(const std::string &key, const boost::spirit::hold_any &data);

	template<class T>
	T getData(const std::string &key) const {
		std::shared_lock<std::shared_timed_mutex> lock(dataMutex);
		try {
			return boost::spirit::any_cast<T>(data.at(key));
		} catch (boost::spirit::bad_any_cast &e) {
			throw std::logic_error(e.what());
		}
	}

private:
	mutable std::shared_timed_mutex dataMutex;
	std::unordered_map<std::string, boost::spirit::hold_any> data;
};


#endif //BOTBUSTERSREBIRTH_POLYMORPHICMAP_H
