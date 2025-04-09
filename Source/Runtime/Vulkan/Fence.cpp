#pragma once

#include "Vulkan/Fence.hpp"
#include "Vulkan/Device.hpp"

namespace Vulkan {

Fence::Fence(const Device& device, bool signaled): m_device(device) {
    VkFenceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    create_info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
    VK_CHECK(vkCreateFence(device.handle(), &create_info, nullptr, &m_handle), "create fence");
}

Fence::Fence(Fence&& other) noexcept: m_device(other.m_device), m_handle(other.m_handle) {
    other.m_handle = nullptr;
}

void Fence::Reset() const {
    VK_CHECK(vkResetFences(m_device.handle(), 1, &m_handle), "reset fence");
}

void Fence::Wait(const uint64_t timeout) const {
    VK_CHECK(vkWaitForFences(m_device.handle(), 1, &m_handle, VK_TRUE, timeout), "wait fence");
}

Fence::~Fence() {
    if (m_handle == nullptr) return;
    vkDestroyFence(m_device.handle(), m_handle, nullptr);
    m_handle = nullptr;
}
} // namespace Vulkan