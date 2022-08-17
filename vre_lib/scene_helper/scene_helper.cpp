#include "scene_helper.hpp"
#include "../shader_manager/shader_manager.hpp"
#include "uniform_buffers/ub_texture.hpp"
#include "uniform_buffers/ub_color.hpp"
#include "uniform_buffers/ub_render_target.hpp"
#include "vertex_layouts/vl_texture.hpp"
#include "vertex_layouts/vl_color.hpp"

namespace vre::SceneHelper
{
	std::shared_ptr<Shaders<UniformBufferTexture>> GetShadersTexturedObject(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		auto fragmentShader = FragmentShader::GetFragmentShader(fragmentShaderFilePath);
		auto vertexShader = VertexShader::GetVertexShader(vertexShaderFilePath, std::make_shared<VertexLayoutTexture>());
		auto shaderCombination = ShaderManager::GetInstance()->GetShaderCombination(vertexShader, fragmentShader, UniformBufferTexture::GetDescriptorSetLayout());
		return std::make_shared<Shaders<UniformBufferTexture>>(shaderCombination);
	}

	std::shared_ptr<Shaders<UniformBufferColor>> GetShadersColoredObject(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		auto fragmentShader = FragmentShader::GetFragmentShader(fragmentShaderFilePath);
		auto vertexShader = VertexShader::GetVertexShader(vertexShaderFilePath, std::make_shared<VertexLayoutColor>());
		auto shaderCombination = ShaderManager::GetInstance()->GetShaderCombination(vertexShader, fragmentShader, UniformBufferColor::GetDescriptorSetLayout());
		return std::make_shared<Shaders<UniformBufferColor>>(shaderCombination);
	}

	std::shared_ptr<Shaders<UniformBufferRenderTarget>> GetRenderTargetShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		auto fragmentShader = FragmentShader::GetFragmentShader(fragmentShaderFilePath);
		auto vertexShader = VertexShader::GetVertexShader(vertexShaderFilePath, std::make_shared<VertexLayoutTexture>());
		auto shaderCombination = ShaderManager::GetInstance()->GetShaderCombination(vertexShader, fragmentShader, UniformBufferRenderTarget::GetDescriptorSetLayout());
		return std::make_shared<Shaders<UniformBufferRenderTarget>>(shaderCombination);
	}

	std::shared_ptr<Mesh> GetMeshSimplePlane(const QVector4D& color)
	{
		std::vector<QVector3D> vertices =
		{
			{-1.0f, 1.0f, 1.0f}, // left top corner
			{1.0f, 1.0f, 1.0f}, // right top corner
			{1.0f, -1.0f, 1.0f}, // right bottom corner
			{-1.0f, -1.0f, 1.0f}, // left bottom corner
		};

		std::vector<uint32_t> indices =
		{
			0, 1, 3,
			3, 1, 2
		};

		std::vector<QVector4D> colors =
		{
			color,
			color,
			color,
			color
		};

		std::vector<QVector2D> texCoords =
		{
			{0.0f, 0.0f}, // left top corner
			{1.0f, 0.0f}, // right top corner
			{1.0f, 1.0f}, // right bottom corner
			{0.0f, 1.0f}, // left bottom corner
		};

		return std::make_shared<Mesh>(vertices, indices, colors, texCoords);
	}
}