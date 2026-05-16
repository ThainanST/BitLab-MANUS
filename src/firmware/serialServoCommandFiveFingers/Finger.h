#ifndef FINGER_H
#define FINGER_H

#include <Arduino.h>
#include <Servo.h>

struct FingerMotionConfig {
  float maxSpeedDegPerMs;
  int overshootDeg;
  int overshootDelayMs;
};

class Finger {
  private:
    Servo motor;
    int target;
    int currentAngle;
    unsigned long lastUpdate;
    unsigned long overshootTimer;
    float maxSpeedDegPerMs;
    int overshootDeg;
    int overshootDelayMs;
    bool active;
    enum State { IDLE, MOVING_TO_OVERSHOOT, WAITING, RETURNING } state;

  public:
    Finger();
    void init(int pin, const FingerMotionConfig& config);
    void move(int angle);
    void tick();
    bool isBusy() const;
};

#endif