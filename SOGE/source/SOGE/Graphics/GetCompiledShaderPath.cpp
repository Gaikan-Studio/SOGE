#include "sogepch.hpp"

#include "SOGE/Graphics/GetCompiledShaderPath.hpp"


namespace soge
{
    std::filesystem::path GetCompiledShaderPath(const GraphicsCore& aCore, const std::filesystem::path& aSourcePath,
                                                const eastl::string_view aEntryName)
    {
        std::filesystem::path destinationPath{aSourcePath};

        if (!aEntryName.empty())
        {
            const auto sourceFilename = aSourcePath.stem();
            const auto destinationFilename = fmt::format("{}_{}", sourceFilename.generic_string(), aEntryName.data());
            destinationPath.replace_filename(destinationFilename);
        }

        const auto extension = aCore.GetCompiledShaderExtension();
        destinationPath.replace_extension(extension.data());

        return destinationPath;
    }
}
