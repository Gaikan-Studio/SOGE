include "Premake5/scripts/helpers.lua"
include "dependencies.lua"

print(os.date() .. " Initialize submodules...")
os.execute("git submodule update --init --recursive")

workspace "SOGE"
    architecture "x64"
    startproject "GAME"
    configurations {"Debug", "Release"}

    buildpattern = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    group "Dependencies"
        include "3rdparty/EASTL/premake5.lua"
        include "3rdparty/kangaru/premake5.lua"

        -- Doesn't shown as subproject in MSVC solution

        include "3rdparty/SDL/premake5.lua"
        include "3rdparty/cppfs/premake5.lua"
        include "3rdparty/glm/premake5.lua"
        include "3rdparty/NRI/premake5.lua"
        include "3rdparty/NVRHI/premake5.lua"
        include "3rdparty/ShaderMake/premake5.lua"
        include "3rdparty/Assimp/premake5.lua"
    group ""

    project "SOGE"
        location "SOGE"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        characterset "MBCS"
        staticruntime "on"

        targetdir("build/bin/" .. buildpattern .. "/%{prj.name}")
        objdir("build/int/" .. buildpattern .. "/%{prj.name}")

        pchheader "sogepch.hpp"
        pchsource "%{wks.location}/%{prj.name}/source/sogepch.cpp"

        files
        {
            "%{wks.location}/%{prj.name}/include/**.hpp",
            "%{wks.location}/%{prj.name}/source/**.cpp",
            "%{wks.location}/%{prj.name}/resources/**"
        }

        includedirs
        {
            "%{wks.location}/%{prj.name}/include",
            "%{wks.location}/%{IncludeThirdpartyDirs.spdlog}",
            "%{wks.location}/%{IncludeThirdpartyDirs.backwardcpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.EASTL}",
            "%{wks.location}/%{IncludeThirdpartyDirs.EABase}",
            "%{wks.location}/%{IncludeThirdpartyDirs.UUID_v4}",
            "%{wks.location}/%{IncludeThirdpartyDirs.eventpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.kangaru}",
            "%{wks.location}/%{IncludeThirdpartyDirs.eventpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.SDL3}",
            "%{wks.location}/%{IncludeThirdpartyDirs.cppfs}",
            "%{wks.location}/%{IncludeThirdpartyDirs.FMOD}",
            "%{wks.location}/%{IncludeThirdpartyDirs.glm}",
            "%{wks.location}/%{IncludeThirdpartyDirs.XoshiroCpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.SDL3}",
            "%{wks.location}/%{IncludeThirdpartyDirs.NRI}",
            "%{wks.location}/%{IncludeThirdpartyDirs.NVRHI}",
            "%{wks.location}/%{IncludeThirdpartyDirs.Assimp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.stb}"
            "%{wks.location}/%{IncludeThirdpartyDirs.CLI11}"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "SPDLOG_WCHAR_TO_UTF8_SUPPORT",
            "GLM_ENABLE_EXPERIMENTAL",

            "SOGE_INPUT_IMPL=SDL", -- SDL
            "SOGE_WINDOW_IMPL=SDL",
            "SOGE_SYSTEM_IMPL=SDL",
            "SOGE_GRAPHICS_IMPL=D3D12", -- D3D11/D3D12/VK
            "SOGE_GRAPHICS_COMPILED_SHADER_EXTENSION_D3D12=dxil",
            "SOGE_GRAPHICS_COMPILED_SHADER_EXTENSION_VK=spirv",
            "SOGE_SOUND_IMPL=FMOD" -- FMOD/OAL
        }

        links
        {
            "EASTL",
            "kangaru",
            "nvrhi",
            "nvrhi_d3d11",
            "nvrhi_d3d12",
            "nvrhi_vk",
            "cppfs",

            "%{wks.location}/%{Libraries.FMOD_WIN64_FSBANK_DLL}",
            "%{wks.location}/%{Libraries.FMOD_WIN64_FSBANK}",
            "%{wks.location}/%{Libraries.FMOD_WIN64_LIBFSVORBIS_DLL}",
            "%{wks.location}/%{Libraries.FMOD_WIN64_OPUS_DLL}"
        }

        postbuildcommands
        {
            "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_FSBANK_DLL} %{wks.location}/GAME",
            "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_LIBFSVORBIS_DLL} %{wks.location}/GAME",
            "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_OPUS_DLL} %{wks.location}/GAME"
        }

        prebuildcommands
        {
            shadermake
            {
                platform = shadermake_platform.DXIL,
                config = "%{!wks.location}/SOGE/resources/shaders/config.shadermake",
                output = "%{!wks.location}/GAME/resources/shaders"
            },
            "{COPYDIR} %[%{!wks.location}SOGE/resources/meshes/] %[%{!wks.location}GAME/resources/meshes/]",
            "{COPYDIR} %[%{!wks.location}SOGE/resources/textures/] %[%{!wks.location}GAME/resources/textures/]",
            "{COPYDIR} %[%{!wks.location}SOGE/resources/sounds/] %[%{!wks.location}GAME/resources/sounds/]"
        }

        filter "files:**.hlsl"
            flags "ExcludeFromBuild"

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "SOGE_WINDOWS",
                "SOGE_COMPILED_PLATFORM=Windows"
            }

        filter "configurations:Debug"
            symbols "on"

            defines
            {
                "SOGE_DEBUG",
                "SOGE_ENABLE_ASSERT",
                "TRACEDESIGNTIME=true"
            }

            links 
            {
                -- sdl
                "%{wks.location}/%{Libraries.SDL_UCLIB_D}",
                "%{wks.location}/%{Libraries.SDL3_DLL_D}",
                "%{wks.location}/%{Libraries.SDL3_LIB_D}",

                -- NRI

                "%{wks.location}/%{Libraries.NRI_D3D11_D}",
                "%{wks.location}/%{Libraries.NRI_D3D12_D}",
                "%{wks.location}/%{Libraries.NRI_NONE_D}",
                "%{wks.location}/%{Libraries.NRI_SHARED_D}",
                "%{wks.location}/%{Libraries.NRI_VALIDATION_D}",
                "%{wks.location}/%{Libraries.NRI_VK_D}",
                "%{wks.location}/%{Libraries.NRI_D}",
                "%{wks.location}/%{Libraries.NRI_DLL_D}",
                "%{wks.location}/%{Libraries.NRI_AMDAGS_DLL_D}",
                -- fmod
                "%{wks.location}/%{Libraries.FMOD_WIN64_CORE_DLL_D}",
                "%{wks.location}/%{Libraries.FMOD_WIN64_CORE_D}",
                "%{wks.location}/%{Libraries.FMOD_WIN64_STUDIO_DLL_D}",
                "%{wks.location}/%{Libraries.FMOD_WIN64_STUDIO_D}",

                -- Assimp
                "%{wks.location}/%{Libraries.ASSIMP_D}",
                "%{wks.location}/%{Libraries.ASSIMP_DLL_D}"
            }

            postbuildcommands
            {
                "{COPYFILE} %{wks.location}/%{Libraries.SDL3_DLL_D} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.NRI_DLL_D} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.NRI_AMDAGS_DLL_D} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_CORE_DLL_D} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_CORE_DLL_R} %{wks.location}/GAME", -- For FMOD Studio 
                "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_STUDIO_DLL_D} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.ASSIMP_DLL_D} %{wks.location}/GAME"
            }

        filter "configurations:Release"
            optimize "on"

            defines
            {
                "SOGE_RELEASE"
            }

            links
            {
                -- sdl
                "%{wks.location}/%{Libraries.SDL_UCLIB_R}",
                "%{wks.location}/%{Libraries.SDL3_DLL_R}",
                "%{wks.location}/%{Libraries.SDL3_LIB_R}",

                -- NRI

                "%{wks.location}/%{Libraries.NRI_D3D11_R}",
                "%{wks.location}/%{Libraries.NRI_D3D12_R}",
                "%{wks.location}/%{Libraries.NRI_NONE_R}",
                "%{wks.location}/%{Libraries.NRI_SHARED_R}",
                "%{wks.location}/%{Libraries.NRI_VALIDATION_R}",
                "%{wks.location}/%{Libraries.NRI_VK_R}",
                "%{wks.location}/%{Libraries.NRI_R}",
                "%{wks.location}/%{Libraries.NRI_DLL_R}",
                "%{wks.location}/%{Libraries.NRI_AMDAGS_DLL_R}",
                -- fmod
                "%{wks.location}/%{Libraries.FMOD_WIN64_CORE_DLL_R}",
                "%{wks.location}/%{Libraries.FMOD_WIN64_CORE_R}",
                "%{wks.location}/%{Libraries.FMOD_WIN64_STUDIO_DLL_R}",
                "%{wks.location}/%{Libraries.FMOD_WIN64_STUDIO_R}",

                -- Assimp
                "%{wks.location}/%{Libraries.ASSIMP_R}",
                "%{wks.location}/%{Libraries.ASSIMP_DLL_R}"
            }

            postbuildcommands
            {
                "{COPYFILE} %{wks.location}/%{Libraries.SDL3_DLL_R} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.NRI_DLL_R} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.NRI_AMDAGS_DLL_R} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_CORE_DLL_R} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.FMOD_WIN64_STUDIO_DLL_R} %{wks.location}/GAME",
                "{COPYFILE} %{wks.location}/%{Libraries.ASSIMP_DLL_R} %{wks.location}/GAME"
            }

