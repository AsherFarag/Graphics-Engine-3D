#include "AStaticMesh.h"

AStaticMesh::AStaticMesh()
	: AStaticMesh(EPM_Box)
{
	
}

AStaticMesh::AStaticMesh(EPrimitiveMesh a_Primitive)
{
	std::string PrimitiveFileName = "Primitives/";

	switch (a_Primitive)
	{
	case EPM_Box:
		PrimitiveFileName.append("Box");
		break;
	case EPM_Cylinder:
		PrimitiveFileName.append("Cylinder");
		break;
	case EPM_Pyramid:
		PrimitiveFileName.append("Pyramid");
		break;
	case EPM_Sphere:
		PrimitiveFileName.append("Sphere");
		break;
	case EPM_Cone:
		PrimitiveFileName.append("Cone");
		break;
	default:
		PrimitiveFileName.append("Box");
		break;
	}

	PrimitiveFileName.append(".obj");

	m_Mesh = AddComponent<UMeshRenderer>(this);
	m_Mesh->SetMesh(PrimitiveFileName.c_str(), false);
}

AStaticMesh::AStaticMesh(const char* a_FilePath, bool a_LoadTextures, bool a_FlipTextureV)
	: AActor()
{
	m_Name = "Static Mesh";
	m_Mesh = AddComponent<UMeshRenderer>(this);
	m_Mesh->SetMesh(a_FilePath, a_LoadTextures, a_FlipTextureV);
}

AStaticMesh::~AStaticMesh()
{
}
