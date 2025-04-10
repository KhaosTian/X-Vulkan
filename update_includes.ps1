# 定义文件所在的层级
$coreFiles = @(
    "Vulkan", "Instance", "Device", "Surface", "Window", 
    "Exception", "Enumerate", "DebugUtilsMessenger", "Console"
)

$resourceFiles = @(
    "Buffer", "Image", "ImageView", "DeviceMemory", 
    "Sampler", "DepthBuffer", "SwapChain"
)

$commandFiles = @(
    "CommandPool", "CommandBuffer", "CommandBuffers", 
    "Fence", "Semaphore"
)

$pipelineFiles = @(
    "Shader", "GraphicsPipeline", "PipelineLayout", 
    "RenderPass", "Framebuffer"
)

$descriptorFiles = @(
    "DescriptorBinding", "DescriptorPool", "DescriptorSetLayout", 
    "DescriptorSets", "DescriptorSetManager"
)

# 获取所有的.cpp和.hpp文件
$allFiles = Get-ChildItem -Path "e:\X-Vulkan\Source\Runtime\Vulkan" -Recurse -Include "*.cpp", "*.hpp"

foreach ($file in $allFiles) {
    $content = Get-Content -Path $file.FullName -Raw
    $modified = $false

    # 更新Core层文件的引用
    foreach ($coreFile in $coreFiles) {
        if ($content -match "#include\s+`"Vulkan/$coreFile\.hpp`"") {
            $content = $content -replace "#include\s+`"Vulkan/$coreFile\.hpp`"", "#include `"Vulkan/Core/$coreFile.hpp`""
            $modified = $true
        }
    }

    # 更新Resource层文件的引用
    foreach ($resourceFile in $resourceFiles) {
        if ($content -match "#include\s+`"Vulkan/$resourceFile\.hpp`"") {
            $content = $content -replace "#include\s+`"Vulkan/$resourceFile\.hpp`"", "#include `"Vulkan/Resource/$resourceFile.hpp`""
            $modified = $true
        }
    }

    # 更新Command层文件的引用
    foreach ($commandFile in $commandFiles) {
        if ($content -match "#include\s+`"Vulkan/$commandFile\.hpp`"") {
            $content = $content -replace "#include\s+`"Vulkan/$commandFile\.hpp`"", "#include `"Vulkan/Command/$commandFile.hpp`""
            $modified = $true
        }
    }

    # 更新Pipeline层文件的引用
    foreach ($pipelineFile in $pipelineFiles) {
        if ($content -match "#include\s+`"Vulkan/$pipelineFile\.hpp`"") {
            $content = $content -replace "#include\s+`"Vulkan/$pipelineFile\.hpp`"", "#include `"Vulkan/Pipeline/$pipelineFile.hpp`""
            $modified = $true
        }
    }

    # 更新Descriptor层文件的引用
    foreach ($descriptorFile in $descriptorFiles) {
        if ($content -match "#include\s+`"Vulkan/$descriptorFile\.hpp`"") {
            $content = $content -replace "#include\s+`"Vulkan/$descriptorFile\.hpp`"", "#include `"Vulkan/Descriptor/$descriptorFile.hpp`""
            $modified = $true
        }
    }

    # 如果文件被修改，则保存更改
    if ($modified) {
        Set-Content -Path $file.FullName -Value $content
        Write-Host "Updated includes in $($file.FullName)"
    }
}

Write-Host "Include paths update completed!"
