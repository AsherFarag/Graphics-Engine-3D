#pragma once
#include "RResource.h"

// --- AIE ---
#include "Texture.h"
#include "Shader.h"
using namespace aie;

class RMaterialInstance;

class RMaterial :
    public RResource
{
public:
	RMaterial();
	~RMaterial();

	RMaterialInstance* CreateMaterialInstance();
	void BindMaterialInstance(RMaterialInstance* a_Instance);
	void UnbindMaterialInstance(RMaterialInstance* a_Instance);
	void UnbindAllMaterialInstances();

private:
	ShaderProgram* m_Shader = nullptr;

	std::vector<RMaterialInstance*> m_Instances;

protected:
	virtual bool Load(const string& a_FileName);

public:
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


};

