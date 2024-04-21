#include "ShaderLoader.h"


ShaderLoader::ShaderLoader()
{
	m_PhongShader = LoadShader( "Content/Shaders/Phong" );
}

ShaderLoader* ShaderLoader::GetInstance()
{
	static ShaderLoader Instance;
	return &Instance;
}

ShaderHandle ShaderLoader::LoadShader( const std::string& a_Path )
{
	auto shaderName = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );

	// Check if this shader has already been loaded
	auto& foundShader = m_LoadedShaders.find( shaderName );
	if ( foundShader != m_LoadedShaders.end() )
		return foundShader->second;

	auto shader = std::make_shared<aie::ShaderProgram>();

	shader->loadShader( aie::eShaderStage::VERTEX, ( a_Path + ".vert" ).c_str() );
	shader->loadShader( aie::eShaderStage::FRAGMENT, ( a_Path + ".frag" ).c_str() );

	if ( shader->link() == false )
	{
		printf( ( shaderName + " Shader Error: %s\n" ).c_str(), shader->getLastError() );
		LOG( Error, ( "Unable to link shader: " + shaderName ).c_str() );
		return nullptr;
	}

	m_LoadedShaders.emplace( shaderName, shader );

	return shader;
}

ShaderHandle  ShaderLoader::GetShader( const std::string& a_Name )
{
	auto instance = ShaderLoader::GetInstance();
	auto& foundShader = instance->m_LoadedShaders.find( a_Name );
	if ( foundShader != instance->m_LoadedShaders.end() )
		return foundShader->second;

	LOG( Warning, ( "Unable to find shader: " + a_Name ).c_str() );
	return nullptr;
}
