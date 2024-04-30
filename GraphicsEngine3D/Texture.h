#pragma once
#include "RResource.h"

// --- OpenGL ---
#include "gl_core_4_4.h"

#pragma region Texture

enum ETextureType
{
	TEXTURE_DEFAULT,
	TEXTURE_DIFFUSE,
	TEXTURE_ALPHA,
	TEXTURE_AMBIENT,
	TEXTURE_SPECULAR,
	TEXTURE_EMISSIVE,
	TEXTURE_NORMAL,
	TEXTURE_DISPLACEMENT,
	TEXTURE_TEXTURE_TYPE_COUNT
};

enum ETextureFormat : unsigned int
{
	RED	 = GL_RED,
	RG	 = GL_RG,
	RGB	 = GL_RGB,
	RGBA = GL_RGBA
};

class RTexture : public RResource
{
	friend class TextureLoader;

public:
	RTexture();

	// 1D texture generation
	void Generate( unsigned int a_Width, void* a_Data, GLenum a_InternalFormat = GL_RGBA, GLenum a_Format = GL_RGBA, GLenum a_Type = GL_UNSIGNED_BYTE );
	// 2D texture generation
	void Generate( unsigned int a_Width, unsigned int a_Height, void* a_Data, GLenum a_InternalFormat = GL_RGBA, GLenum a_Format = GL_RGBA, GLenum a_Type = GL_UNSIGNED_BYTE );
	// 3D texture generation                         
	void Generate( unsigned int a_Width, unsigned int a_Height, unsigned int a_Depth, void* a_Data, GLenum a_InternalFormat = GL_RGBA, GLenum a_Format = GL_RGBA, GLenum a_Type = GL_UNSIGNED_BYTE );

	void Bind( int Unit = -1 );
	void Unbind();

	void Resize( unsigned int a_Width, unsigned int a_Height = 0, unsigned int a_Depth = 0 );

protected:
	unsigned int ID = -1;

	GLenum m_Target			= GL_TEXTURE_2D;           // what type of texture we're dealing with
	GLenum m_InternalFormat = GL_RGBA;                 // number of color components
	GLenum m_Format			= GL_RGBA;                 // the format each texel is stored in
	GLenum m_Type			= GL_UNSIGNED_BYTE;
	GLenum m_FilterMin		= GL_LINEAR_MIPMAP_LINEAR; // what filter method to use during minification
	GLenum m_FilterMax		= GL_LINEAR;               // what filter method to use during magnification
	GLenum m_WrapS			= GL_REPEAT;               // wrapping method of the S coordinate
	GLenum m_WrapT			= GL_REPEAT;               // wrapping method of the T coordinate
	GLenum m_WrapR			= GL_REPEAT;               // wrapping method of the R coordinate

	bool m_Mipmapping	  = false;
	unsigned int m_Width  = 0;
	unsigned int m_Height = 0;
	unsigned int m_Depth  = 0;

	ETextureType m_TextureType = TEXTURE_DEFAULT;

public:
	bool IsUsingMipmapping() { return m_Mipmapping; }
	unsigned int GetID()	 { return ID; }
	unsigned int GetWidth()  { return m_Width; }
	unsigned int GetHeight() { return m_Height; }
	unsigned int GetDepth()  { return m_Depth; }
	ETextureType GetType()	 { return m_TextureType; }
};

using TextureHandle = std::shared_ptr< RTexture >;

#pragma endregion

#pragma region Loader

class TextureLoader
{
private:
	TextureLoader();
	~TextureLoader() = default;

	std::map<string, TextureHandle> m_LoadedTextures;

public:
	TextureLoader( TextureLoader& Other ) = delete;
	void operator=( const TextureLoader& ) = delete;

	static TextureLoader* GetInstance();

	TextureHandle LoadTexture( const string& a_Path, GLenum a_Target, GLenum a_InternalFormat, bool a_srgb = false );
	TextureHandle GetTexture( const std::string& a_Name );
};

#pragma endregion

