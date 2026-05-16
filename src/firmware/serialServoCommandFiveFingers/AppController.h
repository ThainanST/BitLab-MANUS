#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <Arduino.h>
#include "Hand.h"
#include "CliParser.h"

class AppController {
public:
  AppController(const int* servoPins, int closeMaxAngle, const FingerMotionConfig& motionConfig);
  void begin();
  void update();

private:
  Hand hand;
  CliParser parser;
  int maxCloseAngle;
  FingerMotionConfig motionConfig;

  bool isValidFingerId(int id) const;
  void flushSerialInput();
  void handleWaveCommand();
  void handleWave2Command();
  void handleOpenCommand();
  void handleGetStatusCommand();
  void handleFingerCommand(int id, int angle);
  void handleCliLine(const String& line);
};

#endif
