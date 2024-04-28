#include "UAnimatorComponent.h"

UAnimatorComponent::UAnimatorComponent( AActor* a_Owner )
	: UBaseComponent( a_Owner )
{
}

void UAnimatorComponent::UpdateAnimation( float DeltaTime )
{
	m_DeltaTime = DeltaTime;
	if ( !m_CurrentAnimation || !m_Skeleton )
		return;

	m_CurrentTime += m_PlayRate * DeltaTime;
	m_CurrentTime = fmod( m_CurrentTime, m_CurrentAnimation->GetPlayLength() );

	//CalculateBoneTransform( &m_CurrentAnimation->GetRootNode(), mat4( 1.f ) );
	m_Skeleton->EvaluatePose( m_CurrentAnimation, m_CurrentTime, m_SkeletonPose );
}

void UAnimatorComponent::PlayAnimation( SkeletalAnimHandle a_Animation, TimeType a_Time, float a_PlayRate, bool a_Looping, EAnimationPlayMethod a_PlayMethod )
{
	m_CurrentAnimation = a_Animation;
	m_CurrentTime      = a_Time;
	m_PlayRate         = a_PlayRate;
	m_Looping          = a_Looping;
	m_PlayMethod       = a_PlayMethod;
}

void UAnimatorComponent::SetSkeleton( SkeletonHandle a_Skeleton )
{
	m_Skeleton = a_Skeleton;
}

void UAnimatorComponent::CalculateBoneTransform( const AssimpNodeData* node, mat4 parentTransform )
{
	string nodeName = node->Name;
	mat4 nodeTransform = node->Transformation;

	auto boneAnim = m_CurrentAnimation->GetBone( nodeName );

	mat4 boneAnimTransform;
	if ( boneAnim )
	{
		boneAnim->Evaluate( boneAnimTransform, m_CurrentTime );
	}

	mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
	if ( boneInfoMap.find( nodeName ) != boneInfoMap.end() )
	{
		int index = boneInfoMap[ nodeName ].ID;
		mat4 offset = boneInfoMap[ nodeName ].Offset;
		if ( m_SkeletonPose.size() <= index )
			m_SkeletonPose.resize( index + 1 );
		m_SkeletonPose[ index ] = globalTransformation * offset;
	}

	for ( int i = 0; i < node->ChildrenCount; i++ )
		CalculateBoneTransform( &node->Children[ i ], mat4(1) );
}
