#pragma once
#include "../../common.hpp"
#include "drawable_property.hpp"
#include "../../shader_manager/shaders/vertex_shader.hpp"
#include "../../shader_manager/shaders/fragment_shader.hpp"
#include "../../shader_manager/shader_combination.hpp"
#include "texture_2d.hpp"
#include "../../scene_helper/uniform_buffers/ub_texture.hpp"
#include "../../scene_reader/scene_reader.hpp"

namespace vre
{
	/// @brief The shaderbase property. It carries information about the used shader combination. 
	/// This class is a little tricky in order to use individual UniformBuffer classes.
	/// Therefore the actual Shaders property is templated and a UniformBuffer can be specified.
	/// Later on, the generalized ShadersBase class object is used to call CreateUniformBuffer.
	class ShadersBase : public DrawableProperty
	{
	public:
		static DrawablePropertyId GetClassPropertyId() {
			static DrawablePropertyId propertyId = DrawableProperty::GetUniquePropertyId();
			return propertyId;
		}

		/// @brief Constructor of the ShadersBase.
		ShadersBase()
		{
			m_propertyId = ShadersBase::GetClassPropertyId();
		}

		/// @brief Get the ShaderCombination used by this set of shaders.
		/// @return The ShaderCombination.
		std::shared_ptr<ShaderCombination> GetShaderCombination() { return m_shaderCombination; }

		/// @brief Virtual method to create a uniform buffer needed for this ShaderCombination.
		/// @return A shared pointer to a UniformBuffer.
		virtual std::shared_ptr<UniformBuffer> CreateUniformBuffer() = 0;

	protected:
		std::shared_ptr<ShaderCombination> m_shaderCombination;
	};

	/// @brief This is the Shaders property to be used when defining Shaders in a project. It takes the underlying UniformBuffer class as
	/// template argument. The scene reader can then create new UniformBuffers for each drawable.
	template<typename TUniformBufferClass> 
	class Shaders : public ShadersBase
	{
	public:
		/// @brief Templated shaders constructor. Takes the required UniformBuffer class as template argument and the
		/// ShaderCombination as paramater.
		/// @param shaderCombination - The ShaderCombination.
		Shaders(std::shared_ptr<ShaderCombination> shaderCombination)
		{
			m_shaderCombination = shaderCombination;
		}

		/// @brief Creates a UniformBuffer base class object with the TUniformBufferClass as derived class.
		/// @return 
		std::shared_ptr<UniformBuffer> CreateUniformBuffer() override
		{ 
			return std::make_shared<TUniformBufferClass>();
		}

		/// @brief Create the uniform buffer for the drawable.
		/// @param drawableData 
		void AllocateMemory(std::shared_ptr<DrawableData>& drawableData) override
		{
			auto uniformBuffer = CreateUniformBuffer();
			uniformBuffer->Create(drawableData);

			// small hack: uniform buffer needs to know which scene it belongs to, to use scene relevant data like the camera.
			// instead of saving the scene additionally in each drawable, we get the scene ptr via
			// transformation node and scene reader, abusing the availability of the transformation node.
			uniformBuffer->SetScene(drawableData->mTransformationNode->GetSceneReader()->GetScene());
			drawableData->mUniformBuffer = uniformBuffer;
		}

		/// @brief The uniform buffer memory if automatically freed when he drawableData expires.
		void FreeMemory(std::shared_ptr<DrawableData>& drawableData) override
		{
		}
	};
}