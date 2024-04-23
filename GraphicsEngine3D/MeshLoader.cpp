#include "MeshLoader.h"
#include "glm/ext.hpp"

MeshLoader::MeshLoader()
{
    // Load Primitive Shapes
    LoadMesh( "Content/Mesh/Primitives/Box.obj", false );
    LoadMesh( "Content/Mesh/Primitives/Cone.obj", false );
    LoadMesh( "Content/Mesh/Primitives/Cylinder.obj", false );
    LoadMesh( "Content/Mesh/Primitives/Pyramid.obj", false );
    LoadMesh( "Content/Mesh/Primitives/Sphere.obj", false );
}

MeshLoader* MeshLoader::GetInstance()
{
    static MeshLoader Instance;
    return &Instance;
}

MeshHandle MeshLoader::LoadMesh( const string& a_Path, const string& a_Name, const aiScene* a_Scene )
{
    // Check if this mesh has already been loaded
    auto& foundMesh = m_LoadedMeshes.find( a_Name );
    if ( foundMesh != m_LoadedMeshes.end() )
        return foundMesh->second;

    MeshHandle mesh = std::make_shared< RMesh >();

    // === Load Mesh ===

    mesh->ProcessNode( a_Scene->mRootNode, a_Scene );

    // =================


    mesh->ConstuctResourceInfo( a_Path, a_Name );

    m_LoadedMeshes.emplace( mesh->GetResourceName(), mesh );

    return mesh;
}

MeshHandle MeshLoader::LoadMesh( const string& a_Path, bool a_GenerateMaterials )
{
    auto meshName = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );
    meshName = meshName.substr( 0, meshName.find_last_of( '.' ) );
    // Check if this mesh has already been loaded
    auto& foundMesh = m_LoadedMeshes.find( meshName );
    if ( foundMesh != m_LoadedMeshes.end() )
        return foundMesh->second;

    MeshHandle mesh = std::make_shared< RMesh >();
    if ( !mesh->Load( a_Path, a_GenerateMaterials ) )
        return nullptr;

    m_LoadedMeshes.emplace( mesh->GetResourceName(), mesh );

    return mesh;
}

MeshHandle MeshLoader::GetMesh( const string& a_Name )
{
    return m_LoadedMeshes.find( a_Name )->second;
}

template < typename Func >
void ForEachBoneDescendent( const aiNode* a_Node, Func&& Function )
{
    for ( int i = 0; i < a_Node->mNumChildren; ++i )
    {
        Function( a_Node );
        ForEachBoneDescendent( a_Node->mChildren[ i ], Function );
    }
}


SkeletonHandle MeshLoader::LoadSkeleton( const string& a_Path, const string& a_Name, const aiNode* a_BoneRootNode )
{
    SkeletonHandle skeleton = std::make_shared<RSkeleton>();
    
    // Add root bone to the skeleton's bones
    skeleton->m_Bones.emplace_back().Name = a_BoneRootNode->mName.C_Str();

    // Recursive loop for adding each child bone 
    ForEachBoneDescendent( a_BoneRootNode,
        [&skeleton]( const aiNode* a_Node )
        {
            
            auto it = std::find_if( skeleton->m_Bones.begin(), skeleton->m_Bones.end(),
                [a_Node]( const Bone& a )
                {
                    return a.Name == a_Node->mParent->mName.C_Str();
                } );

            if ( it == skeleton->m_Bones.end() )
            {
                LOG( Error, ( "Child Bone doesn't have parent " + string( a_Node->mName.C_Str() ) ).c_str() );
                return;
            }

            int parentIndex = it - skeleton->m_Bones.begin();

            auto& bone = skeleton->m_Bones.emplace_back();
            bone.Name = a_Node->mName.C_Str();
            bone.Parent = parentIndex;

            // Assimp loads matricies as Row Major so we must convert it to Column major
            bone.BindTransform = glm::transpose( glm::make_mat4x4( ( ( ai_real* ) & a_Node->mTransformation )) );
        }
    );

    skeleton->GenerateBoneData();

    skeleton->ConstuctResourceInfo( a_Path, a_Name );

    return skeleton;
}
