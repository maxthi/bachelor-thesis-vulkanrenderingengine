#pragma once
#include "../../common.hpp"
#include "../../scene_manager/drawable_properties/mesh.hpp"
#include "../../vulkan_manager/buffer.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"

namespace vre
{
	/// @brief Helps with creating a vertex buffer based on a mesh and a vertex layout.
	/// Use WriteVertexData to write the vertex blocks. The indexbuffer part is written automatically.
	template<typename T>
	class VertexBufferCreator
	{
	public:
		VertexBufferCreator(std::shared_ptr<Mesh> mesh, uint32_t vertexStrideSize) :
			m_vertexStrideSize(vertexStrideSize)
		{
			auto numVertices = mesh->GetVertices()->size();
			// first part of the buffer contains the vertex data, second part contains the indices
			m_vertexPartSize = numVertices * vertexStrideSize;
			m_indexPartSize = mesh->GetIndices()->size();
			auto bufferSize = m_vertexPartSize + m_indexPartSize;
			m_vertexData = std::make_shared<std::vector<T>>(bufferSize);

			// write indices to buffer
			auto indices = mesh->GetIndices();
			auto indexPartByteSize = m_indexPartSize * 4; // indices are 4 byte values
			uint32_t offset = m_vertexPartSize;
			memcpy(m_vertexData->data() + offset, indices->data(), indexPartByteSize);
		}

		/// @brief Expects an array of type T with the size of the vertexStride that was used in the constructor, namely vertexStrideSize.
		/// Example: Creating a vertexBufferCreator with VertexBufferCreator<float>(mesh, 5),
		/// WriteVertexData expects a float data[5] array, where the array size corresponds to the stride value size.
		/// @param vertexStride - Array of type T with vertexStrideSize elements.
		void WriteVertexData(T* vertexStride)
		{
			if (m_dataWritten + m_vertexStrideSize > m_vertexPartSize)
			{
				err("VertexBufferCreator::WriteVertexData: Vertex stride data cannot be written "
					"because the vertex stride part of the buffer is full!");
				return;
			}

			memcpy(m_vertexData->data() + m_dataWritten, vertexStride, m_vertexStrideSize*sizeof(T));
			m_dataWritten += m_vertexStrideSize;
		}

		std::shared_ptr<VertexAndIndexBuffer> CreateVertexAndIndexBuffer()
		{
			// create a complete vk buffer
			BufferInfo bufferInfo{};
			bufferInfo.mDataSize = m_vertexData->size() * sizeof(T);
			bufferInfo.mData = m_vertexData->data();
			bufferInfo.mUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			bufferInfo.mName = "VertexAndIndexBuffer";

			auto vertexAndIndexBuffer = std::make_shared<VertexAndIndexBuffer>();
			vertexAndIndexBuffer->mBuffer = VulkanManager::GetInstance()->CreateBufferDeviceLocal(bufferInfo);
			vertexAndIndexBuffer->mOffsetToIndexBuffer = m_vertexPartSize * sizeof(T);
			vertexAndIndexBuffer->mIndexBufferSize = m_indexPartSize; // vkCmdDrawIndexed has been prepared to accept 4 byte values as indices
			return vertexAndIndexBuffer;
		}

	private:
		/// @brief The amount of floats written in each WriteVertexDataCall.
		uint32_t m_vertexStrideSize;

		/// @brief The size of the first part of the buffer in terms of the size of the template argument.
		uint32_t m_vertexPartSize;

		/// @brief The size of the second part of the buffer, the index buffer.
		size_t m_indexPartSize;

		/// @brief Number of bytes written.
		uint32_t m_dataWritten{ 0 };

		/// @brief The buffer holding the vertex data.
		std::shared_ptr<std::vector<T>> m_vertexData;
	};
}