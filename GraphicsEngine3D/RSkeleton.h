#pragma once
#include "RResource.h"
#include "RSkeletalAnim.h"

constexpr int NO_PARENT_INDEX = ~0;

struct Bone
{
    int Parent = NO_PARENT_INDEX;
    string Name;
    // The transformation of this bone in the bind pose
    mat4 BindTransform = mat4( 1 );
    // In Model Space
    mat4 WorldTransform = mat4( 1 );
};

class RSkeleton :
    public RResource
{
    friend class MeshLoader;

public:
    RSkeleton();

    void Draw();

    void EvaluatePose( SkeletalAnimHandle a_Anim, TimeType a_Time, std::vector<mat4>& o_Pose );

    std::vector<Bone> m_Bones;

    std::vector<mat4> m_Pose;

    void GenerateBoneData();
};

using SkeletonHandle = std::shared_ptr<RSkeleton>;

