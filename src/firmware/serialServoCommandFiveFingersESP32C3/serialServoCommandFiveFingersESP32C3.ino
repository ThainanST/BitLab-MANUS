#include "AppController.h"

// CLI commands:
// - set wave1
// - set wave2
// - set open
// - set one
// - set two
// - set three
// - set four
// - set five
// - get status
// - mv finger <id> to <angle>
// - mv finger 2 to 90
// - mv finger 2 to 0


// Hardware configuration
const int SERVO_PINS[] = {3, 4, 5, 6, 7};
const int CLOSE_MAX_ANGLES[] = {80, 120, 130, 100, 100};
const float FINGER_MAX_SPEED_DEG_PER_MS = 0.30f;
const int FINGER_OVERSHOOT_DEG = 5;
const int FINGER_OVERSHOOT_DELAY_MS = 400;

const FingerMotionConfig FINGER_MOTION = {
  FINGER_MAX_SPEED_DEG_PER_MS,
  FINGER_OVERSHOOT_DEG,
  FINGER_OVERSHOOT_DELAY_MS
};

AppController app(SERVO_PINS, CLOSE_MAX_ANGLES, FINGER_MOTION);

void setup() {
  app.begin();
}

void loop() {
  app.update();
}