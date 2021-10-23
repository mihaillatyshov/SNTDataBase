project "SNTDataBase"
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
		"src/**.c",
		"../vendor/imgui/backends/imgui_impl_glfw.h",
		"../vendor/imgui/backends/imgui_impl_glfw.cpp",
		"../vendor/imgui/backends/imgui_impl_opengl2.h",
		"../vendor/imgui/backends/imgui_impl_opengl2.cpp"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGui_backends}",
		"%{IncludeDir.nlohmann}"
	}

	links
	{
		"glfw",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
