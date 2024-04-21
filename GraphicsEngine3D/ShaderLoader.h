#pragma once
#include "Shader.h"

#include <map>
#include <string>
using std::map;
using std::string;

class ShaderLoader
{
private:
	ShaderLoader();
	~ShaderLoader() = default;

public:

	ShaderLoader( ShaderLoader& Other ) = delete;
	void operator=( const ShaderLoader& ) = delete;

	static ShaderLoader* GetInstance();

	ShaderHandle m_PhongShader;
	map< string, ShaderHandle > m_LoadedShaders;

	ShaderHandle LoadShader( const std::string& a_Path );
	ShaderHandle GetShader( const std::string& a_Name );
	static auto GetDefaultShader() { return GetInstance()->m_PhongShader; }
};

//template < typename T >
//struct ISingletone
//{
//	static struct {
//		T* operator->() { statc T Instance; return &Instance; }
//		T& operator*() { return *operator->(); }
//	} This;
//};
//
//
//
//
//struct SomeSingletonType : public ISingletone< SomeSingletonType >
//{
//private:
//	float SomeData;
//public:
//
//	static double GetData() { return This->SomeData; }
//};
