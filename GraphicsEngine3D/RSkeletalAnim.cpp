#include "RSkeletalAnim.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

void RSkeletalAnim::ReadHeirarchyData( AssimpNodeData& dest, const aiNode* src )
{
    assert( src );

    dest.Name = src->mName.data;
    dest.Transformation = glm::transpose( glm::make_mat4x4( ( (ai_real*)&src->mTransformation ) ) );
    dest.ChildrenCount = src->mNumChildren;

    for ( int i = 0; i < src->mNumChildren; i++ )
    {
        AssimpNodeData newData;
        ReadHeirarchyData( newData, src->mChildren[ i ] );
        dest.Children.push_back( newData );
    }
}

void RSkeletalAnim::ReadMissingBones( const aiAnimation* a_Animation, RSkeletalMesh& a_SkMesh )
{
    //int size = a_Animation->mNumChannels;

    //auto& boneInfoMap = a_SkMesh.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    //int& boneCount = a_SkMesh.GetBoneCount(); //getting the m_BoneCounter from Model class

    ////reading channels(bones engaged in an animation and their keyframes)
    //for ( int i = 0; i < size; i++ )
    //{
    //    auto channel = a_Animation->mChannels[ i ];
    //    std::string boneName = channel->mNodeName.data;

    //    if ( boneInfoMap.find( boneName ) == boneInfoMap.end() )
    //    {
    //        boneInfoMap[ boneName ].ID = boneCount;
    //        boneCount++;
    //    }
    //    m_BoneAnimations.push_back( BoneAnimation( channel->mNodeName.data, boneInfoMap[ channel->mNodeName.data ].ID, channel ) );
    //}

    //m_BoneInfoMap = boneInfoMap;
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

void BoneAnimation::Evaluate( mat4& o_Transform, const TimeType a_Time )
{
	size_t Before, After;
	auto Position = PositionTrack.GetData( a_Time, &Before, &After );
	auto Rotation = RotationTrack.GetData( a_Time, Before, After );
	auto Scale = ScaleTrack.GetData( a_Time, Before, After );

	o_Transform = glm::translate( glm::mat4( 1 ), Position ) * glm::toMat4( Rotation ) * glm::scale( glm::mat4( 1 ), Scale );
}
