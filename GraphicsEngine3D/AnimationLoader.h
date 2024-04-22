#pragma once
#include "Animation.h"
#include "assimp/scene.h"

class AnimationLoader
{
	friend class ImGui_ResourceManager;

private:
	AnimationLoader() = default;
	~AnimationLoader() = default;

	std::map< string, AnimationHandle > m_Animations;

public:
	AnimationLoader( AnimationLoader& Other ) = delete;
	void operator=( const AnimationLoader& ) = delete;

	static AnimationLoader* GetInstance();

	AnimationHandle LoadAnimation( const string& a_Path, const string& a_Name, const aiScene* a_Scene, size_t a_Index = 0 );
	AnimationHandle GetAnimation( const string& a_Name );
};