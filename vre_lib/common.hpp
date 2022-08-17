#pragma once

// vulkan
#include <vulkan/vulkan.h>

// c++ related includes
#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <set>
#include <memory>
#include <array>
#include <functional>
#include <unordered_map>
#include <unordered_set>

// qt related includes
#include <QVulkanInstance>
#include <QVulkanFunctions>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>

// project includes
#include "file_system/file_system.hpp"
#include "core/error_handling.hpp"
#include "vulkan_manager/helper_structs.hpp"
#include "vulkan_interface/vulkan_interface.hpp"