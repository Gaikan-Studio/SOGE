print(os.date() .. " Prepare flecs submodule...")

project "flecs"
    kind "StaticLib"
    language "C"
    cdialect "C99"
    location "./"
    staticruntime "on"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./flecs/include/**.h",
        "./flecs/include/**.hpp",
        "./flecs/src/*.c"
    }

    includedirs
    {
        "./flecs/include"
    }

    defines
    {
        "FLECS_NO_LOG",
        "FLECS_NO_JSON",
        "FLECS_NO_HTTP",
        "FLECS_NO_REST",
        "FLECS_NO_LOG",
        "FLECS_NO_JOURNAL",
        "FLECS_NO_APP",
        "FLECS_NO_OS_API_IMPL",
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS"
    }

    filter "toolset:gcc or clang"
        cdialect "gnu99"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
