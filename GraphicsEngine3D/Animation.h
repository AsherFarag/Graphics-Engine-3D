#pragma once
#include <map>
#include <vector>
#include <type_traits>
#include "glm/ext.hpp"
#include "RResource.h"

namespace Animation
{
	using TimeType = float;
}

using namespace Animation;

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

	const T& GetKeyFrame( const size_t a_Index ) const { return KeyFrames[ a_Index ]; }
	DataType& GetData( const size_t a_Index ) { return KeyFrames[ a_Index ].Data; }
	const DataType& GetData( const size_t a_Index ) const { return KeyFrames[ a_Index ].Data; }
	DataType GetData( const TimeType a_Time, const size_t a_Before, const size_t a_After )
	{
		const KeyFrameType& Before = KeyFrames[ a_Before ];
		const KeyFrameType& After = KeyFrames[ a_After ];

		TimeType BeforeTime = Before.Time;
		TimeType AfterTime = After.Time;

		TimeType DiffTime = AfterTime - BeforeTime;
		TimeType OffTime = a_Time - BeforeTime;

		TimeType NormTime = DiffTime != (TimeType)0.0 ? OffTime / DiffTime : (TimeType)0.0;

		DataType BeforeData = Before.Data;
		DataType AfterData = After.Data;

		DataType OffData;
		if constexpr ( std::is_same_v<T, RotationKeyFrame> )
		{
			OffData = glm::normalize( glm::slerp( BeforeData, AfterData, NormTime ) );
		}
		else
		{
			OffData = glm::mix( BeforeData, AfterData, NormTime );
		}

		return OffData;
	}

	DataType GetData( const TimeType a_Time, size_t* o_Before = nullptr, size_t* o_After = nullptr )
	{
		if ( KeyFrames.size() <= 2 )
		{
			if ( o_Before ) *o_Before = 0;
			if ( o_After ) *o_After = KeyFrames.size() - 1;
			return GetData( a_Time, 0, KeyFrames.size() - 1 );;
		}

		if ( a_Time == ( TimeType )0.0 )
		{
			if ( o_Before ) *o_Before = 0;
			if ( o_After ) *o_After = 0;
			return KeyFrames[ 0 ].Data;
		}

		if ( auto& KeyFrame = KeyFrames[ KeyFrames.size() - 1 ]; a_Time == KeyFrame.Time )
		{
			if ( o_Before ) *o_Before = KeyFrames.size() - 1;
			if ( o_After ) *o_After = KeyFrames.size() - 1;
			return KeyFrame.Data;
		}

		for ( size_t i = 0; i < KeyFrames.size() - 1; ++i )
		{
			auto& KeyFrame = KeyFrames[ i ];

			if ( a_Time <= KeyFrame.Time )
			{
				if ( o_Before ) *o_Before = i;
				if ( o_After ) *o_After = i + 1;

				return GetData( a_Time, i, i + 1 );
			}
		}

		if ( o_Before ) *o_Before = 0;
		if ( o_After ) *o_After = 0;
		return KeyFrames[ 0 ].Data;
	}
};

using PositionAnimationTrack = AnimationTrack< PositionKeyFrame >;
using RotationAnimationTrack = AnimationTrack< RotationKeyFrame >;
using ScaleAnimationTrack = AnimationTrack< ScaleKeyFrame >;

class RAnimation 
	: public RResource
{
	friend class AnimationLoader;

public:

	RAnimation();

	// Duration in ticks
	const auto GetDuration() { return m_Duration; }

	// Ticks per second
	const auto GetTickRate() { return m_TicksPerSecond; }

	// Duration in seconds
	const TimeType GetPlayLength() { return (TimeType)( m_Duration / m_TicksPerSecond ); }

protected:

	TimeType m_Duration;
	uint32_t m_TicksPerSecond;
};

using AnimationHandle = std::shared_ptr < RAnimation >;