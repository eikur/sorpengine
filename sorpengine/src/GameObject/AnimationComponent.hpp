#pragma once

#include "MathGeoLibFwd.h"
#include "Component.hpp"
#include <map>

class GameObject;

struct Animation;
struct AnimationChannel;

class AnimationComponent : public Component
{
    using ChannelIdToGameObject = std::map<int, GameObject*>;

  public:
    AnimationComponent(Animation* animation);

    // Component methods
    bool init() override;
    UpdateStatus update(float dt = 0.0f) override;
    void onEditor() override;

  private:
      void updateChannelGameObject(const AnimationChannel& channel, GameObject& gameObject);
      
      float3 interpolateFloat3(float3 start, float3 finish, float lambda);
      Quat interpolateQuat(const Quat& start, const Quat& finish, const float lambda);

      Animation* _currentAnimation = nullptr;
      float _elapsedTimeCurrent = 0.f;
      bool _loopCurrent = true;

      Animation* _nextAnimation = nullptr;
      float _blendDuration = 0.f;
      float _blendElapsedTime = 0.f;

      ChannelIdToGameObject _channelGameObjects;
};