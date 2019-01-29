#pragma once

#include "Component.hpp"

struct Animation;

class AnimationComponent : public Component
{
  public:
    AnimationComponent(Animation* animation);

    UpdateStatus update(float dt = 0.0f) override;

    void onEditor() override;

  private:
      Animation* _currentAnimation = nullptr;
      float _elapsedTimeCurrent = 0.f;
      bool _loopCurrent = true;

      Animation* _nextAnimation = nullptr;
      float _blendDuration = 0.f;
      float _blendElapsedTime = 0.f;
};