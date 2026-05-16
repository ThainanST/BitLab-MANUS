#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include <Arduino.h>

enum CommandType {
  COMMAND_NONE,
  COMMAND_FINGER,
  COMMAND_SET_WAVE1,
  COMMAND_SET_WAVE2,
  COMMAND_SET_OPEN,
  COMMAND_GET_STATUS,
  COMMAND_INVALID
};

struct CliCommand {
  CommandType type;
  int id;
  int angle;
};

class CliParser {
public:
  CliCommand parse(const String& rawLine) const;

private:
  int splitTokens(const String& line, String tokens[], int maxTokens) const;
  bool parseIntegerToken(const String& token, int& value) const;
  bool parseVerboseMoveCommand(const String tokens[], int tokenCount, int& id, int& angle) const;
  bool isSetWave1Command(const String tokens[], int tokenCount) const;
  bool isSetWave2Command(const String tokens[], int tokenCount) const;
  bool isSetOpenCommand(const String tokens[], int tokenCount) const;
  bool isGetStatusCommand(const String tokens[], int tokenCount) const;
};

#endif
