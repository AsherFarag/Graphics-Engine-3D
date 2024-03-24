#pragma once

// --- STD ---
#include <string>

// --- GLM ---
#include "glm/common.hpp"
#include "glm/glm.hpp"
using namespace glm;

#define RESOURCE_PATH "./bin/Resources/"

class RResource
{
public:

protected:
	std::string	m_FilePath;		 // Full File path
	std::string	m_ResourceName;	 //	The File name			E.g. 'Soulspear'
	std::string m_FileType;		 // The File Extension		E.g. '.obj'

public:
	const std::string GetFilePath()		const { return m_FilePath; }
	const std::string GetResourceName() const { return m_ResourceName; }
	const std::string GetFileType()		const { return m_FileType; }
};

