#include "Texture.h"

#pragma region Texture

RTexture::RTexture()
{
}

void RTexture::Generate( unsigned int a_Width, void* a_Data, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type )
{
	glGenTextures( 1, &ID );

	m_Width = a_Width;
	m_Height = 0;
	m_Depth = 0;
	m_InternalFormat = a_InternalFormat;
	m_Format = a_Format;
	m_Type = a_Type;

	assert( m_Target == GL_TEXTURE_1D );
	Bind();
	glTexImage1D( m_Target, 0, m_InternalFormat, m_Width, 0, m_Format, m_Type, a_Data );
	glTexParameteri( m_Target, GL_TEXTURE_MIN_FILTER, m_FilterMin );
	glTexParameteri( m_Target, GL_TEXTURE_MAG_FILTER, m_FilterMax );
	glTexParameteri( m_Target, GL_TEXTURE_WRAP_S, m_WrapS );
	if ( m_Mipmapping )
		glGenerateMipmap( m_Target );
	Unbind();
}

void RTexture::Generate( unsigned int a_Width, unsigned int a_Height, void* a_Data, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type )
{
	glGenTextures( 1, &ID );

	m_Width = a_Width;
	m_Height = a_Height;
	m_Depth = 0;
	m_InternalFormat = a_InternalFormat;
	m_Format = a_Format;
	m_Type = a_Type;

	assert( m_Target == GL_TEXTURE_2D );
	Bind();
	glTexImage2D( m_Target, 0, m_InternalFormat, m_Width, a_Height, 0, m_Format, m_Type, a_Data );
	glTexParameteri( m_Target, GL_TEXTURE_MIN_FILTER, m_FilterMin );
	glTexParameteri( m_Target, GL_TEXTURE_MAG_FILTER, m_FilterMax );
	glTexParameteri( m_Target, GL_TEXTURE_WRAP_S, m_WrapS );
	if ( m_Mipmapping )
		glGenerateMipmap( m_Target );
	Unbind();
}

void RTexture::Generate( unsigned int a_Width, unsigned int a_Height, unsigned int a_Depth, void* a_Data, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type )
{
	glGenTextures( 1, &ID );

	m_Width = a_Width;
	m_Height = a_Height;
	m_Depth = a_Depth;
	m_InternalFormat = a_InternalFormat;
	m_Format = a_Format;
	m_Type = a_Type;

	assert( m_Target == GL_TEXTURE_3D );
	Bind();
	glTexImage3D( m_Target, 0, m_InternalFormat, m_Width, a_Height, a_Depth, 0, m_Format, m_Type, a_Data );
	glTexParameteri( m_Target, GL_TEXTURE_MIN_FILTER, m_FilterMin );
	glTexParameteri( m_Target, GL_TEXTURE_MAG_FILTER, m_FilterMax );
	glTexParameteri( m_Target, GL_TEXTURE_WRAP_S, m_WrapS );
	if ( m_Mipmapping )
		glGenerateMipmap( m_Target );
	Unbind();
}

void RTexture::Bind( int Unit )
{
	if ( Unit >= 0 )
		glActiveTexture( GL_TEXTURE0 + Unit );

	glBindTexture( m_Target, ID );
}

void RTexture::Unbind()
{
	glBindTexture( m_Target, 0 );
}

void RTexture::Resize( unsigned int a_Width, unsigned int a_Height, unsigned int a_Depth )
{
	Bind();
	switch ( m_Target )
	{
	case GL_TEXTURE_1D:
		glTexImage1D( GL_TEXTURE_1D, 0, m_InternalFormat, a_Width, 0, m_Format, m_Type, 0 );
		break;
	case GL_TEXTURE_2D:
		assert( a_Height > 0 );
		glTexImage2D( GL_TEXTURE_2D, 0, m_InternalFormat, a_Width, a_Height, 0, m_Format, m_Type, 0 );
		break;
	case GL_TEXTURE_3D:
		assert( a_Depth > 0 );
		glTexImage3D( GL_TEXTURE_2D, 0, m_InternalFormat, a_Width, a_Height, a_Depth, 0, m_Format, m_Type, 0 );
		break;
	default:
		break;
	}
}

#pragma endregion

#pragma region Loader

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
			tex->Generate( width, data, tex->m_InternalFormat, format, GL_UNSIGNED_BYTE );
		else if ( a_Target == GL_TEXTURE_2D )
			tex->Generate( width, height, data, tex->m_InternalFormat, format, GL_UNSIGNED_BYTE );
		// STBi Clean up
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
	if ( auto foundTex = m_LoadedTextures.find( a_Name ); foundTex != m_LoadedTextures.end() )
		return foundTex->second;

	return nullptr;
}

#pragma endregion