#include "AppController.h"

AppController::AppController(const int* servoPins, int closeMaxAngle, const FingerMotionConfig& config)
  : hand(servoPins, closeMaxAngle, config), maxCloseAngle(closeMaxAngle), motionConfig(config) {}

void AppController::begin() {
  Serial.begin(9600);

  hand.begin();

  Serial.println(F("BITLAB-MANUS ready."));
}

void AppController::update() {
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    handleCliLine(line);
    flushSerialInput();
  }

  hand.update();
}

bool AppController::isValidFingerId(int id) const {
  return id >= 1 && id <= 5;
}

void AppController::flushSerialInput() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void AppController::handleWaveCommand() {
  if (hand.startWave1()) {
    Serial.println(F("[SET] Wave1 started."));
    return;
  }

  Serial.println(F("[BUSY] Hand is busy."));
}

void AppController::handleWave2Command() {
  if (hand.startWave2()) {
    Serial.println(F("[SET] Wave2 started."));
    return;
  }

  Serial.println(F("[BUSY] Hand is busy."));
}

void AppController::handleOpenCommand() {
  if (hand.startOpen()) {
    Serial.println(F("[SET] Open sequence started."));
    return;
  }

  Serial.println(F("[BUSY] Hand is busy."));
}

void AppController::handleGetStatusCommand() {
  Serial.print(F("[STATUS] mode="));
  if (hand.isWaveActive()) {
    Serial.print(hand.isWaveReverse() ? F("wave2") : F("wave1"));
  } else if (hand.isOpenActive()) {
    Serial.print(F("open"));
  } else {
    Serial.print(F("idle"));
  }

  Serial.print(F(" busy="));
  Serial.print(hand.isBusy() ? F("yes") : F("no"));
  Serial.print(F(" maxAngle="));
  Serial.print(hand.getMaxAngle());
  Serial.print(F(" maxSpeedDegPerMs="));
  Serial.print(motionConfig.maxSpeedDegPerMs, 3);
  Serial.print(F(" overshootDeg="));
  Serial.print(motionConfig.overshootDeg);
  Serial.print(F(" overshootDelayMs="));
  Serial.println(motionConfig.overshootDelayMs);
}

void AppController::handleFingerCommand(int id, int angle) {
  if (!isValidFingerId(id)) {
    Serial.println(F("[ERROR] Invalid Finger ID. Use 1-5."));
    return;
  }

  if (hand.isBusy()) {
    Serial.println(F("[BUSY] Hand is busy."));
    return;
  }

  Serial.print(F("[INPUT] Commanding Finger "));
  Serial.print(id);
  Serial.print(F(" to position "));
  Serial.print(angle);
  Serial.println(F(" deg."));
  hand.commandFinger(id, angle);
}

void AppController::handleCliLine(const String& line) {
  CliCommand command = parser.parse(line);

  if (command.type == COMMAND_NONE) {
    return;
  }

  if (command.type == COMMAND_SET_WAVE1) {
    handleWaveCommand();
    return;
  }

  if (command.type == COMMAND_SET_WAVE2) {
    handleWave2Command();
    return;
  }

  if (command.type == COMMAND_SET_OPEN) {
    handleOpenCommand();
    return;
  }

  if (command.type == COMMAND_GET_STATUS) {
    handleGetStatusCommand();
    return;
  }

  if (command.type == COMMAND_FINGER) {
    handleFingerCommand(command.id, command.angle);
    return;
  }

  Serial.println(F("[ERROR] Invalid command."));
}
