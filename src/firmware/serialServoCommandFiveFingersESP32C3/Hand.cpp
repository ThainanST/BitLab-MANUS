#include "Hand.h"

Hand::Hand(const int* servoPins, const int* closeMaxAngles, const FingerMotionConfig& config)
        : pins(servoPins),
            motionConfig(config),
            waveActive(false),
            openActive(false),
            waveFinger(0),
            openFinger(0),
            waveDirection(WAVE_FORWARD),
            waveState(WAVE_IDLE),
            openState(OPEN_IDLE) {
    for (int i = 0; i < FINGER_COUNT; i++) {
        maxAngles[i] = constrain(closeMaxAngles[i], 0, 180);
    }
}

void Hand::begin() {
    for (int i = 0; i < FINGER_COUNT; i++) {
        fingers[i].init(pins[i], motionConfig);
    }
}

void Hand::commandFinger(int id, int angle) {
    if (waveActive || openActive) {
        return;
    }

    if (id >= 1 && id <= FINGER_COUNT) {
        fingers[id - 1].move(constrain(angle, 0, maxAngles[id - 1]));
    }
}

bool Hand::startWave(WaveDirection direction) {
    if (waveActive || openActive) {
        return false;
    }

    waveActive = true;
    waveDirection = direction;
    waveFinger = (direction == WAVE_FORWARD) ? 0 : (FINGER_COUNT - 1);
    waveState = WAVE_OPENING;
    return true;
}

bool Hand::startWave1() {
    return startWave(WAVE_FORWARD);
}

bool Hand::startWave2() {
    return startWave(WAVE_REVERSE);
}

bool Hand::startOpen() {
    if (waveActive || openActive) {
        return false;
    }

    openActive = true;
    openFinger = 0;
    openState = OPEN_MOVING;
    return true;
}

bool Hand::startPose(int openFingerId) {
    if (waveActive || openActive) {
        return false;
    }

    if (openFingerId < 1 || openFingerId > FINGER_COUNT) {
        return false;
    }

    for (int i = 0; i < FINGER_COUNT; i++) {
        if (i == openFingerId - 1) {
            fingers[i].move(0);
        } else {
            fingers[i].move(maxAngles[i]);
        }
    }

    return true;
}

bool Hand::isOpenActive() const {
    return openActive;
}

bool Hand::isWaveActive() const {
    return waveActive;
}

bool Hand::isWaveReverse() const {
    return waveActive && waveDirection == WAVE_REVERSE;
}

bool Hand::anyFingerActive() const {
    for (int i = 0; i < FINGER_COUNT; i++) {
        if (fingers[i].isBusy()) {
            return true;
        }
    }

    return false;
}

bool Hand::isBusy() const {
    return waveActive || openActive || anyFingerActive();
}

int Hand::getMaxAngle(int id) const {
    if (id < 1 || id > FINGER_COUNT) {
        return 0;
    }

    return maxAngles[id - 1];
}

void Hand::update() {
    for (int i = 0; i < FINGER_COUNT; i++) {
        fingers[i].tick();
    }

    if (waveActive) {
        if (waveState == WAVE_OPENING) {
            fingers[waveFinger].move(maxAngles[waveFinger]);
            waveState = WAVE_WAIT_OPEN;
        } else if (waveState == WAVE_WAIT_OPEN) {
            if (!fingers[waveFinger].isBusy()) {
                waveState = WAVE_CLOSING;
            }
        } else if (waveState == WAVE_CLOSING) {
            fingers[waveFinger].move(0);
            waveState = WAVE_WAIT_CLOSE;
        } else if (waveState == WAVE_WAIT_CLOSE) {
            if (!fingers[waveFinger].isBusy()) {
                waveFinger += (waveDirection == WAVE_FORWARD) ? 1 : -1;
                if (waveFinger < 0 || waveFinger >= FINGER_COUNT) {
                    waveActive = false;
                    waveState = WAVE_IDLE;
                } else {
                    waveState = WAVE_OPENING;
                }
            }
        }
    }

    if (openActive) {
        if (openState == OPEN_MOVING) {
            fingers[openFinger].move(0);
            openState = OPEN_WAIT;
        } else if (openState == OPEN_WAIT) {
            if (!fingers[openFinger].isBusy()) {
                openFinger++;
                if (openFinger >= 5) {
                    openActive = false;
                    openState = OPEN_IDLE;
                } else {
                    openState = OPEN_MOVING;
                }
            }
        }
    }
}