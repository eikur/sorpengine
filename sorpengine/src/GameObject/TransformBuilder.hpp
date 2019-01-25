#pragma once

#include "Transform.hpp"
#include "Macros.hpp"

class TransformBuilder
{
	using Class = TransformBuilder;
	
    const float3 kDefaultPos = float3(0.f, 0.f, -5.f);
    const float kDefaultRot = 0.f;
    const float2 kDefaultScale = float2::one;

  public:
	WITH(Position, float3, _pos)
	WITH(Rotation, float, _rot)
	WITH(Scale, float2, _scale)

	TransformBuilder() : _pos(kDefaultPos), _rot(kDefaultRot), _scale(kDefaultScale)	{}

	std::shared_ptr<Transform> build() const
	{
		return std::make_shared<Transform>(_pos, _rot, _scale);
	}
	const TransformBuilder& operator() ()
	{ 
		return *this; 
	};
};