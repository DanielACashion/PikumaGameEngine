#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>

enum LogType { LOG_INFO = 0, ERROR };

class LogEntry {
public:
  LogType type;
  std::string message;
};

class Logger {
  static std::vector<LogEntry> messages;

public:
  static void Log(const std::string &message);
  static void Error(const std::string &message);
};

#endif