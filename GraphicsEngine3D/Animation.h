#pragma once
#include <map>
#include <vector>
#include <type_traits>
#include "RResource.h"

using TimeType = float;

template < typename T >
struct KeyFrame
{
	using DataType = T;

	DataType Data;
	TimeType Time;
};

using PositionKeyFrame = KeyFrame< vec3 >;
using RotationKeyFrame = KeyFrame< quat >;
using ScaleKeyFrame	   = KeyFrame< vec3 >;

template < typename T >
struct AnimationTrack
{
	using KeyFrameType = T;
	using DataType = typename KeyFrameType::DataType;

	std::vector< KeyFrameType > KeyFrames;

	const T& GetKeyFrame(size_t a_Index ) const { return KeyFrames[ a_Index ]; }
	DataType& GetData(size_t a_Index ) { return KeyFrames[ a_Index ].Data; }
	const DataType& GetData(size_t a_Index ) const { return KeyFrames[ a_Index ].Data; }
	/* Additive Animation */
	DataType GetData( TimeType a_Time, size_t a_Before, size_t a_After )
	{
		if (a_After >= KeyFrames.size() )
			return KeyFrames[ a_Before ].Data;

		const KeyFrameType& Before = KeyFrames[ a_Before ];
		const KeyFrameType& After = KeyFrames[ a_After ];

		TimeType BeforeTime = Before.Time;
		TimeType AfterTime = After.Time;

		TimeType DiffTime = AfterTime - BeforeTime;
		TimeType OffTime = a_Time - BeforeTime;

		// Get the Normalised Time in between the before and after keyframes
		TimeType NormTime = OffTime / DiffTime;

		DataType BeforeData = Before.Data;
		DataType AfterData = After.Data;

		DataType DiffData;
		DataType OffData;
		if constexpr ( std::is_same_v<T, RotationKeyFrame> )
		{
			DiffData = AfterData * glm::inverse( BeforeData );
			OffData = glm::normalize( glm::slerp( BeforeData, AfterData, NormTime ) );
		}
		else
		{
			//DiffData = AfterData - BeforeData;
			//OffData = DiffData * NormTime;
			OffData = glm::mix( BeforeData, AfterData, NormTime );
		}

		return OffData;
	}
	DataType GetData( TimeType a_Time )
	{
		//for ( auto& KeyFrame : KeyFrames )
		for ( size_t i = 0; i < KeyFrames.size(); ++i )
		{
			auto& KeyFrame = KeyFrames[ i ];

			if ( a_Time == KeyFrame.Time )
			{
				return KeyFrame.Data;
			}

			if ( a_Time > KeyFrame.Time )
			{
				return GetData( a_Time, i, i + 1 );
			}
		}
	}
};

using PositionAnimationTrack = AnimationTrack< PositionKeyFrame >;
using RotationAnimationTrack = AnimationTrack< RotationKeyFrame >;
using ScaleAnimationTrack = AnimationTrack< ScaleKeyFrame >;

struct BoneAnimation
{
	PositionAnimationTrack PositionTrack;
	RotationAnimationTrack RotationTrack;
	ScaleAnimationTrack ScaleTrack;
};

class Animation : public RResource
{
	friend class AnimationLoader;

public:
	Animation();

	std::map< string, BoneAnimation > BoneAnimations;

	const auto GetDuration() { return m_Duration; }
	const auto GetTickRate() { return m_TicksPerSecond; }
	const auto GetPlayLength() { return m_Duration / m_TicksPerSecond; }

protected:
	TimeType m_Duration;
	unsigned int m_TicksPerSecond;
};

using AnimationHandle = std::shared_ptr < Animation >;