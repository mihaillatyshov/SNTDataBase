project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
		"src/**.c"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Vega/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glew}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.assimp}"
	}

	libdirs 
	{
		"%{wks.location}/deps/assimp141/lib"
	}

	links
	{
		"glfw",
		"glew",
		"ImGui",
		"opengl32.lib",
		"assimp-vc141-mt",
		"Vega"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
