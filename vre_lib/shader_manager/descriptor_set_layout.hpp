#pragma once
#include "../common.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"

namespace vre
{
	/// @brief Abstract class to handle the creation and deletion of vulkan descriptor set layouts.
	/// Each dsl class needs to override the create method and provide a static GetLayout method.
	/// See dsl_color.hpp for an example.
	class DescriptorSetLayout
	{
	public:
		/// @brief Destructor destroy the descriptor set layout if it was created.
		virtual ~DescriptorSetLayout()
		{
			if (m_descriptorSetLayout == VK_NULL_HANDLE)
			{
				return;
			}

			VkDevice device = VulkanManager::GetInstance()->GetDevice();
			VulkanInterface::vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
			m_descriptorSetLayout = VK_NULL_HANDLE;
		}

		/// @brief Whether push constants are used in this dsl. Push constants are not yet implemented. 
		bool UsesPushConstants()
		{
			return m_pushConstantRanges.size() > 0;
		}

		uint32_t GetPushConstantRangeCount()
		{
			return m_pushConstantRanges.size();
		}

		VkPushConstantRange* GetPushConstantRanges()
		{
			return m_pushConstantRanges.data();
		}

		/// @brief Method to access the vulkan descriptor set layout. Creates the dsl if it is null.
		/// @return The handle to the vk dsl.
		virtual VkDescriptorSetLayout GetVkDescriptorSetLayout()
		{
			if (m_descriptorSetLayout == VK_NULL_HANDLE)
			{
				create();
			}
			checkPtr(m_descriptorSetLayout, "DescriptorSetLayoutColor:GetDescriptorSetLayout: DescriptorSetLayout not created!");
			return m_descriptorSetLayout;
		}

	protected:
		/// @brief Local struct to simplify dsl creation. Pass a vector of DslDescription objects to createBasicDsl for auto creation.
		struct DslDescription
		{
			VkDescriptorType mDescriptorType{};
			VkShaderStageFlags mShaderStageFlags{};
			uint32_t mDescriptorCount{ 1 };
		};

		/// @brief Holds the push constant ranges used during pipeline creation for pipelines with this descriptor set layout.
		/// Initialize and add VkPushConstantRange structs in the derived DescriptorSetLayout.
		std::vector<VkPushConstantRange> m_pushConstantRanges{};

		/// @brief The pointer to the vulkan resource.
		VkDescriptorSetLayout m_descriptorSetLayout{ VK_NULL_HANDLE };

		/// @brief Creates the descriptor layout. Needs to be defined individually by each dsl class.
		virtual void create() = 0;

		/// @brief Helper method to simplify creating simple descriptor set layouts. Just pass an vector of objects that define the type,
		/// the desired shader stage flags and optional the descriptor count.
		/// @param dslDescriptions - Data to create the bindings.
		/// @param name - Name of the descriptor set layout for debugging purposes.
		void createBasicDsl(std::vector<DslDescription>& dslDescriptions, std::string name = "UnnamedDescriptSetLayout");
	};
}