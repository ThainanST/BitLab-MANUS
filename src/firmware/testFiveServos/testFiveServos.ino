#include <Servo.h>

#define PULSE_MIN  500
#define PULSE_MAX  2500
#define STEP_DELAY 15 

const int servoPins[] = {9, 10, 11, 12, 13};
const int numServos = sizeof(servoPins) / sizeof(servoPins[0]);
Servo myServos[numServos];

void attachServoPins(Servo* servos, const int* pins, int n);

void setup() {
  attachServoPins(myServos, servoPins, numServos);
}

void loop() {
  for (int pos = 0; pos <= 180; pos++) {
    updateServos(pos);
    delay(STEP_DELAY);
  }
  for (int pos = 180; pos >= 0; pos--) {
    updateServos(pos);
    delay(STEP_DELAY);
  }
}

void updateServos(int angulo) {
  for (int i = 0; i < numServos; i++) {
    myServos[i].write(angulo);
  }
}

void attachServoPins(Servo* servos, const int* pins, int n) {
  for (int i = 0; i < n; i++) {
    servos[i].attach(pins[i], PULSE_MIN, PULSE_MAX);
  }
}