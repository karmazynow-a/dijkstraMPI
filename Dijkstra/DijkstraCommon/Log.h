/**
*	@file Log.h
*	@brief This file contains template classes responsible for logging.
*/

#pragma once

#include <iostream>
#include <string>


/// <summary>
/// Simple template class designed for logging purposes. Its template parameter
/// turns logging on if set to true. If set to false, specialization of this class
/// is used.
/// </summary>
template <bool shouldLog>
class Log final
{
public:

	/// <summary>
	/// Simple Log constructor. Takes up to two arguments.
	/// </summary>
	/// <param name="stream">
	/// Stream to which messages should be printed. Can be standard output or 
	/// file. Please note that this class stores this stream in form of a reference,
	/// so this class is valid as long as stream itself is valid.
	/// </param>
	/// <param name="processRank">
	/// Number that should be printed before every message. Indicates number of 
	/// process that is printing the message - meaningful in parallel programs.
	/// In serial implementations, this parameter should not be provided as there
	/// is no meaning to it - default value of 0 will be used then.
	/// </param>
	Log(std::ostream& stream, int processRank = 0) : stream(stream), processRank(processRank) {}


	/// <summary>
	/// Method used for information logging. Uses variadic templates to take as
	/// many arguments as needed (one or more). Please note that those arguments
	/// will be used with ostream operator. Before messages, special tag is printed
	/// (it contains process rank, for example: [Process 1] ). At the end, endl is
	/// used.
	/// </summary>
	/// <param name="informations">
	/// Parameter pack that contains all messages that should be printed. Those
	/// messages should contains all needed separators - nothing is added between
	/// messages.
	/// </param>
	template<typename... Ts>
	void logMessage(const Ts& ... informations) {
		stream << "[Process " << processRank << "] ";
		logMessageBackend(informations...);
		stream << std::endl;
	}

private:

	/// <summary>
	/// Backend method specialization for one argument.
	/// </summary>
	template<typename T>
	void logMessageBackend(const T& information) {
		stream << information;
	}

	/// <summary>
	/// Backend method that uses variadic templates to print all messages. 
	/// </summary>
	template<typename T, typename... Ts>
	void logMessageBackend(const T& information, const Ts& ... informations) {
		stream << information;
		logMessageBackend(informations...);
	}

	std::ostream& stream;
	int processRank;

};


/// <summary>
/// Specialization of Log class used, when template parameter value is false. 
/// In this case, some methods are not generated and nothing happens when 
/// logMessage method is called.
/// </summary>
template <>
class Log<false> final
{
public:

	/// <summary>
	/// Simple Log constructor. Takes up to two arguments.
	/// </summary>
	/// <param name="stream">
	/// Stream to which messages should be printed.  When using this
	/// specialization of Log class, parameters are not used at all.
	/// </param>
	/// <param name="processRank">
	/// Number that should be printed before every message. When using this
	/// specialization of Log class, parameters are not used at all.
	/// </param>
	Log(std::ostream& stream, int processRank = 0) : stream(stream), processRank(processRank) {}


	/// <summary>
	/// Method used for information logging. When using this specialization of Log 
	/// class, this method does nothing.
	/// </summary>
	/// <param name="informations">
	/// Parameter pack that contains all messages that should be printed. When 
	/// using this specialization of Log class, parameters are not used at all.
	/// </param>
	template<typename... Ts>
	void logMessage(const Ts& ... informations) {}

private:

	std::ostream& stream;
	int processRank;

};
