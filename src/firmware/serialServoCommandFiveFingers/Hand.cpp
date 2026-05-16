#include "Hand.h"

Hand::Hand(const int* servoPins, int closeMaxAngle, const FingerMotionConfig& config)
        : pins(servoPins),
            motionConfig(config),
            maxAngle(constrain(closeMaxAngle, 0, 180)),
            waveActive(false),
            openActive(false),
            waveFinger(0),
            openFinger(0),
            waveDirection(WAVE_FORWARD),
            waveState(WAVE_IDLE),
            openState(OPEN_IDLE) {}

void Hand::begin() {
    for (int i = 0; i < 5; i++) {
        fingers[i].init(pins[i], motionConfig);
    }
}

void Hand::commandFinger(int id, int angle) {
    if (waveActive || openActive) {
        return;
    }

    if (id >= 1 && id <= 5) {
        fingers[id - 1].move(constrain(angle, 0, maxAngle));
    }
}

bool Hand::startWave(WaveDirection direction) {
    if (waveActive || openActive) {
        return false;
    }

    waveActive = true;
    waveDirection = direction;
    waveFinger = (direction == WAVE_FORWARD) ? 0 : 4;
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
    for (int i = 0; i < 5; i++) {
        if (fingers[i].isBusy()) {
            return true;
        }
    }

    return false;
}

bool Hand::isBusy() const {
    return waveActive || openActive || anyFingerActive();
}

int Hand::getMaxAngle() const {
    return maxAngle;
}

void Hand::update() {
    for (int i = 0; i < 5; i++) {
        fingers[i].tick();
    }

    if (waveActive) {
        if (waveState == WAVE_OPENING) {
            fingers[waveFinger].move(maxAngle);
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
                if (waveFinger < 0 || waveFinger >= 5) {
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