project "RTTR"
    kind "StaticLib"
    language "C++"
    location "./"
    staticruntime "on"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./rttr/src/rttr/**.h",
        "./rttr/src/rttr/**.cpp",
        "./rttr/src/rttr/**.in"
    }

    includedirs
    {
        "./rttr/src",
        "./rttr/3rd_party"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"