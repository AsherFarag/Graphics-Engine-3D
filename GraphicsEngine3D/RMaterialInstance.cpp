#include "RMaterialInstance.h"
//
//#include "RTexture.h"
//#include "ResourceManager.h"
//
//RMaterialInstance::RMaterialInstance( MaterialHandle a_MasterMaterial)
//{
//	m_MasterMaterial = a_MasterMaterial;
//	auto whiteTex  = Resource::GetTexture( "White" );
//	auto grayTex   = Resource::GetTexture( "Gray" );
//	auto blackTex  = Resource::GetTexture( "Black" );
//	auto normalTex = Resource::GetTexture( "Normal" );
//
//	DiffuseTexture			 = grayTex;
//	AlphaTexture			 = blackTex;
//	AmbientTexture			 = grayTex;
//	SpecularTexture			 = grayTex;
//	SpecularHighlightTexture = blackTex;
//	NormalTexture			 = normalTex;
//	DisplacementTexture		 = blackTex;
//}
//
//bool RMaterialInstance::Load(const string& a_FileName)
//{
//	TODO("Make a material instance save file");
//	return false;
//}
//
//void RMaterialInstance::Bind()
//{
//	ShaderHandle Shader = m_MasterMaterial->GetShader();
//
//	TODO("Should use uniform location int instead of the name!");
//
//	Shader->bindUniform("Ka", Ambient);
//	Shader->bindUniform("Kd", Diffuse);
//	Shader->bindUniform("Ks", Specular);
//	Shader->bindUniform("SpecularPower", SpecularPower);
//
//	if (DiffuseTexture)
//	{
//		DiffuseTexture->Bind(0);
//		Shader->bindUniform("DiffuseTex", 0);
//	}
//	if (AlphaTexture)
//	{
//		AlphaTexture->Bind(1);
//		Shader->bindUniform("AlphaTexture", 1);
//	}
//	if (AmbientTexture)
//	{
//		AmbientTexture->Bind(2);
//		Shader->bindUniform("AmbientTexture", 2);
//	}
//	if (SpecularTexture)
//	{
//		SpecularTexture->Bind(3);
//		Shader->bindUniform("SpecularTexture", 3);
//	}
//	if ( SpecularHighlightTexture )
//	{
//		SpecularHighlightTexture->Bind( 4 );
//		Shader->bindUniform( "SpecularHighlightTexture", 4 );
//	}
//	if (NormalTexture)
//	{
//		NormalTexture->Bind( 5 );
//		Shader->bindUniform("NormalTexture", 5);
//	}
//	if (DisplacementTexture)
//	{
//		DisplacementTexture->Bind(6);
//		Shader->bindUniform("DisplacementTexture", 6);
//	}
//}
