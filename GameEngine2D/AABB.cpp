#include "AABB.h"

constexpr bool AABB::Inside(const vec2& P) const
{
	return !(P.x < m_Pos.x || P.y < m_Pos.y || P.x >= (m_Pos.x + m_Size.x) || P.y >= (m_Pos.y + m_Size.y));
}

constexpr bool AABB::Inside(const AABB& R) const
{
	return (R.m_Pos.x >= m_Pos.x) && (R.m_Pos.x + R.m_Size.x < m_Pos.x + m_Size.x) && (R.m_Pos.y >= m_Pos.y) && (R.m_Pos.y + R.m_Size.y < m_Pos.y + m_Size.y);
}

constexpr bool AABB::Overlaps(const AABB& R) const
{
	return (m_Pos.x < R.m_Pos.x + R.m_Size.x && m_Pos.x + m_Size.x >= R.m_Pos.x && m_Pos.y < R.m_Pos.y + R.m_Size.y && m_Pos.y + m_Size.y >= R.m_Pos.y);
}
