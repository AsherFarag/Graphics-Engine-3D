//#include "AnimationLoader.h"
//#include "Animation.h"
//
//
//AnimationLoader* AnimationLoader::GetInstance()
//{
//	static AnimationLoader Instance;
//	return &Instance;
//}
//
//SkeletalAnimHandle AnimationLoader::LoadAnimation( const string& a_Path, const string& a_Name, const aiScene* a_Scene, size_t a_Index )
//{
//	if ( !a_Scene->HasAnimations() || a_Index >= a_Scene->mNumAnimations )
//	{
//		return {};
//	}
//
//	SkeletalAnimHandle Result = std::make_shared< RSkeletalAnim >();
//
//	Result->m_FrameCount = a_Scene->mAnimations[ a_Index ]->mDuration;
//	Result->m_FramesPerSecond = a_Scene->mAnimations[ a_Index ]->mTicksPerSecond;
//
//	aiAnimation* Anim = a_Scene->mAnimations[ a_Index ];
//	Result->ReadHeirarchyData( Result->m_RootNode, a_Scene->mRootNode->FindNode( "mixamorig:Hips" ));
//
//	for ( size_t i = 0; i < Anim->mNumChannels; ++i ) 
//	{
//		aiNodeAnim* NodeAnim = Anim->mChannels[ i ];
//		auto& BoneTrack = Result->m_BoneAnimations[ NodeAnim->mNodeName.C_Str() ];
//
//		TimeType InverseTicksPerSecond = (TimeType)1.0 / Result->m_FramesPerSecond;
//		BoneTrack.ExtractKeys( NodeAnim, InverseTicksPerSecond );
//	}
//
//	Result->ConstuctResourceInfo( a_Path, a_Name );
//
//	m_SkeletalAnimations.emplace( a_Name, Result );
//
//	return Result;
//}
//
//SkeletalAnimHandle AnimationLoader::LoadAnimation( const aiAnimation* a_AssimpAnim )
//{
//	SkeletalAnimHandle Result = std::make_shared< RSkeletalAnim >();
//
//	Result->m_FrameCount = a_AssimpAnim->mDuration;
//	Result->m_FramesPerSecond = a_AssimpAnim->mTicksPerSecond;
//
//	//Result->ReadHeirarchyData( Result->m_RootNode, a_Scene->mRootNode->FindNode( "mixamorig:Hips" ) );
//
//	for ( size_t i = 0; i < a_AssimpAnim->mNumChannels; ++i )
//	{
//		aiNodeAnim* NodeAnim = a_AssimpAnim->mChannels[ i ];
//		auto& BoneTrack = Result->m_BoneAnimations[ NodeAnim->mNodeName.C_Str() ];
//
//		TimeType InverseTicksPerSecond = (TimeType)1.0 / Result->m_FramesPerSecond;
//		BoneTrack.ExtractKeys( NodeAnim, InverseTicksPerSecond );
//	}
//
//	m_SkeletalAnimations.emplace( a_AssimpAnim->mName, Result );
//
//	return Result;
//}
//
//SkeletalAnimHandle AnimationLoader::GetAnimation( const string& a_Name )
//{
//	auto it = m_SkeletalAnimations.find( a_Name );
//	if ( it != m_SkeletalAnimations.end() )
//		return it->second;
//	return nullptr;
//}