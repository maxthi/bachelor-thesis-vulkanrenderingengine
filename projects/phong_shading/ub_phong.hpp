#pragma once
#include <project_relevant_headers.hpp>

namespace vre
{

	class UniformBufferPhong : public UniformBuffer
	{
		/// @brief Describing the uniform buffer object for this uniform buffer class
		struct UniformBufferObject {
			float model[4][4];
			float view[4][4];
			float proj[4][4];
			float lightPosition[3] = {10, 10, 0};
		};

	public:
		static std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout();
		void Create(std::shared_ptr<DrawableData> drawableData);
		void UpdateUniformBuffer(uint32_t swapchainImageIndex) override;

	private:
		static inline std::shared_ptr<DescriptorSetLayout> s_descriptorSetLayout{};
	};
}
