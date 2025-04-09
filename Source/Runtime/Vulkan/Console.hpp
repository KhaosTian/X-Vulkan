#pragma once

#include "Vulkan/Vulkan.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define CONSOLE_RED_COLOR "\033[1;31m"
#define CONSOLE_GREEN_COLOR "\033[1;32m"
#define CONSOLE_GOLD_COLOR "\033[1;33m"
#define CONSOLE_DEFAULT_COLOR "\033[0m"

namespace Utils {

enum class Severity {
    Verbos  = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
    Info    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
    Warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
    Error   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    Fatal   = VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT
};

class Console final {
private:
    static std::shared_ptr<spdlog::logger> s_logger;
    static bool                            s_initialized;

public:
    // 初始化 Console 类（应在应用启动时调用一次）
    static void Initialize(const std::string& loggerName = "console");

    // 将 Vulkan 消息严重程度转换为 spdlog 级别
    static spdlog::level::level_enum SeverityToLevel(Severity severity) noexcept;

    // 带格式的日志输出方法
    template<typename... Args>
    static void Log(const Severity severity, const std::string& fmt, Args&&... args) {
        if (!s_initialized) {
            Initialize();
        }

        s_logger->log(SeverityToLevel(severity), fmt, std::forward<Args>(args)...);
    }

    // 便捷方法
    template<typename... Args>
    static void Verbos(const std::string& fmt, Args&&... args) {
        Log(Severity::Verbos, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Info(const std::string& fmt, Args&&... args) {
        Log(Severity::Info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warning(const std::string& fmt, Args&&... args) {
        Log(Severity::Warning, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(const std::string& fmt, Args&&... args) {
        Log(Severity::Error, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Fatal(const std::string& fmt, Args&&... args) {
        Log(Severity::Fatal, fmt, std::forward<Args>(args)...);
    }

private:
    static Severity s_currentSeverity;
};

} // namespace Utils