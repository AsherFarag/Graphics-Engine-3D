#pragma once
#include "Transform.h"
class AActor :
    public Transform
{
public:
    AActor(vec2 a_Pos = vec2(), float a_Rot = 0.f);
    virtual ~AActor() override;

    virtual void Update(float DeltaTime) {};
};

