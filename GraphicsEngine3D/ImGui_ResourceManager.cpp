#include "ImGui_ResourceManager.h"

ImGui_ResourceManager::ImGui_ResourceManager()
{
	m_WindowName = "Resources";
}

ImGui_ResourceManager::~ImGui_ResourceManager()
{
}

void ImGui_ResourceManager::BeginDraw()
{
	//m_ResourceManager = ResourceManager::GetInstance();

	ImGui::Begin(m_WindowName.c_str());
}

void ImGui_ResourceManager::OnDraw()
{
	//if (m_ResourceManager == nullptr)
	//	return;

	//if (ImGui::TreeNode("Meshes"))
	//{
	//	auto& LoadedMeshes = ResourceManager::GetLoadedOBJMeshes();
	//	for (auto i : LoadedMeshes)
	//	{
	//		ImGui::Text(i.second->GetResourceName().c_str());
	//	}

	//	ImGui::TreePop();
	//}

	//if (ImGui::TreeNode("Materials"))
	//{
	//	auto& LoadedMaterials = ResourceManager::GetLoadedMaterials();
	//	for (auto i : LoadedMaterials)
	//	{
	//		if (ImGui::TreeNode(i.first.c_str()))
	//		{
	//			NOT_IMPLEMENTED;
	//			//ImGui::Text("Diffuse:  [%s]", i.second->DiffuseTexture.getFilename().c_str());
	//			//ImGui::Text("Normal:   [%s]", i.second->NormalTexture.getFilename().c_str());
	//			//ImGui::Text("Specular: [%s]", i.second->SpecularTexture.getFilename().c_str());

	//			ImGui::TreePop();
	//		}
	//	}

	//	ImGui::TreePop();
	//}
}

void ImGui_ResourceManager::EndDraw()
{
	ImGui::End();
}
