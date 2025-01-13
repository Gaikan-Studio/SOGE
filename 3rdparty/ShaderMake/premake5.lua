print(os.date() .. " Prepare ShaderMake submodule...")

module = "ShaderMake"
if not isdir("./bin") then
    cmake = "cmake"
    os.execute("{MKDIR} bin")

    os.execute(cmake .. " -S ./" .. module .. " -B ./" .. module .. "/auto_build -DSHADERMAKE_SEARCH_FOR_COMPILERS=ON")
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Release")
    os.execute("{MOVE} ./" .. module .. "/bin/Release/* ./bin")
end

local cmake_cache_file = io.open("./" .. module .. "/auto_build/CMakeCache.txt", "r")
local cmake_cache = cmake_cache_file:read("*a")
cmake_cache_file:close()

local FXC_PATH = cmake_cache:match("FXC_PATH:FILEPATH=(.-)\n")
local DXC_PATH = cmake_cache:match("DXC_PATH:FILEPATH=(.-)\n")
local DXC_SPIRV_PATH = cmake_cache:match("DXC_SPIRV_PATH:FILEPATH=(.-)\n")

local SHADERMAKE_PATH = path.getabsolute("./bin/ShaderMake")
if os.host() == "windows" then
    SHADERMAKE_PATH = path.appendExtension(SHADERMAKE_PATH, ".exe")
end

shadermake_platform = {
    DXBC = "DXBC",
    DXIL = "DXIL",
    SPIRV = "SPIRV",
}

shadermake_compiler = {
    FXC = "FXC",
    DXC = "DXC",
    DXC_SPIRV = "DXC_SPIRV",
}

function shadermake(options)
    if type(options.platform) ~= "string" then
        error("shadermake: platform should be specified")
    end
    if shadermake_platform[options.platform] == nil then
        error("shadermake: platform should be one of 'DXBC', 'DXIL' or 'SPIRV'")
    end

    if type(options.output) ~= "string" then
        error("shadermake: output path should be specified")
    end

    if type(options.config) ~= "string" then
        error("shadermake: config path should be specified")
    end

    local compiler
    if options.compiler ~= nil then
        if type(options.compiler) ~= "string" then
            error("shadermake: compiler should be a path or one of 'FXC', 'DXC' or 'DXC_SPIRV'")
        end
        if shadermake_compiler[options.compiler] ~= nil then
            local compiler_to_path = {
                [shadermake_compiler.FXC] = FXC_PATH,
                [shadermake_compiler.DXC] = DXC_PATH,
                [shadermake_compiler.DXC_SPIRV] = DXC_SPIRV_PATH,
            }
            compiler = compiler_to_path[options.compiler]
        else
            compiler = options.compiler
        end
    else
        compiler = os.host() == "windows" and DXC_PATH or DXC_SPIRV_PATH
    end

    local function to_path(path)
        return string.format("%%[%s]", path)
    end

    local command = to_path(SHADERMAKE_PATH)
    command = command .. " --binary"
    command = command .. " --verbose"
    command = command .. " --shaderModel 6_5"
    command = command .. " --platform " .. options.platform
    command = command .. " --config " .. to_path(options.config)
    command = command .. " --out " .. to_path(options.output)
    command = command .. " --compiler " .. to_path(compiler)
    return command
end
