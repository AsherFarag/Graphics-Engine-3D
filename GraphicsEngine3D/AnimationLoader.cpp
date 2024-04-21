#include "AnimationLoader.h"
#include "Animation.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"


AnimationLoader* AnimationLoader::GetInstance()
{
	static AnimationLoader Instance;
	return &Instance;
}

// 0 1 0 1 1 1
// 1 0 0 0 0 1
// 1 1 0 1 1 1 |
// 
// 
// 0 1 0 1 1 1
// 1 0 0 0 0 1
// 0 0 0 0 0 1 &

// 0 1 0 1 1 0
// 1 0 1 0 0 1 ~

AnimationHandle AnimationLoader::LoadAnimation( const string& a_Name, const aiScene* a_Scene, size_t a_Index )
{
	if ( !a_Scene->HasAnimations() || a_Index >= a_Scene->mNumAnimations )
	{
		return {};
	}

	AnimationHandle Result = std::make_shared< Animation >();

	aiAnimation* Anim = a_Scene->mAnimations[ a_Index ];

	for ( size_t i = 0; i < Anim->mNumChannels; ++i ) 
	{
		aiNodeAnim* NodeAnim = Anim->mChannels[ i ];
		auto& BoneTrack = Result->BoneAnimations[ NodeAnim->mNodeName.C_Str() ];

		// Position keys
		BoneTrack.PositionTrack.KeyFrames.reserve( NodeAnim->mNumPositionKeys );

		for ( size_t j = 0; j < NodeAnim->mNumPositionKeys; ++j ) 
		{
			aiVectorKey PositionKey = NodeAnim->mPositionKeys[ j ];

			BoneTrack.PositionTrack.KeyFrames.emplace_back( PositionKeyFrame{ vec3{
				PositionKey.mValue.x,
				PositionKey.mValue.y,
				PositionKey.mValue.z },
				PositionKey.mTime
				} );
		}

		// Rotation keys
		BoneTrack.RotationTrack.KeyFrames.reserve( NodeAnim->mNumRotationKeys );

		for ( size_t j = 0; j < NodeAnim->mNumRotationKeys; ++j )
		{
			aiQuatKey RotationKey = NodeAnim->mRotationKeys[ j ];
			BoneTrack.RotationTrack.KeyFrames.emplace_back( RotationKeyFrame{ glm::quat{
				RotationKey.mValue.w,
				RotationKey.mValue.x,
				RotationKey.mValue.y,
				RotationKey.mValue.z },
				RotationKey.mTime } );
		}

		// Scale keys
		BoneTrack.ScaleTrack.KeyFrames.reserve( NodeAnim->mNumScalingKeys );

		for ( size_t j = 0; j < NodeAnim->mNumScalingKeys; ++j )
		{
			aiVectorKey ScaleKey = NodeAnim->mScalingKeys[ j ];
			BoneTrack.ScaleTrack.KeyFrames.emplace_back( ScaleKeyFrame{ vec3{
					ScaleKey.mValue.x,
					ScaleKey.mValue.y,
					ScaleKey.mValue.z },
				ScaleKey.mTime } );
		}
	}

	return Result;
}

AnimationHandle AnimationLoader::GetAnimation( const string& a_Name )
{
	return {};
}