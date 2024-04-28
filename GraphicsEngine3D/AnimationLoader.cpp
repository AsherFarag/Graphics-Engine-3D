#include "AnimationLoader.h"
#include "Animation.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"


AnimationLoader* AnimationLoader::GetInstance()
{
	static AnimationLoader Instance;
	return &Instance;
}

SkeletalAnimHandle AnimationLoader::LoadAnimation( const string& a_Path, const string& a_Name, const aiScene* a_Scene, size_t a_Index )
{
	if ( !a_Scene->HasAnimations() || a_Index >= a_Scene->mNumAnimations )
	{
		return {};
	}

	SkeletalAnimHandle Result = std::make_shared< RSkeletalAnim >();

	Result->m_Duration = a_Scene->mAnimations[ a_Index ]->mDuration;
	Result->m_TicksPerSecond = a_Scene->mAnimations[ a_Index ]->mTicksPerSecond;
	TimeType InverseTicksPerSecond = ( TimeType )1.0 / Result->m_TicksPerSecond;

	Result->ReadHeirarchyData( Result->m_RootNode, a_Scene->mRootNode->FindNode( "mixamorig:Hips" ));

	aiAnimation* Anim = a_Scene->mAnimations[ a_Index ];

	for ( size_t i = 0; i < Anim->mNumChannels; ++i ) 
	{
		aiNodeAnim* NodeAnim = Anim->mChannels[ i ];
		auto& BoneTrack = Result->m_BoneAnimations[ NodeAnim->mNodeName.C_Str() ];

		// Position keys
		BoneTrack.PositionTrack.KeyFrames.reserve( NodeAnim->mNumPositionKeys );

		for ( size_t j = 0; j < NodeAnim->mNumPositionKeys; ++j ) 
		{
			aiVectorKey PositionKey = NodeAnim->mPositionKeys[ j ];

			BoneTrack.PositionTrack.KeyFrames.emplace_back( PositionKeyFrame{ vec3{
				PositionKey.mValue.x,
				PositionKey.mValue.y,
				PositionKey.mValue.z },
				(TimeType)PositionKey.mTime * InverseTicksPerSecond } );
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
				(TimeType)RotationKey.mTime * InverseTicksPerSecond } );
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
				(TimeType)ScaleKey.mTime * InverseTicksPerSecond } );
		}
	}

	Result->ConstuctResourceInfo( a_Path, a_Name );

	m_SkeletalAnimations.emplace( a_Name, Result );

	return Result;
}

SkeletalAnimHandle AnimationLoader::GetAnimation( const string& a_Name )
{
	auto it = m_SkeletalAnimations.find( a_Name );
	if ( it != m_SkeletalAnimations.end() )
		return it->second;
	return nullptr;
}