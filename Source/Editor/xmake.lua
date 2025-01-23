target("Editor")
    set_kind("binary")  -- 或 "shared" 如果需要动态库
    add_packages("vulkansdk", "spdlog", "glfw", "glm", "stb")
    add_deps("Runtime")
    add_files("./**.cpp")
    add_includedirs(".")
    add_includedirs("../Runtime")
target_end()