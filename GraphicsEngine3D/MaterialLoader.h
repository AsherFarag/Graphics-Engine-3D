#pragma once

#include "RMaterial.h"
#include "RMaterialInstance.h"
#include <map>
#include "Material.h"

//class MaterialLoader
//{
//private:
//	MaterialLoader();
//	~MaterialLoader() = default;
//
//public:
//	MaterialLoader( MaterialLoader& Other ) = delete;
//	void operator=( const MaterialLoader& ) = delete;
//
//	static MaterialLoader* GetInstance();
//
//	std::map<string, MaterialHandle > m_Materials;
//	std::map<string, MaterialInstanceHandle > m_MaterialInstances;
//
//	MaterialHandle LoadMaterial( const string& a_Path ) {}
//	MaterialHandle GetMaterial( const string& a_Name );
//	MaterialHandle InstantiateMaterial( const string& a_Name, ShaderHandle a_Shader = nullptr );
//	MaterialInstanceHandle GetMaterialInstance( const string& a_Name );
//	MaterialInstanceHandle InstantiateMaterialInstance( MaterialHandle a_Master, const string& a_Name );
//	MaterialInstanceHandle InstantiateMaterialInstance( const string& a_MasterName, const string& a_Name );
//};
