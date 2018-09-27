#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <thread>
#include <string>

namespace core {
namespace utils {

enum LogLevel : unsigned { kInfo, kWarn, kError };

void log(std::string message, LogLevel level);
void cout_info(LogLevel level);

} // namespace utils
} // namespace core

#endif // LOGGER_HPP_
