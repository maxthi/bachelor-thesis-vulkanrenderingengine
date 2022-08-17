#pragma once
#include "drawable_property.hpp"
#include "../../common.hpp"
#include "../../vulkan_manager/vulkan_manager.hpp"
#include <QImage>

namespace vre
{
	struct DrawableData;
	class Texture2D : public DrawableProperty
	{
	public:
		static DrawablePropertyId GetClassPropertyId() {
			static DrawablePropertyId propertyId = DrawableProperty::GetUniquePropertyId();
			return propertyId;
		}


		Texture2D(const std::string& imagePath);
		~Texture2D();

		std::string& GetImagePath() { return m_imagePath; }

		std::shared_ptr<Image> GetImage();

		void AllocateMemory(std::shared_ptr<DrawableData>& drawableData) override;

	private:

		/// @brief Path to the image.
		std::string m_imagePath;

		/// @brief Width of the image in pixel.
		uint32_t m_width{};

		/// @brief Height of the image in pixel.
		uint32_t m_height{};

		/// @brief Pointer to the raw pixel data of the image.
		void* m_data{};

		/// @brief The Qt image handle.
		QImage m_image;

		/// @brief Handle to the vulkan image.
		std::shared_ptr<Image> m_vulkanImage;
	};
}