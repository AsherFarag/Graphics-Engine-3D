#pragma once
#include "UBaseObject.h"
#include "RResource.h"

// --- AIE ---
#include "Shader.h"
using namespace aie;

class RMaterialInstance;

class RMaterial :
    public RResource
{
	friend class RenderManager;
	friend class MaterialLoader;

public:
	RMaterial();
	~RMaterial();

	ShaderHandle GetShader() { return m_Shader; }
	bool IsValid();

private:
	ShaderHandle m_Shader = nullptr;

	bool Load(const string& a_FileName);

	//void Bind();

	//void BindAmbient();
	//void BindDiffuse();
	//void BindSpecular();
	//void BindEmissive();
	//void BindSpecularPower();
	//void BindOpacity();

	//bool LoadMaterial(const char* a_FileName);
	//bool LoadShader(const char* a_FileName);
	//bool LoadTexture(const char* a_FileName);
};

using MaterialHandle = std::shared_ptr<RMaterial>;