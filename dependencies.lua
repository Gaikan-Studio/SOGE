-- Include directories
IncludeThirdpartyDirs = {}
IncludeThirdpartyDirs["spdlog"] = "3rdparty/spdlog/spdlog/include"
IncludeThirdpartyDirs["backwardcpp"] = "3rdparty/backwardcpp/backwardcpp"
IncludeThirdpartyDirs["EASTL"] = "3rdparty/EASTL/EASTL/include"
IncludeThirdpartyDirs["EABase"] = "3rdparty/EASTL/EASTL/deps/EABase/include/Common/"
IncludeThirdpartyDirs["UUID_v4"] = "3rdparty/uuid_v4/uuid_v4/include"
IncludeThirdpartyDirs["eventpp"] = "3rdparty/eventpp/eventpp/include"
IncludeThirdpartyDirs["kangaru"] = "3rdparty/kangaru/kangaru/include"
IncludeThirdpartyDirs["SDL3"] = "3rdparty/SDL/SDL/include"
IncludeThirdpartyDirs["vgjs"] = "3rdparty/vgjs/vgjs/include"

-- Include libs
Libraries = {}
Libraries["SDL_UCLIB_D"] = "3rdparty/SDL/lib/Debug/SDL_uclibc.lib"
Libraries["SDL3_DLL_D"] = "3rdparty/SDL/lib/Debug/SDL3.dll"
Libraries["SDL3_LIB_D"] = "3rdparty/SDL/lib/Debug/SDL3.lib"

Libraries["SDL_UCLIB_R"] = "3rdparty/SDL/lib/Release/SDL_uclibc.lib"
Libraries["SDL3_DLL_R"] = "3rdparty/SDL/lib/Release/SDL3.dll"
Libraries["SDL3_LIB_R"] = "3rdparty/SDL/lib/Release/SDL3.lib"
