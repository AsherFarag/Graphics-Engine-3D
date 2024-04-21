#pragma once
#include "RTexture.h"

#include <map>
#include <string>
using std::map;
using std::string;

class TextureLoader
{
private:
	TextureLoader();
	~TextureLoader() = default;

	map<string, TextureHandle> m_LoadedTextures;

public:
	TextureLoader( TextureLoader& Other ) = delete;
	void operator=( const TextureLoader& ) = delete;

	static TextureLoader* GetInstance();

	TextureHandle LoadTexture( const string& a_Path, GLenum a_Target, GLenum a_InternalFormat, bool a_srgb = false );
	TextureHandle GetTexture( const std::string& a_Name );
};


// Singleton
static TextureLoader* s_TextureLoaderInstance;