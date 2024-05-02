template < typename Func >
void ForEachBoneDescendent( const aiNode* a_Node, Func&& Function )
{
    Function( a_Node );
    for ( int i = 0; i < a_Node->mNumChildren; ++i )
    {
        ForEachBoneDescendent( a_Node->mChildren[ i ], Function );
    }
}


//SkeletonHandle MeshLoader::LoadSkeleton( const string& a_Path, const string& a_Name, const aiNode* a_BoneRootNode, const aiScene* a_Scene )
//{
//    SkeletonHandle skeleton = std::make_shared<RSkeleton>();
//    
//    // Add root bone to the skeleton's bones
//    skeleton->m_Bones.emplace_back().Name = a_BoneRootNode->mName.C_Str();
//
//    // Recursive loop for adding each child bone 
//    ForEachBoneDescendent( a_BoneRootNode,
//        [&skeleton,a_Scene]( const aiNode* a_Node )
//        {
//            
//            auto it = std::find_if( skeleton->m_Bones.begin(), skeleton->m_Bones.end(),
//                [a_Node]( const Bone& a )
//                {
//                    return a.Name == a_Node->mParent->mName.C_Str();
//                } );
//
//            if ( it == skeleton->m_Bones.end() )
//            {
//                LOG( Error, ( "Child Bone doesn't have parent " + string( a_Node->mName.C_Str() ) ).c_str() );
//                return;
//            }
//
//            int parentIndex = it - skeleton->m_Bones.begin();
//
//            auto& bone = skeleton->m_Bones.emplace_back();
//            bone.Name = a_Node->mName.C_Str();
//            bone.Parent = parentIndex;
//
//            // Assimp loads matricies as Row Major so we must convert it to Column major
//            bone.BindTransform = Math::AssimpMatToGLM( a_Node->mTransformation );
//            //bone.OffsetMatrix = Math::AssimpMatToGLM(  a_Scene->mSkeletons[ 0 ]->mBones[ parentIndex + 1 ]->mOffsetMatrix );
//        }
//    );
//    
//    skeleton->GenerateBoneData();
//
//    skeleton->ConstuctResourceInfo( a_Path, a_Name );
//
//    return skeleton;
//}
