#include "Finger.h"

Finger::Finger()
    : active(false),
      target(0),
      currentAngle(0),
      maxSpeedDegPerMs(0.3f),
      overshootDeg(5),
      overshootDelayMs(300),
      state(IDLE) {}

void Finger::init(int pin, const FingerMotionConfig& config) {
    currentAngle = 0;
    maxSpeedDegPerMs = (config.maxSpeedDegPerMs > 0.0f) ? config.maxSpeedDegPerMs : 0.3f;
    overshootDeg = constrain(config.overshootDeg, 0, 20);
    overshootDelayMs = constrain(config.overshootDelayMs, 0, 2000);
    motor.attach(pin, 500, 2500);
    motor.write(0);
    lastUpdate = millis();
}

void Finger::move(int angle) {
    target = constrain(angle, 0, 180);
    active = true;
    state = MOVING_TO_OVERSHOOT;
    lastUpdate = millis();
}

void Finger::tick() {
    if (!active) return;
    
    unsigned long now = millis();
    unsigned long elapsed = now - lastUpdate;
    float maxDelta = elapsed * maxSpeedDegPerMs;
    int overshootTarget = min(target + overshootDeg, 180);
    
    if (state == MOVING_TO_OVERSHOOT) {
        if (maxDelta < 1.0f) return;

        int step = (int)maxDelta;

        // Move gradually to target + overshoot
        if (currentAngle < overshootTarget) {
            currentAngle = min(currentAngle + step, overshootTarget);
        } else if (currentAngle > overshootTarget) {
            currentAngle = max(currentAngle - step, overshootTarget);
        }
        
        motor.write(currentAngle);
        lastUpdate = now;
        
        // Reached overshoot, wait before returning
        if (currentAngle == overshootTarget) {
            state = WAITING;
            overshootTimer = now;
        }
    }
    else if (state == WAITING) {
        // Wait overshootDelayMs before returning
        if (now - overshootTimer >= (unsigned long)overshootDelayMs) {
            state = RETURNING;
            lastUpdate = now;
        }
    }
    else if (state == RETURNING) {
        if (maxDelta < 1.0f) return;

        int step = (int)maxDelta;

        // Move gradually back to target
        if (currentAngle > target) {
            currentAngle = max(currentAngle - step, target);
        } else if (currentAngle < target) {
            currentAngle = min(currentAngle + step, target);
        }
        
        motor.write(currentAngle);
        lastUpdate = now;
        
        // Reached final target
        if (currentAngle == target) {
            state = IDLE;
            active = false;
        }
    }
}

bool Finger::isBusy() const {
    return active;
}