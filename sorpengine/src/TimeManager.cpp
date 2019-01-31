#include "TimeManager.hpp"

TimeManager::TimeManager()
{
    _appTimer.start();
}

void TimeManager::frameWasCompleted()
{
     _appTimer.frameWasCompleted();
     _gameTimer.frameWasCompleted();
}

float TimeManager::getAppDeltaInSec() const
{
    return _appTimer.delta() / 1000.f;
}

float TimeManager::getAppFrameRate() const
{
    return 1.f / getAppDeltaInSec();
}

float TimeManager::getGameDelta() const
{
    return _gameTimer.delta();
}

void TimeManager::setTimeScale(const float timeScale)
{
    _gameTimer.setTimeScale(timeScale);
}

bool TimeManager::isGameRunning() const
{
    return _gameTimer.isRunning();
}

bool TimeManager::isGamePaused() const
{
    return _gameTimer.isPaused();
}

void TimeManager::startGame()
{
    _gameTimer.start();
}

void TimeManager::togglePauseGame()
{
    _gameTimer.togglePause();
}

void TimeManager::stopGame()
{
    _gameTimer.stop();
}
