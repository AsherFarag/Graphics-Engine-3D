#pragma once
#include <map>
#include <vector>
#include <type_traits>
#include "RResource.h"

#pragma region Animation

#pragma region Data

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

#pragma endregion

class RAnimation : public RResource
{
	friend class AnimationLoader;

public:

	RAnimation();

	// Duration in ticks
	const uint32_t KeyFrameCount() { return m_FrameCount; }

	// Ticks per second
	const uint32_t GetTickRate() { return m_FramesPerSecond; }

	// Duration in seconds
	const TimeType GetDuration() { return m_FramesPerSecond != 0 ? (TimeType)( m_FrameCount / m_FramesPerSecond ) : 0; }

protected:
	uint32_t m_FrameCount = 0;
	uint32_t m_FramesPerSecond = 0;
};

using AnimationHandle = std::shared_ptr < RAnimation >;

#pragma region Skeletal Animation

using PositionAnimationTrack = AnimationTrack< PositionKeyFrame >;
using RotationAnimationTrack = AnimationTrack< RotationKeyFrame >;
using ScaleAnimationTrack	 = AnimationTrack< ScaleKeyFrame >;

struct BoneAnimation
{
	PositionAnimationTrack PositionTrack;
	RotationAnimationTrack RotationTrack;
	ScaleAnimationTrack ScaleTrack;
	BoneAnimation( const aiNodeAnim* a_NodeAnim = nullptr, const TimeType a_InverseTicksPerSecond = (TimeType)-1.0 );
	void ExtractKeys( const aiNodeAnim* a_NodeAnim, const TimeType a_InverseTicksPerSecond );
	void Evaluate( mat4& o_Transform, const TimeType a_Time );
};

class RSkeletalAnim :
	public RAnimation
{
	friend class AnimationLoader;
	friend class ImGui_ResourceManager;

protected:
	std::map< string, BoneAnimation > m_BoneAnimations;

	//void ReadHeirarchyData( AssimpNodeData& dest, const aiNode* src );
	void ReadMissingBones( const aiAnimation* a_Animation, MeshHandle a_SkMesh );

public:
	BoneAnimation* GetBoneAnimation( const string& a_BoneName );
	const auto& GetBoneInfoMap() const;
	bool GetBoneMatrix( const string& a_BoneName, mat4& o_BoneMatrix, TimeType a_Time );
	void BindToSkeletalMesh( const aiNode* a_Node, const aiAnimation* a_Anim, MeshHandle a_Mesh );

	std::map< string, BoneInfo > m_BoneInfoMap;
};

using SkeletalAnimHandle = std::shared_ptr < RSkeletalAnim >;

#pragma endregion


#pragma endregion

#pragma region Loader

class AnimationLoader
{
private:
	AnimationLoader() = default;
	~AnimationLoader() = default;

	std::map< string, SkeletalAnimHandle > m_SkeletalAnimations;

public:
	AnimationLoader( AnimationLoader& Other ) = delete;
	void operator=( const AnimationLoader& ) = delete;

	static AnimationLoader* GetInstance();

	SkeletalAnimHandle LoadAnimation( const string& a_Path, const string& a_Name, const aiScene* a_Scene, size_t a_Index = 0 );
	SkeletalAnimHandle LoadAnimation( const aiAnimation* a_AssimpAnim );
	SkeletalAnimHandle GetAnimation( const string& a_Name );
	const auto& GetLoadedAnimations() const { return m_SkeletalAnimations; }
};

#pragma endregion
