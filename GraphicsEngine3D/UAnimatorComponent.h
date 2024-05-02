#pragma once
#include "UBaseComponent.h"

#include "RSkeleton.h"
#include "RSkeletalAnim.h"

using Animation::TimeType;

enum EAnimationPlayMethod
{
    APM_Forward = 0,
    APM_Reverse,
    APM_PingPong,
};

class UAnimatorComponent :
    public UBaseComponent
{
public:
    UAnimatorComponent( AActor* a_Owner );

    void UpdateAnimation( float DeltaTime );
    void PlayAnimation( SkeletalAnimHandle a_Animation, TimeType a_Time = 0.0, float a_PlayRate = 1.f, bool a_Looping = true, EAnimationPlayMethod a_PlayMethod = APM_Forward );

    auto& GetPlayRate() { return m_PlayRate; }
    auto& GetTime() { return m_CurrentTime;  }
    void  SetTime( const TimeType a_Time );

    // Gets array of transforms to transform bone space vertex into model space.
    auto& GetPose() { return m_SkeletonPose; }
    void SetSkeleton( SkeletonHandle a_Skeleton );
    SkeletonHandle GetSkeleton() { return m_Skeleton; }

protected:
    EAnimationPlayMethod m_PlayMethod;
    bool m_Looping = true;
    bool m_Finished = false;

    TimeType m_CurrentTime = 0.0;
    float m_PlayRate = 1.f;
    float m_DeltaTime = 0.f;

    SkeletalAnimHandle m_CurrentAnimation = nullptr;
    SkeletonHandle m_Skeleton = nullptr;

    std::vector<mat4> m_SkeletonPose;

protected:
    //void CalculateBoneTransform( const AssimpNodeData* node, mat4 parentTransform );
};

