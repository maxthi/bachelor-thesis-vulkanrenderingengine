#pragma once
#include "../../common.hpp"
#include "../../shader_manager/descriptor_set_layout.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"

namespace vre
{
	class DescriptorSetLayoutTexture : public DescriptorSetLayout
	{
	public:
		/// @brief GetLayout returns a strong reference to the object managing the vk descriptor set layout.
		/// Only one descriptor set layout is needed, so the getter is static.
		static std::shared_ptr<DescriptorSetLayoutTexture> GetLayout()
		{
			std::shared_ptr<DescriptorSetLayoutTexture> dsl = s_descriptorSetLayout.lock();
			if (dsl == nullptr)
			{
				// create this descriptor set
				dsl = std::make_shared<DescriptorSetLayoutTexture>();
				s_descriptorSetLayout = dsl;
			}
			return dsl;
		}

	protected:
		/// @brief Weak pointer to the descriptor set layout. This means if all objects that store a strong reference to the
		/// dsl are deleted, the dsl is deleted as well.
		static inline std::weak_ptr<DescriptorSetLayoutTexture> s_descriptorSetLayout{};

		/// @brief Create the vulkan descriptor set layout describing the structure of its uniform buffer.
		void create() override
		{
			if (m_descriptorSetLayout != VK_NULL_HANDLE)
			{
				return;
			}

			/*VkPushConstantRange pushConstantRange{};
			pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			pushConstantRange.offset = 0;
			pushConstantRange.size = m_pushConstantSize;
			m_pushConstantRanges.push_back(pushConstantRange);*/

			std::vector<DslDescription> dslDescriptions = {
				{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
				{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
			};

			createBasicDsl(dslDescriptions, "DescriptionSetLayoutTexture");
		}

	};
}