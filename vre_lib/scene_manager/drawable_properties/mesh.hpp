#pragma once

#include "../../common.hpp"
#include "drawable_property.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace vre
{
	struct DrawableData;
	struct VertexAndIndexBuffer;

	/// @brief The mesh property contains vertices, indices, normals, texCoords and colors.
	class Mesh : public DrawableProperty
	{
	public:
		static DrawablePropertyId GetClassPropertyId() { 
			static DrawablePropertyId propertyId = DrawableProperty::GetUniquePropertyId();
			return propertyId;
		}

		/// @brief Creating a mesh based on a 3D Object file and a color.
		/// @param meshPath - The path to the file containing the 3D mesh.
		/// @param meshColor - A color that a fragment shader can use to color the vertices.
		/// @param randomizeSurfaceColors - If set to true, the surfaces colors will be randomzed a bit in order to make the object better visible.
		/// @return The mesh property
		Mesh(const std::string& meshPath, const QVector4D& meshColor, bool randomizeSurfaceColors = false);

		/// @brief Create a mesh property from a custom vertexBuffer, an indexBuffer and a colorBuffer. All need to have the same count.
		/// @param vertexBuffer - A vertex buffer.
		/// @param indexBuffer - An index buffer.
		/// @param meshColor - Colors for each vertex.
		/// @param texCoords - The texture coordinates for this mesh
		/// @return The mesh property.
		Mesh(const std::vector<QVector3D>& vertexBuffer, const std::vector<uint32_t>& indexBuffer, const std::vector<QVector4D>& colorBuffer, const std::vector<QVector2D>& texCoords);

		/// @brief Allocate a vulkan buffer saving the mesh data according to the used vertex layout.
		/// @param drawableData - The drawable data to access the vertex layout.
		void AllocateMemory(std::shared_ptr<DrawableData>& drawableData) override;

		/// @brief Mesh data is tied to the drawables and freed automatically when a drawable is destroyed. Since multiple drawables can contain the same mesh,
		/// freeing a single drawable won't do anything here.
		void FreeMemory(std::shared_ptr<DrawableData>& drawableData) override {};

		/// @brief Returns a vector containing all vertices required for drawing;
		/// @return A vertex vector.
		std::vector<QVector3D>* GetVertices();

		/// @brief Returns a vector containing the vertex indices.
		/// @return The vertex indices.
		std::vector<uint32_t>* GetIndices();

		/// @brief May return a vector of tex coords. The tex coords correspond the vertices vector by index.
		/// Its possible that a model doesn't have texture coords, then the vectors size is null.
		/// @return A vector with the tex coords.
		std::vector<QVector2D>* GetTexCoords();

		/// @brief Returns a vector containing the object vertex normals
		/// @return The objects vertex normals. 
		std::vector<QVector3D>* GetNormals();

		/// @brief Returns a vector with either a single color or a color per vertex. If no color was set in the constructor,
		/// white will be the default color.
		/// If the vectors size equals 1, use this color for all vertices.
		/// @return 
		std::vector<QVector4D>* GetColors();

		/// @brief Returns the size of the stored mesh in bytes.
		/// @return Meshsize in bytes. 
		//size_t GetMeshSize() { return m_meshSize; }

		/// @brief Get the number of indices describing the mesh.
		/// @return Number of indices.
		size_t GetIndexBufferSize() { return m_indices.size(); }

	private:

		/// @brief Map holding weak pointers as lookup for meshes.
		/// If a mesh together with a shader combination has created a vertex buffer it can be reused.
		static inline std::unordered_map<Mesh*, std::shared_ptr<std::unordered_map<uint64_t, std::weak_ptr<VertexAndIndexBuffer>>>> s_mapMeshToShaderId{};

		std::string m_meshPath;
		std::vector<QVector3D> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<QVector2D> m_texCoords;
		std::vector<QVector4D> m_colors;
		std::vector<QVector3D> m_normals;

		static inline Assimp::Importer s_importer{};
	};
}
