#include "Logger.h"
#include <ctime>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
// Popular Logging Libraries
// gabime/spdlog
// rxi/log.c
std::vector<LogEntry> Logger::messages;
std::string NowAsString() {
  std::time_t now = std::time({});
  char time_as_string[std::size("dd-mmm-yyyy hh:mm:ss")];
  std::strftime(std::data(time_as_string), std::size(time_as_string),
                "%b-%d-%Y %T", std::localtime(&now));
  return time_as_string;
}

void Logger::Log(const std::string &message) {
  LogEntry entry;
  entry.type = LogType::LOG_INFO;
  entry.message = "\e[0;32mLOG | " + NowAsString() + " - " + message + "\n";
  std::cout << entry.message;
  messages.push_back(entry);
}

void Logger::Error(const std::string &message) {
  LogEntry entry;
  entry.type = LogType::ERROR;
  entry.message = "\e[0;31mERR | " + NowAsString() + " - " + message + "\n";
  std::cout << entry.message;
  messages.push_back(entry);
}
