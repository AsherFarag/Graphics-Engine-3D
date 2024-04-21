#pragma once
#include "Animation.h"
#include "ResourceManager.h"

class AnimationLoader
{
private:
	AnimationLoader() = default;
	~AnimationLoader() = default;

public:
	AnimationLoader( AnimationLoader& Other ) = delete;
	void operator=( const AnimationLoader& ) = delete;

	static AnimationLoader* GetInstance();

	AnimationHandle LoadAnimation( const string& a_Name, const aiScene* a_Scene, size_t a_Index = 0 );
	AnimationHandle GetAnimation( const string& a_Name );
};