#pragma once
#include <QWidget>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include "vulkan_window.hpp"

/// @brief A widget containing the VulkanWindow. Can be used as promoting widget in a Qt GUI created by the Qt designer.
class Vulkan3DViewWidget : public QWidget
{
	Q_OBJECT
public:
	Vulkan3DViewWidget(QWidget* parent = nullptr) : QWidget(parent)
	{
		QGridLayout* layout = new QGridLayout;
		vre::VulkanWindow* window = new vre::VulkanWindow();
		layout->addWidget(this->createWindowContainer(window, parent));
		setLayout(layout);
	};
};