#pragma once

// --- STD ---
#include <string>
using std::string;

// --- GLM ---
#include "glm/common.hpp"
#include "glm/glm.hpp"
using namespace glm;

#define RESOURCE_PATH "Resources/"

class RResource
{
public:

	friend class ResourceManager;

protected:
	string m_FilePath;		 // Full File path			E.g. 'FilePath/Soulspear.obj'
	string m_ResourceName;	 //	The File name			E.g. 'Soulspear'
	string m_FileType;		 // The File Extension		E.g. '.obj'

public:
	const string GetFilePath()		const { return m_FilePath; }
	const string GetResourceName()  const { return m_ResourceName; }
	const string GetFileType()		const { return m_FileType; }
};

