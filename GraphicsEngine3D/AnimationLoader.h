#pragma once
#include "RSkeletalAnim.h"
#include "assimp/scene.h"

class AnimationLoader
{
	friend class ImGui_ResourceManager;

private:
	AnimationLoader() = default;
	~AnimationLoader() = default;

	std::map< string, SkeletalAnimHandle > m_SkeletalAnimations;

public:
	AnimationLoader( AnimationLoader& Other ) = delete;
	void operator=( const AnimationLoader& ) = delete;

	static AnimationLoader* GetInstance();

	SkeletalAnimHandle LoadAnimation( const string& a_Path, const string& a_Name, const aiScene* a_Scene, size_t a_Index = 0 );
	SkeletalAnimHandle LoadAnimation( const aiAnimation* a_AssimpAnim );
	SkeletalAnimHandle GetAnimation( const string& a_Name );
};