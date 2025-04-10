#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <string>
#include <cstdint>
#include <array>

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

namespace Vulkan {
const char* ToString(VkResult result);
void        Check(VkResult result, const char* operation, const char* file, int line);
} // namespace Vulkan

#define VK_CHECK(result, operation) Vulkan::Check(result, operation, __FILE__, __LINE__)
