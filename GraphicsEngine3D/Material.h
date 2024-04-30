#pragma once
#include "RResource.h"

#pragma region Material
class RMaterial :
	public RResource
{
	friend class MaterialLoader;

	RMaterial();
	~RMaterial();

public:
	ShaderHandle GetShader() { return m_Shader; }
	void SetShader( ShaderHandle a_Shader );
	bool IsValid();

	void Use();

private:
	ShaderHandle m_Shader = nullptr;

	vec3 Ambient  = vec3(1);
	vec3 Diffuse  = vec3(1);
	vec3 Specular = vec3(1);
	vec3 Emissive = vec3(0);

	float SpecularPower = 1.f;
	float Opacity		= 1.f;

	TextureHandle DiffuseTexture			 = nullptr;	// bound slot 0
	TextureHandle AlphaTexture				 = nullptr;	// bound slot 1
	TextureHandle AmbientTexture			 = nullptr;	// bound slot 2
	TextureHandle SpecularTexture			 = nullptr;	// bound slot 3
	TextureHandle EmissiveTexture			 = nullptr;	// bound slot 4
	TextureHandle NormalTexture				 = nullptr;	// bound slot 5
	TextureHandle DisplacementTexture		 = nullptr;	// bound slot 6
};

using MaterialHandle = std::shared_ptr<RMaterial>;
#pragma endregion

#pragma region Loader
class MaterialLoader
{
private:
	MaterialLoader();
	~MaterialLoader() = default;

	std::map<string, MaterialHandle > m_Materials;

public:
	MaterialLoader( MaterialLoader& Other ) = delete;
	void operator=( const MaterialLoader& ) = delete;

	static MaterialLoader* GetInstance();

	static MaterialHandle Create( const string& a_Name, ShaderHandle a_Shader = nullptr );
	MaterialHandle LoadMaterial( const string& a_Path ) { NOT_IMPLEMENTED; }
	MaterialHandle GetMaterial( const string& a_Name );
};
#pragma endregion