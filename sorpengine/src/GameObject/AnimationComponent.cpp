#include "AnimationComponent.hpp"

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

UpdateStatus AnimationComponent::update(float /*dt*/)
{
    if (!_active || !_currentAnimation)
    {
        return;
    }

    // update animation and stuff
}
