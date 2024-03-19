#pragma once

#include "AABB.h"

// --- std ---
#include <array>
#include <vector>
#include <list>
#include <memory>

constexpr size_t MAX_DEPTH = 8;

template <typename TObject>
class StaticQuadTree
{
//public:
//	StaticQuadTree(const AABB& a_Area = AABB(vec2(), vec2(1280, 720)), const size_t a_Depth = 0)
//	{
//		m_Depth = a_Depth;
//		Resize(a_Area);
//	}
//
//	void Resize(const AABB& a_Area)
//	{
//		Clear();
//		m_Area = a_Area;
//		vec2 ChildSize = m_Area.GetSize() * 0.5f;
//
//		m_ChildAreas =
//		{
//			AABB(m_Area.GetPosition(), ChildSize),												 // Bottom Left
//			AABB(vec2(m_Area.GetPosition().x + ChildSize.x, m_Area.GetPosition().y), ChildSize), // Bottom Right
//			AABB(vec2(m_Area.GetPosition().x, m_Area.GetPosition().y + ChildSize.y), ChildSize), // Top Left
//			AABB(m_Area.GetPosition() + ChildSize, ChildSize)									 // Top Right
//		};
//	}
//
//	void Clear()
//	{
//		m_Objects.clear();
//
//		for (int i = 0; i < 4; ++i)
//		{
//			if (m_ChildQuads[i])
//			{
//				m_ChildQuads[i]->Clear();
//				//m_ChildQuads[i]->Reset();
//			}
//		}
//	}
//
//	size_t Size() const
//	{
//		size_t Count = m_Objects.size();
//		for (int i = 0; i < 4; ++i)
//			if (m_ChildQuads[i]) Count += m_ChildQuads[i].Size();
//
//		return Count;
//	}
//
//	void Insert(TObject* a_Object, AABB* a_ObjectSize)
//	{
//		for (int i = 0; i < 4; ++i)
//		{
//			// Is the Object inside this area
//			if (m_ChildAreas[i].Inside(*a_ObjectSize) == false)
//				continue;
//
//			// Have we reached the Max Depth
//			if (m_Depth + 1 >= MAX_DEPTH)
//				continue;
//
//			// Does the Child Exist
//			if (!m_ChildQuads[i])
//				// Create the Child if not
//				m_ChildQuads[i] = std::make_shared< StaticQuadTree<TObject> >(m_ChildAreas[i], m_Depth + 1);
//
//			// Recursive Call
//			m_ChildQuads[i]->Insert(a_Object, a_ObjectSize);
//			return;
//		}
//
//		// Object didn't fit
//		m_Objects.push_back({ a_ObjectSize, a_Object });
//	}
//
//	std::list<TObject> Search(const AABB& a_Area)
//	{
//		std::list<TObject> ObjectsInside;
//		Search(a_Area, ObjectsInside);
//		return ObjectsInside;
//	}
//
//protected:
//	void Search(const AABB& a_Area, std::list<TObject>& ObjectsInside) const
//	{
//		// Check for objects in this Quad and add them to the list
//		for (const auto& Object : m_Objects)
//		{
//			if (a_Area.Overlaps(Object.first))
//				ObjectsInside.push_back(Object.second);
//		}
//
//		// Check if Child Quads can add to the list
//		for (int i = 0; i < 4; ++i)
//		{
//			if (!m_ChildQuads[i])
//				continue;
//
//			if (a_Area.Inside(m_ChildAreas[i]))
//				m_ChildQuads[i]->PushObjects(ObjectsInside);
//			else if (m_ChildAreas[i].Overlaps(a_Area))
//				m_ChildQuads[i]->Search(a_Area, ObjectsInside);
//		}
//	}
//
//	void PushObjects(std::list<TObject>& ObjectsInside) const
//	{
//		for (const auto& Object : m_Objects)
//			ObjectsInside.push_back(Object.second);
//
//		for (int i = 0; i < 4; ++i)
//		{
//			if (m_ChildQuads[i])
//				m_ChildQuads[i]->PushObjects(ObjectsInside);
//		}
//	}
//
//	const AABB& Area() { return m_Area; }
//
//protected:
//	size_t m_Depth;
//
//	AABB m_Area;
//
//	// 4 Areas of the children
//	std::array<AABB, 4> m_ChildAreas{};
//	// 4 Potential Children
//	std::array< std::shared_ptr<StaticQuadTree<TObject>>, 4 > m_ChildQuads{};
//	// Objects in this Quad
//	std::vector<std::pair<AABB*, TObject*>> m_Objects;
};