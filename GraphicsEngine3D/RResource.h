#pragma once

// --- STD ---
#include <string>

// --- GLM ---
#include "glm/common.hpp"
#include "glm/glm.hpp"
using namespace glm;

#define RESOURCE_PATH "./Resources/"

class RResource
{
public:

protected:
	std::string	m_ResourceName; 
	std::string	m_FilePath;

public:
	const std::string GetResourceName() const { return m_ResourceName; }
	const std::string GetFilePath() const { return m_FilePath; }
};

