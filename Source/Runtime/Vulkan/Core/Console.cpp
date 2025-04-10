#include "Vulkan/Core/Console.hpp"
#include <consoleapi2.h>
#include <windows.h>

namespace Utils {
int Console::SetColorBySeverity(const Severity serverity) noexcept {
    const HANDLE               hConsole = GetStdHandle(STD_ERROR_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info     = {};
    GetConsoleScreenBufferInfo(HANDLE hConsoleOutput, PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo)
}
} // namespace Utils
