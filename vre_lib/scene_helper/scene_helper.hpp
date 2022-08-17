#include "../scene_manager/drawable_properties/shaders.hpp"
#include "../scene_manager/drawable_properties/mesh.hpp"
#include <QVector4D>

namespace vre
{
	// prototypes
	class UniformBufferTexture;
	class UniformBufferColor;
	class UniformBufferRenderTarget;

	/// @brief The SceneHelper namespace provides implementations that are focusing on common needs, like shader setups and basic meshes.
	namespace SceneHelper
	{
		/// @brief Use this if you need shaders for an object with a texture. The vertex shader will receive 3 floats as position variables and 2 floats
		/// as texture coords.
		/// @param vertexShaderFilePath - The Vertexshader.
		/// @param fragmentShaderFilePath - The Fragmentshader.
		/// @return A drawable property of type shaders.
		std::shared_ptr<Shaders<UniformBufferTexture>> GetShadersTexturedObject(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/// @brief Returns the shaders property for a vertex and fragment shader, using colors.
		/// @param vertexShaderFilePath - The Vertexshader.
		/// @param fragmentShaderFilePath - The Fragmentshader.
		/// @return The Shaders property.
		std::shared_ptr<Shaders<UniformBufferColor>> GetShadersColoredObject(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/// @brief Creates a shaders property based on a vertex shader and a fragment shader, that use render targets
		/// @param vertexShaderFilePath - The Vertexshader file path.
		/// @param fragmentShaderFilePath - The Fragmentshader file path.
		/// @return The shaders property.
		std::shared_ptr<Shaders<UniformBufferRenderTarget>> GetRenderTargetShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/// @brief Returns a mesh property containing a mesh in plane shape.
		/// @param color - The mesh base color.
		/// @return The mesh property.
		std::shared_ptr<Mesh> GetMeshSimplePlane(const QVector4D& color);
	};
}