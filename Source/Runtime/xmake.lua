target("Runtime")
    set_kind("static")

    add_packages("glfw", "spdlog", "stb", "glm", "vulkansdk")

    add_files("./**.cpp")
    add_headerfiles("./**.hpp")
    add_includedirs(".")
target_end()
    
