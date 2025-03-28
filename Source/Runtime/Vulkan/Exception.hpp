#pragma once
#include <iostream>

template<class E>
[[noreturn]] void Throw(const E& e) {
    // 简单的控制台输出
    std::cerr << "\n\033[31mException: " << e.what() << "\033[0m\n";
    throw e;
}
