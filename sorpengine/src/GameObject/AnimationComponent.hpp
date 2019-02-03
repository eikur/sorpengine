#pragma once

#include "MathGeoLibFwd.h"
#include "Component.hpp"
#include <map>
#include <list>

class GameObject;

struct Animation;
struct AnimationChannel;

class AnimationComponent : public Component
{
    using ChannelIdToGameObject = std::list<std::pair<int, GameObject*>>;

  public:
    AnimationComponent(const Animation* animation);

    // Component methods
    bool init() override;
    UpdateStatus update(float dt = 0.0f) override;
	bool cleanUp() override;

    void onEditor() override;

  private:
      void updateChannelGameObject(const AnimationChannel& channel, GameObject& gameObject);
      
      float3 interpolateFloat3(float3 start, float3 finish, const float lambda);
      Quat interpolateQuat(const Quat& start, const Quat& finish, const float lambda);

      const Animation* _currentAnimation = nullptr;
      float _elapsedTimeCurrent = 0.f;
      bool _loopCurrent = true;

      const Animation* _nextAnimation = nullptr;
      float _blendDuration = 0.f;
      float _blendElapsedTime = 0.f;

      ChannelIdToGameObject _channelGameObjects;
};