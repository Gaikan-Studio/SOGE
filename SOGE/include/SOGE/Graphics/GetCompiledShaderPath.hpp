#ifndef SOGE_GRAPHICS_GETCOMPILEDSHADERPATH_HPP
#define SOGE_GRAPHICS_GETCOMPILEDSHADERPATH_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"

#include <filesystem>


namespace soge
{
    [[nodiscard]]
    std::filesystem::path GetCompiledShaderPath(const GraphicsCore& aCore, const std::filesystem::path& aSourcePath,
                                                eastl::string_view aEntryName = "");
}

#endif // SOGE_GRAPHICS_GETCOMPILEDSHADERPATH_HPP
