#include "MaterialLoader.h"
#include "ShaderLoader.h"
#include <utility>

MaterialLoader::MaterialLoader()
{
    auto defualtMasterMaterial = InstantiateMaterial( "Default" );
    auto defualtMatInstance = InstantiateMaterialInstance( defualtMasterMaterial, "Default" );
}


MaterialLoader* MaterialLoader::GetInstance()
{
    static MaterialLoader Instance;
    return &Instance;
}

MaterialHandle MaterialLoader::GetMaterial( const string& a_Name )
{
    return m_Materials.find(a_Name)->second;
}

MaterialHandle MaterialLoader::InstantiateMaterial( const string& a_Name, ShaderHandle a_Shader )
{
    // Check if this material has already been loaded
    auto& foundMaterial = m_Materials.find( a_Name );
    if ( foundMaterial != m_Materials.end() )
    {
        printf( R"(Cannot instantiate new material with name: [%s] as a material with this name already exists)", a_Name );
        LOG( Default, ( "Cannot instantiate new material with name: " + a_Name + " as a material with this name already exists" ).c_str());
        return foundMaterial->second;
    }

    auto& newMaterial = m_Materials.insert( { a_Name, std::make_shared<RMaterial>()}).first->second;
    a_Shader ? newMaterial->m_Shader = a_Shader : newMaterial->m_Shader = ShaderLoader::GetDefaultShader();
    return newMaterial;
}

MaterialInstanceHandle MaterialLoader::GetMaterialInstance( const string& a_Name )
{
    return m_MaterialInstances.find(a_Name)->second;
}

MaterialInstanceHandle MaterialLoader::InstantiateMaterialInstance( MaterialHandle a_Master, const string& a_Name )
{
    // Check if this material instance has already been made
    auto& foundInstance = m_MaterialInstances.find( a_Name );
    if ( foundInstance != m_MaterialInstances.end() )
    {
        printf( R"(Cannot instantiate new material instance with name: [%s] as a material instance with this name already exists)", a_Name );
        LOG( Default, ( "Cannot instantiate new material instance with name: " + a_Name + " as a material instance with this name already exists" ).c_str() );
        return foundInstance->second;
    }

    if ( !a_Master && !a_Master->IsValid() )
        return nullptr;

    return m_MaterialInstances.emplace( a_Name, std::make_shared< RMaterialInstance >( a_Master ) ).first->second;
}

MaterialInstanceHandle MaterialLoader::InstantiateMaterialInstance( const string& a_MasterName, const string& a_Name )
{
    // Find the Master material from the name
    auto& foundMaster = m_Materials.find( a_MasterName );
    if ( foundMaster == m_Materials.end() )
    {
        printf( "Cannot find master material with name: [%s]", a_MasterName );
        LOG( Default, ( "Cannot find master material with name: ", a_MasterName ).c_str() );
        return nullptr;
    }

    return InstantiateMaterialInstance( foundMaster->second, a_Name );
}
