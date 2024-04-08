#pragma once

// --- STD ---
#include <vector>

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- Engine ---
#include "RTexture.h"

/*
*	class RenderTarget
*	- Description:
*	  Stores buffers for colour attachments and stencils with OpenGL
*	- Use:
*	  Can be used to direct rendering into a buffer inside this class.
*	  These buffers are useful for applying post-processing and other effects.
*/

class RenderTarget
{
public:
	RenderTarget(unsigned int a_Width, unsigned int a_Height, GLenum a_Type = GL_UNSIGNED_BYTE, unsigned int NumColourAttachments = 1, bool a_UseDepthStencil = true);

private:
	unsigned int ID;

	unsigned int m_Width;
	unsigned int m_Height;
	GLenum		 m_Type;

	bool UseDepthStencil;

	GLenum m_Target = GL_TEXTURE_2D;
	RTexture m_DepthStencil;
	std::vector<RTexture> m_ColourAttachments;

	// Getters & Setters
public:
	unsigned int GetWidth() { return m_Width; }
	unsigned int GetHeight() { return m_Height; }
	void SetWidth(unsigned int a_Width) { m_Width = a_Width; }
	void SetHeight(unsigned int a_Height) { m_Height = a_Height; }

	bool HasDepth() { return UseDepthStencil; }
};

