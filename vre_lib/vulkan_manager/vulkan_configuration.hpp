#pragma once
namespace vre
{

	struct PipelineConfiguration
	{
		/// @brief Input assembly configuration.
		VkPipelineInputAssemblyStateCreateInfo mPipelineInputAssemblyStateCreateInfo;

		/// @brief Depth stencil configuration.
		VkPipelineDepthStencilStateCreateInfo mPipelineDepthStencilStateCreateInfo;

		/// @brief Rasterizer configuration.
		VkPipelineRasterizationStateCreateInfo mPipelineRasterizationStateCreateInfo;

		/// @brief The colorblend attachment for the colorblend create info.
		VkPipelineColorBlendAttachmentState mPipelineColorBlendAttachmentState;

		/// @brief Color blending.
		VkPipelineColorBlendStateCreateInfo mPipelineColorBlendStateCreateInfo;

		/// @brief Information regarding multisampling
		VkPipelineMultisampleStateCreateInfo mPipelineMultisampleStateCreateInfo;

		/// @brief Constructor to set default values for vre rendering.
		PipelineConfiguration()
		{
			// Input assembler
			mPipelineInputAssemblyStateCreateInfo = {};
			mPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			mPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			mPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

			// depth stencil values.
			mPipelineDepthStencilStateCreateInfo = {};
			mPipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			mPipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
			mPipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
			mPipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
			mPipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
			mPipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

			// default rasterizer values
			mPipelineRasterizationStateCreateInfo = {};
			mPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			mPipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			mPipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			mPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
			mPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
			mPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
			mPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
			mPipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;

			// color blending - do alpha blending
			mPipelineColorBlendAttachmentState = {};
			mPipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			mPipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
			mPipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			mPipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			mPipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
			mPipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			mPipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			mPipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

			mPipelineColorBlendStateCreateInfo = {};
			mPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			mPipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
			mPipelineColorBlendStateCreateInfo.attachmentCount = 1;
			mPipelineColorBlendStateCreateInfo.pAttachments = &mPipelineColorBlendAttachmentState;

			// multisampling
			mPipelineMultisampleStateCreateInfo = {};
			mPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			mPipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;

		}
	};

	/// @brief Configuration for the vulkan engine.
	struct VulkanConfiguration
	{
		/// @brief Switch the viewport Y-Axis to look upwards, so the application uses a right handed coordinate system
		/// instead of the vulkan default left handed system.
		/// Allows for compability with OpenGL shaders.
		bool UseRightHandedSystem{ false };

		/// @brief Enables validation layers if set to true.
		bool mEnableValidationLayers{ true };

		/// @brief The validation layers to activate upon instance creation.
		std::vector<const char*> mValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		/// @brief Configuration regarding the pipeline setup.
		PipelineConfiguration mPipelineConfiguration;
	};
}