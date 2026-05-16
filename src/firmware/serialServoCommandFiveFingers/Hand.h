#ifndef HAND_H
#define HAND_H

#include "Finger.h"

class Hand {
  private:
    enum WaveDirection { WAVE_FORWARD, WAVE_REVERSE };
    enum WaveState { WAVE_IDLE, WAVE_OPENING, WAVE_WAIT_OPEN, WAVE_CLOSING, WAVE_WAIT_CLOSE };
    enum OpenState { OPEN_IDLE, OPEN_MOVING, OPEN_WAIT };
    Finger fingers[5];
    const int* pins;
    FingerMotionConfig motionConfig;
    int maxAngle;
    bool waveActive;
    bool openActive;
    int waveFinger;
    int openFinger;
    WaveDirection waveDirection;
    WaveState waveState;
    OpenState openState;
    bool anyFingerActive() const;
    bool startWave(WaveDirection direction);

  public:
    Hand(const int* servoPins, int closeMaxAngle, const FingerMotionConfig& config);
    void begin();
    void commandFinger(int id, int angle);
    bool startWave1();
    bool startWave2();
    bool startOpen();
    bool isOpenActive() const;
    bool isWaveActive() const;
    bool isWaveReverse() const;
    bool isBusy() const;
    int getMaxAngle() const;
    void update();
};

#endif


