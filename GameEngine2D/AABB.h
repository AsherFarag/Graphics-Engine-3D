#pragma once

#include "glm/glm.hpp"
using namespace glm;

class AABB
{
public:
	AABB(const vec2 a_Pos = vec2(), const vec2 a_Size = vec2(1));

	vec2 GetPosition() { return m_Pos; }
	void SetPosition(vec2 a_Pos = vec2()) { m_Pos = a_Pos; }
	vec2 GetSize() { return m_Size; }
	void SetSize(vec2 a_Size = vec2(1)) { m_Size = a_Size; }

	constexpr bool Inside(const vec2& a_Point) const;
	constexpr bool Inside(const AABB& a_Rect) const;
	constexpr bool Overlaps(const AABB& a_Rect) const;

protected:
	vec2 m_Pos;
	vec2 m_Size;
};

