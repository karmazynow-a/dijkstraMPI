#pragma once

#include <iostream>
#include <string>

template <bool shouldLog>
class Log final
{
public:
	Log(std::ostream& stream, int processRank = 0) : stream(stream), processRank(processRank) {}

	template<typename... Ts>
	void logMessage(const Ts& ... informations) {
		stream << "[Process " << processRank << "] ";
		logMessageBackend(informations...);
		stream << std::endl;
	}

private:

	template<typename T>
	void logMessageBackend(const T& information) {
		stream << information;
	}

	template<typename T, typename... Ts>
	void logMessageBackend(const T& information, const Ts& ... informations) {
		stream << information;
		logMessageBackend(informations...);
	}

	std::ostream& stream;
	int processRank;

};

template <>
class Log<false> final
{
public:

	Log(std::ostream& stream, int processRank = 0) : stream(stream), processRank(processRank) {}

	template<typename... Ts>
	void logMessage(const Ts& ... informations) {}

private:

	std::ostream& stream;
	int processRank;

};
