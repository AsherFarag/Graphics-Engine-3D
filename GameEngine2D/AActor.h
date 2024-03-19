#pragma once
#include "Transform.h"
class AActor :
    public Transform
{
public:
    AActor();
    virtual ~AActor() override;
};

