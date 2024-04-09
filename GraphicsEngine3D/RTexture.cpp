#include "RTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RTexture::RTexture()
{
}

RTexture::~RTexture()
{
}

bool RTexture::Load(const string& a_FileName)
{
	if (m_glHandle != 0) {
		glDeleteTextures(1, &m_glHandle);
		m_glHandle = 0;
		m_width = 0;
		m_height = 0;
		m_filename = "none";
	}

	int x = 0, y = 0, comp = 0;
	m_loadedPixels = stbi_load(filename, &x, &y, &comp, STBI_default);

	if (m_loadedPixels != nullptr) {
		glGenTextures(1, &m_glHandle);
		glBindTexture(GL_TEXTURE_2D, m_glHandle);
		switch (comp) {
		case STBI_grey:
			m_format = RED;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y,
				0, GL_RED, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		case STBI_grey_alpha:
			m_format = RG;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, x, y,
				0, GL_RG, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		case STBI_rgb:
			m_format = RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y,
				0, GL_RGB, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		case STBI_rgb_alpha:
			m_format = RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y,
				0, GL_RGBA, GL_UNSIGNED_BYTE, m_loadedPixels);
			break;
		default:	break;
		};
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_Width = (unsigned int)x;
		m_Height = (unsigned int)y;
		m_Filename = filename;
		return true;
	}
	return false;
}

void RTexture::Generate(unsigned int a_Width, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data)
{
	glGenTextures(1, &ID);

	m_Width	 = a_Width;
	m_Height = 0;
	m_Depth	 = 0;
	m_InternalFormat = a_InternalFormat;
	m_Format = a_Format;
	m_Type = a_Type;
}

void RTexture::Bind(int Unit)
{
	if (Unit >= 0)
		glActiveTexture(GL_TEXTURE0 + Unit);
	else
		glBindTexture(m_Target, ID);
}

void RTexture::Unbind()
{
	glBindTexture(m_Target, 0);
}
