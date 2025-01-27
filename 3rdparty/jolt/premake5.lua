print(os.date() .. " Prepare JOLT submodule...")


project "Jolt"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "./"
    staticruntime "on"

    targetdir("lib/bin/" .. buildpattern .. "/%{prj.name}")
    objdir("lib/int/" .. buildpattern .. "/%{prj.name}")

    files
    {
        "./jolt/Jolt/**.h",
        "./jolt/Jolt/**.cpp"
    }

    includedirs
    {
        "./jolt"
    }

    buildoptions
    {
        "/Zc:__cplusplus"
    }

    defines
    {
        "_HAS_EXCEPTIONS=0",
        "ENABLE_OBJECT_STREAM",
        "JPH_USE_AVX2",
        "JPH_USE_AVX",
        "JPH_USE_SSE4_1",
        "JPH_USE_SSE4_2",
        "JPH_USE_LZCNT",
        "JPH_USE_TZCNT",
        "JPH_USE_F16C",
        "JPH_USE_FMADD",
        "CPP_RTTI_ENABLED",
        "OBJECT_LAYER_BITS",
        "CPP_EXCEPTIONS_ENABLED",
        "USE_ASSERTS"
    }

    filter "files:./jolt/Jolt/Jolt.nativs"
        buildaction "Natvis"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

        defines
        {
            "JPH_FLOATING_POINT_EXCEPTIONS_ENABLED",
            "JPH_DEBUG_RENDERER",
            "JPH_PROFILE_ENABLED"
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "on"