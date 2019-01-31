#pragma once

#include "Timer.hpp"

class TimeManager
{
public:
    TimeManager();

    void frameWasCompleted();

    float getAppDeltaInSec() const;
    float getAppFrameRate() const;

    bool isGameRunning() const;
    bool isGamePaused() const;
    void startGame();
    void togglePauseGame();
    void stopGame();

    float getGameDelta() const;
    void setTimeScale(const float timeScale);

private:
    Timer _appTimer;
    Timer _gameTimer;
};