-----------------------
--- Engine application
-----------------------

    project "GAME"
        location "GAME"
        -- kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"
        characterset "MBCS"

        -- Executable will be placed in the root of application folder
        -- to have easier eaccess to game resources.
        targetdir("%{wks.location}/%{prj.name}/")
        objdir("build/int/" .. buildpattern .. "/%{prj.name}")

        files
        {
            "%{wks.location}/%{prj.name}/include/**.hpp",
            "%{wks.location}/%{prj.name}/source/**.cpp"
        }

        includedirs
        {
            "%{wks.location}/%{prj.name}/include",
            "%{wks.location}/SOGE/include",
            "%{wks.location}/%{IncludeThirdpartyDirs.spdlog}",
            "%{wks.location}/%{IncludeThirdpartyDirs.backwardcpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.EASTL}",
            "%{wks.location}/%{IncludeThirdpartyDirs.EABase}",
            "%{wks.location}/%{IncludeThirdpartyDirs.UUID_v4}",
            "%{wks.location}/%{IncludeThirdpartyDirs.eventpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.kangaru}",
            "%{wks.location}/%{IncludeThirdpartyDirs.eventpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.SDL3}",
            "%{wks.location}/%{IncludeThirdpartyDirs.cppfs}",
            "%{wks.location}/%{IncludeThirdpartyDirs.FMOD}",
            "%{wks.location}/%{IncludeThirdpartyDirs.glm}",
            "%{wks.location}/%{IncludeThirdpartyDirs.XoshiroCpp}",
            "%{wks.location}/%{IncludeThirdpartyDirs.SDL3}",
            "%{wks.location}/%{IncludeThirdpartyDirs.NRI}",
            "%{wks.location}/%{IncludeThirdpartyDirs.NVRHI}"
            "%{wks.location}/%{IncludeThirdpartyDirs.CLI11}"
        }

        defines
        {
            "GLM_ENABLE_EXPERIMENTAL"
        }

        links
        {
            "SOGE"
        }

        filter "system:windows"
            systemversion "latest"
            kind 'WindowedApp'
            entrypoint "wWinMainCRTStartup"

            defines
            {
                "SOGE_WINDOWS"
            }

        filter "configurations:Debug"
            symbols "on"

            defines
            {
                "SOGE_DEBUG"
            }

        filter "configurations:Release"
            optimize "on"

            defines
            {
                "SOGE_RELEASE"
            }

-----------------------
--- Overrides
-----------------------

require "vstudio"

-- Enable clang-tidy option in soultion, based on https://github.com/premake/premake-core/pull/2187
-- Can be replaced with clangtidy("on") option after Premake5 beta 3 release.
local function clangtidy(cfg)
    if _ACTION >= "vs2019" then
        premake.vstudio.vc2010.element("EnableClangTidyCodeAnalysis", nil, "true")
        premake.vstudio.vc2010.element("ClangTidyChecks", nil, "\"--config-file=../.clang-tidy\"")
    end
end

premake.override(premake.vstudio.vc2010.elements, "outputProperties", function(base, cfg)
    local calls = base(cfg)
    table.insert(calls, clangtidy)
    return calls
end)
