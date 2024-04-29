#include "RSkeletalAnim.h"

void RSkeletalAnim::ReadHeirarchyData( AssimpNodeData& dest, const aiNode* src )
{
    assert( src );

    dest.Name = src->mName.data;
    dest.Transformation = Math::AssimpMatToGLM( src->mTransformation );
    dest.ChildrenCount = src->mNumChildren;

    for ( int i = 0; i < src->mNumChildren; i++ )
    {
        AssimpNodeData newData;
        ReadHeirarchyData( newData, src->mChildren[ i ] );
        dest.Children.push_back( newData );
    }
}

void RSkeletalAnim::ReadMissingBones( const aiAnimation* a_Animation, MeshHandle a_SkMesh )
{
    int size = a_Animation->mNumChannels;

    auto& boneInfoMap = a_SkMesh->GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    int& boneCount = a_SkMesh->GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for ( int i = 0; i < size; i++ )
    {
        auto channel = a_Animation->mChannels[ i ];
        std::string boneName = channel->mNodeName.data;

        if ( boneInfoMap.find( boneName ) == boneInfoMap.end() )
        {
            boneInfoMap[ boneName ].ID = boneCount;
            boneCount++;
        }
		m_BoneAnimations.insert( { boneName, BoneAnimation( channel, (TimeType)1.0 / m_TicksPerSecond ) }  );
    }

    m_BoneInfoMap = boneInfoMap;
}

BoneAnimation* RSkeletalAnim::GetBone( const string& a_BoneName )
{
    auto it = m_BoneAnimations.find( a_BoneName );
    if ( it != m_BoneAnimations.end() )
        return &it->second;

	return nullptr;
}

bool RSkeletalAnim::GetBoneMatrix( const string& a_BoneName, mat4& o_BoneMatrix, TimeType a_Time )
{
	auto Iter = m_BoneAnimations.find( a_BoneName );

	if ( Iter == m_BoneAnimations.end() )
	{
		return false;
	}

	a_Time = fmodf( a_Time, GetPlayLength() );
	Iter->second.Evaluate( o_BoneMatrix, a_Time );
	return true;
}

void RSkeletalAnim::BindToSkeletalMesh( const aiNode* a_Node, const aiAnimation* a_Anim, MeshHandle a_Mesh )
{
	ReadHeirarchyData( m_RootNode, a_Node );
	//ReadMissingBones( a_Anim, a_Mesh );
}

BoneAnimation::BoneAnimation( const aiNodeAnim* a_NodeAnim, const TimeType a_InverseTicksPerSecond )
{
    if ( a_NodeAnim && a_InverseTicksPerSecond != (TimeType)-1 )
        ExtractKeys( a_NodeAnim, a_InverseTicksPerSecond );
}

void BoneAnimation::ExtractKeys( const aiNodeAnim* a_NodeAnim, const TimeType a_InverseTicksPerSecond )
{
	// Position keys
	PositionTrack.KeyFrames.reserve( a_NodeAnim->mNumPositionKeys );

	for ( size_t j = 0; j < a_NodeAnim->mNumPositionKeys; ++j )
	{
		aiVectorKey PositionKey = a_NodeAnim->mPositionKeys[ j ];

		PositionTrack.KeyFrames.emplace_back( PositionKeyFrame{ vec3{
			PositionKey.mValue.x,
			PositionKey.mValue.y,
			PositionKey.mValue.z },
			(TimeType)PositionKey.mTime * a_InverseTicksPerSecond } );
	}

	// Rotation keys
	RotationTrack.KeyFrames.reserve( a_NodeAnim->mNumRotationKeys );

	for ( size_t j = 0; j < a_NodeAnim->mNumRotationKeys; ++j )
	{
		aiQuatKey RotationKey = a_NodeAnim->mRotationKeys[ j ];
		RotationTrack.KeyFrames.emplace_back( RotationKeyFrame{ glm::quat{
			RotationKey.mValue.w,
			RotationKey.mValue.x,
			RotationKey.mValue.y,
			RotationKey.mValue.z },
			(TimeType)RotationKey.mTime * a_InverseTicksPerSecond } );
	}

	// Scale keys
	ScaleTrack.KeyFrames.reserve( a_NodeAnim->mNumScalingKeys );

	for ( size_t j = 0; j < a_NodeAnim->mNumScalingKeys; ++j )
	{
		aiVectorKey ScaleKey = a_NodeAnim->mScalingKeys[ j ];
		ScaleTrack.KeyFrames.emplace_back( ScaleKeyFrame{ vec3{
			ScaleKey.mValue.x,
			ScaleKey.mValue.y,
			ScaleKey.mValue.z },
			(TimeType)ScaleKey.mTime * a_InverseTicksPerSecond } );
	}
}

void BoneAnimation::Evaluate( mat4& o_Transform, const TimeType a_Time )
{
	size_t Before, After;
	auto Position = PositionTrack.GetData( a_Time, &Before, &After );
	auto Rotation = RotationTrack.GetData( a_Time, Before, After );
	auto Scale = ScaleTrack.GetData( a_Time, Before, After );

	o_Transform = glm::translate( glm::mat4( 1 ), Position ) * glm::toMat4( Rotation ) * glm::scale( glm::mat4( 1 ), Scale );
}
