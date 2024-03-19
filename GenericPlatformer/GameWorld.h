#pragma once
#include "World.h"
class GameWorld :
    public World
{
public:
    static GameWorld* GetWorld();
};

