#include "RSkeleton.h"
#include "Gizmos.h"

#include <cmath>

RSkeleton::RSkeleton()
{
}

void RSkeleton::Draw()
{
	//for ( auto& bone : m_Pose )
	//{
	//	//mat4 parentWorldTransform = mat4(1);

	//	//int parent = bone.Parent;
	//	//while ( parent != NO_PARENT_INDEX )
	//	//{
	//	//	parentWorldTransform *= glm::inverse( m_Bones[ parent ].BindTransform );
	//	//	parent = m_Bones[parent].Parent;
	//	//}
	//	//parentWorldTransform = glm::inverse( parentWorldTransform );




	//	//vec3 p0;
	//	//vec3 p1;

	//	//// If bone is the Root Bone
	//	//if ( &bone - m_Bones.data() == 0 )
	//	//{
	//	//	p1 = ( bone.OffsetMatrix * bone.BindTransform )[ 3 ];
	//	//	aie::Gizmos::addSphere( p1, 0.035f, 2, 4, {1,1,0,1});
	//	//}
	//	//else
	//	//{
	//	//	aie::Gizmos::addSphere( p1, 0.015f, 2, 4, { 1,0,0,1 } );
	//	//	p0 = m_Bones[ bone.Parent ].OffsetMatrix[ 3 ];
	//	//	p1 = ( m_Bones[ bone.Parent ].OffsetMatrix * bone.BindTransform )[ 3 ];
	//	//	aie::Gizmos::addLine( p0, p1, vec4( 1 ) );
	//	//}
	//}

	for ( int i = 0; i < m_Pose.size(); ++i )
	{
		if ( i >= m_Bones.size() )
			break;

		auto& bonePose = ( m_Pose )[ i ];
		//auto& bonePose = m_Bones[ i ].OffsetMatrix;

		vec3 p0;
		vec3 p1;
		p1 = bonePose[ 3 ];
		// If bone is the Root Bone
		if ( i == 0 )
		{
			p1 = ( bonePose )[ 3 ];
			aie::Gizmos::addSphere( p1, 0.035f, 2, 4, { 1,1,0,1 } );
		}
		else
		{
			aie::Gizmos::addSphere( p1, 0.015f, 2, 4, { 1.f, 0, 0,1 } );
			auto& boneParentPose = ( m_Pose )[ m_Bones[ i ].Parent ];
			p0 = boneParentPose[ 3 ];
			aie::Gizmos::addLine( p0, p1, vec4( 1 ) );
		}

	}
}

void RSkeleton::EvaluatePose( SkeletalAnimHandle a_Anim, TimeType a_Time, std::vector<mat4>& o_Pose )
{
	if ( o_Pose.size() < m_Bones.size() )
	{
		o_Pose.resize( m_Bones.size() );
	}

	for (int i = 0; i < o_Pose.size(); ++i )
	{
		// Get the transform of the bone in the animation
		mat4 eval;
		a_Anim->GetBoneMatrix( m_Bones[ i ].Name, eval, a_Time );

		if ( eval == mat4( 0.f ) )
			eval = mat4( 1.f );

		if ( m_Bones[ i ].Parent == NO_PARENT_INDEX )
		{
			o_Pose[ i ] = eval;
			continue;
		}
		o_Pose[ i ] = o_Pose[ m_Bones[ i ].Parent ] * eval;
	}
}

void RSkeleton::GenerateBoneData()
{
	//TODO( "Redoes calculations multiple times, optimise here" );
	//for ( int i = 0; i < m_Bones.size(); ++i )
	//{
	//	auto& bone = m_Bones[ i ];

	//	mat4 parentWorldTransform = mat4( 1 );

	//	int parent = bone.Parent;
	//	while ( parent != NO_PARENT_INDEX )
	//	{
	//		parentWorldTransform *= glm::inverse( m_Bones[ parent ].BindTransform );
	//		parent = m_Bones[ parent ].Parent;
	//	}
	//	parentWorldTransform = glm::inverse( parentWorldTransform );

	//	bone.OffsetMatrix = parentWorldTransform * bone.BindTransform;

	//}

	for ( int i = 0; i < m_Bones.size(); ++i )
	{
		auto& Bone = m_Bones[ i ];

		if ( Bone.Parent == NO_PARENT_INDEX )
		{
			Bone.OffsetMatrix = mat4( 1 );
			continue;
		}

		Bone.OffsetMatrix = glm::inverse( m_Bones[ Bone.Parent ].OffsetMatrix) * Bone.BindTransform;
	}

}
