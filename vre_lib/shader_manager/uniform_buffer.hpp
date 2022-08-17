#pragma once
#include "../common.hpp"
#include "descriptor_set_layout.hpp"
#include "../vulkan_manager/buffer.hpp"
#include "../scene_reader/structs/drawable_data.hpp"
#include "../scene_manager/scene.hpp"

namespace vre
{

	/// @brief The parentclass for all uniform buffers. Uniformbuffers are referenced in ShaderCombination.
	/// One Uniformbuffer per swapchain image is required and therefore the Uniformbuffers have to be recreated
	/// if the number of swapchain images changed.
	/// See DefaultUnifromBuffer for an example of how to overwrite this class.
	class UniformBuffer
	{
	public:

		UniformBuffer();

		struct ModelViewProjectionUniformBufferObject {
			float model[4][4];
			float view[4][4];
			float proj[4][4];
		};

		/// @brief Creating the vulkan buffer objects, the descriptor pool, the DescriptorSets and connecting the DescriptorSets with the DescriptorSetLayout.
		virtual void Create(std::shared_ptr<DrawableData> drawableData) = 0;
		virtual void Destroy();

		/// @brief Direct memory access to a certain buffer.
		/// @param swapchainImageIndex  - The index of the buffer to gain access too.
		/// @return The uniform buffer.
		virtual std::shared_ptr<Buffer> GetUniformBuffer(uint32_t swapchainImageIndex);

		/// @brief Updating the memory of a uniformbuffer. Each swapchain image has its own set of uniform buffers.
		/// @param imageIndex - The index of the buffer to update.
		virtual void UpdateUniformBuffer(uint32_t swapchainImageIndex);

		/// @brief The DescriptorSet needs to be bind during command buffer creation. One Descriptorset for each commandbuffer.
		/// @param swapchainImageIndex 
		/// @return 
		virtual VkDescriptorSet GetDescriptorSet(uint32_t swapchainImageIndex);

		/// @brief Object destruction.
		virtual ~UniformBuffer();

		/// @brief Set the scene. Used to obtain scene objects like the camera.
		/// @param scene - The Scene.
		void SetScene(std::shared_ptr<Scene> scene) { 
			m_scene = scene;
			m_camera = scene->GetCamera();
			m_proj = m_camera->GetProjectionMatrix();
			m_view = m_camera->GetViewMatrix();
			m_camera->UpdateProjectionMatrix(); // TODO: normally, there needs to be a feedback when swapchain is created, so camera can have a setup
			m_proj = m_camera->GetProjectionMatrix();
		}

	protected:

		/// @brief Helper struct for setting up the desciptor writes in uniform buffers
		struct DescriptorWrite
		{
			uint32_t descriptorCount{ 1 };
			VkDescriptorImageInfo* pImageInfo{};
			VkDescriptorBufferInfo* pBufferInfo{};
		};

		/// @brief The DescriptorSetLayout used by these uniform buffers.
		VkDescriptorSetLayout m_descriptorSetLayout{};

		/// @brief The DescriptorPoolSet to aquire the sets from
		VkDescriptorPool m_descriptorPool{};

		/// @brief The sets to be referenced during command buffer creation.
		std::vector<VkDescriptorSet> m_descriptorSets{};

		/// @brief Memory mapped uniform buffers, to be updated whenever data changes.
		std::vector<std::shared_ptr<Buffer>> m_uniformBuffers;

		/// @brief The drawable which the uniform buffer applies to.
		std::weak_ptr<DrawableData> m_drawableData;

		/// @brief A pointer for quick access to the transformation data.
		float* m_transformationData{};

		/// @brief The scene the drawable is attached to.
		std::weak_ptr<Scene> m_scene;

		/// @brief A default uniform buffer object to store the data.
		ModelViewProjectionUniformBufferObject m_defaultUbo;

		/// @brief Pointer referencing the cameras view matrix.
		QMatrix4x4* m_view{};

		/// @brief A matrix storing the projection.
		QMatrix4x4* m_proj{};

		std::shared_ptr<Camera> m_camera;

		/// @brief Helper for a basic setup of a uniform buffer by passing the relevant data. See other
		/// uniform buffers for example usage.
        /// @param drawableData - The drawable is stored and can be used for property access.
        /// @param uniformBufferSize - Size in bytes of the uniform buffer to be written in total.
        /// @param descriptorTypes - A vector with all types for the descriptor set pool.
        /// @param descriptorSetLayout - The descriptor set layout to be used.
        /// @param uniformBufferName - A name for better identification. Optional.
        void basicSetup(
			std::shared_ptr<DrawableData> drawableData,
			VkDeviceSize uniformBufferSize,
			std::vector<VkDescriptorType>& descriptorTypes,
			VkDescriptorSetLayout descriptorSetLayout,
			std::string uniformBufferName = ""
		);

		/// @brief Helper method for quick descriptor set writes. Updates the allocated descriptor sets with the desired data,
		/// @param descriptorTypes - Vector of the pool types, same as in basic setup.
		/// @param writes - the descriptor writes holding the actual data.
		/// @param swapChainIndex - Which swapchain image is addressed.
		void updateDescriptorSets(std::vector<VkDescriptorType>& descriptorTypes, std::vector<DescriptorWrite>& writes, size_t swapChainIndex);
	};
}
