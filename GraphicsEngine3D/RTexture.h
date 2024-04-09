#pragma once
#include "RResource.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

enum ETextureType
{
    ETT_DIFFUSE,
    ETT_ALPHA,
    ETT_AMBIENT,
    ETT_SPECULAR,
    ETT_SPECULAR_HIGHLIGHT,
    ETT_NORMAL,
    ETT_DISPLACEMENT,
    ETT_TEXTURE_TYPE_COUNT
};

class RTexture :
    public RResource
{
public:
    RTexture();
    ~RTexture();

    // 1D texture generation
    void Generate(unsigned int a_Width, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data);
    // 2D texture generation
    void Generate(unsigned int a_Width, unsigned int a_Height, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data);
    // 3D texture generation                         
    void Generate(unsigned int a_Width, unsigned int a_Height, unsigned int a_Depth, GLenum a_InternalFormat, GLenum a_Format, GLenum a_Type, void* a_Data);

    void Bind(int Unit = -1);
    void Unbind();


private:
    unsigned int ID = -1;

    GLenum m_Target          = GL_TEXTURE_2D;           // what type of texture we're dealing with
    GLenum m_InternalFormat  = GL_RGBA;                 // number of color components
    GLenum m_Format          = GL_RGBA;                 // the format each texel is stored in
    GLenum m_Type            = GL_UNSIGNED_BYTE;
    GLenum m_FilterMin       = GL_LINEAR_MIPMAP_LINEAR; // what filter method to use during minification
    GLenum m_FilterMax       = GL_LINEAR;               // what filter method to use during magnification
    GLenum m_WrapS           = GL_REPEAT;               // wrapping method of the S coordinate
    GLenum m_WrapT           = GL_REPEAT;               // wrapping method of the T coordinate
    GLenum m_WrapR           = GL_REPEAT;               // wrapping method of the R coordinate
    bool m_Mipmapping        = true;

    unsigned int m_Width  = 0;
    unsigned int m_Height = 0;
    unsigned int m_Depth  = 0;

    ETextureType m_TextureType = (ETextureType)0;

protected:
    virtual bool Load(const string& a_FileName) override;

public:
    bool Mipmapping() { return m_Mipmapping; }

    unsigned int GetID()  { return ID; }
    unsigned int Width()  { return m_Width; }
    unsigned int Height() { return m_Height; }
    unsigned int Depth()  { return m_Depth; }
    ETextureType Type()   { return m_TextureType; }
};

