#pragma once

#include "MathGeoLib\include\MathGeoLib.h"
#include <vector>

struct Node
{
    std::string name;
    Node* parent = nullptr;

    float3 position = float3::zero;
    Quat rotation = Quat::identity;
    float3 scale = float3::one;

    std::vector<Node> children;
    std::vector<size_t> meshIds;
    std::vector<size_t> materialIds;
    std::vector<size_t> animationIds;
};

struct AnimationChannel
{
    std::string name;
    std::vector<float3> positions;
    std::vector<Quat> rotations;
};

struct Animation
{
    size_t id;
    std::string name;
    float duration = 0.f;
    std::vector<AnimationChannel> channels;
};