print(os.date() .. " Prepare NRI submodule...")


module = "NRI"
if not isdir("./lib") then
    cmake = "cmake"
    os.execute("mkdir lib")
    os.execute("mkdir \"lib/Frameworks/\"")
    os.execute("mkdir \"lib/NRI/\"")

    os.execute(cmake .. " -S ./" .. module .. " -B ./" .. module .. "/auto_build")

    print("BUILD DEBUG VERSION OF" .. module)
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Debug")
    print("BUILD RELEASE VERSION OF" .. module)
    os.execute(cmake .. " --build ./" .. module .. "/auto_build --config Release")

    os.execute("move ./" .. module .. "/auto_build/Release ./lib/Frameworks")
    os.execute("move ./" .. module .. "/auto_build/Debug ./lib/Frameworks")
    os.execute("move ./" .. module .. "/_Bin/Release ./lib/NRI")
    os.execute("move ./" .. module .. "/_Bin/Debug ./lib/NRI")

end