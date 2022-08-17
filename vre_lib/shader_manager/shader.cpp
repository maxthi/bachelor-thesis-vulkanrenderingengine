#include "shader.hpp"
#include "../vulkan_manager/vulkan_manager.hpp"
#include "../file_system/file_system.hpp"

namespace vre
{
	Shader::Shader(const std::string shaderSourceFile) :
        m_shaderSourceFile(shaderSourceFile)
	{
        Filesystem::readFile(shaderSourceFile, &m_shaderCode);
        m_createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        m_createInfo.codeSize = m_shaderCode.size();
        m_createInfo.pCode = reinterpret_cast<const uint32_t*>(m_shaderCode.data());
	}

    void Shader::CreateShaderModule()
    {
        check(m_shaderModule == VK_NULL_HANDLE, "Shader::Shader: Failed to create ShaderModule: Module already created!");
        checkVk(
            VulkanInterface::vkCreateShaderModule(VulkanManager::GetInstance()->GetDevice(), &m_createInfo, nullptr, &m_shaderModule),
            "Shader::Shader: failed to create shader module!"
        );
    }

    void Shader::DestroyShaderModule()
    {
        check(m_shaderModule != VK_NULL_HANDLE, "Shader::Shader: Failed to create ShaderModule: Module not yet created!");
        VulkanInterface::vkDestroyShaderModule(VulkanManager::GetInstance()->GetDevice(), m_shaderModule, nullptr);
        m_shaderModule = VK_NULL_HANDLE;
    }

    VkShaderModule Shader::GetShaderModule()
    {
        check(m_shaderModule != VK_NULL_HANDLE, "Shader::Shader: Failed to create ShaderModule: Module not yet created!");
        return m_shaderModule;
    }
}