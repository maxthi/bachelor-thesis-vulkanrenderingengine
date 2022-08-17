#include "descriptor_set_layout.hpp"

namespace vre
{
	void DescriptorSetLayout::createBasicDsl(std::vector<DslDescription>& dslDescriptions, std::string name)
	{
		size_t dslSize = dslDescriptions.size();
		std::vector<VkDescriptorSetLayoutBinding> bindings(dslSize);
		for (size_t i = 0; i < dslSize; i++)
		{
			bindings[i].binding = i;
			bindings[i].descriptorType = dslDescriptions[i].mDescriptorType;
			bindings[i].stageFlags = dslDescriptions[i].mShaderStageFlags;
			bindings[i].descriptorCount = dslDescriptions[i].mDescriptorCount;
			bindings[i].pImmutableSamplers = nullptr;
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		VkDevice device = VulkanManager::GetInstance()->GetDevice();
		checkVk(
			VulkanInterface::vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_descriptorSetLayout), // to be referenced in the pipeline
			name + "::create: failed to create descriptor set layout!"
		);
	}
}
