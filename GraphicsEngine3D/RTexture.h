#pragma once
#include "RResource.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

class RTexture :
    public RResource
{
public:
    RTexture();
    ~RTexture();

private:
    unsigned int ID;

    GLenum m_Target          = GL_TEXTURE_2D;           // what type of texture we're dealing with
    GLenum m_InternalFormat  = GL_RGBA;                 // number of color components
    GLenum m_Format          = GL_RGBA;                 // the format each texel is stored in
    GLenum m_Type            = GL_UNSIGNED_BYTE;
    GLenum m_FilterMin       = GL_LINEAR_MIPMAP_LINEAR; // what filter method to use during minification
    GLenum m_FilterMax       = GL_LINEAR;               // what filter method to use during magnification
    GLenum m_WrapS           = GL_REPEAT;               // wrapping method of the S coordinate
    GLenum m_WrapT           = GL_REPEAT;               // wrapping method of the T coordinate
    GLenum m_WrapR           = GL_REPEAT;               // wrapping method of the R coordinate
    bool Mipmapping          = true;

    unsigned int m_Width  = 0;
    unsigned int m_Height = 0;
    unsigned int m_Depth  = 0;

public:
};

