#include "RTexture.h"

RTexture::RTexture()
{
}

void RTexture::Generate( unsigned int a_Width, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data )
{
	glGenTextures( 1, &ID );

	m_Width			 = a_Width;
	m_Height		 = 0;
	m_Depth			 = 0;
	m_InternalFormat = a_InternalFormat;
	m_Format		 = a_Format;
	m_Type			 = a_Type;

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

void RTexture::Generate( unsigned int a_Width, unsigned int a_Height, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data )
{
	glGenTextures( 1, &ID );

	m_Width			 = a_Width;
	m_Height		 = a_Height;
	m_Depth			 = 0;
	m_InternalFormat = a_InternalFormat;
	m_Format		 = a_Format;
	m_Type			 = a_Type;

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

void RTexture::Generate( unsigned int a_Width, unsigned int a_Height, unsigned int a_Depth, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data )
{
	glGenTextures( 1, &ID );

	m_Width			 = a_Width;
	m_Height		 = a_Height;
	m_Depth			 = a_Depth;
	m_InternalFormat = a_InternalFormat;
	m_Format		 = a_Format;
	m_Type			 = a_Type;

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
