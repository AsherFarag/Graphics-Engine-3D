#include "RSkeletalAnim.h"

BoneAnimation* RSkeletalAnim::GetBone( const string& a_BoneName )
{
	return &m_BoneAnimations.find( a_BoneName )->second;
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
