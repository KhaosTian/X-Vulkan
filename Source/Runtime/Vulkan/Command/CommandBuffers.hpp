#pragma once

#include <Vulkan/Command/CommandPool.hpp>
#include <Vulkan/Vulkan.hpp>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Vulkan {
class CommandPool;
class CommandBuffers final {
private:
    std::vector<VkCommandBuffer> m_cmds;
    const CommandPool&           m_cmd_pool;

public:
    CommandBuffers(CommandBuffers&&) = delete;
    CommandBuffers(CommandPool& cmd_pool, uint32_t size);
    ~CommandBuffers();

public:
    uint32_t         size() const { return static_cast<uint32_t>(m_cmds.size()); }
    VkCommandBuffer& operator[](size_t i) { return m_cmds[i]; }

    VkCommandBuffer Begin(size_t i);
    void            End(size_t i);
};
} // namespace Vulkan