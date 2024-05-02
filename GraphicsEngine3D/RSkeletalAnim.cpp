#include "RSkeletalAnim.h"
//
//void RSkeletalAnim::ReadHeirarchyData( AssimpNodeData& dest, const aiNode* src )
//{
//    assert( src );
//
//    dest.Name = src->mName.data;
//    dest.Transformation = Math::AssimpMatToGLM( src->mTransformation );
//    dest.ChildrenCount = src->mNumChildren;
//
//    for ( int i = 0; i < src->mNumChildren; i++ )
//    {
//        AssimpNodeData newData;
//        ReadHeirarchyData( newData, src->mChildren[ i ] );
//        dest.Children.push_back( newData );
//    }
//}
//
//void RSkeletalAnim::BindToSkeletalMesh( const aiNode* a_Node, const aiAnimation* a_Anim, MeshHandle a_Mesh )
//{
//	ReadHeirarchyData( m_RootNode, a_Node );
//	//ReadMissingBones( a_Anim, a_Mesh );
//}
//
//BoneAnimation::BoneAnimation( const aiNodeAnim* a_NodeAnim, const TimeType a_InverseTicksPerSecond )
//{
//    if ( a_NodeAnim && a_InverseTicksPerSecond != (TimeType)-1 )
//        ExtractKeys( a_NodeAnim, a_InverseTicksPerSecond );
//}
//
//void BoneAnimation::ExtractKeys( const aiNodeAnim* a_NodeAnim, const TimeType a_InverseTicksPerSecond )
//{
//}
//
//void BoneAnimation::Evaluate( mat4& o_Transform, const TimeType a_Time )
//{
//	size_t Before, After;
//	auto Position = PositionTrack.GetData( a_Time, &Before, &After );
//	auto Rotation = RotationTrack.GetData( a_Time, Before, After );
//	auto Scale = ScaleTrack.GetData( a_Time, Before, After );
//
//	o_Transform = glm::translate( glm::mat4( 1 ), Position ) * glm::toMat4( Rotation ) * glm::scale( glm::mat4( 1 ), Scale );
//}
