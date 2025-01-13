
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
IncludeThirdpartyDirs["glm"] = "3rdparty/glm/glm/include"
IncludeThirdpartyDirs["XoshiroCpp"] = "3rdparty/XoshiroCpp/XoshiroCpp/include" 
IncludeThirdpartyDirs["NRI"] = "3rdparty/NRI/NRI/Include"
IncludeThirdpartyDirs["NVRHI"] = "3rdparty/NVRHI/NVRHI/include"

-- Include libs
Libraries = {}
Libraries["SDL_UCLIB_D"] = "3rdparty/SDL/lib/Debug/SDL_uclibc.lib"
Libraries["SDL3_DLL_D"] = "3rdparty/SDL/lib/Debug/SDL3.dll"
Libraries["SDL3_LIB_D"] = "3rdparty/SDL/lib/Debug/SDL3.lib"

Libraries["SDL_UCLIB_R"] = "3rdparty/SDL/lib/Release/SDL_uclibc.lib"
Libraries["SDL3_DLL_R"] = "3rdparty/SDL/lib/Release/SDL3.dll"
Libraries["SDL3_LIB_R"] = "3rdparty/SDL/lib/Release/SDL3.lib"

-- NRI

Libraries["NRI_D3D11_D"] = "3rdparty/NRI/lib/Frameworks/Debug/NRI_D3D11.lib"
Libraries["NRI_D3D12_D"] = "3rdparty/NRI/lib/Frameworks/Debug/NRI_D3D12.lib"
Libraries["NRI_NONE_D"] = "3rdparty/NRI/lib/Frameworks/Debug/NRI_NONE.lib"
Libraries["NRI_SHARED_D"] = "3rdparty/NRI/lib/Frameworks/Debug/NRI_Shared.lib"
Libraries["NRI_VALIDATION_D"] = "3rdparty/NRI/lib/Frameworks/Debug/NRI_Validation.lib"
Libraries["NRI_VK_D"] = "3rdparty/NRI/lib/Frameworks/Debug/NRI_VK.lib"
Libraries["NRI_D"] = "3rdparty/NRI/lib/NRI/Debug/NRI.lib"
Libraries["NRI_DLL_D"] = "3rdparty/NRI/lib/NRI/Debug/NRI.dll"
Libraries["NRI_AMDAGS_DLL_D"] = "3rdparty/NRI/lib/NRI/Debug/amd_ags_x64.dll"

Libraries["NRI_D3D11_R"] = "3rdparty/NRI/lib/Frameworks/Release/NRI_D3D11.lib"
Libraries["NRI_D3D12_R"] = "3rdparty/NRI/lib/Frameworks/Release/NRI_D3D12.lib"
Libraries["NRI_NONE_R"] = "3rdparty/NRI/lib/Frameworks/Release/NRI_NONE.lib"
Libraries["NRI_SHARED_R"] = "3rdparty/NRI/lib/Frameworks/Release/NRI_Shared.lib"
Libraries["NRI_VALIDATION_R"] = "3rdparty/NRI/lib/Frameworks/Release/NRI_Validation.lib"
Libraries["NRI_VK_R"] = "3rdparty/NRI/lib/Frameworks/Release/NRI_VK.lib"
Libraries["NRI_R"] = "3rdparty/NRI/lib/NRI/Release/NRI.lib"
Libraries["NRI_DLL_R"] = "3rdparty/NRI/lib/NRI/Release/NRI.dll"
Libraries["NRI_AMDAGS_DLL_R"] = "3rdparty/NRI/lib/NRI/Release/amd_ags_x64.dll"