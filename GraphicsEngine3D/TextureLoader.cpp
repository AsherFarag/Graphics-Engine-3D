#include "TextureLoader.h"

#include <stb_image.h>

TextureLoader::TextureLoader()
{
	LoadTexture( "Content/Textures/Black.tga", GL_TEXTURE_2D, GL_RGBA );
	LoadTexture( "Content/Textures/Gray.tga", GL_TEXTURE_2D, GL_RGBA );
	LoadTexture( "Content/Textures/White.tga", GL_TEXTURE_2D, GL_RGBA );
	LoadTexture( "Content/Textures/Normal.tga", GL_TEXTURE_2D, GL_RGBA );
	LoadTexture( "Content/Textures/Sky.jpeg", GL_TEXTURE_2D, GL_RGBA );
}

TextureLoader* TextureLoader::GetInstance()
{
	static TextureLoader Instance;
	return &Instance;
}

TextureHandle TextureLoader::LoadTexture( const string& a_Path, GLenum a_Target, GLenum a_InternalFormat, bool a_srgb )
{
	auto textureName = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );

	// Check if this texture has already been loaded
	auto& foundTexture = m_LoadedTextures.find( textureName );
	if ( foundTexture != m_LoadedTextures.end() )
		return foundTexture->second;

	TextureHandle tex = std::make_shared< RTexture >();

	tex->m_Target = a_Target;
	tex->m_InternalFormat = a_InternalFormat;
	if ( a_InternalFormat == GL_RGB || a_InternalFormat == GL_SRGB )
		tex->m_InternalFormat = a_srgb ? GL_SRGB : GL_RGB;
	if ( a_InternalFormat == GL_RGBA || a_InternalFormat == GL_SRGB_ALPHA )
		tex->m_InternalFormat = a_srgb ? GL_SRGB_ALPHA : GL_RGBA;

	// Flip textures on their y coordinate while loading
	stbi_set_flip_vertically_on_load( false );

	int width, height, numComponents;
	unsigned char* data = stbi_load( a_Path.c_str(), &width, &height, &numComponents, 0 );
	if ( data )
	{
		GLenum format;
		if ( numComponents == 1 )
			format = GL_RED;
		else if ( numComponents == 3 )
			format = GL_RGB;
		else if ( numComponents == 4 )
			format = GL_RGBA;

		if ( a_Target == GL_TEXTURE_1D )
			tex->Generate( width, tex->m_InternalFormat, format, GL_UNSIGNED_BYTE, data );
		else if ( a_Target == GL_TEXTURE_2D )
			tex->Generate( width, height, tex->m_InternalFormat, format, GL_UNSIGNED_BYTE, data );
		// STB Clean up
		stbi_image_free( data );
	}
	else
	{
		printf( "Texture failed to load at path: [%s]", a_Path.c_str() );
		// STB Clean up
		stbi_image_free( data );
		return nullptr;
	}
	tex->m_Width = width;
	tex->m_Height = height;

	tex->ConstuctResourceInfo( a_Path );

	m_LoadedTextures.emplace( tex->GetResourceName(), tex );

	tex->ConstuctResourceInfo( a_Path );

	return tex;
}

TextureHandle TextureLoader::GetTexture( const std::string& a_Name )
{
	return m_LoadedTextures.find( a_Name )->second;
}
