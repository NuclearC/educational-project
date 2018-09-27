#include "utils/logger.hpp"

namespace core {
namespace utils {

void log(std::string message, LogLevel level) {
  switch (level) {
  case kInfo:
    message = "[Info] " + message;
    std::cout << "Thread " << std::this_thread::get_id() << ": " << message
              << std::endl;
    break;
  case kWarn:
    message = "[Warn] " + message;
    std::cout << "Thread " << std::this_thread::get_id() << ": " << message
              << std::endl;
    break;
  case kError:
    message = "[Error] " + message;
    std::cerr << "Thread " << std::this_thread::get_id() << ": " << message
              << std::endl;
  }
}

void cout_info(LogLevel level) {
  switch (level) {
  case kInfo:
    std::cout << "Thread " << std::this_thread::get_id() << ": [Info] ";
    break;
  case kWarn:
    std::cout << "Thread " << std::this_thread::get_id() << ": [Warn] ";
    break;
  case kError:
    std::cout << "Thread " << std::this_thread::get_id() << ": [Error] ";
  }
}

} // namespace utils
} // namespace core
