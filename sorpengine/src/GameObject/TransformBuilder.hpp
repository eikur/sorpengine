#pragma once

#include "Transform.hpp"
#include "Macros.hpp"

class TransformBuilder
{
	using Class = TransformBuilder;
	
    const float3 kDefaultPos = float3::zero;
	const Quat kDefaultRot = Quat::identity;
    const float3 kDefaultScale = float3::one;

  public:
	WITH(Position, float3, _pos)
	WITH(Rotation, Quat, _rot)
	WITH(Scale, float3, _scale)

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