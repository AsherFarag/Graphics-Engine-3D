#include "MeshLoader.h"

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
