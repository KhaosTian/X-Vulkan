--项目设置
set_project("Nova")
set_version("0.1.0")

--编译设置
set_arch("x64")
set_plat("windows")
set_toolchains("msvc")
set_languages("c++20")

--规则
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("plugin.vsxmake.autoupdate")

--包依赖
add_requires("vulkansdk", "spdlog", "glfw", "glm", "stb")

--宏定义
if is_mode("debug") then
    add_defines("NOVA_DEBUG")
elseif is_mode("release") then 
    add_defines("NOVA_RELEASE")
end

--包含子项
includes("Source/Runtime/xmake.lua")
includes("Source/Editor/xmake.lua")