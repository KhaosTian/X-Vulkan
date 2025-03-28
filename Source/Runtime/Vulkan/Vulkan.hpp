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

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#define NON_COPY(Type) Type(Type&&) = delete;
#define VULKAN_HANDLE(Type, name) \
private: \
    Type name {}; \
\
public: \
    Type handle() const { \
        return name; \
    }

namespace Vulkan {
const char* ToString(VkResult result);
void        Check(VkResult result, const char* operation, const char* file, int line);
} // namespace Vulkan

#define VK_CHECK(result, operation) Vulkan::Check(result, operation, __FILE__, __LINE__)
#define STR(r) \
    case VK_##r: \
        return #r