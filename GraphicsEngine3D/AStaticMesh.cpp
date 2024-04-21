#include "AStaticMesh.h"

AStaticMesh::AStaticMesh()
{
	m_Name = "Static Mesh";
	m_MeshRenderer = AddComponent<UMeshRenderer>( this );
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

	m_Name = "Static Mesh";
	m_MeshRenderer = AddComponent<UMeshRenderer>(this);
	m_MeshRenderer->SetMesh(PrimitiveFileName.c_str(), false);
}

AStaticMesh::AStaticMesh(const char* a_FilePath, bool a_LoadTextures, bool a_FlipTextureV)
	: AActor()
{
	m_Name = "Static Mesh";
	m_MeshRenderer = AddComponent<UMeshRenderer>(this);
	m_MeshRenderer->SetMesh(a_FilePath, a_LoadTextures, a_FlipTextureV);
}

AStaticMesh::~AStaticMesh()
{
}
