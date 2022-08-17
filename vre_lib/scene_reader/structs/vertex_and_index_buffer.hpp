#pragma once
#include "../../vulkan_manager/buffer.hpp"

namespace vre
{
	/// @brief Struct that describes the vertex data used in the draw calls of the command buffers.
	/// For indexed drawing the buffer contains the vertex information in the first part and 
	/// the indices in the second part
	struct VertexAndIndexBuffer
	{
		/// @brief Offset to the index buffer - which is the size of the vertex buffer part at the same time.
		size_t mOffsetToIndexBuffer{};

		/// @brief Size of the index buffer.
		size_t mIndexBufferSize{};

		/// @brief The buffer containg the data.
		std::shared_ptr<Buffer> mBuffer;
	};
}