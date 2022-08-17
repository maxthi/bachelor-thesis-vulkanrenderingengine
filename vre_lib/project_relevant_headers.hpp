// This header file includes all headers relevant for a project to setup.
#pragma once

#include <vulkan/vulkan.h>
#include "core/vre.hpp"
#include "core/project.hpp"
#include "core/vre_core.hpp"
#include "core/vre_configuration.hpp"
#include "core/vulkan_window.hpp"
#include "core/vulkan_3d_view_widget.hpp"

#include "file_system/file_system.hpp"

#include "common.hpp"
#include "vulkan_manager/vulkan_manager.hpp"
#include "scene_manager/scene_manager.hpp"
#include "scene_manager/drawable_properties/model_transformation.hpp"
#include "scene_manager/drawable_properties/render_target.hpp"
#include "scene_helper/scene_helper.hpp"
#include "scene_helper/vertex_layouts/vl_color.hpp"
#include "scene_helper/vertex_layouts/vl_texture.hpp"
#include "scene_helper/uniform_buffers/ub_color.hpp"
#include "scene_helper/uniform_buffers/ub_texture.hpp"
#include "scene_helper/uniform_buffers/ub_render_target.hpp"

#include "shader_manager/shader_manager.hpp"

// allow projects to manipulate renderer
#include "vulkan_renderer/renderer.hpp"
#include "vulkan_renderer/vre_renderer/vre_attachments.hpp"
#include "vulkan_renderer/vre_renderer/vre_commandbuffer.hpp"
#include "vulkan_renderer/vre_renderer/vre_commandpool.hpp"
#include "vulkan_renderer/vre_renderer/vre_framebuffer.hpp"
#include "vulkan_renderer/vre_renderer/vre_graphicspipelines.hpp"
#include "vulkan_renderer/vre_renderer/vre_renderer.hpp"
#include "vulkan_renderer/vre_renderer/vre_renderlist.hpp"
#include "vulkan_renderer/vre_renderer/vre_renderpass.hpp"
#include "vulkan_renderer/vre_renderer/vre_drawer.hpp"