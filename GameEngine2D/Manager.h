#pragma once

#include <vector>

#include "glm/glm.hpp"
using namespace glm;

class Manager
{
public:
	virtual void Init() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void Draw() = 0;
};

