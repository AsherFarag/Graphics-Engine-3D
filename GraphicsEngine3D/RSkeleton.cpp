#include "RSkeleton.h"
#include "Gizmos.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

RSkeleton::RSkeleton()
{
}

void RSkeleton::Draw()
{
	for ( auto& bone : m_Pose )
	{
		//mat4 parentWorldTransform = mat4(1);

		//int parent = bone.Parent;
		//while ( parent != NO_PARENT_INDEX )
		//{
		//	parentWorldTransform *= glm::inverse( m_Bones[ parent ].BindTransform );
		//	parent = m_Bones[parent].Parent;
		//}
		//parentWorldTransform = glm::inverse( parentWorldTransform );




		//vec3 p0;
		//vec3 p1;

		//// If bone is the Root Bone
		//if ( &bone - m_Bones.data() == 0 )
		//{
		//	p1 = ( bone.WorldTransform * bone.BindTransform )[ 3 ];
		//	aie::Gizmos::addSphere( p1, 0.035f, 2, 4, {1,1,0,1});
		//}
		//else
		//{
		//	aie::Gizmos::addSphere( p1, 0.015f, 2, 4, { 1,0,0,1 } );
		//	p0 = m_Bones[ bone.Parent ].WorldTransform[ 3 ];
		//	p1 = ( m_Bones[ bone.Parent ].WorldTransform * bone.BindTransform )[ 3 ];
		//	aie::Gizmos::addLine( p0, p1, vec4( 1 ) );
		//}
	}

	for ( int i = 0; i < m_Pose.size(); ++i )
	{
		auto& bonePose = m_Pose[ i ];
		//auto& bonePose = m_Bones[ i ].WorldTransform;

		vec3 p0;
		vec3 p1;
		p1 = ( bonePose )[ 3 ];
		// If bone is the Root Bone
		if ( i == 0 )
		{
			p1 = ( bonePose )[ 3 ];
			aie::Gizmos::addSphere( p1, 0.035f, 2, 4, { 1,1,0,1 } );
		}
		else
		{
			aie::Gizmos::addSphere( p1, 0.015f, 2, 4, { 1.f, 0, 0,1 } );
			auto& boneParentPose = m_Pose[ m_Bones[ i ].Parent ];
			//auto& boneParentPose = m_Bones[ m_Bones[ i ].Parent ].WorldTransform;
			p0 = boneParentPose[ 3 ];
			//p1 = ( m_Bones[ bone.Parent ].WorldTransform * bone.BindTransform )[ 3 ];
			aie::Gizmos::addLine( p0, p1, vec4( 1 ) );
		}
	}
}

void RSkeleton::EvaluatePose( AnimationHandle a_Anim, TimeType a_Time, std::vector<mat4>& o_Pose )
{
	if ( o_Pose.size() < m_Bones.size() )
	{
		o_Pose.resize( m_Bones.size() );
	}

	for (int i = 0; i < o_Pose.size(); ++i )
	{
		TODO( "This only needs to be calculated once" );
		mat4 bind = ( m_Bones[ i ].WorldTransform );
		//mat4 bind = m_Bones[i].BindTransform;

		auto j = a_Anim->BoneAnimations.find( m_Bones[ i ].Name );
		if ( j == a_Anim->BoneAnimations.end() )
			continue;
		BoneAnimation& boneAnim = j->second;

		auto& posAnim = boneAnim.PositionTrack.GetData( a_Time );
		auto& rotAnim = boneAnim.RotationTrack.GetData( a_Time );
		auto& scaleAnim = boneAnim.ScaleTrack.GetData( a_Time );

		// Create a translation matrix
		//mat4 translationMatrix = glm::translate( mat4( 1 ), posAnim );

		// Convert quaternion rotation to rotation matrix
		//mat4 rotationMatrix = glm::toMat4( rotAnim );

		// Create a scale matrix with identity matrix
		//mat4 scaleMatrix = glm::scale( mat4( 1 ), scaleAnim );

		// Combine the translation, rotation, and scale matrices
		mat4 eval;// = translationMatrix * rotationMatrix * scaleMatrix;
		a_Anim->GetBoneMatrix( m_Bones[ i ].Name, eval, a_Time );
		//mat4 eval = scaleMatrix* rotationMatrix * translationMatrix;

		if ( m_Bones[ i ].Parent == NO_PARENT_INDEX )
		{
			o_Pose[ i ] = eval * bind;
			continue;
		}
		o_Pose[ i ] = o_Pose[ m_Bones[ i ].Parent ] * eval;
	}
}

void RSkeleton::GenerateBoneData()
{
	TODO( "Redoes calculations multiple times, optimise here" );
	for ( int i = 0; i < m_Bones.size(); ++i )
	{
		auto& bone = m_Bones[ i ];

		mat4 parentWorldTransform = mat4( 1 );

		int parent = bone.Parent;
		while ( parent != NO_PARENT_INDEX )
		{
			parentWorldTransform *= glm::inverse( m_Bones[ parent ].BindTransform );
			parent = m_Bones[ parent ].Parent;
		}
		parentWorldTransform = glm::inverse( parentWorldTransform );

		bone.WorldTransform = parentWorldTransform * bone.BindTransform;

		m_Pose.resize( m_Bones.size() );
		m_Pose[ i ] = bone.WorldTransform;

	}
}
