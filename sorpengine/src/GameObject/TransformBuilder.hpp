#pragma once

#include "Transform.hpp"

class TransformBuilder
{
public:
	TransformBuilder() : _pos(float3::zero), _rot(Quat::identity), _scale(float3::one)	{}

	std::shared_ptr<Transform> build() const
	{
		return std::make_shared<Transform>(_pos, _rot, _scale);
	}
	const TransformBuilder& operator() ()
	{ 
		return *this; 
	};

	TransformBuilder& withPos(const float3 pos) { 
		_pos = pos; 
		return *this;
	}

	TransformBuilder& withRot(const Quat rot) { 
		_rot = rot; 
		return *this;
	}
	TransformBuilder& withScale(const float3 scale) { 
		_scale = scale; 
		return *this;
	}

private:
	float3 _pos = float3::zero;
	Quat _rot = Quat::identity;
	float3 _scale = float3::zero;
};