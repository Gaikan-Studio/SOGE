print(os.date() .. " Prepare NVRHI submodule...")

------------------------
-- NVRHI
------------------------

project "nvrhi"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "./"
    staticruntime "On"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./NVRHI/include/nvrhi/**.h",
        "./NVRHI/src/common/*.h",
        "./NVRHI/src/common/*.cpp",
        "./NVRHI/src/validation/*.h",
        "./NVRHI/src/validation/*.cpp"
    }

    includedirs
    {
        "./NVRHI/include"
    }

    defines
    {
        "NVRHI_WITH_AFTERMATH=0"
    }

    filter "files:./NVRHI/tools/nvrhi.nativs"
        buildaction "Natvis"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

-- PreprocessorDefinitions
-- AdditionalIncludeDirectories
-- AdditionalDependencies

------------------------
-- NVRHI D3D11
------------------------

project "nvrhi_d3d11"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "./"
    staticruntime "On"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./NVRHI/src/d3d11/*.h",
        "./NVRHI/src/d3d11/*.cpp"
    }

    includedirs
    {
        "./NVRHI/include"
    }

    links
    {
        "d3d11.lib",
        "dxgi.lib",
        "d3dcompiler.lib",
        "dxguid.lib",
        "dxgidebug.dll"
    }

    defines
    {
        "NVRHI_D3D11_WITH_NVAPI=0",
        "NVRHI_WITH_AFTERMATH=0"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

------------------------
-- NVRHI D3D12
------------------------

project "nvrhi_d3d12"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "./"
    staticruntime "On"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./NVRHI/src/d3d12/*.h",
        "./NVRHI/src/d3d12/*.cpp"
    }

    includedirs
    {
        "./NVRHI/include"
    }

    links
    {
        "d3d12.lib",
        "dxgi.lib",
        "d3dcompiler.lib",
        "dxguid.lib",
        "dxgidebug.dll"
    }
    
    defines
    {
        "NVRHI_D3D12_WITH_NVAPI=0",
        "NVRHI_WITH_AFTERMATH=0"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

------------------------
-- NVRHI 
------------------------

project "nvrhi_vk"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "./"
    staticruntime "On"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./NVRHI/src/vulkan/*.h",
        "./NVRHI/src/vulkan/*.cpp"
    }

    includedirs
    {
        "./NVRHI/include",
        "./NVRHI/thirdparty/Vulkan-Headers/include"
    }

    defines
    {
        "VK_USE_PLATFORM_WIN32_KHR",
        "NOMINMAX",
        "NVRHI_WITH_AFTERMATH=0"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"