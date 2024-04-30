#include "Material.h"

#pragma region Material

RMaterial::~RMaterial()
{
}

void RMaterial::SetShader( ShaderHandle a_Shader )
{
	m_Shader = a_Shader;
}

bool RMaterial::IsValid()
{
	if ( !m_Shader )
		return false;

	return true;
}

void RMaterial::Use()
{
	TODO( "Should use uniform location int instead of the name!" );

	m_Shader->bindUniform( "Ka", Ambient );
	m_Shader->bindUniform( "Kd", Diffuse );
	m_Shader->bindUniform( "Ks", Specular );
	m_Shader->bindUniform( "SpecularPower", SpecularPower );

	if ( DiffuseTexture )
	{
		DiffuseTexture->Bind( 0 );
		m_Shader->bindUniform( "DiffuseTexture", 0 );
	}
	if ( AlphaTexture )
	{
		AlphaTexture->Bind( 1 );
		m_Shader->bindUniform( "AlphaTexture", 1 );
	}
	if ( AmbientTexture )
	{
		AmbientTexture->Bind( 2 );
		m_Shader->bindUniform( "AmbientTexture", 2 );
	}
	if ( SpecularTexture )
	{
		SpecularTexture->Bind( 3 );
		m_Shader->bindUniform( "SpecularTexture", 3 );
	}
	if ( EmissiveTexture )
	{
		EmissiveTexture->Bind( 4 );
		m_Shader->bindUniform( "EmissiveTexture", 4 );
	}
	if ( NormalTexture )
	{
		NormalTexture->Bind( 5 );
		m_Shader->bindUniform( "NormalTexture", 5 );
	}
	if ( DisplacementTexture )
	{
		DisplacementTexture->Bind( 6 );
		m_Shader->bindUniform( "DisplacementTexture", 6 );
	}
}

#pragma endregion

#pragma region Loader

MaterialHandle MaterialLoader::Create( const string& a_Name, ShaderHandle a_Shader = nullptr )
{
	if ( auto foundMaterial = GetInstance()->GetMaterial( a_Name ); foundMaterial != nullptr )
		return foundMaterial;

	MaterialHandle newMaterial = std::make_shared<RMaterial>();
	newMaterial->SetResourceName( a_Name );
	newMaterial->SetShader( a_Shader );

	return newMaterial;
}

MaterialHandle MaterialLoader::GetMaterial( const string& a_Name )
{
	auto it = m_Materials.find( a_Name );
	if ( it != m_Materials.end() )
		return it->second;

	return nullptr;
}

#pragma endregion