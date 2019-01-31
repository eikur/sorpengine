#include "Timer.hpp"

const Uint64 Timer::_frequency = SDL_GetPerformanceFrequency();

void Timer::start()
{
    if (_state != State::Stopped)
    {
        return;
    }

    _startTime = SDL_GetPerformanceCounter();
    _previousFrameTime = _startTime;
    _currentFrameTime = _startTime;

    _state = State::Running;
}

void Timer::togglePause()
{
    switch (_state)
    {
        case State::Stopped:
        {
            break;
        }
        case State::Running:
        {
            _previousFrameTime = _currentFrameTime;
            _state = State::Paused;
            break;
        }
        case State::Paused:
        {
            const Uint64 now = SDL_GetPerformanceCounter();
            const Uint64 elapsedTimeInPause = now - _previousFrameTime;

            _startTime += elapsedTimeInPause;
            _previousFrameTime = now;
            _currentFrameTime = now;

            _state = State::Running;
        }
    }
}

void Timer::stop()
{
    _previousFrameTime = _currentFrameTime;
    _state = State::Stopped;
}

float Timer::delta() const
{
    return _timeScale * float(_currentFrameTime - _previousFrameTime) * 1000.f / float(_frequency);
}

void Timer::frameWasCompleted()
{
    _previousFrameTime = _currentFrameTime;
    if (_state == State::Running)
    {
        _currentFrameTime = SDL_GetPerformanceCounter();
    }
}

void Timer::setTimeScale(const float timeScale)
{
    _timeScale = timeScale;
}

bool Timer::isRunning() const
{
    return _state == State::Running;
}

bool Timer::isPaused() const
{
    return _state == State::Paused;
}