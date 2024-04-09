#pragma once
#include "RResource.h"

class RTexture;
class RMaterial;

class RMaterialInstance :
    public RResource
{
public:
	RMaterialInstance(RMaterial* a_MasterMaterial = nullptr);

	friend class RMaterial;

public:	TODO("Make these members private with getters");
	vec3 Ambient  = vec3(1);
	vec3 Diffuse  = vec3(1);
	vec3 Specular = vec3(1);
	vec3 Emissive = vec3(0);

	float SpecularPower = 1.f;
	float Opacity		= 1.f;

	RTexture* DiffuseTexture			 = nullptr;	// bound slot 0
	RTexture* AlphaTexture				 = nullptr;	// bound slot 1
	RTexture* AmbientTexture			 = nullptr;	// bound slot 2
	RTexture* SpecularTexture			 = nullptr;	// bound slot 3
	RTexture* SpecularHighlightTexture	 = nullptr;	// bound slot 4
	RTexture* NormalTexture				 = nullptr;	// bound slot 5
	RTexture* DisplacementTexture		 = nullptr;	// bound slot 6

	const RMaterial* GetMaster() const { return m_MasterMaterial; }

private:
	// This is the owner of this material instance.
	RMaterial* m_MasterMaterial;
};

