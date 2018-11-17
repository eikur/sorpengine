#pragma once

#include "Transform.hpp"
#include "Macros.hpp"

class TransformBuilder
{
	using Class = TransformBuilder;
	
  public:
	WITH(Position, float3, _pos)
	WITH(Rotation, float, _rot)
	WITH(Scale, float2, _scale)

	TransformBuilder() : _pos(float3::zero), _rot(0.f), _scale(float2::one)	{}

	std::shared_ptr<Transform> build() const
	{
		return std::make_shared<Transform>(_pos, _rot, _scale);
	}
	const TransformBuilder& operator() ()
	{ 
		return *this; 
	};
};