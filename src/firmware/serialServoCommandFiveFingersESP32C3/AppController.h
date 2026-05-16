#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include <Arduino.h>
#include "Hand.h"
#include "CliParser.h"

class AppController {
public:
  AppController(const int* servoPins, const int* closeMaxAngles, const FingerMotionConfig& motionConfig);
  void begin();
  void update();

private:
  Hand hand;
  CliParser parser;
  const int* maxCloseAngles;
  FingerMotionConfig motionConfig;

  bool isValidFingerId(int id) const;
  void flushSerialInput();
  void handleWaveCommand();
  void handleWave2Command();
  void handleOpenCommand();
  void handleFingerPoseCommand(int openFingerId, const __FlashStringHelper* label);
  void handleGetStatusCommand();
  void handleFingerCommand(int id, int angle);
  void handleCliLine(const String& line);
};

#endif
