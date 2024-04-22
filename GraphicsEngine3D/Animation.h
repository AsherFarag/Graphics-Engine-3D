#pragma once
#include <map>
#include <vector>
#include <type_traits>
#include "glm/ext.hpp"
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

	const T& GetKeyFrame( const size_t a_Index ) const { return KeyFrames[ a_Index ]; }
	DataType& GetData( const size_t a_Index ) { return KeyFrames[ a_Index ].Data; }
	const DataType& GetData( const size_t a_Index ) const { return KeyFrames[ a_Index ].Data; }
	/* Additive Animation */
	DataType GetData( const TimeType a_Time, const size_t a_Before, const size_t a_After )
	{
		if ( a_After >= KeyFrames.size() )
		{
			return KeyFrames.back().Data;
		}

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

		//OffData = glm::lerp( AfterData,  )

		return OffData;
	}

	DataType GetData( const TimeType a_Time, size_t* o_Before = nullptr, size_t* o_After = nullptr )
	{
		if ( a_Time == ( TimeType )0.0 )
		{
			if ( o_Before ) *o_Before = 0;
			if ( o_After ) *o_After = 0;
			return KeyFrames[ 0 ].Data;
		}

		if ( auto& KeyFrame = KeyFrames[ KeyFrames.size() - 1u ]; a_Time == KeyFrame.Time )
		{
			if ( o_Before ) *o_Before = KeyFrames.size() - 1u;
			if ( o_After ) *o_After = KeyFrames.size() - 1u;
			return KeyFrame.Data;
		}

		for ( size_t i = 0; i < KeyFrames.size() - 1u; ++i )
		{
			auto& KeyFrame = KeyFrames[ i ];

			if ( a_Time >= KeyFrame.Time )
			{
				if ( o_Before ) *o_Before = i;
				if ( o_After ) *o_After = i + 1;

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

	void Evaluate( mat4& o_Transform, const TimeType a_Time )
	{
		size_t Before, After;
		auto Position = PositionTrack.GetData( a_Time, &Before, &After );
		auto Rotation = RotationTrack.GetData( a_Time, Before, After );
		auto Scale = ScaleTrack.GetData( a_Time, Before, After );
		
		o_Transform = glm::translate( glm::mat4( 1 ), Position ) * glm::toMat4( Rotation ) * glm::scale( glm::mat4( 1 ), Scale );
	}
};

class Animation : public RResource
{
	friend class AnimationLoader;

public:

	Animation();

	std::map< string, BoneAnimation > BoneAnimations;

	// Duration in ticks.
	const auto GetDuration() { return m_Duration; }

	// Ticks per second.
	const auto GetTickRate() { return m_TicksPerSecond; }

	// Duration in seconds.
	const auto GetPlayLength() { return m_Duration / m_TicksPerSecond; }

	bool GetBoneMatrix( const string& a_BoneName, mat4& o_BoneMatrix, TimeType a_Time )
	{
		auto Iter = BoneAnimations.find( a_BoneName );

		if ( Iter == BoneAnimations.end() )
		{
			return false;
		}

		a_Time = fmodf( a_Time, GetPlayLength() );
		Iter->second.Evaluate( o_BoneMatrix, a_Time );
		return true;
	}

protected:

	TimeType m_Duration;
	uint32_t m_TicksPerSecond;
};

using AnimationHandle = std::shared_ptr < Animation >;