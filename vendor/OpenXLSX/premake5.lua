project "OpenXLSX"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	systemversion "latest"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"sources/**.h",
		"sources/**.hpp",
		"sources/**.cpp",
		"sources/**.c",
		"headers/**.h",
		"headers/**.hpp",
		"headers/**.cpp",
		"headers/**.c"
	}

	includedirs
	{
		"",
		"headers",
		"external/nowide",
		"external/pugixml",
		"external/zippy",
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"