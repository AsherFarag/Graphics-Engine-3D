#pragma once
#include "RResource.h"

constexpr int NO_PARENT_INDEX = ~0;

struct Bone
{
    int Parent = NO_PARENT_INDEX;
    string Name;
    mat4 LocalTransform = mat4(1);
};

class RSkeleton :
    public RResource
{
public:
    RSkeleton();

    void Draw();

    std::vector<Bone> m_Bones;
};

using SkeletonHandle = std::shared_ptr<RSkeleton>;

