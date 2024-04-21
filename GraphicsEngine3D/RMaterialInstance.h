#pragma once
#include "RResource.h"
#include "RTexture.h"
#include "RMaterial.h"

// --- STD ---
#include <memory>

//
// Master materials should have a default set of properties.
// A Material instance should only override the properties that are different.
// Set up a property system or something
//
// P.s. Master Materials in unreal complile into their own shader and material instances just bind the uniforms they need to.

class RMaterialInstance :
    public RResource
{
	friend class RenderManager;
	friend class MaterialLoader;

private:

	MaterialHandle m_MasterMaterial;

public:
	DEFINE_CONSTRUCTORS( RMaterialInstance );

	RMaterialInstance( MaterialHandle a_MasterMaterial );

	bool Load( const string& a_FileName );
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
	TextureHandle SpecularHighlightTexture	 = nullptr;	// bound slot 4
	TextureHandle NormalTexture				 = nullptr;	// bound slot 5
	TextureHandle DisplacementTexture		 = nullptr;	// bound slot 6

	auto& GetMaster() { return m_MasterMaterial; }

	void Bind();
};

using MaterialInstanceHandle = std::shared_ptr< RMaterialInstance >;