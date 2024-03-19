#pragma once

#include <glm/mat4x4.hpp>
using namespace glm;

class FMath
{
public:
	static mat4 MakeRotationXMatrix(float AngleRad)
	{
		mat4 Matrix;
		Matrix[0][0] = 1.0f;
		Matrix[1][1] = cosf(AngleRad);
		Matrix[1][2] = sinf(AngleRad);
		Matrix[2][1] = -sinf(AngleRad);
		Matrix[2][2] = cosf(AngleRad);
		Matrix[3][3] = 1.0f;
		return Matrix;
	}

	static mat4 MakeRotationYMatrix(float AngleRad)
	{
		mat4 Matrix;
		Matrix[0][0] = cosf(AngleRad);
		Matrix[0][2] = sinf(AngleRad);
		Matrix[2][0] = -sinf(AngleRad);
		Matrix[1][1] = 1.0f;
		Matrix[2][2] = cosf(AngleRad);
		Matrix[3][3] = 1.0f;
		return Matrix;
	}

	static mat4 MakeRotationZMatrix(float AngleRad)
	{
		mat4 Matrix;
		Matrix[0][0] = cosf(AngleRad);
		Matrix[0][1] = sinf(AngleRad);
		Matrix[1][0] = -sinf(AngleRad);
		Matrix[1][1] = cosf(AngleRad);
		Matrix[2][2] = 1.0f;
		Matrix[3][3] = 1.0f;
		return Matrix;
	}
};