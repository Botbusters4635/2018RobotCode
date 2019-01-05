//
// Created by Abiel on 10/16/18.
//

#ifndef BOTBUSTERSREBIRTH_ROBOTDATA_H
#define BOTBUSTERSREBIRTH_ROBOTDATA_H

#include <memory>
#include <vector>
#include <string>
#include <future>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include <boost/optional.hpp>
#include <boost/convert/lexical_cast.hpp>
#include "Utilities/PolymorphicMap.h"

#define USE_SMARTDASHBOARD

#ifdef USE_SMARTDASHBOARD
#include <frc/smartdashboard/SmartDashboard.h>
#endif

/**
 * Database which can be used to pass data between subsystems
 */
class RobotData final {
public:
	static RobotData &getInstance() {
		static RobotData singletonInstance;
		return singletonInstance;
	}

	void addData(const std::string &key, const boost::spirit::hold_any &data) {
		if (enableDataLogging) {
			std::string dataAsString;

			if (boost::conversion::try_lexical_convert(data, dataAsString)) {
				fileLogger->info("Writing to: " + key + " with type: " + boost::core::demangled_name(data.type()) +
				                 " with data: " + dataAsString);
			} else {
				fileLogger->info("Writing to: " + key + " with type: " + data.type().name());
			}


#ifdef USE_SMARTDASHBOARD
			auto publishHandle = std::async(std::launch::async, frc::SmartDashboard::PutString, "RobotData/" + key, dataAsString);
#endif
		}


		this->data.writeData(key, data);
	}

	template<class T>
	void addData(const std::string &key, const T &data) {
		addData(key, boost::spirit::hold_any(data));
	}

	template<class T>
	T getData(const std::string &key) const {
		T data = this->data.getData<T>(key);

		if (enableDataLogging) {
			std::string dataAsString;

			if (boost::conversion::try_lexical_convert<std::string>(data, dataAsString)) {
				fileLogger->info("Reading from: " + key + " with type: " + boost::core::demangled_name(typeid(data)) +
				                 " with data: " + dataAsString);
			} else {
				fileLogger->info("Reading from: " + key + " with type: " + boost::core::demangled_name(typeid(data)));
			}
		}

		return data;
	}

private:
	RobotData() {
		spdlog::register_logger(fileLogger);
		logger->info("Started RobotData");
	}

	RobotData &operator=(const RobotData &) = delete;

	static constexpr bool enableDataLogging = true;

	//In MB
	static constexpr std::size_t maximumLogFileSize = 15;

	spdlog::sink_ptr rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			"/home/abiel/CLionProjects/BoostAnyTests/RobotDataLog",
			1048576 *
			maximumLogFileSize, 3);

	std::vector<spdlog::sink_ptr> sinks = {rotating_sink};
	std::shared_ptr<spdlog::async_logger> fileLogger = std::make_shared<spdlog::async_logger>("RobotDataFile",
	                                                                                          sinks.begin(),
	                                                                                          sinks.end(),
	                                                                                          spdlog::thread_pool(),
	                                                                                          spdlog::async_overflow_policy::block);

	std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("RobotData");

	PolymorphicMap data;

};

#endif //BOTBUSTERSREBIRTH_ROBOTDATA_H
