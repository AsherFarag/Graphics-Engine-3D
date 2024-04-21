#pragma once
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "RResource.h"

template < typename T >
struct KeyFrame
{
	using DataType = T;
	using TimeType = double;

	DataType Data;
	TimeType Time;
};

using PositionKeyFrame = KeyFrame< vec3 >;
using RotationKeyFrame = KeyFrame< glm::quat >;
using ScaleKeyFrame = KeyFrame< vec3 >;

template < typename T >
struct AnimationTrack
{
	using KeyFrameType = T;
	using DataType = typename KeyFrameType::DataType;
	using TimeType = typename KeyFrameType::TimeType;
	using SizeType = size_t;

	std::vector< KeyFrameType > KeyFrames;

	const T& GetKeyFrame( SizeType a_Index ) const { return KeyFrames[ a_Index ]; }
	DataType& GetData( SizeType a_Index ) { return KeyFrames[ a_Index ].Data; }
	const DataType& GetData( SizeType a_Index ) const { return KeyFrames[ a_Index ].Data; }
	DataType GetData( TimeType a_Time, SizeType a_Before, SizeType a_After ) const
	{
		const KeyFrameType& Before = KeyFrames[ a_Before ];
		const KeyFrameType& After = KeyFrames[ a_After ];

		TimeType BeforeTime = Before.Time;
		TimeType AfterTime = After.Time;

		TimeType DiffTime = AfterTime - BeforeTime;
		TimeType OffTime = a_Time - BeforeTime;

		TimeType NormTime = OffTime / DiffTime;

		DataType BeforeData = Before.Data;
		DataType AfterData = After.Data;

		DataType DiffData = AfterData - BeforeData;
		DataType OffData = DiffData * NormTime;

		return BeforeData + OffData;
	}
	DataType GetData( TimeType a_Time ) const
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
public:

	std::map< string, BoneAnimation > BoneAnimations;
};

using AnimationHandle = std::shared_ptr < Animation >;