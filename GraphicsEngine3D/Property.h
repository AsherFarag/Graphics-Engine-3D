#pragma once

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <variant>
#include <tuple>

enum class EPropertyType
{
	Int,
	Float,
	Vec2, Vec3, Vec4,
	Bool,
};

class Property
{
public:
//	template<class T>
//	T as() { throw new NotImplementedException(); }
//
//	template<>
//	int as()
//	{
//		assert(m_Type == PropertyType::Int, "Invalid type");
//		return std::get<float>(m_Value);
//	}
//
//	template<>
//	float as()
//	{
//		assert(m_Type == PropertyType::Float, "Invalid type");
//
//		return ;
//	}
//
//private:
//	EPropertyType m_Type;
//	std::variant<int, unsigned int, float, double, vec3> m_Value;
};