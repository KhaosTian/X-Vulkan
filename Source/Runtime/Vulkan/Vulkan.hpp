#pragma once

// std
#include <functional>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <string>
#include <cstdint>

// external
#ifndef NOMINMAX
    #define NOMINMAX
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>