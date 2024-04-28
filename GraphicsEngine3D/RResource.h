#pragma once

// --- STD ---
#include <string>
using std::string;

// --- GLM ---
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include <glm/gtc/quaternion.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;

#define RESOURCE_PATH "Resources/"

#define DEFINE_CONSTRUCTORS( Name ) \
	Name() = default; \
	Name(const Name&) = default; \
	Name(Name&&) = default; \
	Name& operator=(const Name&) = default; \
	Name& operator=(Name&&) = default;

class RResource
{

	friend class ShaderLoader;
protected:
	string m_FilePath;		 // Full File path			E.g. 'FilePath/Soulspear.obj'
	string m_ResourceName;	 //	The File name			E.g. 'Soulspear'
	string m_FileType;		 // The File Extension		E.g. '.obj'

	void ConstuctResourceInfo( const string& a_Path );
	void ConstuctResourceInfo( const string& a_Path, const string& a_ResourceName );
	void SetFilePath( const string& a_Path );
	void SetResourceName( const string& a_ResourceName );
	void SetFileType( const string& a_FileType );

public:
	const string GetFilePath()		const { return m_FilePath; }
	const string GetResourceName()  const { return m_ResourceName; }
	const string GetFileType()		const { return m_FileType; }
};

using ResourceHandle = std::shared_ptr< RResource >;