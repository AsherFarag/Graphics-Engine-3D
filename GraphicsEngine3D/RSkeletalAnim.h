#pragma once
#include "Animation.h"

struct BoneAnimation
{
	PositionAnimationTrack PositionTrack;
	RotationAnimationTrack RotationTrack;
	ScaleAnimationTrack ScaleTrack;

	void Evaluate( mat4& o_Transform, const TimeType a_Time );
};

class RSkeletalAnim :
    public RAnimation
{
	friend class AnimationLoader;
	friend class ImGui_ResourceManager;

protected:
    std::map< string, BoneAnimation > m_BoneAnimations;

public:
	BoneAnimation* GetBone( const string& a_BoneName );

    bool GetBoneMatrix( const string& a_BoneName, mat4& o_BoneMatrix, TimeType a_Time );
};

using SkeletalAnimHandle = std::shared_ptr < RSkeletalAnim >;

