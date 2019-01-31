#pragma once

#include "SDL/include/SDL.h"

class Timer
{
    enum class State
    {
        Paused,
        Running,
        Stopped
    };

  public:
    void start();
    void togglePause();
    void stop();

    void frameWasCompleted();
    float delta() const;

    void setTimeScale(const float timeScale);
    bool isRunning() const;
    bool isPaused() const;

  private:
    Uint64 _startTime = 0;
    Uint64 _previousFrameTime = 0;
    Uint64 _currentFrameTime = 0;
    float _timeScale = 1.f;

    State _state = State::Stopped;

    static const Uint64 _frequency;
};
