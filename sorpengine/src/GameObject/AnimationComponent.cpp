#include "AnimationComponent.hpp"

#include "GameObject/GameObject.hpp"
#include "ModelHelper.fwd.hpp"
#include "ImGui/imgui.h"

AnimationComponent::AnimationComponent(Animation* animation) 
    : Component(ComponentType::Animation, animation != nullptr)
    , _currentAnimation(animation)
{
}

void AnimationComponent::onEditor()
{
    if (ImGui::CollapsingHeader("Animation"))
    {
        if (_currentAnimation)
        {
            ImGui::Text("%s animation assigned", _currentAnimation->name.c_str());
        }
        else
        {
            ImGui::Text("No animation has been assigned to this component");
        }
    }
}

UpdateStatus AnimationComponent::update(float dt)
{
    if (!_active || !_currentAnimation)
    {
        return UpdateStatus::Continue;
    }

    _elapsedTimeCurrent += dt;
    //remove me when passing dt from outside 
    _elapsedTimeCurrent += 0.02f;
    if (_elapsedTimeCurrent >= _currentAnimation->duration)
    {
        _elapsedTimeCurrent -= _currentAnimation->duration;
    }

    for (const AnimationChannel& channel : _currentAnimation->channels)
    {
        GameObject* go = _parent->findChildRecursivelyByName(channel.name);
        if (go == nullptr)
        {
            continue;
        }

        updateChannelGameObject(channel, *go);
    }

    return UpdateStatus::Continue;
}

void AnimationComponent::updateChannelGameObject(const AnimationChannel& channel, GameObject& gameObject)
{
    const float& duration = _currentAnimation->duration;
    const size_t numPositions = channel.positions.size();
    const size_t numRotations = channel.rotations.size();

    const float posKey = float(_elapsedTimeCurrent * numPositions) / duration;
    const float rotKey = float(_elapsedTimeCurrent * numRotations) / duration;

    const size_t posIdx = size_t(posKey);
    const size_t rotIdx = size_t(rotKey);

    const float posLambda = posKey - float(posIdx);
    const float rotLambda = rotKey - float(rotIdx);

    float3 newPosition;
    Quat newRotation;

    const bool isLastPosIdx = posIdx == numPositions - 1;
    const bool uniquePosIdx = numPositions == 1;
    if (isLastPosIdx)
    {
        if (uniquePosIdx)
        {
            newPosition = channel.positions.at(posIdx);
        }
        else
        {
            newPosition = interpolateFloat3(channel.positions.at(posIdx), channel.positions.at(0), posLambda);
        }
    }
    else
    {
        newPosition = interpolateFloat3(channel.positions.at(posIdx), channel.positions.at(posIdx + 1), posLambda);
    }

    const bool isLastRotIdx = rotIdx == numRotations - 1;
    const bool uniqueRotIdx = numRotations == 1;
    if (isLastRotIdx)
    {
        if (uniqueRotIdx)
        {
            newRotation = channel.rotations.at(rotIdx);
        }
        else
        {
            newRotation = interpolateQuat(channel.rotations.at(rotIdx), channel.rotations.at(0), posLambda);
        }
    }
    else
    {
        newRotation = interpolateQuat(channel.rotations.at(rotIdx), channel.rotations.at(rotIdx + 1), posLambda);
    }

    gameObject.updateTransform(newPosition, newRotation);
}

float3 AnimationComponent::interpolateFloat3(float3 start, float3 finish, float lambda)
{
    return start * (1.f - lambda) + finish * lambda;
}

Quat AnimationComponent::interpolateQuat(const Quat& start, const Quat& finish, const float lambda)
{
    Quat result;

    float dot = start.x*finish.x + start.y*finish.y + start.z*finish.z + start.w*finish.w;

    if (dot >= 0.0f)
    {
        result.x = start.x*(1.0f - lambda) + finish.x*lambda;
        result.y = start.y*(1.0f - lambda) + finish.y*lambda;
        result.z = start.z*(1.0f - lambda) + finish.z*lambda;
        result.w = start.w*(1.0f - lambda) + finish.w*lambda;
    }
    else
    {
        result.x = start.x*(1.0f - lambda) - finish.x*lambda;
        result.y = start.y*(1.0f - lambda) - finish.y*lambda;
        result.z = start.z*(1.0f - lambda) - finish.z*lambda;
        result.w = start.w*(1.0f - lambda) - finish.w*lambda;
    }

    return result.Normalized();
}