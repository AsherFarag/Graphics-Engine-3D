#pragma once
#include "ACamera.h"
class AOrbitCamera :
    public ACamera
{
public:
    AOrbitCamera();
    virtual ~AOrbitCamera();

protected:

    float m_DistanceFromTarget;
    mat4 m_Transform;

public:

    void SetTarget(mat4 a_Transform, float a_DistanceFromTarget);
};

