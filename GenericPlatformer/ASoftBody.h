#pragma once

#include "AActor.h"

// std
#include <string>

class ASoftBody
	: public AActor
{
public:
	enum ESoftBodyShape
	{
		MAGNET,
		BOX,
	};

public:
	ASoftBody() {};
	ASoftBody(ESoftBodyShape a_Shape, vec2 a_Position = vec2(), float a_Damping = 5.f, float a_SpringForce = 100.f, float a_Spacing = 30.f);
	virtual ~ASoftBody() = default;

	void Build(vec2 a_Position, float a_Damping, float a_SpringForce, float a_Spacing, std::vector<std::string>& a_Joints);

protected:
};

