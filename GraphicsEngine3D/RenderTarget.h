#pragma once

// --- STD ---
#include <vector>

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- Engine ---
#include "RTexture.h"

class RenderTarget
{
public:
	RenderTarget(unsigned int a_Width, unsigned int a_Height, GLenum a_Type = GL_UNSIGNED_BYTE, unsigned int NumColourAttachments = 1, bool UseDepthAndStencil = true);

private:
	unsigned int ID;

	unsigned int m_Width;
	unsigned int m_Height;
	GLenum		 m_Type;

	bool HasDepthAndStencil;

	GLenum m_Target = GL_TEXTURE_2D;
	RTexture m_DepthStencil;
	std::vector<RTexture> m_ColourAttachments;

};

