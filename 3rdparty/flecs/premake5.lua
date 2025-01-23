print(os.date() .. " Prepare flecs submodule...")
print(os.date() .. " Building flecs with CMake...")

module = "flecs"
if not isdir("./lib") then
    cmake = "cmake"
    os.execute("{MKDIR} lib")

    os.execute(cmake .. " -S ./" .. module .. " -B ./" .. module .. "/auto_build")

    print("BUILD DEBUG VERSION OF" .. module)
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Debug")
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Release")

    os.execute("{MOVE} ./" .. module .. "/auto_build/Debug ./lib")
    os.execute("{MOVE} ./" .. module .. "/auto_build/Release ./lib")
end
