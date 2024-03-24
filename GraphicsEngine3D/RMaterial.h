#pragma once
#include "RResource.h"

// --- AIE ---
#include "Texture.h"
#include "Shader.h"
using namespace aie;

class RMaterial :
    public RResource
{
public:
	RMaterial();
	~RMaterial() {}

	void Bind();

	void BindAmbient();
	void BindDiffuse();
	void BindSpecular();
	void BindEmissive();
	void BindSpecularPower();
	void BindOpacity();

	bool LoadMaterial(const char* a_FileName);
	bool LoadShader(const char* a_FileName);
	bool LoadTexture(const char* a_FileName);

	ShaderProgram* m_Shader;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Emissive;

	float SpecularPower;
	float Opacity;

	Texture DiffuseTexture;				// bound slot 0
	Texture AlphaTexture;				// bound slot 1
	Texture AmbientTexture;				// bound slot 2
	Texture SpecularTexture;			// bound slot 3
	Texture SpecularHighlightTexture;	// bound slot 4
	Texture NormalTexture;				// bound slot 5
	Texture DisplacementTexture;		// bound slot 6
};

