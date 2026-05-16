#include "CliParser.h"

CliCommand CliParser::parse(const String& rawLine) const {
  String line = rawLine;
  line.trim();

  if (line.length() == 0) {
    return {COMMAND_NONE, 0, 0};
  }

  String tokens[6];
  int tokenCount = splitTokens(line, tokens, 6);
  int id = 0;
  int angle = 0;

  if (isSetWave1Command(tokens, tokenCount)) {
    return {COMMAND_SET_WAVE1, 0, 0};
  }

  if (isSetWave2Command(tokens, tokenCount)) {
    return {COMMAND_SET_WAVE2, 0, 0};
  }

  if (isSetOpenCommand(tokens, tokenCount)) {
    return {COMMAND_SET_OPEN, 0, 0};
  }

  if (isGetStatusCommand(tokens, tokenCount)) {
    return {COMMAND_GET_STATUS, 0, 0};
  }

  if (parseVerboseMoveCommand(tokens, tokenCount, id, angle)) {
    return {COMMAND_FINGER, id, angle};
  }

  return {COMMAND_INVALID, 0, 0};
}

int CliParser::splitTokens(const String& line, String tokens[], int maxTokens) const {
  int count = 0;
  int start = -1;

  for (int i = 0; i <= line.length(); i++) {
    bool atEnd = (i == line.length());
    bool isSpace = !atEnd && (line[i] == ' ' || line[i] == '\t');

    if (!atEnd && !isSpace && start < 0) {
      start = i;
    }

    if ((atEnd || isSpace) && start >= 0) {
      if (count < maxTokens) {
        tokens[count] = line.substring(start, i);
      }
      count++;
      start = -1;
    }
  }

  return count;
}

bool CliParser::parseIntegerToken(const String& token, int& value) const {
  if (token.length() == 0) {
    return false;
  }

  int start = 0;
  if (token[0] == '+' || token[0] == '-') {
    start = 1;
  }

  if (start >= token.length()) {
    return false;
  }

  for (int i = start; i < token.length(); i++) {
    if (token[i] < '0' || token[i] > '9') {
      return false;
    }
  }

  value = token.toInt();
  return true;
}

bool CliParser::parseVerboseMoveCommand(const String tokens[], int tokenCount, int& id, int& angle) const {
  if (tokenCount != 5) {
    return false;
  }

  if (!tokens[0].equalsIgnoreCase("mv") ||
      !tokens[1].equalsIgnoreCase("finger") ||
      !tokens[3].equalsIgnoreCase("to")) {
    return false;
  }

  return parseIntegerToken(tokens[2], id) && parseIntegerToken(tokens[4], angle);
}

bool CliParser::isSetWave1Command(const String tokens[], int tokenCount) const {
  if (tokenCount == 2 &&
      tokens[0].equalsIgnoreCase("set") &&
      tokens[1].equalsIgnoreCase("wave1")) {
    return true;
  }

  return false;
}

bool CliParser::isSetWave2Command(const String tokens[], int tokenCount) const {
  if (tokenCount == 2 &&
      tokens[0].equalsIgnoreCase("set") &&
      tokens[1].equalsIgnoreCase("wave2")) {
    return true;
  }

  return false;
}

bool CliParser::isSetOpenCommand(const String tokens[], int tokenCount) const {
  if (tokenCount == 2 &&
      tokens[0].equalsIgnoreCase("set") &&
      tokens[1].equalsIgnoreCase("open")) {
    return true;
  }

  return false;
}

bool CliParser::isGetStatusCommand(const String tokens[], int tokenCount) const {
  if (tokenCount == 2 &&
      tokens[0].equalsIgnoreCase("get") &&
      tokens[1].equalsIgnoreCase("status")) {
    return true;
  }

  return false;
}
