local action = _ACTION or ""

solution "tinyboard-tools"
	-- location("build")
	-- targetdir("bin")
	includedirs { "ftd2xx", "src" }
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

	project "i2cdetect"
		language "C"
		kind "ConsoleApp"
		files { "src/i2c_detect.c", "src/i2c_bitbang.c" }

	project "i2cdump"
		language "C"
		kind "ConsoleApp"
		files { "src/i2c_dump.c", "src/i2c_bitbang.c" }

	project "i2ce2rom"
		language "C"
		kind "ConsoleApp"
		files { "src/i2c_e2rom.c", "src/i2c_bitbang.c" }

	project "spitest"
		language "C"
		kind "ConsoleApp"
		files { "src/spi_test.c", "src/spi_bitbang.c" }

