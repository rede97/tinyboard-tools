local action = _ACTION or ""

solution "tinyboard-tools"
	-- location("build")
	-- targetdir("bin")
	includedirs { "ftd2xx" }
	libdirs {"ftd2xx"}
	configurations { "Debug", "Release" }
	platforms {"native", "x64", "x32"}
	buildoptions {"-std=c99"} 
	linkoptions {"-l:ftd2xx.lib"}
	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols", "ExtraWarnings"}
	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize", "ExtraWarnings"}

	project "i2c-detect"
		language "C"
		kind "ConsoleApp"
		files { "src/*.c" }

