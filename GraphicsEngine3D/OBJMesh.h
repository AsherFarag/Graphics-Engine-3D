#pragma once

// --- STD ---
#include <string>
#include <vector>
#include <map>

// --- GLM ---
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// --- AIE ---
#include "Texture.h"

// --- Engine ---
#include "RMaterial.h"

namespace aie
{

// a simple triangle mesh wrapper
class OBJMesh
{
public:

	// a vertex structure for obj files
	struct Vertex
	{
		glm::vec4 position; // added to attrib location 0
		glm::vec4 normal;	// added to attrib location 1
		glm::vec2 texcoord;	// added to attrib location 2

							// normal-mapping data
		glm::vec4 tangent;	// added to attrib location 3
	};

	OBJMesh() { }
	~OBJMesh();

	// will fail if a mesh has already been loaded in to this instance
	bool load(const char* filename, bool loadTextures = true, bool flipTextureV = false);

	// allow option to draw as patches for tessellation
	void draw(bool usePatches = false);

	// access to the filename that was loaded
	const std::string& getFilename() const { return m_filename; }

	// material access
	size_t getMaterialCount() const { return m_materials.size();  }
	RMaterial& getMaterial(size_t index) { return m_materials[index];  }

private:

	void calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	struct MeshChunk
	{
		unsigned int	vao, vbo, ibo;
		unsigned int	indexCount;
		int				materialID;
	};

	std::string				m_filename;
	std::vector<MeshChunk>	m_meshChunks;
	std::vector<RMaterial>	m_materials;
};

} // namespace aie